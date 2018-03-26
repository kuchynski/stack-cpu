//---------------------------------------------------------------------------
#pragma hdrstop

#include "CIRQ.h"
CIRQ IRQ;
//---------------------------------------------------------------------------
void CIRQ::AddIRQ(AnsiString n, int p)
{
  name.push_back(n);
  state.push_back(false);
  run.push_back(false);
  priority.push_back(p);
  address.push_back(0);
}
//---------------------------------------------------------------------------
bool CIRQ::IsIRQ(AnsiString n)
{
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++)
    if(*p == n)
      return true;
  return false;    
}
//---------------------------------------------------------------------------
int CIRQ::GetSizeLow()
{
  int ret = 0;
  for(vector <int>:: iterator p = priority.begin(); p < priority.end(); p ++)
    if(*p > 9)
      ret ++;
  return ret;
}
//---------------------------------------------------------------------------
int CIRQ::GetSizeHigh()
{
  int ret = 0;
  for(vector <int>:: iterator p = priority.begin(); p < priority.end(); p ++)
    if(*p <= 9)
      ret ++;
  return ret;
}
//---------------------------------------------------------------------------
unsigned CIRQ::GetAddress(AnsiString n)
{
  int i = 0;
  bool yes = false;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
    if(*p == n)
    {
      yes = true;
      break;
    }
  if(yes)
    return address[i];
  return false;
}
//---------------------------------------------------------------------------
unsigned CIRQ::SetAddress(AnsiString n, unsigned a)
{
  int i = 0;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
    if(*p == n)
    {
      address[i] = a;
      break;
    }  
}
//---------------------------------------------------------------------------
void CIRQ::DeleteIRQ(AnsiString n)
{
  int i = 0;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
    if(*p == n)
    {
      name.erase(p);
      state.erase(state.begin() + i);
      address.erase(address.begin() + i);
      priority.erase(priority.begin() + i);
      break;
    }
}
//---------------------------------------------------------------------------
bool CIRQ::GetState(AnsiString n)
{
  int i = 0;
  bool yes = false;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
    if(*p == n)
    {
      yes = true;
      break;
    }
  if(yes)
    return state[i];
  return false;
}
//---------------------------------------------------------------------------
void CIRQ::SetState(AnsiString n, bool s)
{
  int i = 0;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
    if((*p == n) && !run[i])
    {
      state[i] = s;
      break;
    }
}
//---------------------------------------------------------------------------
bool CIRQ::GetState(int n)
{
  if(n >= name.size())
    return false;
  return state[n];
}
//---------------------------------------------------------------------------
void CIRQ::SetState(int n, bool s)
{
  if(n >= name.size())
    return;
  state[n] = s;  
}
//---------------------------------------------------------------------------
void CIRQ::PrintAll(TStringGrid *sg)
{
  if(!sg)
    return;
  grid_irq = sg;
  grid_irq->RowCount = name.size() + 1;
  if(grid_irq->RowCount > 1)
    grid_irq->FixedRows = 1;
  for(int i = 1; i < grid_irq->RowCount; i ++)
  {
    grid_irq->Cells[0][i] = name[i-1];
    if(run[i-1])
      grid_irq->Cells[1][i] = "выполн.";
    else
      grid_irq->Cells[1][i] = state[i-1]? "активное" : "";
    grid_irq->Cells[2][i] = IntToStr(priority[i-1]);
  }
}
//---------------------------------------------------------------------------
void CIRQ::PrintAll2(TStringGrid *sg)
{
  if(!sg)
    return;
  sg->RowCount = name.size() + 1;
  if(sg->RowCount > 1)
    sg->FixedRows = 1;
  for(int i = 1; i < sg->RowCount; i ++)
  {
    sg->Cells[0][i] = name[i-1];
    sg->Cells[1][i] = state[i-1]? "активное" : "";
  }
}
//---------------------------------------------------------------------------
void CIRQ::PrintValue()
{
  if(!grid_irq)
    return;
  for(int i = 1; i < grid_irq->RowCount; i ++)
    if(run[i-1])
      grid_irq->Cells[1][i] = "выполн.";
    else
      grid_irq->Cells[1][i] = state[i-1]? "активное" : "";
}
//---------------------------------------------------------------------------
void CIRQ::Run()
{
  vector <AnsiString>:: iterator p = name.begin();
  while(p != name.end())
  {
    unsigned a = ROM.GetAddressFunction(*p);
    if(a != 0)
    {
      SetAddress(*p, a);
      p ++;
    }
    else
    {
      DeleteIRQ(*p);
      p = name.begin();
    }
  }
}
//---------------------------------------------------------------------------
void CIRQ::AddUseToFunctions()
{
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++)
    dictionary.AddUse(*p);
}
//---------------------------------------------------------------------------
int CIRQ::NewIRQ(unsigned &a, bool real)
{
  int i = 0;

  int priority_state = 10000;
  int new_number = -1;
  for(vector <bool>:: iterator p = state.begin(); p < state.end(); p ++, i ++)
    if(*p == true)
      if(priority_state > priority[i])
      {
        priority_state = priority[i];
        new_number = i;
      }
  if(new_number == -1)
    return 0;
  if(GetPriorityRun() > priority[new_number])
  {
    if(real)
    {
      run[new_number] = true;
      state[new_number] = false;
    }
    a = address[new_number];
    PrintValue();
    return new_number+1;
  }
 /*i = 0;
  for(vector <bool>:: iterator p = state.begin(); p < state.end(); p ++, i ++)
    if(*p == true)
    {
      if(GetPriorityRun() > priority[i])
      {
        if(real)
        {
          run[i] = true;
          state[i] = false;
        }
        a = address[i];
        PrintValue();
        return i+1;
      }  
    }       */
  return 0;  
}
//---------------------------------------------------------------------------
int CIRQ::GetPriorityRun()
{
  int i = 0;
  int priority_run = 10000;
  for(vector <bool>:: iterator p = run.begin(); p < run.end(); p ++, i ++)
    if(*p)
      if(priority_run > priority[i])
        priority_run = priority[i];
  return priority_run;
}
//---------------------------------------------------------------------------
void CIRQ::ResetIRQ(unsigned a)
{
  if((a <= run.size()) && a)
    run[a-1] = false;
  PrintValue();
}
//---------------------------------------------------------------------------
void CIRQ::Reset()
{
  for(vector <bool>:: iterator p = state.begin(); p < state.end(); p ++)
    *p = false;
  for(vector <bool>:: iterator p = run.begin(); p < run.end(); p ++)
    *p = false;
}
//---------------------------------------------------------------------------
void CIRQ::PrintVHDLSignals(TMemo *memo)
{
  if(!state.size())
    return;
  AnsiString str = "";
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++)
    if(str == "") str = *p;
    else str += (", " + *p);
    //0 - state0, 1 - state1, 2 - run, 3 - reset
  memo->Lines->Add("   signal " + str + " : std_logic_vector(2 downto 0);");
  if(name.size() > 1) memo->Lines->Add("   signal sc_irq_run_line: std_logic_vector" + RAM.GetDownto(name.size() - 1, 0) + ";");
  else memo->Lines->Add("   signal sc_irq_run_line, sc_irq_run_line_ok : std_logic;");
  if(stack_return.GetKolBitIrq() > 1) memo->Lines->Add("   signal sc_irq_run_number : std_logic_vector" + RAM.GetDownto(stack_return.GetKolBitIrq() - 1, 0) + ";");
  else memo->Lines->Add("   signal sc_irq_run_number : std_logic;");
  memo->Lines->Add("   signal sc_address_irq : std_logic_vector" + RAM.GetDownto(ROM.GetKolBitAddress() - 1, 0) + ";");

}
//---------------------------------------------------------------------------
void CIRQ::PrintVHDLProcess(TMemo *memo)
{
  if(!state.size())
    return;
  int i = 0;
  memo->Lines->Add("--так будут происходить прерывания");
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
  {
    memo->Lines->Add("p_irq_state_"+*p+": process (sc_rst, clk, " + *p + "(2)) begin");
    memo->Lines->Add("      if sc_rst = '1' or " + *p + "(2) = '1' then " + *p + "(1) <= '0';");
    memo->Lines->Add("      else if clk = '1' and clk'event then  if " + *p + "(0) = '1' then " + *p + "(1) <= '1'; end if; end if; end if;");
    memo->Lines->Add("   end process;");
  }
  i = 0;
  memo->Lines->Add("p_irq_run: process (clk) begin");
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
  {
    memo->Lines->Add("      if clk = '1' and clk'event then");
    memo->Lines->Add("         if sc_rst = '1' or (sc_return = '1' and sc_stack_return(0)"
        + RAM.GetDownto(stack_return.GetKolBitData()-1, stack_return.GetKolBitData()-stack_return.GetKolBitIrq()) + " = "
        + RAM.GetNumberBin(i+1,stack_return.GetKolBitIrq())+ ") then " + *p + "(2) <= '0';");
    if(name.size() > 1)
      memo->Lines->Add("         else if sc_irq_run_line(" + IntToStr(i) + ") = '1' then " + *p + "(2) <= '1'; end if; end if; end if;");
    else
      memo->Lines->Add("         else if sc_irq_run_line = '1' then " + *p + "(2) <= '1'; end if; end if; end if;");
  }
  memo->Lines->Add("   end process;");
  i = 0;
  if(ports.IsUse("irq_en_low")) memo->Lines->Add("   irq_en_low_in <= irq_en_low_out;");
  if(ports.IsUse("irq_en")) memo->Lines->Add("   irq_en_in <= irq_en_out;");
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
  {
    AnsiString str = *p + "(1)";
    if(priority[i] > 9)
    {
      if(ports.IsUse("irq_en"))
        str += " and not irq_en_out";
      if(ports.IsUse("irq_en_low"))
        str += " and not irq_en_low_out";
    }
    else if(ports.IsUse("irq_en"))
        str += " and not irq_en_out";
    int j = 0;
    for(vector <AnsiString>:: iterator r = name.begin(); r < name.end(); r ++, j ++)
      if(i != j)
        if((priority[j] < priority[i]) || ((priority[j] == priority[i]) && (j < i)))
          str += (" and not " + *r + "(1)");
    j = 0;
    for(vector <AnsiString>:: iterator r = name.begin(); r < name.end(); r ++, j ++)
      if(i != j)
        if(priority[j] <= priority[i])
          str += (" and not " + *r + "(2)");
    if(name.size() > 1)
      memo->Lines->Add("   sc_irq_run_line(" + IntToStr(i) + ") <= " + str + ";");
    else
      memo->Lines->Add("   sc_irq_run_line <= " + str + ";");
  }
  i = 0;
  memo->Lines->Add("   sc_irq_run_number <= ");
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
    if(name.size() > 1)
      memo->Lines->Add("      " + RAM.GetNumberBin(i+1, stack_return.GetKolBitIrq()) + " when sc_irq_run_line(" + IntToStr(i) + ") = '1' else");
    else
      memo->Lines->Add("      " + RAM.GetNumberBin(i+1, stack_return.GetKolBitIrq()) + " when sc_irq_run_line = '1' else");
  memo->Lines->Add("      " + RAM.GetNumberBin(0, stack_return.GetKolBitIrq()) + ";");
  memo->Lines->Add("   with sc_irq_run_number select sc_address_irq <=");
  i = 0;
  for(vector <AnsiString>:: iterator p = name.begin(); p < name.end(); p ++, i ++)
     memo->Lines->Add("      " + RAM.GetNumberBin(address[i], ROM.GetKolBitAddress()) + " when " + RAM.GetNumberBin(i+1, stack_return.GetKolBitIrq()) + ",");
  memo->Lines->Add("      " + RAM.GetNumberBin(0, ROM.GetKolBitAddress()) + " when others;");
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
