//---------------------------------------------------------------------------
#pragma hdrstop

#include "CRAM.h"

CRAM RAM;
extern COptionsProc options_proc;
//---------------------------------------------------------------------------
void CRAM::Clear()
{
  data.clear();
  udata_reset.clear();
  udata.clear();
  size = 0;
  kol_enabled_ramb = 0;
}
//---------------------------------------------------------------------------
void CRAM::Reset()
{
  udata = udata_reset;
  PrintValue();
}
//---------------------------------------------------------------------------
void CRAM::AddData(CVariableWord d)
{
  size = d.SetAddress(size);
  data.push_back(d);
}
//---------------------------------------------------------------------------
void CRAM::Linker()
{
  int n;
  for(unsigned i = 0; i < size; i ++)
    for(vector <CVariableWord>:: iterator p = data.begin(); p < data.end(); p ++)
      if(p->GetValue(i, n))
      {
        udata.push_back(ALU.GetHex(n));
        udata_reset.push_back(ALU.GetHex(n));
        break;
      }
}
//---------------------------------------------------------------------------
unsigned CRAM::GetKolBitAddress()
{
  int ret = GetSize();
  if(ret)
    for(int i = 1; i < 33; i++)
      if(IntPower(2, i) >= ret)
      {
        ret = i;
        break;
      }
  return ret;
}
//---------------------------------------------------------------------------
bool CRAM::GetData(unsigned address, unsigned &ch)
{
  ch = 0;
  bool yes = false;

  if(address >= udata.size())
    return false;
  ch = udata[address];  
  return true;
  //for(vector <CVariableWord>:: iterator p = data.begin(); p < data.end(); p ++)
    // if(p->GetValue(address, ch))
}
//---------------------------------------------------------------------------
unsigned CRAM::GetData(unsigned address)
{
  unsigned ch;
  GetData(address, ch);
  return ch;
}
//---------------------------------------------------------------------------
void CRAM::SetData(unsigned address, unsigned d)
{
  if(address >= udata.size())
    return;
  vector <CVariableWord>:: iterator p = data.begin();
  int ch;
  for(; p < data.end(); p ++)
    if(p->GetValue(address, ch)) break;
  AnsiString err = "Изменена константа '" + p->GetName() + "'";
  if(p->GetType() == twConst)
    MessageBox(0, err.c_str(), "ОЗУ", MB_ICONSTOP | MB_TASKMODAL | MB_OK);
  udata[address] = ALU.Norma(d);
  PrintValue();
}
//---------------------------------------------------------------------------
AnsiString CRAM::GetDataName(unsigned address)
{
  int n;
  AnsiString ret;
  for(vector <CVariableWord>:: iterator p = data.begin(); p < data.end(); p ++)
     if(p->GetValue(address, n))
     {
       ret = p->GetName();
       if(p->GetKolCells() > 1)
         ret += "_" + IntToStr(address - p->GetAddress());
       break;
     }
  return ret;
}
//---------------------------------------------------------------------------
void CRAM::PrintAll(TStringGrid *sg)
{
  grid_debug_RAM = sg;
  grid_debug_RAM->RowCount = GetSize() + 1;
  if(grid_debug_RAM->RowCount > 1)
    grid_debug_RAM->FixedRows = 1;
  int kol_hex = GetKolBitAddress()/4 + ((GetKolBitAddress()%4)? 1 : 0);
  for(int i = 1; i < grid_debug_RAM->RowCount; i ++)
  {
    grid_debug_RAM->Cells[0][i] = IntToHex(i-1, kol_hex);
    grid_debug_RAM->Cells[1][i] = GetDataName(i-1);
  }
  PrintValue();
}
//---------------------------------------------------------------------------
void CRAM::PrintValue(int s)
{
  if(!grid_debug_RAM)
    return;
  if((s == 10) || (s == 16))
    ss = s;
  int kol_hex = kol_bit_data/4 + ((kol_bit_data%4)? 1 : 0);
  for(int i = 1; i < grid_debug_RAM->RowCount; i ++)
    if(ss == 10)
      grid_debug_RAM->Cells[2][i] = ALU.GetInt(udata[i-1]);
    else
    {
      unsigned hex_value = udata[i-1];
      grid_debug_RAM->Cells[2][i] = IntToHex((__int64)udata[i-1], kol_hex);
    }
}
//---------------------------------------------------------------------------
void CRAM::PrintVHDLSignals(TMemo *memo)
{
  switch(options_proc.GetChip())
  {
    case 0: PrintVHDLSignalsSpartan2(memo); break;
    case 1: PrintVHDLSignalsSpartan3(memo); break;
    case 2: PrintVHDLSignalsSpartan6(memo); break;
    default: return;
  }
  ramb_kol[0] = size / GetSizeRAMB(ramb_size[0]) + ((size % GetSizeRAMB(ramb_size[0]))? 1 : 0);
  ramb_kol[1] = (!ramb_size[1])? 0 : (size / GetSizeRAMB(ramb_size[1]) + ((size % GetSizeRAMB(ramb_size[1]))? 1 : 0));
  for(int i = 0; i < 2; i ++)
  {
    for(int j = 0; j < ramb_kol[i]; j ++)
    {
      if(!i)
      {
        memo->Lines->Add("   signal sc_ram_low_" + IntToStr(j) + " : std_logic_vector" + RAM.GetDownto(ramb_size[0] - 1, 0) + ";");
        memo->Lines->Add("   signal sc_ram_low_spartan6_" + IntToStr(j) + " : std_logic_vector(31 downto 0);");
        if(ramb_kol[i] > 1)
          memo->Lines->Add("   signal sc_en_ram_low_" + IntToStr(j) + " : std_logic;");
      }
      else
      {
        memo->Lines->Add("   signal sc_ram_height_" + IntToStr(j) + " : std_logic_vector" + RAM.GetDownto(ramb_size[1] - 1, 0) + ";");
        memo->Lines->Add("   signal sc_ram_height_spartan6_" + IntToStr(j) + " : std_logic_vector(31 downto 0);");
        if(ramb_kol[i] > 1)
          memo->Lines->Add("   signal sc_en_ram_height_" + IntToStr(j) + " : std_logic;");
      }
      if(options_proc.GetChip() < 2)
      {
        if(IsEnabledRamB("RAMB_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i])))
          continue;
        AddEnabledRamB("RAMB_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i]));
        if(!options_proc.GetChip())
        {
          memo->Lines->Add("   component RAMB4_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i]));
          memo->Lines->Add("     generic(INIT_00, INIT_01,  INIT_02,  INIT_03,  INIT_04,  INIT_05,  INIT_06,  INIT_07, INIT_08, INIT_09,  INIT_0A,  INIT_0B,  INIT_0C,  INIT_0D,  INIT_0E,  INIT_0F : bit_vector);");
          memo->Lines->Add("        port(DIA, DIB : in std_logic_vector" + RAM.GetDownto(ramb_size[i] - 1, 0) + ";");
          memo->Lines->Add("             ENA, ENB, WEA, WEB, RSTA, RSTB, CLKA, CLKB: in std_logic; ");
          memo->Lines->Add("             ADDRA, ADDRB: in std_logic_vector" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ";");
          memo->Lines->Add("             DOA, DOB : out std_logic_vector" + RAM.GetDownto(ramb_size[i] - 1, 0) + ");");
          memo->Lines->Add("   end component;");
        }
        else
        {
          int s = ramb_size[i] + ramb_size[i] / 8;
          memo->Lines->Add("   component RAMB16_S" + IntToStr(s) + "_S" + IntToStr(s));
          memo->Lines->Add("     generic(INIT_A, INIT_B : bit_vector;");
          memo->Lines->Add("            INIT_00, INIT_01,  INIT_02,  INIT_03,  INIT_04,  INIT_05,  INIT_06,  INIT_07, INIT_08, INIT_09,  INIT_0A,  INIT_0B,  INIT_0C,  INIT_0D,  INIT_0E,  INIT_0F : bit_vector;");
          memo->Lines->Add("            INIT_10, INIT_11,  INIT_12,  INIT_13,  INIT_14,  INIT_15,  INIT_16,  INIT_17, INIT_18, INIT_19,  INIT_1A,  INIT_1B,  INIT_1C,  INIT_1D,  INIT_1E,  INIT_1F : bit_vector;");
          memo->Lines->Add("            INIT_20, INIT_21,  INIT_22,  INIT_23,  INIT_24,  INIT_25,  INIT_26,  INIT_27, INIT_28, INIT_29,  INIT_2A,  INIT_2B,  INIT_2C,  INIT_2D,  INIT_2E,  INIT_2F : bit_vector;");
          memo->Lines->Add("            INIT_30, INIT_31,  INIT_32,  INIT_33,  INIT_34,  INIT_35,  INIT_36,  INIT_37, INIT_38, INIT_39,  INIT_3A,  INIT_3B,  INIT_3C,  INIT_3D,  INIT_3E,  INIT_3F : bit_vector;");
          if(ramb_size[i] >= 8)
            memo->Lines->Add("            INITP_00, INITP_01,  INITP_02,  INITP_03,  INITP_04,  INITP_05,  INITP_06,  INITP_07 : bit_vector;");
          memo->Lines->Add("            SRVAL_A, SRVAL_B : bit_vector := X\"0\";");
          memo->Lines->Add("            WRITE_MODE_A, WRITE_MODE_B : string := \"WRITE_FIRST\");");
          memo->Lines->Add("        port(DIA, DIB : in std_logic_vector" + RAM.GetDownto(ramb_size[i] - 1, 0) + ";");
          if(ramb_size[i] >= 8)
            memo->Lines->Add("            DIPA, DIPB : in std_logic_vector(" + IntToStr(ramb_size[i]/8 - 1) + " downto 0);");
          memo->Lines->Add("             ENA, ENB, WEA, WEB, SSRA, SSRB, CLKA, CLKB: in std_logic; ");
          memo->Lines->Add("             ADDRA, ADDRB: in std_logic_vector" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ";");
          if(ramb_size[i] >= 8)
            memo->Lines->Add("             DOPA, DOPB : out std_logic_vector(" + IntToStr(ramb_size[i]/8 - 1) + " downto 0);");
          memo->Lines->Add("             DOA, DOB : out std_logic_vector" + RAM.GetDownto(ramb_size[i] - 1, 0) + ");");
          memo->Lines->Add("   end component;");
        }
      }
    }
  }
  if(options_proc.GetChip() == 2)
    memo->Lines->Add("   signal sc_vz4 : std_logic_vector" + RAM.GetDownto(3, 0) + ";");

  if((GetKolBitDataWhithZnak() < ramb_size[0]) && ramb_kol[0])
    memo->Lines->Add("   signal sc_ram_di_low : std_logic_vector" + RAM.GetDownto(ramb_size[0] - 1, 0) + ";");
  if((GetKolBitDataWhithZnak() < (ramb_size[0] + ramb_size[1])) && ramb_kol[1])
    memo->Lines->Add("   signal sc_ram_di_height : std_logic_vector" + RAM.GetDownto(ramb_size[1] - 1, 0) + ";");
  if((linker.GetSizeShinaData() < GetShARAMB(ramb_size[0])) && ramb_kol[0])
    memo->Lines->Add("   signal sc_ram_addr_low : std_logic_vector" + RAM.GetDownto(GetShARAMB(ramb_size[0]) - 1, 0) + ";");
  if((linker.GetSizeShinaData() < GetShARAMB(ramb_size[1])) && ramb_kol[1])
    memo->Lines->Add("   signal sc_ram_addr_height : std_logic_vector" + RAM.GetDownto(GetShARAMB(ramb_size[1]) - 1, 0) + ";");
  memo->Lines->Add("   signal sc_ram : std_logic_vector" + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, 0) + ";");
}
//---------------------------------------------------------------------------
void CRAM::PrintVHDLSignalsSpartan6(TMemo *memo)
{
//  ramb_size[0] = 32; ramb_size[1] = 0;
  PrintVHDLSignalsSpartan3(memo);
}
//---------------------------------------------------------------------------
void CRAM::PrintVHDLSignalsSpartan3(TMemo *memo)
{
  switch(kol_bit_data)
  {
    case 32: case 31: case 30: case 29: case 28: case 27: case 26: case 25:
      ramb_size[0] = 32; ramb_size[1] = 0; break;
    case 24: case 23: case 22: case 21:
      if(size <= 512)
      { ramb_size[0] = 32; ramb_size[1] = 0; break;}
      ramb_size[0] = 16; ramb_size[1] = 8; break;
    case 20: case 19:
      if(size <= 512)
      { ramb_size[0] = 32; ramb_size[1] = 0; break;}
      ramb_size[0] = 16; ramb_size[1] = 4; break;
    case 18:
      if(size <= 512)
      { ramb_size[0] = 32; ramb_size[1] = 0; break;}
      ramb_size[0] = 16; ramb_size[1] = 2; break;
    case 17:
      if(size <= 512)
      { ramb_size[0] = 32; ramb_size[1] = 0; break;}
      ramb_size[0] = 16; ramb_size[1] = 1; break;
    case 16: case 15: case 14: case 13:
      ramb_size[0] = 16; ramb_size[1] = 0; break;
    case 12: case 11:
      if(size <= 1024)
      { ramb_size[0] = 16; ramb_size[1] = 0; break;}
      ramb_size[0] = 8;  ramb_size[1] = 4; break;
    case 10:
      if(size <= 1024)
      { ramb_size[0] = 16; ramb_size[1] = 0; break;}
      ramb_size[0] = 8;  ramb_size[1] = 2; break;
    case  9:
      if(size <= 1024)
      { ramb_size[0] = 16; ramb_size[1] = 0; break;}
      ramb_size[0] = 8;  ramb_size[1] = 1; break;
    case  8: case  7:
      ramb_size[0] = 8;  ramb_size[1] = 0; break;
    case  6:
      if(size <= 2048)
      { ramb_size[0] = 8; ramb_size[1] = 0; break;}
      ramb_size[0] = 4;  ramb_size[1] = 2; break;
    case  5:
      if(size <= 2048)
      { ramb_size[0] = 8; ramb_size[1] = 0; break;}
      ramb_size[0] = 4;  ramb_size[1] = 1; break;
    case  4: case 3:
      ramb_size[0] = 4;  ramb_size[1] = 0; break;
    case  2:
      ramb_size[0] = 2;  ramb_size[1] = 0; break;
    case  1:
      ramb_size[0] = 1;  ramb_size[1] = 0; break;
  }
}
//---------------------------------------------------------------------------
void CRAM::PrintVHDLSignalsSpartan2(TMemo *memo)
{
  switch(kol_bit_data)
  {
    case 32: case 31: case 30: case 29: case 28: case 27: case 26: case 25:
      ramb_size[0] = 16; ramb_size[1] = 16; break;
    case 24: case 23: case 22: case 21:
      ramb_size[0] = 16; ramb_size[1] = 8; break;
    case 20: case 19:
      ramb_size[0] = 16; ramb_size[1] = 4; break;
    case 18:
      ramb_size[0] = 16; ramb_size[1] = 2; break;
    case 17:
      ramb_size[0] = 16; ramb_size[1] = 1; break;
    case 16: case 15: case 14: case 13:
      ramb_size[0] = 16; ramb_size[1] = 0; break;
    case 12: case 11:
      if(size <= 256)
      { ramb_size[0] = 16; ramb_size[1] = 0; break;}
      ramb_size[0] = 8;  ramb_size[1] = 4; break;
    case 10:
      if(size <= 256)
      { ramb_size[0] = 16; ramb_size[1] = 0; break;}
      ramb_size[0] = 8;  ramb_size[1] = 2; break;
    case  9:
      if(size <= 256)
      { ramb_size[0] = 16; ramb_size[1] = 0; break;}
      ramb_size[0] = 8;  ramb_size[1] = 1; break;
    case  8: case  7:
      ramb_size[0] = 8;  ramb_size[1] = 0; break;
    case  6:
      if(size <= 512)
      { ramb_size[0] = 8; ramb_size[1] = 0; break;}
      ramb_size[0] = 4;  ramb_size[1] = 2; break;
    case  5:
      if(size <= 512)
      { ramb_size[0] = 8; ramb_size[1] = 0; break;}
      ramb_size[0] = 4;  ramb_size[1] = 1; break;
    case  4: case 3:
      ramb_size[0] = 4;  ramb_size[1] = 0; break;
    case  2:
      ramb_size[0] = 2;  ramb_size[1] = 0; break;
    case  1:
      ramb_size[0] = 1;  ramb_size[1] = 0; break;
  }
}
//---------------------------------------------------------------------------
unsigned CRAM::GetSizeRAMB(int sh_d)
{
  if(!options_proc.GetChip())
    switch(sh_d)
    {
      case 16: return 256;
      case  8: return 512;
      case  4: return 1024;
      case  2: return 2048;
      case  1: return 4096;
    }
  switch(sh_d)
  {
    case 32: return 512;
    case 16: return 1024;
    case  8: return 2048;
    case  4: return 4096;
    case  2: return 8192;
    case  1: return 16384;
  }
  return 0;
}
//---------------------------------------------------------------------------
unsigned CRAM::GetShARAMB(int sh_d)
{
  if(!options_proc.GetChip())
    switch(sh_d)
    {
      case 16: return 8;
      case  8: return 9;
      case  4: return 10;
      case  2: return 11;
      case  1: return 12;
    }
 // if(options_proc.GetChip() == 1)
    switch(sh_d)
    {
      case 32: return 9;
      case 16: return 10;
      case  8: return 11;
      case  4: return 12;
      case  2: return 13;
      case  1: return 14;
    }
 // if(options_proc.GetChip() == 2)
  //  return 14;
  return 0;
}
//---------------------------------------------------------------------------
AnsiString CRAM::GetNumberBin(unsigned ch, int kol_bit)
{
  AnsiString ret = "";
  for(int i = 0; i < kol_bit; i ++, ch /= 2)
    ret = ((ch % 2)? "1" : "0") + ret;
  return ((kol_bit > 1)? "\"" : "'") + ret + ((kol_bit > 1)? "\"" : "'");
}
//---------------------------------------------------------------------------
AnsiString CRAM::GetDownto(int begin, int end)
{
  if(begin == end)
    return "(" + IntToStr(begin) + ")";
  return "(" + IntToStr(begin) + " downto " + IntToStr(end) + ")";
}
//---------------------------------------------------------------------------
AnsiString CRAM::GetDownto2(int begin, int end)
{
  return "(" + IntToStr(begin) + " downto " + IntToStr(end) + ")";
}
//---------------------------------------------------------------------------
void CRAM::PrintVHDLProcess(TMemo *memo)
{
  if(!size)
  {
    memo->Lines->Add("   sc_ram <= (others => '0');");
    return;
  }
  memo->Lines->Add("--храниние данных");
  RunRAMBHex();
  if((GetKolBitDataWhithZnak() < ramb_size[0]) && ramb_kol[0])
    memo->Lines->Add("   sc_ram_di_low <= ("+ stack_return.GetChNULL(ramb_size[0] - GetKolBitDataWhithZnak()) +" & sc_stack_data(1)" + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, 0) + ");");
  if((GetKolBitDataWhithZnak() < (ramb_size[0] + ramb_size[1])) && ramb_kol[1])
    memo->Lines->Add("   sc_ram_di_height <=("+ stack_return.GetChNULL((ramb_size[0] + ramb_size[1]) - GetKolBitDataWhithZnak()) +" & sc_stack_data(1)" + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, ramb_size[0]) + ");");
  if((linker.GetSizeShinaData() < GetShARAMB(ramb_size[0])) && ramb_kol[0])
  {
    if(options_proc.GetChip() == 2)
    {
      int kol_first_null = 0;
      switch(ramb_size[0])
      {
        case 2: kol_first_null = 1; break;
        case 4: kol_first_null = 2; break;
        case 8: kol_first_null = 3; break;
        case 16: kol_first_null = 4; break;
        case 32: kol_first_null = 5; break;
      }
      memo->Lines->Add("   sc_ram_addr_low <= (" + stack_return.GetChNULL(GetShARAMB(ramb_size[0]) - linker.GetSizeShinaData() - kol_first_null) +
        " & sc_address_data & " + stack_return.GetChNULL(kol_first_null) + ");");
    }
    else
      memo->Lines->Add("   sc_ram_addr_low <= (" + stack_return.GetChNULL(GetShARAMB(ramb_size[0]) - linker.GetSizeShinaData()) + " & sc_address_data);");
  }
  if((linker.GetSizeShinaData() < GetShARAMB(ramb_size[1])) && ramb_kol[1])
  {
    if(options_proc.GetChip() == 2)
    {
      int kol_first_null = 0;
      switch(ramb_size[1])
      {
        case 2: kol_first_null = 1; break;
        case 4: kol_first_null = 2; break;
        case 8: kol_first_null = 3; break;
        case 16: kol_first_null = 4; break;
        case 32: kol_first_null = 5; break;
      }
      memo->Lines->Add("   sc_ram_addr_height <= (" + stack_return.GetChNULL(GetShARAMB(ramb_size[1]) - linker.GetSizeShinaData() - kol_first_null) +
        " & sc_address_data & " + stack_return.GetChNULL(kol_first_null) + ");");
    }
    else
      memo->Lines->Add("   sc_ram_addr_height <= (" + stack_return.GetChNULL(GetShARAMB(ramb_size[1]) - linker.GetSizeShinaData()) + " & sc_address_data);");
  }
  switch(options_proc.GetChip())
  {
    case 0: PrintVHDLProcessSpartan2(memo); break;
    case 1: PrintVHDLProcessSpartan3(memo); break;
    case 2: PrintVHDLProcessSpartan6(memo); break;
    default: return;
  }
}
//---------------------------------------------------------------------------
void CRAM::PrintVHDLProcessSpartan6(TMemo *memo)
{
  for(int i = 0; i < 2; i ++)
  {
    AnsiString str = "";
    bool first = true;
    for(int j = 0; j < ramb_kol[i]; j ++)
    {
      memo->Lines->Add("   sc_vz4 <= (others => sc_vz);");
      memo->Lines->Add("sc_ram_" + IntToStr(i) + "_" + IntToStr(j) + ":RAMB16BWER");
      memo->Lines->Add("   generic map(");
      int z = ramb_size[i];
      switch(z)
      {
        case 8: z = 9; break;
        case 16: z = 18; break;
        case 32: z = 36; break;
        default: break;
      }
      memo->Lines->Add("       DATA_WIDTH_A=>" + IntToStr(z) + ",");
      memo->Lines->Add("       DATA_WIDTH_B=>" + IntToStr(z) + ",");
      memo->Lines->Add("       DOA_REG=>0, DOB_REG=>0,");
      memo->Lines->Add("       EN_RSTRAM_A=>FALSE, EN_RSTRAM_B=>FALSE,");
      for(int k = 0; k < 8; k ++)
        memo->Lines->Add("       INITP_" + IntToHex(k, 2) + " => X\"0000000000000000000000000000000000000000000000000000000000000000\",");
      for(int k = 0; k < 64; k ++)
        memo->Lines->Add("       INIT_" + IntToHex(k, 2) + " => X\"" + ranb_hex[i][j][k] + "\",");
      memo->Lines->Add("           INIT_A => X\"000000000\", INIT_B => X\"000000000\",");
      memo->Lines->Add("           INIT_FILE=>\"NONE\",");
      memo->Lines->Add("           RSTTYPE=>\"ASYNC\",");
      memo->Lines->Add("           RST_PRIORITY_A=>\"SR\",");
      memo->Lines->Add("           RST_PRIORITY_B=>\"SR\",");
      memo->Lines->Add("           SIM_COLLISION_CHECK=>\"ALL\",");
      memo->Lines->Add("           SIM_DEVICE=>\"SPARTAN3ADSP\",");
      memo->Lines->Add("           SRVAL_A => X\"000000000\", SRVAL_B => X\"000000000\",");
      memo->Lines->Add("           WRITE_MODE_A=>\"WRITE_FIRST\",");
      memo->Lines->Add("           WRITE_MODE_B=>\"WRITE_FIRST\")");
      memo->Lines->Add("           ");
      if(!i)
      {
        if(GetKolBitDataWhithZnak() >= ramb_size[0])
        {
          if(ramb_size[0] == 32)
            memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_stack_data(1)" + RAM.GetDownto(ramb_size[0] - 1, 0) + ",");
          else
            memo->Lines->Add("  port map(DIA => (others => '0'), DIB => (" + stack_return.GetChNULL(32 - ramb_size[0]) + " & sc_stack_data(1)" + RAM.GetDownto(ramb_size[0] - 1, 0) + "),");
        }
        else
        {
          if(ramb_size[0] == 32)
            memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_ram_di_low,");
          else
            memo->Lines->Add("  port map(DIA => (others => '0'), DIB => (" + stack_return.GetChNULL(32 - ramb_size[0]) + " & sc_ram_di_low),");
        }
      }
      else
      {
        if(GetKolBitDataWhithZnak() >= (ramb_size[0] + ramb_size[1]))
        {
          if(ramb_size[1] == 32)
            memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_stack_data(1)" + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, ramb_size[0]) + ",");
          else
            memo->Lines->Add("  port map(DIA => (others => '0'), DIB => (" + stack_return.GetChNULL(32 - ramb_size[1]) + " & sc_stack_data(1)" + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, ramb_size[0]) + "),");
        }
        else
        {
          if(ramb_size[1] == 32)
            memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_ram_di_height,");
          else
            memo->Lines->Add("  port map(DIA => (others => '0'), DIB => (" + stack_return.GetChNULL(32 - ramb_size[1]) + " & sc_ram_di_height),");
        }  
      }    
      if(ramb_kol[i] > 1)
        if(!i)
        {
          memo->Lines->Add("           RSTA => sc_en_ram_low_" + IntToStr(j)+ ", RSTB => '1',");
          memo->Lines->Add("           WEA => \"0000\", WEB => sc_vz4, ENA => '1', ENB => not sc_en_ram_low_" + IntToStr(j)+ ", CLKA => clk_not, CLKB => clk,");
        }
        else
        {
          memo->Lines->Add("           RSTA => sc_en_ram_height_" + IntToStr(j)+ ", RSTB => '1',");
          memo->Lines->Add("           WEA => \"0000\", WEB => sc_vz4, ENA => '1', ENB => not sc_en_ram_height_" + IntToStr(j)+ ", CLKA => clk_not, CLKB => clk,");
        }
      else
      {
        memo->Lines->Add("           RSTA => '0', RSTB => '0',");
        memo->Lines->Add("           WEA => \"0000\", WEB => sc_vz4, ENA => '1', ENB => '1', CLKA => clk_not, CLKB => clk,");
      }
      memo->Lines->Add("           REGCEA => '1', REGCEB => '1',");
      if(options_proc.GetChip())
        if(ramb_size[i] >= 8)
          memo->Lines->Add("           DIPA => (others => '0'), DIPB => (others => '0'),");

      AnsiString str_addr;
      if(linker.GetSizeShinaData() >= GetShARAMB(ramb_size[i]))
      {
          if(GetShARAMB(ramb_size[i]) == 14)
            str_addr = "sc_address_data" + GetDownto(GetShARAMB(ramb_size[i]) - 1, 0);
          else
            str_addr = "(" + stack_return.GetChNULL(14 - GetShARAMB(ramb_size[i])) + " & sc_address_data" + RAM.GetDownto(GetShARAMB(ramb_size[i]) - 1, 0) + ")";
      }
      else
        if(!i)
        {
          if(GetShARAMB(ramb_size[0]) == 14)
            str_addr = "sc_ram_addr_low";
          else
            str_addr = "(" + stack_return.GetChNULL(14 - GetShARAMB(ramb_size[0])) + " & sc_ram_addr_low)";
        }
        else
        {
          if(GetShARAMB(ramb_size[1]) == 14)
            str_addr = "sc_ram_addr_height";
          else
            str_addr = "(" + stack_return.GetChNULL(14 - GetShARAMB(ramb_size[1])) + " & sc_ram_addr_height)";
        }
      memo->Lines->Add("           ADDRA => " + str_addr + ", ADDRB => " + str_addr + ",");
      if(!i)
      {
        memo->Lines->Add("           DOA => sc_ram_low_spartan6_" + IntToStr(j) + ");");
      //  if(ramb_size[0] == 32)
      //    memo->Lines->Add("   sc_ram_low_" + IntToStr(j) + " <= sc_ram_low_spartan6_" + IntToStr(j) + ";");
      //  else
        memo->Lines->Add("   sc_ram_low_" + IntToStr(j) + " <= sc_ram_low_spartan6_" + IntToStr(j) + RAM.GetDownto(ramb_size[0] - 1, 0) + ";");
        if(!first) str += " or ";
        if(GetKolBitDataWhithZnak() >= ramb_size[0])
          str += ("sc_ram_low_" + IntToStr(j));
        else
          str += ("sc_ram_low_" + IntToStr(j) + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, 0));
        first = false;
      }
      else
      {
        memo->Lines->Add("           DOA => sc_ram_height_spartan6_" + IntToStr(j) + ");");
      //  if(ramb_size[1] == 32)
      //    memo->Lines->Add("   sc_ram_height_" + IntToStr(j) + " <= sc_ram_height_spartan6_" + IntToStr(j) + ";");
       // else
        memo->Lines->Add("   sc_ram_height_" + IntToStr(j) + " <= sc_ram_height_spartan6_" + IntToStr(j) + RAM.GetDownto(ramb_size[1] - 1, 0) + ";");
        if(!first) str += " or ";
        if(GetKolBitDataWhithZnak() >= (ramb_size[0] + ramb_size[1]))
          str += ("sc_ram_height_" + IntToStr(j));
        else
          str += ("sc_ram_height_" + IntToStr(j) + RAM.GetDownto(GetKolBitDataWhithZnak() - ramb_size[0] - 1, 0));
        first = false;
      }
      if(ramb_kol[i] > 1)
        if(!i)
          memo->Lines->Add("   sc_en_ram_low_" + IntToStr(j) + " <= '0' when sc_address_data" + RAM.GetDownto(GetKolBitAddress() - 1, GetShARAMB(ramb_size[0])) +
                           " = " + GetNumberBin(j , GetKolBitAddress() - GetShARAMB(ramb_size[0])) + " else '1';");
        else
           memo->Lines->Add("   sc_en_ram_height_" + IntToStr(j) + " <= '0' when sc_address_data" + RAM.GetDownto(GetKolBitAddress() - 1, GetShARAMB(ramb_size[1])) +
                           " = " + GetNumberBin(j , GetKolBitAddress() - GetShARAMB(ramb_size[1])) + " else '1';");
    }
    if(!i)
      if(GetKolBitDataWhithZnak() >= ramb_size[0])
        memo->Lines->Add("   sc_ram" + RAM.GetDownto(ramb_size[0] - 1, 0) + " <= " + str + ";");
      else
        memo->Lines->Add("   sc_ram <= " + str + ";");
    else
      if(str != "")
        memo->Lines->Add("   sc_ram" + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, ramb_size[0]) + " <= " + str + ";");
  }
}
//---------------------------------------------------------------------------
void CRAM::PrintVHDLProcessSpartan3(TMemo *memo)
{
  for(int i = 0; i < 2; i ++)
  {
    AnsiString str = "";
    bool first = true;
    for(int j = 0; j < ramb_kol[i]; j ++)
    {
      int s = ramb_size[i] + ramb_size[i] / 8;
      memo->Lines->Add("sc_ram_" + IntToStr(i) + "_" + IntToStr(j) + ":RAMB16_S" + IntToStr(s) + "_S" + IntToStr(s));
      if(ramb_size[i] < 8)
        memo->Lines->Add("   generic map(INIT_A => X\"0\", INIT_B => X\"0\",");
      else if(ramb_size[i] == 8)
        memo->Lines->Add("   generic map(INIT_A => X\"000\", INIT_B => X\"000\",");
      else if(ramb_size[i] == 16)
        memo->Lines->Add("   generic map(INIT_A => X\"00000\", INIT_B => X\"00000\",");
      else
        memo->Lines->Add("   generic map(INIT_A => X\"000000000\", INIT_B => X\"000000000\",");
      if(s >= 8)
        if(options_proc.GetChip())
          for(int k = 0; k < 8; k ++)
            memo->Lines->Add("       INITP_" + IntToHex(k, 2) + " => X\"" + IntToHex(0, 16) + "\",");
      for(int k = 0; k < 64; k ++)
        memo->Lines->Add("       INIT_" + IntToHex(k, 2) + " => X\"" + ranb_hex[i][j][k] + "\"" + ((k != 63)? "," : ")"));
      if(!i)
        if(GetKolBitDataWhithZnak() >= ramb_size[0])
          memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_stack_data(1)" + RAM.GetDownto(ramb_size[0] - 1, 0) + ",");
        else
          memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_ram_di_low,");
      else
        if(GetKolBitDataWhithZnak() >= (ramb_size[0] + ramb_size[1]))
          memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_stack_data(1)" + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, ramb_size[0]) + ",");
        else
          memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_ram_di_height,");
      if(ramb_kol[i] > 1)
        if(!i)
        {
        //  memo->Lines->Add("           SSRA => sc_en_ram_low_" + IntToStr(j)+ ", SSRB => '1',");
          memo->Lines->Add("           WEA => '0', WEB => sc_vz, ENA => '1', ENB => not sc_en_ram_low_" + IntToStr(j)+ ", CLKA => clk_not, CLKB => clk,");
        }
        else
        {
        //  memo->Lines->Add("           SSRA => sc_en_ram_height_" + IntToStr(j)+ ", SSRB => '1',");
          memo->Lines->Add("           WEA => '0', WEB => sc_vz, ENA => '1', ENB => not sc_en_ram_height_" + IntToStr(j)+ ", CLKA => clk_not, CLKB => clk,");
        }
      else
      {
      //  memo->Lines->Add("           SSRA => '0', SSRB => '1',");
        memo->Lines->Add("           WEA => '0', WEB => sc_vz, ENA => '1', ENB => '1', CLKA => clk_not, CLKB => clk,");
      }
      if(options_proc.GetChip())
        if(ramb_size[i] >= 8)
          memo->Lines->Add("           DIPA => (others => '0'), DIPB => (others => '0'),");

      if(linker.GetSizeShinaData() >= GetShARAMB(ramb_size[i]))
        memo->Lines->Add("           ADDRA => sc_address_data" + RAM.GetDownto(GetShARAMB(ramb_size[i]) - 1, 0) + ", ADDRB => sc_address_data" + RAM.GetDownto(GetShARAMB(ramb_size[i]) - 1, 0) + ",");
      else
        if(!i)
          memo->Lines->Add("           ADDRA => sc_ram_addr_low, ADDRB => sc_ram_addr_low,");
        else
          memo->Lines->Add("           ADDRA => sc_ram_addr_height, ADDRB => sc_ram_addr_height,");
      if(!i)
      {
        memo->Lines->Add("           DOA => sc_ram_low_" + IntToStr(j) + ");");
        if(!first) str += " or ";
        if(GetKolBitDataWhithZnak() >= ramb_size[0])
          str += ("sc_ram_low_" + IntToStr(j));
        else
          str += ("sc_ram_low_" + IntToStr(j) + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, 0));
        first = false;
      }
      else
      {
        memo->Lines->Add("           DOA => sc_ram_height_" + IntToStr(j) + ");");
        if(!first) str += " or ";
        if(GetKolBitDataWhithZnak() >= (ramb_size[0] + ramb_size[1]))
          str += ("sc_ram_height_" + IntToStr(j));
        else
          str += ("sc_ram_height_" + IntToStr(j) + RAM.GetDownto(GetKolBitDataWhithZnak() - ramb_size[0] - 1, 0));
        first = false;
      }
      if(ramb_kol[i] > 1)
        if(!i)
          memo->Lines->Add("   sc_en_ram_low_" + IntToStr(j) + " <= '0' when sc_address_data" + RAM.GetDownto(GetKolBitAddress() - 1, GetShARAMB(ramb_size[0])) +
                           " = " + GetNumberBin(j , GetKolBitAddress() - GetShARAMB(ramb_size[0])) + " else '1';");
        else
           memo->Lines->Add("   sc_en_ram_height_" + IntToStr(j) + " <= '0' when sc_address_data" + RAM.GetDownto(GetKolBitAddress() - 1, GetShARAMB(ramb_size[1])) +
                           " = " + GetNumberBin(j , GetKolBitAddress() - GetShARAMB(ramb_size[1])) + " else '1';");
    }
    if(!i)
      if(GetKolBitDataWhithZnak() >= ramb_size[0])
        memo->Lines->Add("   sc_ram" + RAM.GetDownto(ramb_size[0] - 1, 0) + " <= " + str + ";");
      else
        memo->Lines->Add("   sc_ram <= " + str + ";");
    else
      if(str != "")
        memo->Lines->Add("   sc_ram" + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, ramb_size[0]) + " <= " + str + ";");
  }
}
//---------------------------------------------------------------------------
void CRAM::PrintVHDLProcessSpartan2(TMemo *memo)
{
  for(int i = 0; i < 2; i ++)
  {
    AnsiString str = "";
    bool first = true;
    for(int j = 0; j < ramb_kol[i]; j ++)
    {
      memo->Lines->Add("sc_ram_" + IntToStr(i) + "_" + IntToStr(j) + ":RAMB4_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i]));
      memo->Lines->Add("   generic map(");
      for(int k = 0; k < 16; k ++)
        memo->Lines->Add("       INIT_0" + IntToHex(k, 1) + " => X\"" + ranb_hex[i][j][k] + "\"" + ((k != 15)? "," : ")"));
      if(!i)
        if(GetKolBitDataWhithZnak() >= ramb_size[0])
          memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_stack_data(1)" + RAM.GetDownto(ramb_size[0] - 1, 0) + ",");
        else
          memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_ram_di_low,");
      else
        if(GetKolBitDataWhithZnak() >= (ramb_size[0] + ramb_size[1]))
          memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_stack_data(1)" + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, ramb_size[0]) + ",");
        else
          memo->Lines->Add("  port map(DIA => (others => '0'), DIB => sc_ram_di_height,");
      if(ramb_kol[i] > 1)
        if(!i)
        {
          memo->Lines->Add("           RSTA => sc_en_ram_low_" + IntToStr(j)+ ", RSTB => '1',");
          memo->Lines->Add("           WEA => '0', WEB => sc_vz, ENA => '1', ENB => not sc_en_ram_low_" + IntToStr(j)+ ", CLKA => clk_not, CLKB => clk,");
        }
        else
        {
          memo->Lines->Add("           RSTA => sc_en_ram_height_" + IntToStr(j)+ ", RSTB => '1',");
          memo->Lines->Add("           WEA => '0', WEB => sc_vz, ENA => '1', ENB => not sc_en_ram_height_" + IntToStr(j)+ ", CLKA => clk_not, CLKB => clk,");
        }
      else
      {
        memo->Lines->Add("           RSTA => '0', RSTB => '0',");
        memo->Lines->Add("           WEA => '0', WEB => sc_vz, ENA => '1', ENB => '1', CLKA => clk_not, CLKB => clk,");
      }

      if(linker.GetSizeShinaData() >= GetShARAMB(ramb_size[i]))
        memo->Lines->Add("           ADDRA => sc_address_data" + RAM.GetDownto(GetShARAMB(ramb_size[i]) - 1, 0) + ", ADDRB => sc_address_data" + RAM.GetDownto(GetShARAMB(ramb_size[i]) - 1, 0) + ",");
      else
        if(!i)
          memo->Lines->Add("           ADDRA => sc_ram_addr_low, ADDRB => sc_ram_addr_low,");
        else
          memo->Lines->Add("           ADDRA => sc_ram_addr_height, ADDRB => sc_ram_addr_height,");
      if(!i)
      {
        memo->Lines->Add("           DOA => sc_ram_low_" + IntToStr(j) + ");");
        if(!first) str += " or ";
        if(GetKolBitDataWhithZnak() >= ramb_size[0])
          str += ("sc_ram_low_" + IntToStr(j));
        else
          str += ("sc_ram_low_" + IntToStr(j) + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, 0));
        first = false;
      }
      else
      {
        memo->Lines->Add("           DOA => sc_ram_height_" + IntToStr(j) + ");");
        if(!first) str += " or ";
        if(GetKolBitDataWhithZnak() >= (ramb_size[0] + ramb_size[1]))
          str += ("sc_ram_height_" + IntToStr(j));
        else
          str += ("sc_ram_height_" + IntToStr(j) + RAM.GetDownto(GetKolBitDataWhithZnak() - ramb_size[0] - 1, 0));
        first = false;
      }
      if(ramb_kol[i] > 1)
        if(!i)
          memo->Lines->Add("   sc_en_ram_low_" + IntToStr(j) + " <= '0' when sc_address_data" + RAM.GetDownto(GetKolBitAddress() - 1, GetShARAMB(ramb_size[0])) +
                           " = " + GetNumberBin(j , GetKolBitAddress() - GetShARAMB(ramb_size[0])) + " else '1';");
        else
           memo->Lines->Add("   sc_en_ram_height_" + IntToStr(j) + " <= '0' when sc_address_data" + RAM.GetDownto(GetKolBitAddress() - 1, GetShARAMB(ramb_size[1])) +
                           " = " + GetNumberBin(j , GetKolBitAddress() - GetShARAMB(ramb_size[1])) + " else '1';");
    }
    if(!i)
      if(GetKolBitDataWhithZnak() >= ramb_size[0])
        memo->Lines->Add("   sc_ram" + RAM.GetDownto(ramb_size[0] - 1, 0) + " <= " + str + ";");
      else
        memo->Lines->Add("   sc_ram <= " + str + ";");
    else
      if(str != "")
        memo->Lines->Add("   sc_ram" + RAM.GetDownto(GetKolBitDataWhithZnak() - 1, ramb_size[0]) + " <= " + str + ";");
  }
}
//---------------------------------------------------------------------------
void CRAM::RunRAMBHex()
{
  int size_real = ramb_size[0] + ramb_size[1];
  AnsiString strb[2]; strb[0] = strb[1] = "";
  AnsiString strh[2]; strh[0] = strh[1] = "";
  for(int i = 0; i < udata.size(); i ++)
  {
    unsigned ch = udata[i];
    for(int j = 0; j < size_real; j ++)
      if(j < ramb_size[0])
        strb[0] += (((ch >> j) & 0x00000001)? "1" : "0");
      else
        strb[1] += (((ch >> j) & 0x00000001)? "1" : "0");
  }
  for(int i = 0; i < 2; i ++)
  {
    for(int j = 1; j <= strb[i].Length(); j += 4)
    {
      int h = 0;
      for(int k = 0; (k < 4) && (k+j <= strb[i].Length()); k ++)
        h = h + ((strb[i][j+k] == '1')? (1 << k) : 0);
      strh[i] = IntToHex(h, 1) + strh[i];
    }
    int end_k = (options_proc.GetChip())? 64 : 16;
    int add_null = ramb_kol[i] * end_k * 64 - strh[i].Length();
    for(int j = 0; j < add_null; j ++)
      strh[i] = "0" + strh[i];
    for(int j = 0; j < ramb_kol[i]; j ++)
      for(int k = 0; k < end_k; k ++)
      {
        ranb_hex[i][j][k] = "";
        for(int l = j*end_k*64 + k*64; l < (j*end_k*64 + k*64 + 64); l ++)
        {
          AnsiString add = strh[i][strh[i].Length() - l];
          ranb_hex[i][j][k] = add + ranb_hex[i][j][k];
        }
      }
  }
}
//---------------------------------------------------------------------------
void CRAM::AddEnabledRamB(AnsiString str)
{
  enabled_ramb[kol_enabled_ramb++] = str;
}
//---------------------------------------------------------------------------
bool CRAM::IsEnabledRamB(AnsiString str)
{
  for(int i = 0; i < kol_enabled_ramb;i ++)
    if(enabled_ramb[i] == str)
      return true;
  return false;    
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
