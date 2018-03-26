//---------------------------------------------------------------------------
#pragma hdrstop

#include "CDelit.h"

CDelits delits;
//---------------------------------------------------------------------------
void CDelits::AddDelit(AnsiString n)
{
  modules.push_back(SDelit(n));
}
//---------------------------------------------------------------------------
void CDelits::Reset()
{
  for(vector <SDelit>:: iterator p = modules.begin(); p < modules.end(); p ++)
    p->state = false;
}
//---------------------------------------------------------------------------
void CDelits::NewTakt(AnsiString name_frequency)
{
/*  unsigned ch, ch2;
  for(vector <SDelit>:: iterator p = modules.begin(); p < modules.end(); p ++)
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
  PrintValue();*/
}
//---------------------------------------------------------------------------
void CDelits::NewDataPort(AnsiString name_port, unsigned d)
{
/*  for(vector <SDelit>:: iterator p = modules.begin(); p < modules.end(); p ++)
    if(p->name == name_port)
    {
      p->state = (d != 0);
      if(!d)
        ports.SetDataIn(name_port, 0);
    }*/
}
//---------------------------------------------------------------------------
void CDelits::PrintAll(TStringGrid *sg)
{
/*  grid = sg;
  grid->RowCount = modules.size() + 1;
  if(grid->RowCount > 1)
    grid->FixedRows = 1;
  for(int i = 1; i < grid->RowCount; i ++)
    grid->Cells[0][i] = modules[i-1].name;
  PrintValue();*/
}
//---------------------------------------------------------------------------
void CDelits::PrintValue()
{
 /* if(!grid)
    return;
  for(int i = 1; i < grid->RowCount; i ++)
    grid->Cells[1][i] = modules[i-1].state? "счёт" : "";*/
}
//---------------------------------------------------------------------------
void CDelits::PrintVHDLSignals(TMemo *memo)
{
  for(vector <SDelit>:: iterator p = modules.begin(); p < modules.end(); p ++)
  {
    int size = (options_proc.GetZnakData())? (options_proc.GetaRazradData()-1) : options_proc.GetaRazradData();
    memo->Lines->Add("   signal "+p->name + "_i3_pr : std_logic_vector" + RAM.GetDownto(options_proc.GetaRazradData()-1, 0) + ";");
    memo->Lines->Add("   signal "+p->name + "_i1_pr, "+p->name+"_i2_pr : std_logic_vector" + RAM.GetDownto(size-1, 0) + ";");
    memo->Lines->Add("   signal "+p->name + "_de, "+p->name+"_res, "+p->name+"_dt : std_logic_vector" + RAM.GetDownto((size-1)*2, 0) + ";");
    memo->Lines->Add("   signal "+p->name + "_work, "+p->name+"_run, " + p->name + "_bm, " + p->name + "_end_work : std_logic;");
    memo->Lines->Add("   signal "+p->name + "_st : std_logic_vector" + RAM.GetDownto(div_frequency.GetSizeSt(size), 0) + ";");
  }
}
//---------------------------------------------------------------------------
void CDelits::PrintVHDLProcess(TMemo *memo)
{
  if(!modules.size())          //" + p->name + "_work = "+ RAM.GetNumberBin(0, p->size)
    return;
  memo->Lines->Add("--Del");

  for(vector <SDelit>:: iterator p = modules.begin(); p < modules.end(); p ++)
  {
    int size = (options_proc.GetZnakData())? (options_proc.GetaRazradData()-1) : options_proc.GetaRazradData();
    if(options_proc.GetZnakData())
    {
      memo->Lines->Add("  with "+p->name+"1_out("+IntToStr(size)+") select "+p->name+"_i1_pr <= "+p->name+"1_out" + RAM.GetDownto(size-1, 0) +
        " when '0', ((not "+p->name+"1_out" + RAM.GetDownto(size-1, 0) +") + 1) when others;");
      memo->Lines->Add("  with "+p->name+"2_out("+IntToStr(size)+") select "+p->name+"_i2_pr <= "+p->name+"2_out" + RAM.GetDownto(size-1, 0) +
        " when '0', ((not "+p->name+"2_out" + RAM.GetDownto(size-1, 0) +") + 1) when others;");
    }
    else
    {
      memo->Lines->Add("  "+p->name+"_i1_pr <= "+p->name+"1_out;");
      memo->Lines->Add("  "+p->name+"_i2_pr <= "+p->name+"2_out;");
    }

    memo->Lines->Add("p_"+p->name+"run: process(sc_rst, "+p->name+"_end_work, "+p->name+"2_out_irq) begin");
    memo->Lines->Add("      if sc_rst = '1' or "+p->name+"_end_work = '1' then "+p->name+"_run <= '0';");
    memo->Lines->Add("      else if "+p->name+"2_out_irq = '1' and "+p->name+"2_out_irq'event then "+ p->name + "_run <= '1';");
    memo->Lines->Add("        end if; end if;");
    memo->Lines->Add("   end process;");
    memo->Lines->Add("p_"+p->name+"work: process("+p->name+"_run, clk) begin");
    memo->Lines->Add("      if "+p->name+"_run = '0' then "+p->name+"_work <= '0';");
    memo->Lines->Add("      else if clk = '1' and clk'event then "+p->name+"_work <= '1'; end if; end if;");
    memo->Lines->Add("   end process;");
    memo->Lines->Add("p_"+p->name+"end_work: process(sc_rst, clk) begin");
    memo->Lines->Add("      if sc_rst = '1' then "+p->name+"_end_work <= '0';");
    memo->Lines->Add("      else if clk = '1' and clk'event then ");
    memo->Lines->Add("        if "+p->name+"_st = "+RAM.GetNumberBin(size, div_frequency.GetSizeSt(size))+" then "+p->name+"_end_work <= '1';");
    memo->Lines->Add("        else "+p->name+"_end_work <= '0'; end if; end if;end if;");
    memo->Lines->Add("   end process;");
    memo->Lines->Add("p_"+p->name+"st: process("+p->name+"_work, clk) begin");
    memo->Lines->Add("      if "+p->name+"_work = '0' then "+p->name+"_st <= (others => '0');");
    memo->Lines->Add("      else if clk = '1' and clk'event then "+p->name+"_st <= "+p->name+"_st + 1; end if; end if;");
    memo->Lines->Add("   end process;");
    memo->Lines->Add("p_"+p->name+"de: process(sc_rst, clk) begin");
    memo->Lines->Add("      if sc_rst = '1' then "+p->name+"_de <= (others => '0');");
    memo->Lines->Add("      else if clk = '1' and clk'event then if "+p->name+"_st = "+RAM.GetNumberBin(0, div_frequency.GetSizeSt(size))+" then");
    memo->Lines->Add("        "+p->name+"_de <= "+RAM.GetNumberBin(0, (size-1))+" & "+p->name + "_i1_pr;");
    memo->Lines->Add("        else "+p->name+"_de <= "+p->name+"_res; end if; end if; end if;");
    memo->Lines->Add("   end process;");
    memo->Lines->Add("p_"+p->name+"res: process("+p->name + "_de, "+p->name+"_bm, "+p->name+"_dt) begin");
    memo->Lines->Add("      if "+p->name + "_de < "+p->name + "_dt then "+p->name+"_bm <= '0'; else "+p->name+"_bm <= '1'; end if;");
    memo->Lines->Add("      if "+p->name + "_bm = '0' then "+p->name+"_res <= "+p->name+"_de; else "+p->name+"_res <= "+p->name+"_de - "+p->name+"_dt; end if;");
    memo->Lines->Add("   end process;");
    memo->Lines->Add("p_"+p->name+"dt: process(sc_rst, clk) begin");
    memo->Lines->Add("      if sc_rst = '1' then "+p->name+"_dt <= (others => '0');");
    memo->Lines->Add("      else if clk = '1' and clk'event then if "+p->name+"_st = "+RAM.GetNumberBin(0, div_frequency.GetSizeSt(size))+" then");
    memo->Lines->Add("        "+p->name+"_dt <=  "+p->name + "_i2_pr & "+RAM.GetNumberBin(0, (size-1))+";");
    memo->Lines->Add("        else "+p->name+"_dt <= '0' & "+p->name+"_dt" + RAM.GetDownto((size-1)*2, 1) + "; end if; end if; end if;");
    memo->Lines->Add("   end process;");
    memo->Lines->Add("p_"+p->name+"o: process(sc_rst, clk) begin");
    memo->Lines->Add("      if sc_rst = '1' then "+p->name+"_i3_pr" + RAM.GetDownto(size-1, 0) + " <= (others => '0');");
    memo->Lines->Add("      else if clk = '1' and clk'event then if "+p->name+"_work = '1' then");
    memo->Lines->Add("        "+p->name+"_i3_pr" + RAM.GetDownto(size-1, 0) + " <= "+p->name+"_i3_pr" + RAM.GetDownto(size-2, 0) + " & "+p->name+"_bm; end if; end if; end if;");
    memo->Lines->Add("   end process;");
    if(options_proc.GetZnakData())
    {
      memo->Lines->Add("   "+p->name+"_i3_pr("+IntToStr(size)+") <= "+p->name+"1_out("+IntToStr(size)+") xor "+p->name+"2_out("+IntToStr(size)+");");
      memo->Lines->Add("   with "+p->name+"_i3_pr("+IntToStr(size)+") select "+p->name+"_in <= "
        +p->name+"_i3_pr when '0', ("+p->name+"_i3_pr("+IntToStr(size)+") & (not "+p->name+"_i3_pr"+RAM.GetDownto(size-1, 0)+") + 1) when others;");
    }
    else
      memo->Lines->Add("  "+p->name+"_in <= "+p->name+"_i3_pr;");
  }
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
