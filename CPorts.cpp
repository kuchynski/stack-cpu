//---------------------------------------------------------------------------
#pragma hdrstop

#include "CPorts.h"
CPorts ports;
//---------------------------------------------------------------------------
void CPorts::AddData(CPort d)
{
  data.push_back(d);
}
//---------------------------------------------------------------------------
void CPorts::DeletePort(AnsiString name)
{
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
    if(name == p->GetName())
    {
      data.erase(p);
      return;
    }
}
//---------------------------------------------------------------------------
void CPorts::AddUse(AnsiString name)
{
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
    if(name == p->GetName())
      p->AddUse();
}
//---------------------------------------------------------------------------
bool CPorts::IsUse(AnsiString name)
{
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
    if(name == p->GetName())
      return p->GetUse() > 0;
  return false;    
}
//---------------------------------------------------------------------------
unsigned CPorts::GetData(unsigned address)
{
  if(address < sm_address)
    return 0;
  unsigned a = address - sm_address;
  unsigned ch = (a < data.size())? data[a].GetIn() : 0;
  return ALU.Norma(ch);
}
//---------------------------------------------------------------------------
void CPorts::SetData(unsigned address, unsigned d)
{
  if(address < sm_address)
    return;
  unsigned a = address - sm_address;
  if(a >= data.size())
    return;
  data[a].SetOut(d);
  PrintValue();
  uarts.NewDataPort(data[a].GetName());
  timers.NewDataPort(data[a].GetName(), data[a].GetOut());
}
//---------------------------------------------------------------------------
void CPorts::SetDataIn(unsigned address, unsigned d)
{
  if(address < sm_address)
    return;
  unsigned a = address - sm_address;
  if(a >= data.size())
    return;
  data[a].SetIn(d);
  PrintValue();
}
//---------------------------------------------------------------------------
bool CPorts::GetDataIn(AnsiString name, unsigned &ch)
{
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
    if(name == p->GetName())
    {
      ch = p->GetIn();
      return true;
    }
  return false;
}
//---------------------------------------------------------------------------
bool CPorts::GetDataOut(AnsiString name, unsigned &ch)
{
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
    if(name == p->GetName())
    {
      ch = p->GetOut();
      return true;
    }
  return false;
}
//---------------------------------------------------------------------------
void CPorts::SetDataIn(AnsiString name, unsigned ch)
{
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
    if(name == p->GetName())
    {
      p->SetIn(ch);
      PrintValue();
      break;
    }
}
//---------------------------------------------------------------------------
void CPorts::Reset()
{
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
    p->Reset();
  PrintValue();
}
//---------------------------------------------------------------------------
AnsiString CPorts::GetDataName(unsigned address)
{
}
//---------------------------------------------------------------------------
void CPorts::PrintAll(TStringGrid *sg)
{
  grid_debug_ports = sg;
  grid_debug_ports->RowCount = data.size() + 1;
  if(grid_debug_ports->RowCount > 1)
    grid_debug_ports->FixedRows = 1;
  int kol_hex = RAM.GetKolBitAddress()/4 + ((RAM.GetKolBitAddress()%4)? 1 : 0);
  for(int i = 1; i < grid_debug_ports->RowCount; i ++)
  {
    grid_debug_ports->Cells[0][i] = IntToHex((__int64)(i-1 + sm_address), kol_hex);
    grid_debug_ports->Cells[1][i] = data[i-1].GetName();
  }
  PrintValue();
}
//---------------------------------------------------------------------------
void CPorts::PrintAllInputs(TStringGrid *sg)
{
  int size_inputs = 0;
  for(int i = 0; i < data.size(); i ++)
    if(data[i].GetEnabledIn())
      size_inputs ++;
  sg->RowCount = size_inputs + 1;
  if(sg->RowCount > 1)
    sg->FixedRows = 1;
  int kol_hex = RAM.GetKolBitAddress()/4 + ((RAM.GetKolBitAddress()%4)? 1 : 0);
  int x = 1;
  for(int i = 1; i <= data.size(); i ++)
    if(data[i-1].GetEnabledIn())
    {
      sg->Cells[0][x] = IntToHex((int)(i-1 + sm_address), kol_hex);
      sg->Cells[1][x] = data[i-1].GetName();
      int kol_hex = data[i-1].GetSize()/4 + ((data[i-1].GetSize()%4)? 1 : 0);
      sg->Cells[2][x++] = IntToHex((int)data[i-1].GetIn(), kol_hex);
    }
}
//---------------------------------------------------------------------------
void CPorts::PrintValue()
{
  if(!grid_debug_ports)
    return;
  for(int i = 1; i < grid_debug_ports->RowCount; i ++)
  {
    int kol_hex = data[i-1].GetSize()/4 + ((data[i-1].GetSize()%4)? 1 : 0);
    grid_debug_ports->Cells[2][i] = (data[i-1].GetEnabledIn())? IntToHex((__int64)data[i-1].GetIn(), kol_hex) : AnsiString("");
    grid_debug_ports->Cells[3][i] = (data[i-1].GetEnabledOut())? IntToHex((__int64)data[i-1].GetOut(), kol_hex) : AnsiString("");
  }
}
//---------------------------------------------------------------------------
bool CPorts::IsPort(AnsiString n)
{
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
    if(n == p->GetName())
      return true;
  return false;
}
//---------------------------------------------------------------------------
CCommandForth CPorts::GetKod(AnsiString name)
{
  for(int i = 0; i < data.size(); i ++)
    if(name == data[i].GetName())
      return CCommandForth(data[i].GetName(), kod_address, i + sm_address, "ADDRESS");
}
//---------------------------------------------------------------------------
void CPorts::PrintVHDLPort(TMemo *memo)
{
  kol_out = 0;
  kol_in = 0;
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
    if(!p->IsInternal())
    {
      if(p->GetEnabledIn())
      {
        kol_in ++;
        if(p->GetSize() > 1)
          memo->Lines->Add("           " + p->GetName() + "_in : in std_logic_vector" + RAM.GetDownto(p->GetSize()-1, 0) + ";");
        else if(p->GetSize() == 1)
          memo->Lines->Add("           " + p->GetName() + "_in : in std_logic;");
        if(IRQ.IsIRQ("IRQ_" + p->GetName()))
          memo->Lines->Add("           " + p->GetName() + "_irq : in std_logic;");
      }
      if(p->GetEnabledOut())
      {
        kol_out ++;
        if(p->GetSize() > 1)
          memo->Lines->Add("           " + p->GetName() + "_out : out std_logic_vector" + RAM.GetDownto(p->GetSize()-1, 0) + ";");
        else if(p->GetSize() == 1)
          memo->Lines->Add("           " + p->GetName() + "_out : out std_logic;");
        if(p->GetEnabledOutIrq())
          memo->Lines->Add("           " + p->GetName() + "_out_irq : out std_logic;");
      }
    }
}
//---------------------------------------------------------------------------
void CPorts::PrintVHDLSignals(TMemo *memo)
{
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
  {
    if(p->IsInternal())
    {
      if(p->GetEnabledIn())
      {
        kol_in ++;
        if(p->GetSize() > 1)
          memo->Lines->Add("   signal " + p->GetName() + "_in : std_logic_vector" + RAM.GetDownto(p->GetSize()-1, 0) + ";");
        else
          memo->Lines->Add("   signal " + p->GetName() + "_in : std_logic;");
      }
      if(p->GetEnabledOut())
      {
        kol_out ++;
        if(p->GetSize() > 1)
          memo->Lines->Add("   signal " + p->GetName() + "_out : std_logic_vector" + RAM.GetDownto(p->GetSize()-1, 0) + ";");
        else
          memo->Lines->Add("   signal " + p->GetName() + "_out : std_logic;");
        if(p->GetEnabledOutIrq())
          memo->Lines->Add("   signal " + p->GetName() + "_out_irq : std_logic;");
      }
    }
    if(p->GetEnabledOut())
    {
      kol_in ++;
      if(p->GetSize() > 1)
        memo->Lines->Add("   signal sc_" + p->GetName() + "_out : std_logic_vector" + RAM.GetDownto(p->GetSize()-1, 0) + ";");
      else
        memo->Lines->Add("   signal sc_" + p->GetName() + "_out : std_logic;");
    }
  }
}
//---------------------------------------------------------------------------
void CPorts::PrintVHDLProcess(TMemo *memo)
{
  if(!data.size())
    return;
  memo->Lines->Add("--порты в реализации");
  AnsiString str = "";
  if(kol_out)
  {
    memo->Lines->Add("p_port: process (clk, sc_rst)");
    memo->Lines->Add("   begin");
    memo->Lines->Add("      if sc_rst = '1' then");
    for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
      if(p->GetEnabledOut())
      {
        if(p->GetSize() > 1)
          memo->Lines->Add("         sc_" + p->GetName() + "_out <= " + RAM.GetNumberBin(0, p->GetSize()) + ";");
        else if(p->GetSize() == 1)
          memo->Lines->Add("         sc_" + p->GetName() + "_out <= '0';");
        if(p->GetEnabledOutIrq())
          memo->Lines->Add("         " + p->GetName() + "_out_irq <= '0';");
      }
    memo->Lines->Add("      else if clk = '1' and clk'event then");
    unsigned i = 0;
    for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++, i ++)
      if(p->GetEnabledOut())
      {
        if(p->GetEnabledOutIrq())
        {
          memo->Lines->Add("            if (sc_address_data =  " + GetAddressBin(i) + ") and (sc_vz = '1')" +
          " then sc_" + p->GetName() + "_out <= sc_stack_data(1)" + RAM.GetDownto(p->GetSize() - 1, 0) + "; " + p->GetName() + "_out_irq <= '1';");
          memo->Lines->Add("            else " + p->GetName() + "_out_irq <= '0'; end if;");
        }
        else
          memo->Lines->Add("            if (sc_address_data =  " + GetAddressBin(i) + ") and (sc_vz = '1')" +
          " then sc_" + p->GetName() + "_out <= sc_stack_data(1)" + RAM.GetDownto(p->GetSize() - 1, 0) + "; end if;");
      }
    memo->Lines->Add("         end if;");
    memo->Lines->Add("      end if;");
    memo->Lines->Add("   end process;");
    for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++, i ++)
      if(p->GetEnabledOut())
        memo->Lines->Add("   " + p->GetName() + "_out <= sc_" + p->GetName() + "_out;");
  }
  if(kol_in)
  {
    int i = 0;
    memo->Lines->Add("   with sc_address_data select sc_bus_data <= ");
    for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++, i ++)
      if(p->GetEnabledIn())
      {
        if(RAM.GetKolBitDataWhithZnak() == p->GetSize())
          memo->Lines->Add("      " + p->GetName() + "_in when " + GetAddressBin(i) + ",");
        else if(p->GetSize() > 0)
          memo->Lines->Add("      (" + stack_return.GetChNULL(RAM.GetKolBitDataWhithZnak() - p->GetSize()) + " & " + p->GetName() + "_in) when " + GetAddressBin(i) + ",");
        else
          memo->Lines->Add("      " + stack_return.GetChNULL(RAM.GetKolBitDataWhithZnak() - p->GetSize()) + " when " + GetAddressBin(i) + ",");
      }
      else if(p->GetEnabledOut())
      {
        if(RAM.GetKolBitDataWhithZnak() == p->GetSize())
          memo->Lines->Add("      sc_" + p->GetName() + "_out when " + GetAddressBin(i) + ",");
        else if(p->GetSize() > 0)
          memo->Lines->Add("      (" + stack_return.GetChNULL(RAM.GetKolBitDataWhithZnak() - p->GetSize()) + " & sc_" + p->GetName() + "_out) when " + GetAddressBin(i) + ",");
        else
          memo->Lines->Add("      " + stack_return.GetChNULL(RAM.GetKolBitDataWhithZnak() - p->GetSize()) + " when " + GetAddressBin(i) + ",");
      //  memo->Lines->Add("      (others => '0') when " + GetAddressBin(i) + ",");
      }
    memo->Lines->Add("      sc_ram when others;");
  }
  else
    memo->Lines->Add("   sc_bus_data <= sc_ram;");

  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++)
    if(!p->IsInternal() & p->GetEnabledIn() & IRQ.IsIRQ("IRQ_" + p->GetName()))
    {
      memo->Lines->Add("p_port_" + p->GetName() + "_irq: process (sc_rst, " + p->GetName() + "_irq, IRQ_" + p->GetName() + "(1)) begin");
      memo->Lines->Add("      if sc_rst = '1' or IRQ_" + p->GetName() + "(1) = '1' then IRQ_" + p->GetName() + "(0) <= '0';");
      memo->Lines->Add("      else if " + p->GetName() + "_irq = '1' and " + p->GetName() + "_irq'event then IRQ_" + p->GetName() + "(0) <= '1'; end if; end if;");
      memo->Lines->Add("   end process;");
    }
}
//---------------------------------------------------------------------------
AnsiString CPorts::GetAddressBin(unsigned a)
{
  unsigned address = a + sm_address;
  AnsiString ret = "";
  for(int i = 0; i < linker.GetSizeShinaData(); i ++, address /= 2)
    ret = ((address % 2)? "1" : "0") + ret;
  return "\"" + ret + "\"";
}
//---------------------------------------------------------------------------
AnsiString CPorts::GetAddressBin(AnsiString n)
{
  int i = 0;
  for(vector <CPort>:: iterator p = data.begin(); p < data.end(); p ++, i ++)
    if(n == p->GetName())
      return GetAddressBin(i);
  return "";  
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
