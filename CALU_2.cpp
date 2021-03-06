//---------------------------------------------------------------------------
#pragma hdrstop

#include "CALU.h"

CALU ALU;
//---------------------------------------------------------------------------
void CALU::SetCommands(vector <CStandartWord> c)
{
  const int k = 26;
  AnsiString q[k] = {"+", "-", "*", "NOT", "OR", "AND", "XOR", "=", "<>", "<", ">=", "<=",
                       ">", "0=", "0<", "0>", "MIN", "MAX", "2*", "2/", "ABS", "NEGATE", "1+", "1-", "2+","2-"};
  for(vector<CStandartWord>:: iterator p = c.begin(); p < c.end(); p++)
    for(int i = 0; i < k; i ++)
      if(p->IsThe(q[i], true))
        if(p->GetKolUse())
          commands.push_back(*p);
} 
//---------------------------------------------------------------------------
unsigned CALU::Run(AnsiString command, unsigned a, unsigned b)
{
  a = Norma(a);
  b = Norma(b);
  data_plus = RunPlus(a, b);
  data_minus = RunMinus(a, b);
  data_mul = RunMul(a, b);
  data_div = RunDiv(a, b);
  data_not = RunNot(a);
  data_or = RunOr(a, b);
  data_and = RunAnd(a, b);
  data_xor = RunXor(a, b);
  data_compare = RunCompare(command, a, b);
  data_sign = RunSign(command, a);
  data_min = RunMin(a, b);
  data_max = RunMax(a, b);
  data_shr = RunShr(a);
  data_shl = RunShl(a);
  data_1plus = Run1Plus(a);
  data_1minus = Run1Minus(a);
  data_2plus = Run2Plus(a);
  data_2minus = Run2Minus(a);
  if(command == "+")
    return data_plus;
  if(command == "-")
    return data_minus;
  if(command == "1+")
    return data_1plus;
  if(command == "1-")
    return data_1minus;
  if(command == "2+")
    return data_2plus;
  if(command == "2-")
    return data_2minus;
  if(command == "*")
    return data_mul;
  if(command == "NOT")
    return data_not;
  if(command == "OR")
    return data_or;
  if(command == "AND")
    return data_and;
  if(command == "XOR")
    return data_xor;
  if(command == "MIN")
    return data_min;
  if(command == "MAX")
    return data_max;
  if(command == "2/")
    return data_shr;
  if(command == "2*")
    return data_shl;
  if((command == "=") || (command == "<>") || (command == "<") || (command == ">") ||
     (command == ">=") || (command == "<=") || (command == "0=") || (command == "0<") || (command == "0>"))
    return data_compare;
  if((command == "ABS") || (command == "NEGATE"))
    return data_sign;
  return 0;
}
//---------------------------------------------------------------------------
unsigned CALU::RunPlus(unsigned a, unsigned b)
{
  if(!znak)
    return Norma(b + a);
  return Norma(GetHex(GetInt(a) + GetInt(b)));
}
//---------------------------------------------------------------------------
unsigned CALU::RunMinus(unsigned a, unsigned b)
{
  if(!znak)
    return Norma(b - a);
  return Norma(GetHex(GetInt(b) - GetInt(a)));
}
//---------------------------------------------------------------------------
unsigned CALU::Run1Plus(unsigned a)
{
  if(!znak)
    return Norma(a + 1);
  return Norma(GetHex(GetInt(a) + 1));
}
//---------------------------------------------------------------------------
unsigned CALU::Run1Minus(unsigned a)
{
  if(!znak)
    return Norma(a - 1);
  return Norma(GetHex(GetInt(a) - 1));
}
//---------------------------------------------------------------------------
unsigned CALU::Run2Plus(unsigned a)
{
  if(!znak)
    return Norma(a + 2);
  return Norma(GetHex(GetInt(a) + 2));
}
//---------------------------------------------------------------------------
unsigned CALU::Run2Minus(unsigned a)
{
  if(!znak)
    return Norma(a - 2);
  return Norma(GetHex(GetInt(a) - 2));
}
//---------------------------------------------------------------------------
unsigned CALU::RunMul(unsigned a, unsigned b)
{
  if(!znak)
    return Norma(b * a);
  return Norma(GetHex(GetInt(a) * GetInt(b)));
}
//---------------------------------------------------------------------------
unsigned CALU::RunDiv(unsigned a, unsigned b)
{
  if(a == 0)
    return 0xFFFFFFFF;
  if(!znak)
    return Norma(b / a);
  return Norma(GetHex(GetInt(a) / GetInt(b)));
}
//---------------------------------------------------------------------------
unsigned CALU::RunNot(unsigned a)
{
  a = a ^ 0xffffffff;
  return Norma(a);
}
//---------------------------------------------------------------------------
unsigned CALU::RunOr(unsigned a, unsigned b)
{
  return Norma(a | b);
}
//---------------------------------------------------------------------------
unsigned CALU::RunAnd(unsigned a, unsigned b)
{
  return Norma(a & b);
}
//---------------------------------------------------------------------------
unsigned CALU::RunXor(unsigned a, unsigned b)
{
  return Norma(a ^ b);
}
//---------------------------------------------------------------------------
unsigned CALU::RunCompare(AnsiString command, unsigned a, unsigned b)
{
  int t = (znak)? Norma(-1) : Norma(0x7fffffff);
  int f = Norma(0);
  if(!znak)
  {
    if(command == "=")
      return Norma((a == b)? 0xffffffff : 0);
    else if(command == "<>")
      return Norma((a != b)? 0xffffffff : 0);
    else if(command == "<")
      return Norma((b < a)? 0xffffffff : 0);
    else if(command == ">")
      return Norma((b > a)? 0xffffffff : 0);
    else if(command == ">=")
      return Norma((b >= a)? 0xffffffff : 0);
    else if(command == "<=")
      return Norma((b <= a)? 0xffffffff : 0);
    else if(command == "0>")
      return 0xffffffff;
    else if(command == "0<")
      return 0xffffffff;
  //  if(command == "0=")
    return Norma((a == 0)? 0xffffffff : 0);
  }

  if(command == "=")
    return Norma((GetInt(a) == GetInt(b))? 0xffffffff : 0);
  else if(command == "<>")
    return Norma((GetInt(a) != GetInt(b))? 0xffffffff : 0);
  else if(command == "<")
    return Norma((GetInt(b) < GetInt(a))? 0xffffffff : 0);
  else if(command == ">")
    return Norma((GetInt(b) > GetInt(a))? 0xffffffff : 0);
  else if(command == ">=")
    return Norma((GetInt(b) >= GetInt(a))? 0xffffffff : 0);
  else if(command == "<=")
    return Norma((GetInt(b) <= GetInt(a))? 0xffffffff : 0);
  else if(command == "0>")
    return Norma((GetInt(a) > 0)? 0xffffffff : 0);
  else if(command == "0<")
    return Norma((GetInt(a) < 0)? 0xffffffff : 0);
  else if(command == "0=")
    return Norma((GetInt(a) == 0)? 0xffffffff : 0);
  return 0;  
}
//---------------------------------------------------------------------------
unsigned CALU::RunMin(unsigned a, unsigned b)
{
  return(GetInt(a) < GetInt(b))? a : b;
}
//---------------------------------------------------------------------------
unsigned CALU::RunMax(unsigned a, unsigned b)
{
  return(GetInt(a) > GetInt(b))? a : b;
}
//---------------------------------------------------------------------------
unsigned CALU::RunSign(AnsiString command, unsigned a)
{
  if(!znak)
    return a;
  if(command == "ABS")
    return GetHex(abs(GetInt(a)));
  return GetHex(GetInt(a) * -1);
}
//---------------------------------------------------------------------------
unsigned CALU::RunShr(unsigned a)
{
  if(!znak)
    return Norma(Norma(a) / 2);
  return Norma(GetHex(GetInt(a) / 2));
}
//---------------------------------------------------------------------------
unsigned CALU::RunShl(unsigned a)
{
  if(!znak)
    return Norma(Norma(a) * 2);
  return Norma(GetHex(GetInt(a) * 2));
}
//---------------------------------------------------------------------------
unsigned CALU::Norma(unsigned ch)
{
  unsigned maska = 0xFFFFFFFF >> (32 - kol_bit_data);
  return ch & maska;
}
//---------------------------------------------------------------------------
int CALU::GetInt(unsigned ch)
{
  unsigned maska_ch   = (znak)? (0xFFFFFFFF >> (33 - kol_bit_data)) : (0xFFFFFFFF >> (32 - kol_bit_data));
  if(!znak)
    return ch & maska_ch;
  int ret = ch & maska_ch;
  unsigned maska_ch2   = 0xFFFFFFFF << kol_bit_data;
  unsigned maska_znak = 0x80000000 >> (32 - kol_bit_data);
  return (ch & maska_znak)? (ch | maska_ch2) : (ch & maska_ch);
}
//---------------------------------------------------------------------------
unsigned CALU::GetHex(int ch)
{
  if(!znak)
    return abs(ch) & (0xFFFFFFFF >> (32 - kol_bit_data));
  if(ch >= 0)
    return ch & (0xFFFFFFFF >> (33 - kol_bit_data));

  unsigned maska_znak = 0x80000000 >> (32 - kol_bit_data);
  unsigned maska_ch   = 0xFFFFFFFF >> (33 - kol_bit_data);
  unsigned ret = ch & maska_ch;
  return ret | maska_znak;
}
//---------------------------------------------------------------------------
void CALU::PrintVHDLSignals(TMemo *memo)
{
  if(!GetKolCommands())
    return;
  memo->Lines->Add("   signal sc_alu : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("MIN"))
    memo->Lines->Add("   signal sc_alu_min : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("MAX"))
    memo->Lines->Add("   signal sc_alu_max : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("ABS"))
    memo->Lines->Add("   signal sc_alu_abs : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("NEGATE") || (dictionary.GetKolUse("ABS") && znak))
    memo->Lines->Add("   signal sc_alu_negate : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
//  if(dictionary.GetKolUse("=") || dictionary.GetKolUse("<>") || dictionary.GetKolUse("<") || dictionary.GetKolUse(">") ||
//     dictionary.GetKolUse("0>") || dictionary.GetKolUse("0<") || dictionary.GetKolUse("0="))
//    memo->Lines->Add("   signal sc_alu_compare : std_logic_vector(" + IntToStr(kol_bit_data - 1) + " downto 0);");
  if(dictionary.GetKolUse("="))
    memo->Lines->Add("   signal sc_alu_1r : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("<>"))
    memo->Lines->Add("   signal sc_alu_2r : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse(">"))
    memo->Lines->Add("   signal sc_alu_3r : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("<"))
    memo->Lines->Add("   signal sc_alu_4r : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse(">="))
    memo->Lines->Add("   signal sc_alu_8r : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("<="))
    memo->Lines->Add("   signal sc_alu_9r : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("0="))
    memo->Lines->Add("   signal sc_alu_5r : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("0>"))
    memo->Lines->Add("   signal sc_alu_6r : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("0<"))
    memo->Lines->Add("   signal sc_alu_7r : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  if(dictionary.GetKolUse("*"))
    if(znak)
      memo->Lines->Add("   signal sc_alu_mul : std_logic_vector" + RAM.GetDownto(2*(kol_bit_data-1), 0) + ";");
    else
      memo->Lines->Add("   signal sc_alu_mul : std_logic_vector" + RAM.GetDownto(2*kol_bit_data - 1, 0) + ";");
/*  if(dictionary.GetKolUse("/"))
    if(znak)
      memo->Lines->Add("   signal sc_alu_div : std_logic_vector" + RAM.GetDownto(2*(kol_bit_data-1), 0) + ";");
    else
      memo->Lines->Add("   signal sc_alu_div : std_logic_vector" + RAM.GetDownto(2*kol_bit_data - 1, 0) + ";");*/
}
//---------------------------------------------------------------------------
void CALU::PrintVHDLProcess(TMemo *memo)
{
  if(!GetKolCommands())
    return;
  AnsiString Op1 = "sc_stack_data(0)", Op2 = "sc_stack_data(1)";
  if(kol_bit_data != stack_data.GetKolBitDataWhithZnak())
  {
    Op1 += RAM.GetDownto(kol_bit_data - 1, 0);
    Op2 += RAM.GetDownto(kol_bit_data - 1, 0);
  }
  memo->Lines->Add("--� ����� ��������� ������ ���������� ���� ������� ���");
  if(dictionary.GetKolUse("MIN"))
    if(!znak)
      memo->Lines->Add("   sc_alu_min <= " + Op1 + " when " + Op1 + " < " + Op2 + " else " + Op2 + ";");
    else
      memo->Lines->Add("   sc_alu_min <= " + Op1 + " when((not sc_stack_data(0)(" + IntToStr(kol_bit_data-1) + ") & " + Op1 + ") < " +
                                                   " (not sc_stack_data(1)(" + IntToStr(kol_bit_data-1) + ") & " + Op2 + ")) else " + Op2 + ";");
  if(dictionary.GetKolUse("MAX"))
    if(!znak)
      memo->Lines->Add("   sc_alu_max <= " + Op1 + " when " + Op1 + " > " + Op2 + " else " + Op2 + ";");
    else
      memo->Lines->Add("   sc_alu_min <= " + Op1 + " when((not sc_stack_data(0)(" + IntToStr(kol_bit_data-1) + ") & " + Op1 + ") > " +
                                                   " (not sc_stack_data(1)(" + IntToStr(kol_bit_data-1) + ") & " + Op2 + ")) else " + Op2 + ";");
  if(dictionary.GetKolUse("ABS"))
    if(!znak)
      memo->Lines->Add("   sc_alu_abs <= " + Op1 + ";");
    else
      memo->Lines->Add("   with sc_stack_data(0)(" + IntToStr(kol_bit_data - 1) + ") select sc_alu_abs <= " + Op1 + " when '0', sc_alu_negate when others;");
  if(dictionary.GetKolUse("NEGATE") || (dictionary.GetKolUse("ABS") && znak))
    if(!znak)
      memo->Lines->Add("   sc_alu_negate <= " + Op1 + ";");
    else
      memo->Lines->Add("   with sc_stack_data(0) select sc_alu_negate <= "+stack_return.GetChNULL(kol_bit_data)+" when "+stack_return.GetChNULL(kol_bit_data)+", (not sc_stack_data(0)(" + IntToStr(kol_bit_data - 1) + ") & not (sc_stack_data(0)" + RAM.GetDownto(kol_bit_data - 2, 0) + " - 1)) when others;");
   //   memo->Lines->Add("   sc_alu_negate <= not sc_stack_data(0)(" + IntToStr(kol_bit_data - 1) + ") & not (sc_stack_data(0)" + RAM.GetDownto(kol_bit_data - 2, 0) + " - 1);");
  if(dictionary.GetKolUse("*"))
    if(znak)
    {
      memo->Lines->Add("   sc_alu_mul(" + IntToStr(2*(kol_bit_data-1)) + ") <= '0' when (" + Op1 + " = " + stack_return.GetChNULL(kol_bit_data) + " or " + Op2 + " = " + stack_return.GetChNULL(kol_bit_data) +
                       ") else (sc_stack_data(0)(" + IntToStr(kol_bit_data - 1) + ") xor sc_stack_data(1)(" + IntToStr(kol_bit_data - 1) + "));");
      memo->Lines->Add("   sc_alu_mul" + RAM.GetDownto(2*(kol_bit_data-1)-1, 0) + " <= sc_stack_data(0)" + RAM.GetDownto(kol_bit_data - 2, 0) + " * sc_stack_data(1)" + RAM.GetDownto(kol_bit_data - 2, 0) + ";");
    }
    else
      memo->Lines->Add("   sc_alu_mul <= " + Op1 + " * " + Op2 + ";");
 /* if(dictionary.GetKolUse("/"))
    if(znak)
    {
      memo->Lines->Add("   sc_alu_mul(" + IntToStr(2*(kol_bit_data-1)) + ") <= '0' when (" + Op1 + " = " + stack_return.GetChNULL(kol_bit_data) + " or " + Op2 + " = " + stack_return.GetChNULL(kol_bit_data) +
                       ") else (sc_stack_data(0)(" + IntToStr(kol_bit_data - 1) + ") xor sc_stack_data(1)(" + IntToStr(kol_bit_data - 1) + "));");
      memo->Lines->Add("   sc_alu_mul" + RAM.GetDownto(2*(kol_bit_data-1)-1, 0) + " <= sc_stack_data(0)" + RAM.GetDownto(kol_bit_data - 2, 0) + " * sc_stack_data(1)" + RAM.GetDownto(kol_bit_data - 2, 0) + ";");
    }
    else
      memo->Lines->Add("   sc_alu_mul <= " + Op1 + " * " + Op2 + ";");*/
  if(dictionary.GetKolUse("="))
    memo->Lines->Add("   sc_alu_1r <= (others => '1') when " + Op1 + " = " + Op2 + " else (others => '0');");
  if(dictionary.GetKolUse("<>"))
    memo->Lines->Add("   sc_alu_2r <= (others => '1') when " + Op1 + " /= " + Op2 + " else (others => '0');");
  if(dictionary.GetKolUse(">"))
    if(znak)
      memo->Lines->Add("   sc_alu_3r <= (others => '1') when((not sc_stack_data(1)(" + IntToStr(kol_bit_data-1) + ") & " + Op2 + ") > " +
                       " (not sc_stack_data(0)(" + IntToStr(kol_bit_data-1) + ") & " + Op1 + ")) else (others => '0');");
    else
      memo->Lines->Add("   sc_alu_3r <= (others => '1') when " + Op2 + " > " + Op1 + " else (others => '0');");
  if(dictionary.GetKolUse("<"))
    if(znak)
      memo->Lines->Add("   sc_alu_4r <= (others => '1') when((not sc_stack_data(1)(" + IntToStr(kol_bit_data-1) + ") & " + Op2 + ") < " +
                       " (not sc_stack_data(0)(" + IntToStr(kol_bit_data-1) + ") & " + Op1 + ")) else (others => '0');");
    else
      memo->Lines->Add("   sc_alu_4r <= (others => '1') when " + Op2 + " < " + Op1 + " else (others => '0');");
  if(dictionary.GetKolUse(">="))
    if(znak)
      memo->Lines->Add("   sc_alu_8r <= (others => '1') when((not sc_stack_data(1)(" + IntToStr(kol_bit_data-1) + ") & " + Op2 + ") >= " +
                       " (not sc_stack_data(0)(" + IntToStr(kol_bit_data-1) + ") & " + Op1 + ")) else (others => '0');");
    else
      memo->Lines->Add("   sc_alu_8r <= (others => '1') when " + Op2 + " >= " + Op1 + " else (others => '0');");
  if(dictionary.GetKolUse("<="))
    if(znak)
      memo->Lines->Add("   sc_alu_9r <= (others => '1') when((not sc_stack_data(1)(" + IntToStr(kol_bit_data-1) + ") & " + Op2 + ") <= " +
                       " (not sc_stack_data(0)(" + IntToStr(kol_bit_data-1) + ") & " + Op1 + ")) else (others => '0');");
    else
      memo->Lines->Add("   sc_alu_9r <= (others => '1') when " + Op2 + " <= " + Op1 + " else (others => '0');");
  if(dictionary.GetKolUse("0="))
    memo->Lines->Add("   sc_alu_5r <= (others => '1') when " + Op1 + " = " + stack_return.GetChNULL(kol_bit_data) + " else (others => '0');");
  if(dictionary.GetKolUse("0>"))
    if(znak)
      memo->Lines->Add("   sc_alu_6r <= (others => '0') when " + Op1 + " = " + stack_return.GetChNULL(kol_bit_data) + " else" +
                       " (others => not sc_stack_data(0)(" + IntToStr(kol_bit_data - 1) + "));");
    else
      memo->Lines->Add("   sc_alu_6r <= (others => '1') when " + Op1 + " /= " + stack_return.GetChNULL(kol_bit_data) + " else (others => '0');");
  if(dictionary.GetKolUse("0<"))
    if(znak)
      memo->Lines->Add("   sc_alu_7r <= (others => sc_stack_data(0)(" + IntToStr(kol_bit_data - 1) + "));");
    else
      memo->Lines->Add("   sc_alu_7r <= (others => '0');");
  memo->Lines->Add("   with sc_kod1 select sc_alu <=");
  if(dictionary.GetKolUse("+")) memo->Lines->Add("      " + Op2 + " + " + Op1 + " when " + YY.GetCommandKodBin("+") + ",");
  if(dictionary.GetKolUse("-")) memo->Lines->Add("      " + Op2 + " - " + Op1 + " when " + YY.GetCommandKodBin("-") + ",");
  if(dictionary.GetKolUse("*"))
    if(znak)
      memo->Lines->Add("      (sc_alu_mul(" + IntToStr(2*(kol_bit_data-1)) + ") & sc_alu_mul" + RAM.GetDownto(kol_bit_data - 2, 0) + ") when " + YY.GetCommandKodBin("*") + ",");
    else
      memo->Lines->Add("      sc_alu_mul" + RAM.GetDownto(kol_bit_data - 1, 0) + " when " + YY.GetCommandKodBin("*") + ",");
  if(dictionary.GetKolUse("1+")) memo->Lines->Add("      " + Op1 + " + 1 when " + YY.GetCommandKodBin("1+") + ",");
  if(dictionary.GetKolUse("1-")) memo->Lines->Add("      " + Op1 + " - 1 when " + YY.GetCommandKodBin("1-") + ",");
  if(dictionary.GetKolUse("2+")) memo->Lines->Add("      " + Op1 + " + 2 when " + YY.GetCommandKodBin("2+") + ",");
  if(dictionary.GetKolUse("2-")) memo->Lines->Add("      " + Op1 + " - 2 when " + YY.GetCommandKodBin("2-") + ",");
  if(dictionary.GetKolUse("NOT")) memo->Lines->Add("      not " + Op1 + " when " + YY.GetCommandKodBin("NOT") + ",");
  if(dictionary.GetKolUse("AND")) memo->Lines->Add("      " + Op1 + " and " + Op2 + "when " + YY.GetCommandKodBin("AND") + ",");
  if(dictionary.GetKolUse("OR")) memo->Lines->Add("      " + Op1 + " or " + Op2 + "when " + YY.GetCommandKodBin("OR") + ",");
  if(dictionary.GetKolUse("XOR")) memo->Lines->Add("      " + Op1 + " xor " + Op2 + "when " + YY.GetCommandKodBin("XOR") + ",");
  if(dictionary.GetKolUse("MIN")) memo->Lines->Add("      sc_alu_min when " + YY.GetCommandKodBin("MIN") + ",");
  if(dictionary.GetKolUse("MAX")) memo->Lines->Add("      sc_alu_max when " + YY.GetCommandKodBin("MAX") + ",");
  if(dictionary.GetKolUse("2/"))
    if(!znak) memo->Lines->Add("      '0' & sc_stack_data(0)" + RAM.GetDownto(kol_bit_data-1, 1) + " when " + YY.GetCommandKodBin("2/") + ",");
    else memo->Lines->Add("      sc_stack_data(0)(" + IntToStr(kol_bit_data-1) +") & sc_stack_data(0)" + RAM.GetDownto(kol_bit_data-1, 1) + " when " + YY.GetCommandKodBin("2/") + ",");
  if(dictionary.GetKolUse("2*"))
    if(!znak) memo->Lines->Add("      sc_stack_data(0)" + RAM.GetDownto(kol_bit_data-2, 0) + " & '0' when " + YY.GetCommandKodBin("2*") + ",");
    else memo->Lines->Add("      sc_stack_data(0)(" + IntToStr(kol_bit_data-1) +") & sc_stack_data(0)" + RAM.GetDownto(kol_bit_data-3, 0) + " & '0' when " + YY.GetCommandKodBin("2*") + ",");
  if(dictionary.GetKolUse("NEGATE"))
    memo->Lines->Add("      sc_alu_negate when " + YY.GetCommandKodBin("NEGATE") + ",");
  if(dictionary.GetKolUse("ABS"))
    memo->Lines->Add("      sc_alu_abs when " + YY.GetCommandKodBin("ABS") + ",");
/*  if(!dictionary.GetKolUse("ABS") && dictionary.GetKolUse("NEGATE"))
    memo->Lines->Add("      sc_alu_negate when " + YY.GetCommandKodBin("NEGATE") + ",");
  else if(dictionary.GetKolUse("ABS") && !dictionary.GetKolUse("NEGATE"))
    memo->Lines->Add("      sc_alu_abs when " + YY.GetCommandKodBin("ABS") + ",");
  else if(dictionary.GetKolUse("ABS") && dictionary.GetKolUse("NEGATE"))
    memo->Lines->Add("      sc_alu_sign when " + YY.GetCommandKodBin("ABS") + " | " + YY.GetCommandKodBin("NEGATE") + ",");*/
  if(dictionary.GetKolUse("=")) memo->Lines->Add("      sc_alu_1r when " + YY.GetCommandKodBin("=") + ",");
  if(dictionary.GetKolUse("<>")) memo->Lines->Add("      sc_alu_2r when " + YY.GetCommandKodBin("<>") + ",");
  if(dictionary.GetKolUse(">"))  memo->Lines->Add("      sc_alu_3r when " + YY.GetCommandKodBin(">") + ",");
  if(dictionary.GetKolUse("<"))  memo->Lines->Add("      sc_alu_4r when " + YY.GetCommandKodBin("<") + ",");
  if(dictionary.GetKolUse(">="))  memo->Lines->Add("      sc_alu_8r when " + YY.GetCommandKodBin(">=") + ",");
  if(dictionary.GetKolUse("<="))  memo->Lines->Add("      sc_alu_9r when " + YY.GetCommandKodBin("<=") + ",");
  if(dictionary.GetKolUse("0=")) memo->Lines->Add("      sc_alu_5r when " + YY.GetCommandKodBin("0=") + ",");
  if(dictionary.GetKolUse("0>")) memo->Lines->Add("      sc_alu_6r when " + YY.GetCommandKodBin("0>") + ",");
  if(dictionary.GetKolUse("0<")) memo->Lines->Add("      sc_alu_7r when " + YY.GetCommandKodBin("0<") + ",");
  memo->Lines->Add("      (others => '0') when others;");
}
//---------------------------------------------------------------------------
AnsiString CALU::GetStrCompare()
{
  AnsiString ret = "";
  if(dictionary.GetKolUse("="))
    if(ret == "") ret += ("" + YY.GetCommandKodBin("="));
    else ret += (" | " + YY.GetCommandKodBin("="));
  if(dictionary.GetKolUse(">"))
    if(ret == "") ret += ("" + YY.GetCommandKodBin(">"));
    else ret += (" | " + YY.GetCommandKodBin(">"));
  if(dictionary.GetKolUse("<"))
    if(ret == "") ret += ("" + YY.GetCommandKodBin("<"));
    else ret += (" | " + YY.GetCommandKodBin("<"));
  if(dictionary.GetKolUse("<>"))
    if(ret == "") ret += ("" + YY.GetCommandKodBin("<>"));
    else ret += (" | " + YY.GetCommandKodBin("<>"));
  if(dictionary.GetKolUse("0>"))
    if(ret == "") ret += ("" + YY.GetCommandKodBin("0>"));
    else ret += (" | " + YY.GetCommandKodBin("0>"));
  if(dictionary.GetKolUse("0<"))
    if(ret == "") ret += ("" + YY.GetCommandKodBin("0<"));
    else ret += (" | " + YY.GetCommandKodBin("0<"));
  if(dictionary.GetKolUse("0="))
    if(ret == "") ret += ("" + YY.GetCommandKodBin("0="));
    else ret += (" | " + YY.GetCommandKodBin("0="));
  return ret;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
