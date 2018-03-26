//---------------------------------------------------------------------------
#pragma hdrstop

#include "CDiv.h"

CDiv div_frequency;
//---------------------------------------------------------------------------
bool CDiv::AddDiv(AnsiString n, int k, bool e)
{
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++)
    if(*p == n)
      return false;
  name.push_back(n);
  del.push_back(k);
  real_del.push_back(k);
  exit.push_back(e);
  father.push_back(-1);
  return true;
}
//---------------------------------------------------------------------------
bool CDiv::Add4Div(AnsiString n)
{
  int i = 0;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i++)
    if(*p == n)
    {
      AddDiv(n + "4", 4*del[i], false);
      return true;
    }
  return false;
}
//---------------------------------------------------------------------------
void CDiv::RunLinker()
{
  //сортировка
  for(int i = 0; i < del.size(); i ++)
    for(int j = i+1; j < del.size(); j ++)
      if(del[i] > del[j])
      {
        int tmp_del = del[i];
        del[i] = del[j];
        del[j] = tmp_del;
        tmp_del = real_del[i];
        real_del[i] = real_del[j];
        real_del[j] = tmp_del;
        bool tmp_exit = exit[i];
        exit[i] = exit[j];
        exit[j] = tmp_exit;
        AnsiString tmp_name = name[i];
        name[i] = name[j];
        name[j] = tmp_name;
      }
  for(int i = del.size() - 1; i > 0; i --)
    for(int j = i-1; j >= 0; j --)
    {
      int q = del[i], q2= del[j];
      if(!(del[i] % del[j]))
      {
        del[i] /= del[j];
        int q3 = del[i];
        father[i] = j;
        break;
      }
    }
  st = 0;
}
//---------------------------------------------------------------------------
void CDiv::NewTakt()
{
  st ++;
  int i = 0;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
    if(!(st % real_del[i]))// == real_del[i]/2)
    {
      uarts.NewTakt(*p);
      timers.NewTakt(*p);
    }
}
//---------------------------------------------------------------------------
bool CDiv::IsFrequency(AnsiString f)
{
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++)
    if(*p == f)
      return true;
  return false;
}
//---------------------------------------------------------------------------
void CDiv::PrintVHDLPort(TMemo *memo)
{
  int i = 0;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
    if(exit[i])
      memo->Lines->Add("           clk_" + *p + " : out std_logic;");
}
//---------------------------------------------------------------------------
void CDiv::PrintVHDLSignals(TMemo *memo)
{
  int i = 0;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i++)
    if(del[i] >= 2)
    {
      memo->Lines->Add("   signal sc_st_" + *p + " : std_logic_vector" + RAM.GetDownto(GetSizeSt(del[i]/2+del[i]%2-1) - 1, 0) + ";");
      if(del[i]%2)
        memo->Lines->Add("   signal sc_clk_" + *p + ", sc_clk_" + *p + "_real : std_logic;");
     else
        memo->Lines->Add("   signal sc_clk_" + *p + " : std_logic;");
    }
    else
        memo->Lines->Add("   signal sc_clk_" + *p + " : std_logic;");
}
//---------------------------------------------------------------------------
void CDiv::PrintVHDLProcess(TMemo *memo)
{
  if(!name.size())
    return;
  memo->Lines->Add("--делим частоту для внутренних и внешних потребностей");
  int i = 0;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i++)
    if(exit[i])
      memo->Lines->Add("   clk_" + *p + " <= sc_clk_" + *p + ";");
  i = 0;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
  {
    AnsiString f;
    if(father[i] >= 0)
      f = "sc_clk_" + name[father[i]];
    else
      f = "clk";
    if(del[i] < 2)
      memo->Lines->Add("   sc_clk_" + *p + " <= " + f + ";");
    else if(del[i] == 2)
    {
      memo->Lines->Add("p_clk_" + *p + ": process (" + f + ", sc_rst) begin");
      memo->Lines->Add("      if sc_rst = '1' then sc_clk_" + *p + " <= '0'; else");
      memo->Lines->Add("      if " + f + " = '1' and " + f + "'event then sc_clk_" + *p + " <= not sc_clk_" + *p + "; end if; end if;");
      memo->Lines->Add("   end process;");
    }  
    else
    {
      if(!(del[i]%2))
      {
        memo->Lines->Add("p_st_" + *p + ": process (" + f + ", sc_rst) begin");
        memo->Lines->Add("      if sc_rst = '1' then sc_st_" + *p + " <= " + RAM.GetNumberBin(0, GetSizeSt(del[i]/2-1)) + "; sc_clk_" + *p + " <= '0';");
        memo->Lines->Add("      else if "+f+" = '1' and "+f+"'event then");
        memo->Lines->Add("         if sc_st_" + *p + " = " + RAM.GetNumberBin(del[i]/2-1, GetSizeSt(del[i]/2-1)) + " then sc_st_" + *p + " <= " + RAM.GetNumberBin(0, GetSizeSt(del[i]/2-1)) + "; sc_clk_" + *p + " <= not sc_clk_" + *p + "; else sc_st_" + *p + " <= sc_st_" + *p + " + 1; end if; end if; end if;");
        memo->Lines->Add("   end process;");
      }
      else
      {
        memo->Lines->Add("p_st_" + *p + ": process (sc_clk_" + *p + "_real, sc_rst) begin");
        memo->Lines->Add("      if sc_rst = '1' then sc_st_" + *p + " <= " + RAM.GetNumberBin(0, GetSizeSt(del[i]/2)) + "; sc_clk_" + *p + " <= '0';");
        memo->Lines->Add("      else if sc_clk_" + *p + "_real = '1' and sc_clk_" + *p + "_real'event then");
        memo->Lines->Add("         if sc_st_" + *p + " = " + RAM.GetNumberBin(del[i]/2, GetSizeSt(del[i]/2)) + " then sc_st_" + *p + " <= " + RAM.GetNumberBin(0, GetSizeSt(del[i]/2)) + "; sc_clk_" + *p + " <= not sc_clk_" + *p + "; else sc_st_" + *p + " <= sc_st_" + *p + " + 1; end if; end if; end if;");
        memo->Lines->Add("   end process;");
        memo->Lines->Add("   sc_clk_" + *p + "_real <= sc_clk_" + *p + " xor "+f+";");
//        memo->Lines->Add("   sc_clk_" + *p + "_real <= (sc_clk_" + *p + " and not "+f+") or (not sc_clk_" + *p + " and "+f+");");
//        memo->Lines->Add("   with sc_st_" + *p + " select sc_clk_" + *p + "_e <= '1' when " + RAM.GetNumberBin(del[i]/2 + 1, GetSizeSt(del[i]/2)) + ", '0' when others;");
      }
  //    memo->Lines->Add("p_clk_" + *p + ": process (sc_clk_" + *p + "_e, sc_rst) begin");
  //    memo->Lines->Add("      if sc_rst = '1' then sc_clk_" + *p + " <= '0'; else");
  //    memo->Lines->Add("      if sc_clk_" + *p + "_e = '1' and sc_clk_" + *p + "_e'event then sc_clk_" + *p + " <= not sc_clk_" + *p + "; end if; end if;");
  //    memo->Lines->Add("   end process;");
    }  
  }
}
//---------------------------------------------------------------------------
int CDiv::GetSizeSt(int w)
{
  for(int i = 2; i < 100; i ++)
    if(w < IntPower(2, i))
      return i;
  return 2;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
