//---------------------------------------------------------------------------
#pragma hdrstop

#include "CUart.h"

CUarts uarts;
//---------------------------------------------------------------------------
void CUarts::AddUart(AnsiString n, AnsiString f, unsigned s,
             AnsiString ir, AnsiString or)
{
  modules.push_back(SUart(n, f, s, ir, or));
}
//---------------------------------------------------------------------------
void CUarts::Reset()
{
  for(vector <SUart>:: iterator p = modules.begin(); p < modules.end(); p ++)
    p->state_out = -1;
}
//---------------------------------------------------------------------------
void CUarts::NewTakt(AnsiString name_frequency)
{
  bool yes = false;
  for(vector <SUart>:: iterator p = modules.begin(); p < modules.end(); p ++)
    if((p->frequency + "4") == name_frequency)
    {
      if(p->state_out >= 0)
      {
        if(p->state_out == (p->size + 1))
        {
          p->state_out = -1;//закончили передачу
          IRQ.SetState(p->port_irq_out, true);
        }
        else
          p->state_out ++;
        yes = true;  
      }
    }
  if(yes)
    PrintValue();
}
//---------------------------------------------------------------------------
void CUarts::NewDataPort(AnsiString name_port)
{
  for(vector <SUart>:: iterator p = modules.begin(); p < modules.end(); p ++)
    if(p->name == name_port)
      p->state_out = 0;
}
//---------------------------------------------------------------------------
void CUarts::PrintAll(TStringGrid *sg)
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
void CUarts::PrintValue()
{
  if(!grid)
    return;
  for(int i = 1; i < grid->RowCount; i ++)
    if(modules[i-1].state_out >= 0)
      grid->Cells[1][i] = "передача";
    else
      grid->Cells[1][i] = "";
}
//---------------------------------------------------------------------------
void CUarts::PrintVHDLPort(TMemo *memo)
{
  AnsiString str1 = "", str2 = "";
  for(vector <SUart>:: iterator p = modules.begin(); p < modules.end(); p ++)
  {
    if(p->port_irq_in != "")
      if(str1 == "") str1 = p->name + "_rx";
      else str1 += (", " + p->name + "_rx");
    if(p->port_irq_out != "")
      if(str2 == "") str2 = p->name + "_tx";
      else str2 += (", " + p->name + "_tx");
  }
  if(str1 != "")
    memo->Lines->Add("           " + str1 + " : in std_logic;");
  if(str2 != "")
    memo->Lines->Add("           " + str2 + " : out std_logic;");
  is_in = (str1 != "");
  is_out = (str2 != "");
}
//---------------------------------------------------------------------------
void CUarts::PrintVHDLSignals(TMemo *memo)
{
  AnsiString str1 = "", str2 = "";
  for(vector <SUart>:: iterator p = modules.begin(); p < modules.end(); p ++)
  {
    if(p->port_irq_in != "")
      if(str1 == "") str1 = p->name + "_work_in, " + p->name + "_in_irq";
      else str1 += (", " + p->name + "_work_in, " + p->name + "_in_irq");
    if(p->port_irq_out != "")
      if(str1 == "") str1 = p->name + "_work_out, " + p->name + "_out_irq";
      else str1 += (", " + p->name + "_work_out, " + p->name + "_out_irq");
    if(p->port_irq_in != "")
      if(str2 == "") str2 = p->name + "_internal_clk_in";
      else str2 += (", " + p->name + "_internal_clk_in");
    AnsiString str3 = "";
    if(p->port_irq_in != "")
      if(str3 == "") str3 = p->name + "_st_work_in";
      else str3 += (", " + p->name + "_st_work_in");
    if(p->port_irq_out != "")
      if(str3 == "") str3 = p->name + "_st_work_out";
      else str3 += (", " + p->name + "_st_work_out");
    if(str3 != "")
      memo->Lines->Add("   signal " + str3 + " : std_logic_vector" + RAM.GetDownto(div_frequency.GetSizeSt(p->size+2)-1, 0) + ";");
  }
  if(str1 != "")
    memo->Lines->Add("   signal " + str1 + " : std_logic;");
  if(str2 != "")
    memo->Lines->Add("   signal " + str2 + " : std_logic_vector(1 downto 0);");
}
//---------------------------------------------------------------------------
void CUarts::PrintVHDLProcess(TMemo *memo)
{
  if(!modules.size())
    return;
  if(is_out)
  {
    memo->Lines->Add("--внимание UART передаёт данные!");
    for(vector <SUart>:: iterator p = modules.begin(); p < modules.end(); p ++)
    {
      if(p->port_irq_out == "")
        continue;
      int size_st = div_frequency.GetSizeSt(p->size+2);
      memo->Lines->Add("p_uart_" + p->name + "_out: process (sc_rst, clk, " + p->name + "_out_irq, sc_clk_" + p->frequency + "4) begin");
      memo->Lines->Add("      if sc_rst = '1' or " + p->name + "_out_irq = '1' then " + p->name + "_work_out <= '0';");
      memo->Lines->Add("      else if clk = '1' and clk'event then if (sc_address_data =  " + ports.GetAddressBin(p->name) + ") and (sc_vz = '1') then " + p->name + "_work_out <= '1'; end if; end if; end if;");

      memo->Lines->Add("      if sc_rst = '1' then " + p->name + "_st_work_out <= (others => '0');");
      memo->Lines->Add("      else if sc_clk_" + p->frequency + "4 = '1' and sc_clk_" + p->frequency + "4'event then");
      memo->Lines->Add("         if " + p->name + "_work_out = '0' then " + p->name + "_st_work_out <= (others => '0'); else " + p->name + "_st_work_out <= " + p->name + "_st_work_out + 1; end if; end if; end if;");

      memo->Lines->Add("      if sc_rst = '1' then " + p->name + "_out_irq <= '0';");
      memo->Lines->Add("      else if sc_clk_" + p->frequency + "4 = '1' and sc_clk_" + p->frequency + "4'event then");
      memo->Lines->Add("         if " + p->name + "_st_work_out = " + RAM.GetNumberBin(p->size + 2, size_st) + " then " + p->name + "_out_irq <= '1'; else " + p->name + "_out_irq <= '0'; end if; end if; end if;");
      memo->Lines->Add("   end process;");

      memo->Lines->Add("   with " + p->name + "_st_work_out select " + p->name + "_tx <= '0' when " + RAM.GetNumberBin(1, size_st) + ",");
      AnsiString str = "";
      for(int i = 0; i < p->size; i++)
        str += (p->name + "_out(" + IntToStr(i) + ") when " + RAM.GetNumberBin(i+2, size_st) + ", ");
      memo->Lines->Add("      " + str + " '1' when others;");

//      memo->Lines->Add("   " + p->name + "_out_irq <= '1' when " + p->name + "_st_work_out = " + RAM.GetNumberBin(p->size + 2, size_st) + " else '0';");
      if(IRQ.IsIRQ("IRQ_" + p->name + "_out"))
      {
        memo->Lines->Add("p_uart_" + p->name + "_out_irq: process (sc_rst, IRQ_" + p->name + "_out(1), " + p->name + "_out_irq) begin");
        memo->Lines->Add("      if sc_rst = '1' or IRQ_" + p->name + "_out(1) = '1' then IRQ_" + p->name + "_out(0) <= '0';");
        memo->Lines->Add("      else if " + p->name + "_out_irq = '0' and " + p->name + "_out_irq'event then IRQ_" + p->name + "_out(0) <= '1'; end if; end if; end process;");
      }  
    }
  }

  if(is_in)
  {
    memo->Lines->Add("--приём данных UART-ом");
    for(vector <SUart>:: iterator p = modules.begin(); p < modules.end(); p ++)
    {
      if(p->port_irq_in == "")
        continue;

      int size_st = div_frequency.GetSizeSt(p->size+2);
//      memo->Lines->Add("p_uart_" + p->name + "_in0: process (sc_rst, " + p->name + "_rx, " + p->name + "_in_irq) begin");
  //    memo->Lines->Add("      if sc_rst = '1' or " + p->name + "_in_irq = '1' then " + p->name + "_work_in <= '0';");
    //  memo->Lines->Add("      else if " + p->name + "_rx = '0' and " + p->name + "_rx'event then " + p->name + "_work_in <= '1'; end if; end if;");
      memo->Lines->Add("p_uart_" + p->name + "_in0: process (sc_rst, sc_clk_" + p->frequency + ", " + p->name + "_in_irq) begin");
      memo->Lines->Add("      if sc_rst = '1' or " + p->name + "_in_irq = '1' then " + p->name + "_work_in <= '0';");
      memo->Lines->Add("      else if sc_clk_" + p->frequency + " = '1' and sc_clk_" + p->frequency + "'event then ");
      memo->Lines->Add("         if " + p->name + "_rx = '0' then " + p->name + "_work_in <= '1'; end if; end if; end if;");
      memo->Lines->Add("   end process;");

      memo->Lines->Add("p_uart_" + p->name + "_in1: process (" + p->name + "_work_in, sc_clk_" + p->frequency + ") begin");
      memo->Lines->Add("      if " + p->name + "_work_in = '0' then " + p->name + "_internal_clk_in <= \"00\";");
      memo->Lines->Add("      else if sc_clk_" + p->frequency + " = '1' and sc_clk_" + p->frequency + "'event then " + p->name + "_internal_clk_in <= " + p->name + "_internal_clk_in + 1; end if; end if;");
      memo->Lines->Add("   end process;");

      memo->Lines->Add("p_uart_" + p->name + "_in2: process (" + p->name + "_work_in, " + p->name + "_internal_clk_in(1)) begin");
      memo->Lines->Add("      if " + p->name + "_work_in = '0' then " + p->name + "_st_work_in <= (others => '0');");
      memo->Lines->Add("      else if " + p->name + "_internal_clk_in(1) = '1' and " + p->name + "_internal_clk_in(1)'event then " + p->name + "_st_work_in <= " + p->name + "_st_work_in + 1; end if; end if;");
      memo->Lines->Add("   end process;");

      memo->Lines->Add("p_uart_" + p->name + "_in3: process (sc_rst, " + p->name + "_internal_clk_in(1)) begin");
      memo->Lines->Add("      if sc_rst = '1' then " + p->name + "_in <= (others => '0');");
      memo->Lines->Add("      else if " + p->name + "_internal_clk_in(1) = '1' and " + p->name + "_internal_clk_in(1)'event then");
//      memo->Lines->Add("         if " + p->name + "_work_in = '1' and " + p->name + "_st_work_in < " + RAM.GetNumberBin(p->size + 1, size_st) + " then");
      memo->Lines->Add("         if " + p->name + "_work_in = '1'" + " then");
      memo->Lines->Add("         " + p->name + "_in <= " + p->name + "_rx & " + p->name + "_in" + RAM.GetDownto(p->size-1, 1) + "; end if; end if; end if;");
      memo->Lines->Add("   end process;");

      memo->Lines->Add("p_uart_" + p->name + "_in4: process (sc_rst, sc_clk_" + p->frequency + ") begin");
      memo->Lines->Add("      if sc_rst = '1' then " + p->name + "_in_irq <= '0';");
      memo->Lines->Add("      else if sc_clk_" + p->frequency + " = '1' and sc_clk_" + p->frequency + "'event then");
      memo->Lines->Add("         if " + p->name + "_st_work_in = " + RAM.GetNumberBin(p->size + 1, size_st) + " then " + p->name + "_in_irq <= '1';");
      memo->Lines->Add("         else " + p->name + "_in_irq <= '0'; end if; end if; end if;");
      memo->Lines->Add("   end process;");

//      memo->Lines->Add("   " + p->name + "_in_irq <= '1' when " + p->name + "_st_work_in = " + RAM.GetNumberBin(p->size + 2, size_st) + " else '0';");
      if(IRQ.IsIRQ("IRQ_" + p->name + "_in"))
      {
        memo->Lines->Add("p_uart_" + p->name + "_in_irq: process (sc_rst, IRQ_" + p->name + "_in(1), " + p->name + "_in_irq) begin");
        memo->Lines->Add("      if sc_rst = '1' or IRQ_" + p->name + "_in(1) = '1' then IRQ_" + p->name + "_in(0) <= '0';");
        memo->Lines->Add("      else if " + p->name + "_in_irq = '1' and " + p->name + "_in_irq'event then IRQ_" + p->name + "_in(0) <= '1'; end if; end if; end process;");
      }
    }
  }
}
/*      memo->Lines->Add("p_uart_" + p->name + "_in4: process (" + p->name + "_work_in, " + p->name + "_internal_clk_in(1)) begin");
      memo->Lines->Add("      if " + p->name + "_work_in = '0' then " + p->name + "_in_irq <= '0';");
      memo->Lines->Add("      else if " + p->name + "_internal_clk_in(1) = '1' and " + p->name + "_internal_clk_in(1)'event then");
      memo->Lines->Add("         if " + p->name + "_st_work_in = " + RAM.GetNumberBin(p->size + 1, size_st) + " then " + p->name + "_in_irq <= '1'; end if; end if; end if;");
      memo->Lines->Add("   end process;");*/
//---------------------------------------------------------------------------
#pragma package(smart_init)
