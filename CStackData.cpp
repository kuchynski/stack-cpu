//---------------------------------------------------------------------------
#pragma hdrstop

#include "CStackData.h"

CStackData stack_data;
CStackReturn stack_return;
//---------------------------------------------------------------------------
void CStackData::Clear()
{
  for(int i = 0; i < 100; i ++)
    data[i] = 0;
}
//---------------------------------------------------------------------------
void CStackReturn::Clear()
{
  for(int i = 0; i < 100; i ++)
    data[i] = 0;
}
//---------------------------------------------------------------------------
void CStackData::Run(AnsiString command, unsigned alu, unsigned yy, unsigned r, unsigned bus_data, bool print_state)
{
  alu = Norma(alu);
  yy = Norma(yy);
  r = Norma(r);
  bus_data = Norma(bus_data);
  unsigned data_old[100];
  for(int i = 0; i < 100; i ++)
    data_old[i] = data[i];
  prev_data0 = data[0];
  prev_data1 = data[1];
//  int s_ms_all = 1;
//  int s_ms_0 = 4;
  bool ce_all;
  bool ce_2;
  bool ce_1;
  bool ce_0;
  //ce_all
  bool ar = ((command == "+")   || (command == "-")   || (command == "*") ||
             (command == "AND") || (command == "OR") || (command == "XOR") ||
             (command == "=")  || (command == "<>") ||
             (command == "<")   || (command == ">")   ||
             (command == "<=")   || (command == ">=")   ||
             (command == "MIN") || (command == "MAX"));
  ce_all = (ar || (command == "DUP") || (command == "OVER") || (command == "DROP") ||
     (command == "!") || (command == "ADDRESS") || (command == "CONSTANT") ||
     (command == "R>") || (command == ">R") || (command == "JZ"));
  ce_2 = (ce_all || (command == "ROT"));
  ce_1 = (ce_2 || (command == "SWAP"));
  ce_0 = (ce_1 || (command == "@") || (command == "NOT") || (command == "2*") || (command == "2/") ||
     (command == "1+") ||(command == "1-") ||(command == "2+") ||(command == "2-") ||    
     (command == "0<") || (command == "0>") || (command == "0=") || (command == "ABS") || (command == "NEGATE"));

  if(ce_0)
  {
    //s_ms_0
    if((command == "SWAP") || (command == "OVER") || (command == "DROP") ||
       (command == "JZ") || (command == ">R"))
      data[0] = data_old[1];
    else if((command == "!") || (command == "ROT"))
      data[0] = data_old[2];
    else if(command == "DUP")
      data[0] = data_old[0];
    else if((command == "ADDRESS"))
      data[0] = yy;
    else if((command == "CONSTANT") || (command == "@"))
      data[0] = bus_data;
    else if((command == "R>"))
      data[0] = r;
    else
      data[0] = alu;
   }   
  //s_ms_all
  for(int i = 1; i < size; i ++)
  {
    bool ce = (i > 2)? ce_all : ce_2;
    if(i == 1) ce = ce_1;
    if(ce)
    {
      if((command == "SWAP") || (command == "DUP") || (command == "OVER") ||
         (command == "ROT") || (command == "ADDRESS") || (command == "CONSTANT") ||
         (command == "R>"))
        data[i] = data_old[i - 1];
      else if(command == "!")
        data[i] = data_old[i+2];
      else
        data[i] = data_old[i+1];
    }
  }
  if(print_state)
    PrintValue();
}
//---------------------------------------------------------------------------
void CStackData::Reset()
{
  for(int i = 0; i < 100; i ++)
    data[i] = 0;
  PrintValue();
}
//---------------------------------------------------------------------------
void CStackData::PrintAll(TStringGrid *sg)
{
  grid = sg;
  grid->RowCount = size + 1;
  if(grid->RowCount > 1)
    grid->FixedRows = 1;
  for(int i = 1; i < grid->RowCount; i ++)
    grid->Cells[0][i] = i;
  PrintValue();
}
//---------------------------------------------------------------------------
void CStackData::PrintValue(int s)
{
  if(!grid)
    return;
  if((s == 10) || (s == 16))
    ss = s;
  int kol_hex = kol_bit_data/4 + ((kol_bit_data%4)? 1 : 0);
  for(int i = 1; i < grid->RowCount; i ++)
    if(ss == 10)
      grid->Cells[1][i] = IntToStr(ALU.GetInt(data[i-1]));
    else
      grid->Cells[1][i] = IntToHex((__int64)data[i-1], kol_hex);
}
//---------------------------------------------------------------------------
void CStackReturn::Reset()
{
  for(int i = 0; i < 100; i ++)
    data[i] = 0;
  PrintValue();
}
//---------------------------------------------------------------------------
void CStackReturn::Pop(bool print_state)
{
  prev_data0 = data[0];
  for(int i = 0; i < size-1; i ++)
    data[i] = data[i+1];
  data[size-1] = 0;
  if(print_state)
    PrintValue();
}
//---------------------------------------------------------------------------
void CStackReturn::Push(unsigned d, unsigned i, bool print_state)
{
  prev_data0 = data[0];
  d = d + (i << (kol_bit_data - kol_bit_irq));
  d = Norma(d);
  for(int i = size - 1; i > 0; i --)
    data[i] = data[i-1];
  data[0] = d;

  if(print_state)
    PrintValue();
}
//---------------------------------------------------------------------------
void CStackReturn::PrintAll(TStringGrid *sg)
{
  grid = sg;
  grid->RowCount = size + 1;
  if(grid->RowCount > 1)
    grid->FixedRows = 1;
  for(int i = 1; i < grid->RowCount; i ++)
    grid->Cells[0][i] = i;
  PrintValue();
}
//---------------------------------------------------------------------------
void CStackReturn::PrintValue()
{
  if(!grid)
    return;
  int kol_hex = kol_bit_data/4 + ((kol_bit_data%4)? 1 : 0);
  for(int i = 1; i < grid->RowCount; i ++)
    grid->Cells[1][i] = IntToHex((__int64)data[i-1], kol_hex);
}
//---------------------------------------------------------------------------
unsigned CStackReturn::Norma(unsigned ch)
{
  unsigned maska = 0xFFFFFFFF >> (32 - kol_bit_data);
  return ch & maska;
}
//---------------------------------------------------------------------------
unsigned CStackData::Norma(unsigned ch)
{
  unsigned maska = 0xFFFFFFFF >> (32 - kol_bit_data);
  return ch & maska;
}
//---------------------------------------------------------------------------
unsigned CStackReturn::GetDataAddress0()
{
  unsigned maska = 0xFFFFFFFF >> (32 - kol_bit_data + kol_bit_irq);
  return data[0] & maska;
}
//---------------------------------------------------------------------------
unsigned CStackReturn::GetPrevDataAddress0()
{
  unsigned maska = 0xFFFFFFFF >> (32 - kol_bit_data + kol_bit_irq);
  return prev_data0 & maska;
}
//---------------------------------------------------------------------------
unsigned CStackReturn::GetDataIrq0()
{
  unsigned maska = (0xFFFFFFFF >> (32 - kol_bit_irq)) << (kol_bit_data - kol_bit_irq);
  return (data[0] & maska) >> (kol_bit_data - kol_bit_irq);
}
//---------------------------------------------------------------------------
unsigned CStackReturn::GetPrevDataIrq0()
{
  unsigned maska = (0xFFFFFFFF >> (32 - kol_bit_irq)) << (kol_bit_data - kol_bit_irq);
  return (prev_data0 & maska) >> (kol_bit_data - kol_bit_irq);
}
//---------------------------------------------------------------------------
void CStackData::PrintVHDLSignals(TMemo *memo)
{
  memo->Lines->Add("   type sc_type_sd is array" + RAM.GetDownto(size+1, 0) + " of std_logic_vector" + RAM.GetDownto(kol_bit_data-1, 0) + ";");
  memo->Lines->Add("   signal sc_stack_data : sc_type_sd;");
  memo->Lines->Add("   signal sc_sd_ce : std_logic_vector" + RAM.GetDownto(size-1, 0) + ";");
}
//---------------------------------------------------------------------------
void CStackReturn::PrintVHDLSignals(TMemo *memo)
{
  memo->Lines->Add("   type sc_type_sr is array" + RAM.GetDownto(size, 0) + " of std_logic_vector" + RAM.GetDownto(kol_bit_data-1, 0) + ";");
  memo->Lines->Add("   signal sc_stack_return : sc_type_sr;");
  memo->Lines->Add("   signal sc_sr_ce : std_logic;");
}
//---------------------------------------------------------------------------
void CStackData::PrintVHDLProcess(TMemo *memo)
{
  if(!size)
    return;
  memo->Lines->Add("--стек данных в представлении Кучинского Андрея");
  AnsiString str = "";
  if(dictionary.GetKolUse("+")) str += (" " + YY.GetCommandKodBin("+") + " |");
  if(dictionary.GetKolUse("-")) str += (" " + YY.GetCommandKodBin("-") + " |");
  if(dictionary.GetKolUse("*")) str += (" " + YY.GetCommandKodBin("*") + " |");
  if(dictionary.GetKolUse("AND")) str += (" " + YY.GetCommandKodBin("AND") + " |");
  if(dictionary.GetKolUse("OR")) str += (" " + YY.GetCommandKodBin("OR") + " |");
  if(dictionary.GetKolUse("XOR")) str += (" " + YY.GetCommandKodBin("XOR") + " |");
  if(dictionary.GetKolUse("=")) str += (" " + YY.GetCommandKodBin("=") + " |");
  if(dictionary.GetKolUse("<>")) str += (" " + YY.GetCommandKodBin("<>") + " |");
  if(dictionary.GetKolUse("<")) str += (" " + YY.GetCommandKodBin("<") + " |");
  if(dictionary.GetKolUse(">")) str += (" " + YY.GetCommandKodBin(">") + " |");
  if(dictionary.GetKolUse("<=")) str += (" " + YY.GetCommandKodBin("<=") + " |");
  if(dictionary.GetKolUse(">=")) str += (" " + YY.GetCommandKodBin(">=") + " |");
  if(dictionary.GetKolUse("MIN")) str += (" " + YY.GetCommandKodBin("MIN") + " |");
  if(dictionary.GetKolUse("MAX")) str += (" " + YY.GetCommandKodBin("MAX") + " |");
  if(dictionary.GetKolUse("DUP")) str += (" " + YY.GetCommandKodBin("DUP") + " |");
  if(dictionary.GetKolUse("OVER")) str += (" " + YY.GetCommandKodBin("OVER") + " |");
  if(dictionary.GetKolUse("DROP")) str += (" " + YY.GetCommandKodBin("DROP") + " |");
  if(dictionary.GetKolUse("ADDRESS")) str += (" " + YY.GetCommandKodBin("ADDRESS") + " |");
  if(dictionary.GetKolUse("CONSTANT")) str += (" " + YY.GetCommandKodBin("CONSTANT") + " |");
  if(dictionary.GetKolUse("!")) str += (" " + YY.GetCommandKodBin("!") + " |");
  if(dictionary.GetKolUse(">R")) str += (" " + YY.GetCommandKodBin(">R") + " |");
  if(dictionary.GetKolUse("R>")) str += (" " + YY.GetCommandKodBin("R>") + " |");
  if(dictionary.GetKolUse("JZ")) str += (" " + YY.GetCommandKodBin("JZ") + " |");
  if(str == "")
    memo->Lines->Add("   sc_sd_ce(3) <= '0';");
  else
  {
    str[str.Length()] = ' ';
    memo->Lines->Add("   with sc_yy_kod1_2 select");
    memo->Lines->Add("      sc_sd_ce(3) <= '1' when " + str + ", '0' when others;");
  }
  for(int i = 4; i < size; i ++)
    memo->Lines->Add("   sc_sd_ce(" + IntToStr(i) + ") <= sc_sd_ce(3);");
  str = (dictionary.GetKolUse("ROT"))? (" '1' when sc_yy_kod1_2 = " + YY.GetCommandKodBin("ROT") + " else") : AnsiString("");
  memo->Lines->Add("   sc_sd_ce(2) <=" + str + " sc_sd_ce(3);");
  str = (dictionary.GetKolUse("SWAP"))? (" '1' when sc_yy_kod1_2 = " + YY.GetCommandKodBin("SWAP") + " else") : AnsiString("");
  memo->Lines->Add("   sc_sd_ce(1) <=" + str + " sc_sd_ce(2);");
  str = "";
  if(dictionary.GetKolUse("@")) str += (YY.GetCommandKodBin("@") + " |");
  if(dictionary.GetKolUse("NOT")) str += (YY.GetCommandKodBin("NOT") + " |");
  if(dictionary.GetKolUse("2*")) str += (YY.GetCommandKodBin("2*") + " |");
  if(dictionary.GetKolUse("2/")) str += (YY.GetCommandKodBin("2/") + " |");
  if(dictionary.GetKolUse("1+")) str += (YY.GetCommandKodBin("1+") + " |");
  if(dictionary.GetKolUse("1-")) str += (YY.GetCommandKodBin("1-") + " |");
  if(dictionary.GetKolUse("2+")) str += (YY.GetCommandKodBin("2+") + " |");
  if(dictionary.GetKolUse("2-")) str += (YY.GetCommandKodBin("2-") + " |");
  if(dictionary.GetKolUse("0=")) str += (YY.GetCommandKodBin("0=") + " |");
  if(dictionary.GetKolUse("0<")) str += (YY.GetCommandKodBin("0<") + " |");
  if(dictionary.GetKolUse("0>")) str += (YY.GetCommandKodBin("0>") + " |");
  if(dictionary.GetKolUse("ABS")) str += (YY.GetCommandKodBin("ABS") + " |");
  if(dictionary.GetKolUse("NEGATE")) str += (YY.GetCommandKodBin("NEGATE") + " |");
  if(str != "")
  {
    str[str.Length()] = ' ';
    memo->Lines->Add("   with sc_yy_kod1_2 select");
    memo->Lines->Add("      sc_sd_ce(0) <= '1' when " + str + ", sc_sd_ce(1) when others;");
  }
  else
    memo->Lines->Add("   sc_sd_ce(0) <= sc_sd_ce(1);");
  AnsiString str_sm0_0 = "", str_sm0_1 = "", str_sm0_2 = "", str_sm0_3 = "", str_sm0_4 = "", str_sm0_5 = "";
  if(dictionary.GetKolUse("SWAP")) str_sm0_0 += (" " + YY.GetCommandKodBin("SWAP") + " |");
  if(dictionary.GetKolUse("OVER")) str_sm0_0 += (" " + YY.GetCommandKodBin("OVER") + " |");
  if(dictionary.GetKolUse("DROP")) str_sm0_0 += (" " + YY.GetCommandKodBin("DROP") + " |");
  if(dictionary.GetKolUse("JZ")) str_sm0_0 += (" " + YY.GetCommandKodBin("JZ") + " |");
  if(dictionary.GetKolUse(">R")) str_sm0_0 += (" " + YY.GetCommandKodBin(">R") + " |");
  if(dictionary.GetKolUse("!")) str_sm0_1 += (" " + YY.GetCommandKodBin("!") + " |");
  if(dictionary.GetKolUse("ROT")) str_sm0_1 += (" " + YY.GetCommandKodBin("ROT") + " |");
  if(dictionary.GetKolUse("DUP")) str_sm0_2 = (" " + YY.GetCommandKodBin("DUP"));
  if(dictionary.GetKolUse("ADDRESS")) str_sm0_3 = (" " + YY.GetCommandKodBin("ADDRESS"));
  if(dictionary.GetKolUse("CONSTANT")) str_sm0_4 += (" " + YY.GetCommandKodBin("CONSTANT") + " |");
  if(dictionary.GetKolUse("@")) str_sm0_4 += (" " + YY.GetCommandKodBin("@") + " |");
  if(dictionary.GetKolUse("R>")) str_sm0_5 = (" " + YY.GetCommandKodBin("R>"));
  if(str_sm0_0 != "") str_sm0_0[str_sm0_0.Length()] = ' ';
  if(str_sm0_1 != "") str_sm0_1[str_sm0_1.Length()] = ' ';
  if(str_sm0_4 != "") str_sm0_4[str_sm0_4.Length()] = ' ';
  AnsiString str_sm1_0 = "", str_sm1_1 = "";
  if(dictionary.GetKolUse("SWAP")) str_sm1_0 += (" " + YY.GetCommandKodBin("SWAP") + " |");
  if(dictionary.GetKolUse("OVER")) str_sm1_0 += (" " + YY.GetCommandKodBin("OVER") + " |");
  if(dictionary.GetKolUse("DUP")) str_sm1_0 += (" " + YY.GetCommandKodBin("DUP") + " |");
  if(dictionary.GetKolUse("ROT")) str_sm1_0 += (" " + YY.GetCommandKodBin("ROT") + " |");
  if(dictionary.GetKolUse("ADDRESS")) str_sm1_0 += (" " + YY.GetCommandKodBin("ADDRESS") + " |");
  if(dictionary.GetKolUse("CONSTANT")) str_sm1_0 += (" " + YY.GetCommandKodBin("CONSTANT") + " |");
  if(dictionary.GetKolUse("R>")) str_sm1_0 += (" " + YY.GetCommandKodBin("R>") + " |");
  if(dictionary.GetKolUse("!")) str_sm1_1 = (" " + YY.GetCommandKodBin("!") + "");
  if(str_sm1_0 != "") str_sm1_0[str_sm1_0.Length()] = ' ';

  memo->Lines->Add("   sc_stack_data(" + IntToStr(size) + ") <= (others => '0');");
  memo->Lines->Add("   sc_stack_data(" + IntToStr(size+1) + ") <= (others => '0');");
  memo->Lines->Add("p_stack_data: process (clk, sc_rst)");
  memo->Lines->Add("   begin");
  memo->Lines->Add("      if sc_rst = '1' then");
  memo->Lines->Add("         loop_ds_ce:for i in " + IntToStr(size-1) + " downto 0 loop");
  memo->Lines->Add("            sc_stack_data(i) <= (others => '0');");
  memo->Lines->Add("         end loop;");
  memo->Lines->Add("      else");
  memo->Lines->Add("         if clk = '1' and clk'event then");
  memo->Lines->Add("            if sc_sd_ce(0) = '1' then");
  memo->Lines->Add("               case sc_yy_kod1_2 is");
  if(str_sm0_0 != "") memo->Lines->Add("                  when " + str_sm0_0 + " => sc_stack_data(0) <= sc_stack_data(1);");
  if(str_sm0_1 != "") memo->Lines->Add("                  when " + str_sm0_1 + " => sc_stack_data(0) <= sc_stack_data(2);");
  if(str_sm0_2 != "") memo->Lines->Add("                  when " + str_sm0_2 + " => sc_stack_data(0) <= sc_stack_data(0);");
  if(str_sm0_3 != "")
    if(kol_bit_data > ROM.GetKolBitAddrressOp())
      memo->Lines->Add("                  when " + str_sm0_3 + " => sc_stack_data(0) <= " + stack_return.GetChNULL(kol_bit_data - ROM.GetKolBitAddrressOp()) + " & sc_kod2;");
    else if(kol_bit_data < ROM.GetKolBitAddrressOp())
      memo->Lines->Add("                  when " + str_sm0_3 + " => sc_stack_data(0) <= sc_kod2" + RAM.GetDownto(kol_bit_data - 1,0) + ";");
    else
      memo->Lines->Add("                  when " + str_sm0_3 + " => sc_stack_data(0) <= sc_kod2;");
  if(str_sm0_4 != "")
    if(kol_bit_data - ALU.GetKolBitData())
      memo->Lines->Add("                  when " + str_sm0_4 + " => sc_stack_data(0) <= " + stack_return.GetChNULL(kol_bit_data - ALU.GetKolBitData()) + " & sc_bus_data;");
    else
      memo->Lines->Add("                  when " + str_sm0_4 + " => sc_stack_data(0) <= sc_bus_data;");
  if(str_sm0_5 != "") memo->Lines->Add("                  when " + str_sm0_5 + " => sc_stack_data(0) <= sc_stack_return(0)" + RAM.GetDownto(kol_bit_data-1, 0) + ";");
  if(ALU.GetKolCommands())
    if(kol_bit_data - ALU.GetKolBitData())
      memo->Lines->Add("                  when others => sc_stack_data(0) <= " + stack_return.GetChNULL(kol_bit_data - ALU.GetKolBitData()) + " & sc_alu;");
    else
      memo->Lines->Add("                  when others => sc_stack_data(0) <= sc_alu;");
  else
    memo->Lines->Add("                  when others => sc_stack_data(0) <= (others => '0');");
  memo->Lines->Add("               end case;");
  memo->Lines->Add("            end if;");
  memo->Lines->Add("            loop_ds:for i in " + IntToStr(size-1) + " downto 1 loop");
  memo->Lines->Add("               if sc_sd_ce(i) = '1' then");
  memo->Lines->Add("                  case sc_yy_kod1_2 is");
  if(str_sm1_0 != "") memo->Lines->Add("                     when " + str_sm1_0 + " => sc_stack_data(i) <= sc_stack_data(i-1);");
  if(str_sm1_1 != "") memo->Lines->Add("                     when " + str_sm1_1 + " => sc_stack_data(i) <= sc_stack_data(i+2);");
  memo->Lines->Add("                     when others => sc_stack_data(i) <= sc_stack_data(i+1);");
  memo->Lines->Add("                  end case;");
  memo->Lines->Add("               end if;");
  memo->Lines->Add("            end loop;");
  memo->Lines->Add("         end if;");
  memo->Lines->Add("      end if;");
  memo->Lines->Add("   end process;");
}
//---------------------------------------------------------------------------
void CStackReturn::PrintVHDLProcess(TMemo *memo)
{
  if(!size)
    return;
  memo->Lines->Add("--стек возвратов любезно представленный Кучинским Андреем");
  AnsiString str = "";
  if(dictionary.GetKolUse(">R")) str += (" '1' when sc_yy_kod1 = " + YY.GetCommandKodBin(">R") + " else");
  if(dictionary.GetKolUse("R>")) str += (" '1' when sc_yy_kod1 = " + YY.GetCommandKodBin("R>") + " else");
  if(dictionary.GetKolUse("R@")) str += (" '1' when sc_yy_kod1 = " + YY.GetCommandKodBin("R@") + " else");
  if(dictionary.GetKolUse("CALL")) str += (" '1' when sc_yy_kod1 = " + YY.GetCommandKodBin("CALL") + " else");
  if(dictionary.GetKolUse("RETURN")) str += (" '1' when sc_yy_kod1 = " + YY.GetCommandKodBin("RETURN") + " else");
  memo->Lines->Add("   sc_sr_ce <=" + str + " '0';");
  AnsiString str_sm0 = "", str_sm1 = "";
  if(dictionary.GetKolUse(">R")) str_sm0 += (" " + YY.GetCommandKodBin(">R"));
  if(dictionary.GetKolUse("CALL")) str_sm1 += (" " + YY.GetCommandKodBin("CALL"));
  memo->Lines->Add("   sc_stack_return(" + IntToStr(size) + ") <= (others => '0');");
  memo->Lines->Add("p_stack_return: process (clk, sc_rst)");
  memo->Lines->Add("   begin");
  memo->Lines->Add("      if sc_rst = '1' then");
  memo->Lines->Add("         loop_rs_ce:for i in " + IntToStr(size-1) + " downto 0 loop");
  memo->Lines->Add("            sc_stack_return(i) <= (others => '0');");
  memo->Lines->Add("         end loop;");
  memo->Lines->Add("      else");
  memo->Lines->Add("         if clk = '1' and clk'event then");
  memo->Lines->Add("            if sc_sr_ce = '1' then");
  memo->Lines->Add("               case sc_yy_kod1 is");
  if(str_sm1 != "") memo->Lines->Add("                  when " + str_sm1 + " => sc_stack_return(0) <= sc_address_call;");
  if(str_sm0 != "")
    if(kol_bit_data - stack_data.GetKolBitDataWhithZnak())
      memo->Lines->Add("                  when " + str_sm0 + " => sc_stack_return(0) <= " + GetChNULL(kol_bit_data - stack_data.GetKolBitDataWhithZnak()) + " & sc_stack_data(0);");
    else
      memo->Lines->Add("                  when " + str_sm0 + " => sc_stack_return(0) <= sc_stack_data(0);");
  memo->Lines->Add("                  when others => sc_stack_return(0) <= sc_stack_return(1);");
  memo->Lines->Add("               end case;");
  memo->Lines->Add("               loop_rs:for i in " + IntToStr(size-1) + " downto 1 loop");
  memo->Lines->Add("                  case sc_yy_kod1 is");
  if(str_sm0 != "") memo->Lines->Add("                     when " + str_sm0 + " => sc_stack_return(i) <= sc_stack_return(i-1);");
  if(str_sm1 != "") memo->Lines->Add("                     when " + str_sm1 + " => sc_stack_return(i) <= sc_stack_return(i-1);");
  memo->Lines->Add("                     when others => sc_stack_return(i) <= sc_stack_return(i+1);");
  memo->Lines->Add("                  end case;");
  memo->Lines->Add("               end loop;");
  memo->Lines->Add("            end if;");
  memo->Lines->Add("         end if;");
  memo->Lines->Add("      end if;");
  memo->Lines->Add("   end process;");
}
//---------------------------------------------------------------------------
AnsiString CStackReturn::GetChNULL(int ch)
{
  if(ch == 1)
    return "'0'";
  AnsiString ret = "\"";
  for(int i = 0; i < ch; i ++)
     ret += "0";
  return ret + "\"";
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
