//---------------------------------------------------------------------------
#pragma hdrstop

#include "CYY.h"

CYY YY;
//---------------------------------------------------------------------------
void CYY::SetCommands(vector <CStandartWord> c)
{
  commands.clear();
  for(vector<CStandartWord>:: iterator p = c.begin(); p < c.end(); p++)
    if(p->GetKolUse())
      commands.push_back(*p);
}
//---------------------------------------------------------------------------
void CYY::Reset()
{
  st_commands = 0;
  stack_return.Reset();
  stack_data.Reset();
  ports.Reset();
  uarts.Reset();
  timers.Reset();   // Технология разноуровнего обучения
}
//---------------------------------------------------------------------------
bool CYY::Run()
{
  div_frequency.NewTakt();
  unsigned command1, command2;
  if(!ROM.Run(st_commands, command1, command2,true))
    return false;
  command = GetCommand();
  ROM.SetlastCommand(command);
  RunShinaData();
  alu_data = ALU.Run(command, stack_data.GetData0(), stack_data.GetData1());
  RunStackReturn();
  RunStackData();
  RunStCommands();
  RunRAMAndPorts();
  ROM.SetState(st_commands);
}
//---------------------------------------------------------------------------
void CYY::SetStCommands(unsigned st)
{
  st_commands = st;
  ROM.SetState(st);
}
//---------------------------------------------------------------------------
AnsiString CYY::GetCommand(bool real)
{
  unsigned address_irq;  //обратный такт
  work_irq = IRQ.NewIRQ(address_irq, real);

  //прямой такт
  unsigned rom_command1, rom_command2;
  if(!ROM.Run(st_commands, rom_command1, rom_command2, real))
    return "";

  kod_op = rom_command1;
  if(work_irq)
    address = address_irq;
  else
    address = rom_command2;

  AnsiString c = WhatTheCommand(kod_op);

  if(work_irq)
    c = "CALL";
  command2 = c;
  if((c == "JZ") && !stack_data.GetData0())
    c = "JMP";
  return c;
}
//---------------------------------------------------------------------------
AnsiString CYY::WhatTheCommand(unsigned c)
{
  for(vector <CStandartWord>:: iterator p = commands.begin(); p < commands.end(); p ++)
    if(p->GetKolUse())
      if(p->IsMyCommand(c))
        return p->GetName();
  return "";
}
//---------------------------------------------------------------------------
AnsiString CYY::GetCommandKodBin(AnsiString c)
{
  return dictionary.GetCommandKodBin(c);
}
//---------------------------------------------------------------------------
void CYY::RunStCommands()
{
  if((command == "ADDRESS") || (command == "CONSTANT") || (command == "JZ"))
    st_commands += 2;
  else if((command == "CALL") || (command == "JMP"))
    st_commands = address;
  else if(command == "RETURN")
  {
    st_commands = stack_return.GetPrevDataAddress0();
    IRQ.ResetIRQ(stack_return.GetPrevDataIrq0());
  }
  else if(command == "QUIT")
  {
    st_commands = 0;
    RAM.Reset();
    stack_data.Reset();
    stack_return.Reset();
    IRQ.Reset();
    ports.Reset();
  }
  else
    st_commands ++;

  unsigned maska = 0xFFFFFFFF >> (32 - ROM.GetKolBitAddress());
  st_commands = st_commands & maska;
}
//---------------------------------------------------------------------------
void CYY::RunStackReturn()
{
  if(work_irq)
    stack_return.Push(st_commands, work_irq, true);
  else if(command == "CALL")
    stack_return.Push(st_commands+2, work_irq, true);
  else if(command == "RETURN")
    stack_return.Pop(true);
  else if(command == ">R")
    stack_return.Push(stack_data.GetData0(), 0, true);
  else if((command == "R>") || (command == "R@"))
    stack_return.Pop(true);
}
//---------------------------------------------------------------------------
void CYY::RunStackData()
{
  stack_data.Run(command2, alu_data, address, stack_return.GetPrevData0(), shina_data, true);
}
//---------------------------------------------------------------------------
void CYY::RunShinaData()
{
  unsigned a = (command == "CONSTANT")? address : stack_data.GetData0();
  int shina_data1 = ports.GetData(a);
  int shina_data2 = RAM.GetData(a);
  shina_data = shina_data1 | shina_data2;
}
//---------------------------------------------------------------------------
void CYY::RunRAMAndPorts()
{
  if(command == "!")
  {
    ports.SetData(stack_data.GetPrevData0(), stack_data.GetPrevData1());
    RAM.SetData(stack_data.GetPrevData0(), stack_data.GetPrevData1());
  }   
}
//---------------------------------------------------------------------------
void CYY::PrintVHDLSignals(TMemo *memo)
{
//  memo->Lines->Add("   signal sc_shina_data : std_logic_vector(" + IntToStr(RAM.GetKolBitDataWhithZnak() - 1) + " downto 0);");
  memo->Lines->Add("   signal sc_address_data : std_logic_vector" + RAM.GetDownto(linker.GetSizeShinaData() - 1, 0) + ";");
  memo->Lines->Add("   signal sc_yy_kod1, sc_yy_kod1_2 : std_logic_vector" + RAM.GetDownto(ROM.GetKolBitKodOp() - 1, 0) + ";");
  memo->Lines->Add("   signal sc_yy_kod2 : std_logic_vector" + RAM.GetDownto(ROM.GetKolBitAddrressOp() - 1, 0) + ";");
  memo->Lines->Add("   signal sc_vz, sc_return, sc_rst, clk_not : std_logic;");
  memo->Lines->Add("   signal sc_bus_data : std_logic_vector" + RAM.GetDownto(RAM.GetKolBitDataWhithZnak() - 1, 0) + ";");
  memo->Lines->Add("   signal sc_address_call : std_logic_vector" + RAM.GetDownto(stack_return.GetKolBitData() - 1, 0) + ";");
//  memo->Lines->Add("   signal sc_shina_commands : std_logic_vector(" + IntToStr(linker.GetSizeShinaData() - 1) + " downto 0);");
  memo->Lines->Add("   signal sc_address_commands_next, sc_address_commands_next1, sc_address_commands, sc_address_commands1, sc_address_commands2 : std_logic_vector" + RAM.GetDownto(ROM.GetKolBitAddress() - 1, 0) + ";");
}
//---------------------------------------------------------------------------
void CYY::PrintVHDLProcess(TMemo *memo)
{
  AnsiString str1 = "", str2 = "";
  if(dictionary.GetKolUse("CALL"))
    if(str1 == "") str1 += GetCommandKodBin("CALL");
    else str1 += (" | " + GetCommandKodBin("CALL"));
  if(dictionary.GetKolUse("JMP"))
    if(str1 == "") str1 += GetCommandKodBin("JMP");
    else str1 += (" | " + GetCommandKodBin("JMP"));
  if(dictionary.GetKolUse("ADDRESS"))
    if(str2 == "") str2 += GetCommandKodBin("ADDRESS");
    else str2 += (" | " + GetCommandKodBin("ADDRESS"));
  if(dictionary.GetKolUse("CONSTANT"))
    if(str2 == "") str2 += GetCommandKodBin("CONSTANT");
    else str2 += (" | " + GetCommandKodBin("CONSTANT"));
  if(dictionary.GetKolUse("JZ"))
    if(str2 == "") str2 += GetCommandKodBin("JZ");
    else str2 += (" | " + GetCommandKodBin("JZ"));
  memo->Lines->Add("--устройство управления, придуманное Кучинским Андреем");
  memo->Lines->Add("   clk_not <= not clk;");
  if(dictionary.GetKolUse("!"))
    memo->Lines->Add("   sc_vz <= '1' when sc_yy_kod1 = " + GetCommandKodBin("!") + " else '0';");
//  if(dictionary.GetKolUse("RETURN"))
    memo->Lines->Add("   sc_return <= '1' when sc_yy_kod1 = " + GetCommandKodBin("RETURN") + " else '0';");
  memo->Lines->Add("   sc_address_commands1 <= sc_address_commands + 1;");
  memo->Lines->Add("   sc_address_commands2 <= sc_address_commands + 2;");
  memo->Lines->Add("   sc_address_commands_next1 <=sc_address_commands_next + 1;");
  memo->Lines->Add("p_commands: process (sc_rst, clk)");
  memo->Lines->Add("   begin");
  memo->Lines->Add("      if sc_rst = '1' then sc_address_commands <= (others => '0');");
  memo->Lines->Add("	  else if clk='1' and clk'event then sc_address_commands <= sc_address_commands_next; end if;");
  memo->Lines->Add("      end if;");
  memo->Lines->Add("   end process;");
  AnsiString str4 = "sc_rst, sc_yy_kod1, sc_address_commands1";
  if(str1 != "") str4 += ", sc_yy_kod2" + RAM.GetDownto(ROM.GetKolBitAddress() - 1, 0);
  if(str2 != "") str4 += ", sc_address_commands2";
  if(dictionary.GetKolUse("RETURN")) str4 += ", sc_stack_return(0)" + RAM.GetDownto(ROM.GetKolBitAddress() - 1, 0);
  memo->Lines->Add("p_commands_next: process ("+str4+")");
  memo->Lines->Add("   begin");
  memo->Lines->Add("      if sc_rst = '1' then sc_address_commands_next <= (others => '0'); else");
  memo->Lines->Add("      case sc_yy_kod1 is");
  if(str1 != "") memo->Lines->Add("         when " + str1 + " => sc_address_commands_next <= sc_yy_kod2" + RAM.GetDownto(ROM.GetKolBitAddress() - 1, 0) + ";");
  if(str2 != "") memo->Lines->Add("         when " + str2 + " => sc_address_commands_next <= sc_address_commands2;");
  if(dictionary.GetKolUse("RETURN"))
    memo->Lines->Add("         when " + GetCommandKodBin("RETURN") + " => sc_address_commands_next <= sc_stack_return(0)" + RAM.GetDownto(ROM.GetKolBitAddress() - 1, 0) + ";");
  if(dictionary.GetKolUse("QUIT"))
    memo->Lines->Add("         when " + GetCommandKodBin("QUIT") + " => sc_address_commands_next <= (others => '0');");
  memo->Lines->Add("         when others => sc_address_commands_next <= sc_address_commands1;");
  memo->Lines->Add("      end case; end if;");
  memo->Lines->Add("   end process;");
  if(IRQ.GetSize())
  {
    memo->Lines->Add("   sc_address_call" + RAM.GetDownto(stack_return.GetKolBitData()-1, stack_return.GetKolBitData()-stack_return.GetKolBitIrq()) + " <= sc_irq_run_number;");
    int razn = stack_return.GetKolBitData() - (stack_return.GetKolBitIrq() + ROM.GetKolBitAddress());
    if(razn > 1)
      memo->Lines->Add("   sc_address_call" + RAM.GetDownto(stack_return.GetKolBitData()-stack_return.GetKolBitIrq()-1, ROM.GetKolBitAddress()) + " <= (others => '0');");
    else if(razn == 1)
      memo->Lines->Add("   sc_address_call(" + IntToStr(ROM.GetKolBitAddress()) + ") <= '0';");
    memo->Lines->Add("   sc_address_call" + RAM.GetDownto(ROM.GetKolBitAddress()-1, 0) + " <= sc_address_commands when sc_irq_run_number /= " +
                     RAM.GetNumberBin(0, stack_return.GetKolBitIrq()) + " else sc_address_commands2;");
  }
  else
    memo->Lines->Add("   sc_address_call" + RAM.GetDownto(ROM.GetKolBitAddress()-1, 0) + " <= sc_address_commands2;");
  if(stack_return.GetKolBitIrq())
    memo->Lines->Add("   sc_yy_kod1_2 <= " + GetCommandKodBin("CALL") + " when sc_irq_run_number /= " + RAM.GetNumberBin(0, stack_return.GetKolBitIrq()) + " else sc_kod1;");
  else
    memo->Lines->Add("   sc_yy_kod1_2 <=  sc_kod1;");
  if(dictionary.GetKolUse("JZ"))
    memo->Lines->Add("   sc_yy_kod1 <= " + GetCommandKodBin("JMP") + " when (sc_yy_kod1_2 = " + GetCommandKodBin("JZ") + " and sc_stack_data(0) = " + stack_return.GetChNULL(stack_data.GetKolBitDataWhithZnak()) +
                     ") else sc_yy_kod1_2;");
  else
    memo->Lines->Add("   sc_yy_kod1 <= sc_yy_kod1_2;");
  if(stack_return.GetKolBitIrq())
  {
    if(ROM.GetKolBitAddrressOp() != ROM.GetKolBitAddress())
      memo->Lines->Add("   sc_yy_kod2 <= " + stack_return.GetChNULL(ROM.GetKolBitAddrressOp() - ROM.GetKolBitAddress()) + " & sc_address_irq when sc_irq_run_number /= " +
                       RAM.GetNumberBin(0, stack_return.GetKolBitIrq()) + " else sc_kod2;");
    else
      memo->Lines->Add("   sc_yy_kod2 <= sc_address_irq when sc_irq_run_number /= " +
                       RAM.GetNumberBin(0, stack_return.GetKolBitIrq()) + " else sc_kod2;");
  }
  else
    memo->Lines->Add("   sc_yy_kod2 <= sc_kod2;");
  if(dictionary.GetKolUse("CONSTANT"))
    memo->Lines->Add("   sc_address_data <= sc_yy_kod2" + RAM.GetDownto(linker.GetSizeShinaData() - 1, 0) + " when sc_kod1 = " + GetCommandKodBin("CONSTANT") + " else sc_stack_data(0)" + RAM.GetDownto(linker.GetSizeShinaData() - 1, 0) + ";");
  memo->Lines->Add("p_rst: process (rst, clk)");
  memo->Lines->Add("   begin");
  memo->Lines->Add("      if rst = '1' then sc_rst <= '1';");
  memo->Lines->Add("	  else if clk='1' and clk'event then if sc_yy_kod1 = " + GetCommandKodBin("QUIT") + " then sc_rst <= '1'; else sc_rst <= '0'; end if; end if; end if;");
  memo->Lines->Add("   end process;");
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
