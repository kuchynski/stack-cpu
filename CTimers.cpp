//---------------------------------------------------------------------------
#pragma hdrstop

#include "CTimers.h"

CTimers timers;
//---------------------------------------------------------------------------
void CTimers::AddTimer(AnsiString n, AnsiString f, unsigned s, AnsiString ir)
{
  modules.push_back(STimer(n, f, s, ir));
}
//---------------------------------------------------------------------------
void CTimers::Reset()
{
  for(vector <STimer>:: iterator p = modules.begin(); p < modules.end(); p ++)
    p->state = false;
}
//---------------------------------------------------------------------------
void CTimers::NewTakt(AnsiString name_frequency)
{
  unsigned ch, ch2;
  for(vector <STimer>:: iterator p = modules.begin(); p < modules.end(); p ++)
    if(p->frequency == name_frequency)
      if(ports.GetDataOut(p->name, ch))
        if(ch)
          if(ports.GetDataIn(p->name, ch))
          {
            ch ++;
            ports.SetDataIn(p->name, ch);
            ports.GetDataIn(p->name, ch);
            ports.GetDataOut(p->name, ch2);
            if(ch == ch2)
            {
              IRQ.SetState(p->irq, true);
              ports.SetDataIn(p->name, 0);
            }
          }
  PrintValue();
}
//---------------------------------------------------------------------------
void CTimers::NewDataPort(AnsiString name_port, unsigned d)
{
  for(vector <STimer>:: iterator p = modules.begin(); p < modules.end(); p ++)
    if(p->name == name_port)
    {
      p->state = (d != 0);
      if(!d)
        ports.SetDataIn(name_port, 0);
    }
}
//---------------------------------------------------------------------------
void CTimers::PrintAll(TStringGrid *sg)
{
  grid = sg;
  grid->RowCount = modules.size() + 1;
  if(grid->RowCount > 1)
    grid->FixedRows = 1;
  for(int i = 1; i < grid->RowCount; i ++)
    grid->Cells[0][i] = modules[i-1].name;
  PrintValue();
}
//---------------------------------------------------------------------------
void CTimers::PrintValue()
{
  if(!grid)
    return;
  for(int i = 1; i < grid->RowCount; i ++)
    grid->Cells[1][i] = modules[i-1].state? "счёт" : "";
}
//---------------------------------------------------------------------------
void CTimers::PrintVHDLSignals(TMemo *memo)
{
  for(vector <STimer>:: iterator p = modules.begin(); p < modules.end(); p ++)
    memo->Lines->Add("   signal sc_" + p->name + "_new, " + p->name + "_irq, " + p->name + "_work : std_logic;");
}
//---------------------------------------------------------------------------
void CTimers::PrintVHDLProcess(TMemo *memo)
{
  if(!modules.size())          //" + p->name + "_work = "+ RAM.GetNumberBin(0, p->size)
    return;
  memo->Lines->Add("--здесь таймеры будут вести свой счёт");
  for(vector <STimer>:: iterator p = modules.begin(); p < modules.end(); p ++)
  {
    memo->Lines->Add("p_timer_" + p->name + "_in: process (sc_rst, sc_" + p->name + "_new, sc_clk_" + p->frequency + ") begin");
    memo->Lines->Add("      if sc_rst = '1' or sc_" + p->name + "_new = '1' then " + p->name + "_in <= " + RAM.GetNumberBin(1, p->size) + ";");
    memo->Lines->Add("      else if sc_clk_" + p->frequency + " = '1' and sc_clk_" + p->frequency + "'event then");
    memo->Lines->Add("         if " + p->name + "_work = '1' or " + p->name + "_irq = '1' then");
    memo->Lines->Add("            " + p->name + "_in <= " + RAM.GetNumberBin(1, p->size) + ";");
    if(p->size > 1)
      memo->Lines->Add("         else " + p->name + "_in <= " + p->name + "_in + 1; end if; end if; end if; end process;");
    else
      memo->Lines->Add("         else " + p->name + "_in <= not " + p->name + "_in; end if; end if; end if; end process;");
  }
  for(vector <STimer>:: iterator p = modules.begin(); p < modules.end(); p ++)
    memo->Lines->Add("   " + p->name + "_work <= '1' when " + p->name + "_out = "+ RAM.GetNumberBin(0, p->size) + " else '0';");
  for(vector <STimer>:: iterator p = modules.begin(); p < modules.end(); p ++)
    memo->Lines->Add("   " + p->name + "_irq <= '1' when " + p->name + "_in = " + p->name + "_out else '0';");
  for(vector <STimer>:: iterator p = modules.begin(); p < modules.end(); p ++)
  {
    memo->Lines->Add("p_timer_new"+p->name+": process (clk, sc_rst) begin");
    memo->Lines->Add("      if sc_rst = '1' then sc_" + p->name + "_new <= '0';");
    memo->Lines->Add("      else if clk = '1' and clk'event then if (sc_address_data =  " + ports.GetAddressBin(p->name) + ") and (sc_vz = '1') then sc_"+ p->name + "_new <= '1';");
    memo->Lines->Add("         else sc_"+ p->name + "_new <= '0'; end if; end if; end if;");
    memo->Lines->Add("   end process;");
  }

  for(vector <STimer>:: iterator p = modules.begin(); p < modules.end(); p ++)
    if(IRQ.IsIRQ("IRQ_" + p->name))
    {
      memo->Lines->Add("p_timer_" + p->name + "_irq: process (sc_rst, IRQ_" + p->name + "(1), " + p->name + "_work, sc_clk_" + p->frequency + ") begin");
      memo->Lines->Add("      if sc_rst = '1' or IRQ_" + p->name + "(1) = '1' or " + p->name + "_work = '1' then IRQ_" + p->name + "(0) <= '0';");
      memo->Lines->Add("      else if sc_clk_" + p->frequency + " = '1' and sc_clk_" + p->frequency + "'event then IRQ_" + p->name + "(0) <= " + p->name + "_irq; end if; end if; end process;");
    }
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
