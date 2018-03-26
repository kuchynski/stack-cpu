//---------------------------------------------------------------------------
#pragma hdrstop

#include "CROM.h"

CROM ROM;
//---------------------------------------------------------------------------
void CROM::Clear()
{
  functions.clear();
  size = 0;
  cur_address = 0;
  cur_row = 0;
}
//---------------------------------------------------------------------------
int CROM::GetSize()
{
  return size;
}
//---------------------------------------------------------------------------
int CROM::GetKolCommands()
{
  int ret = 0;
  for(vector <CFunction>:: iterator p = functions.begin(); p < functions.end(); p ++)
    ret += p->GetKolCommands();
  return ret;
}
//---------------------------------------------------------------------------
int CROM::GetKolCommandsForth()
{
  int ret = 0;
  for(vector <CFunction>:: iterator p = functions.begin(); p < functions.end(); p ++)
    ret += p->GetKolCommandsForth();
  return ret;
}
//---------------------------------------------------------------------------
void CROM::AddFunction(CFunction c)
{
  unsigned address_new_function = size;
  size = c.SetAddress(address_new_function);
  functions.push_back(c);
  for(vector <CFunction>:: iterator p = functions.begin(); p < functions.end(); p ++)
    p->SetCall(c.GetName(), address_new_function);
}
//---------------------------------------------------------------------------
unsigned CROM::GetKolBitAddress()
{
  int ret = size;
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
void CROM::SetKolBitData(unsigned k1_, unsigned k2_)
{
  k1 = k1_;
  k2 = k2_;
  kol_bit_data = (k1 + k2) / 2 + (k1 + k2) % 2;
}
//---------------------------------------------------------------------------
void CROM::PrintAll(TStringGrid *sg2)
{
  grid_debug_Text2 = sg2;
  grid_debug_Text2->RowCount = GetKolCommandsForth() + 1;
  if(grid_debug_Text2->RowCount > 1)
    grid_debug_Text2->FixedRows = 1;
  int kol_hex = GetKolBitAddress()/4 + ((GetKolBitAddress()%4)? 1 : 0);
  int row = 1;
  int address = 0;
  for(vector <CFunction>:: iterator p = functions.begin(); p < functions.end(); p ++)
  {
    for(int i = 0; i < p->GetKolCommandsForth(); i ++)
    {
      grid_debug_Text2->Cells[0][i+row] = "";
      grid_debug_Text2->Cells[1][i+row] = IntToHex(address, kol_hex);
      grid_debug_Text2->Cells[2][i+row] = "";
      grid_debug_Text2->Cells[3][i+row] = p->GetCommandForthName(i);
      grid_debug_Text2->Cells[4][i+row] = p->GetCommandName2(i, kol_hex);
      address += p->GetKolKod(i);
    }
    grid_debug_Text2->Cells[2][row] = p->GetName();
    row += p->GetKolCommandsForth(); 
  }
}
//---------------------------------------------------------------------------
void CROM::SetState(unsigned a)
{
  int row = 1;
  unsigned address = 0;
  cur_address = a;
  bool ok = false;
  for(vector <CFunction>:: iterator p = functions.begin(); p < functions.end(); p ++)
  {
    for(int i = 0; i < p->GetKolCommandsForth(); i ++)
    {
      if((address == cur_address))
      {
        cur_line = p->GetLine(i);
        cur_page = p->GetPage(i);
        cur_line_real = p->GetLineReal(i);
        cur_row = i+row;
      }
      address += p->GetKolKod(i);
    }
  }
}
//---------------------------------------------------------------------------
int CROM::GetAddressInLine(int w, int l)
{
  unsigned address = 0;
  for(vector <CFunction>:: iterator p = functions.begin(); p < functions.end(); p ++)
    for(int i = 0; i < p->GetKolCommandsForth(); i ++)
    {
      if((p->GetLineReal(i)) == l)
       if((p->GetPage(i) == w))
        return address;
      address += p->GetKolKod(i);
    }
  return 0;
}
//---------------------------------------------------------------------------
CCommandForth* CROM::GetCommandFort(unsigned a)
{
  for(vector <CFunction>:: iterator p = functions.begin(); p < functions.end(); p ++)
    if(p->GetCommandFort(a))
      return p->GetCommandFort(a);
  return NULL;
}
//---------------------------------------------------------------------------
bool CROM::Run(unsigned address, unsigned &command1, unsigned &command2, bool mes)
{
  int kol;
  unsigned kod1, kod2, kod;
  unsigned maska1 = 0xFFFFFFFF >> (32 - k1);
  unsigned maska2 = 0xFFFFFFFF >> (32 - k2);
  unsigned maska3 = 0xFFFFFFFF >> (32 - kol_bit_data);
  command1 = 0;
  command2 = 0;
  for(vector <CFunction>:: iterator p = functions.begin(); p < functions.end(); p ++)
    if(p->Run(address, kod1, kod2, kol))
    {
      kod1 = maska1 & kod1;
      kod2 = maska2 & kod2;
      kod = kod1 << (2*kol_bit_data - k1);
      kod = kod + kod2;
      command1 = kod >> (2*kol_bit_data - k1);
      command2 = kod & maska2;
      cur_address = address;
      return true;
    }
  AnsiString mess = "По адресу " + IntToHex((int)address,4) +
                " у меня не записано начало команды.\nНикому больше команды не дам!";
  if(mes)
    MessageBox(0, mess.c_str(), "ПЗУ", MB_ICONSTOP | MB_TASKMODAL | MB_OK);
  return false;
}
//---------------------------------------------------------------------------
unsigned CROM::GetAddressFunction(AnsiString n)
{
  for(vector <CFunction>:: iterator p = functions.begin(); p < functions.end(); p ++)
    if(p->GetName() == n)
      return p->GetAddress();
  return 0;
}
//---------------------------------------------------------------------------
void CROM::PrintVHDLSignals(TMemo *memo)
{
  switch(options_proc.GetChip())
  {
    case 0: PrintVHDLSignalsSpartan2(memo); break;
    case 1: PrintVHDLSignalsSpartan3(memo); break;
    case 2: PrintVHDLSignalsSpartan6(memo); break;
    default: return;
  }
  ramb_kol[0] = size / RAM.GetSizeRAMB(ramb_size[0]) + ((size % RAM.GetSizeRAMB(ramb_size[0]))? 1 : 0);
  ramb_kol[1] = (!ramb_size[1])? 0 : (size / RAM.GetSizeRAMB(ramb_size[1]) + ((size % RAM.GetSizeRAMB(ramb_size[1]))? 1 : 0));
  for(int i = 0; i < 2; i ++)
  {
    for(int j = 0; j < ramb_kol[i]; j ++)
    {
      if(!i)
      {
        memo->Lines->Add("   signal sc_rom_a_low_" + IntToStr(j) + ",sc_rom_b_low_" + IntToStr(j) + " : std_logic_vector" + RAM.GetDownto2(ramb_size[0] - 1, 0) + ";");
        memo->Lines->Add("   signal sc_rom_a_low_spartan6_" + IntToStr(j) + ",sc_rom_b_low_spartan6_" + IntToStr(j) + " : std_logic_vector(31 downto 0);");
        if(ramb_kol[i] > 1)
          memo->Lines->Add("   signal sc_en_rom_low_a_" + IntToStr(j) + ", sc_en_rom_low_b_" + IntToStr(j) + " : std_logic;");
//        memo->Lines->Add("   signal sc_rom_low_" + IntToStr(j) + " : std_logic_vector" + RAM.GetDownto(ramb_size[0] - 1, 0) + ";");
//        if(ramb_kol[i] > 1)
//          memo->Lines->Add("   signal sc_en_rom_low_" + IntToStr(j) + " : std_logic;");
      }
      else
      {
        memo->Lines->Add("   signal sc_rom_a_height_" + IntToStr(j) + ", sc_rom_b_height_" + IntToStr(j) + " : std_logic_vector" + RAM.GetDownto2(ramb_size[1] - 1, 0) + ";");
        memo->Lines->Add("   signal sc_rom_a_height_spartan6_" + IntToStr(j) + ",sc_rom_b_height_spartan6_" + IntToStr(j) + " : std_logic_vector(31 downto 0);");
        if(ramb_kol[i] > 1)
          memo->Lines->Add("   signal sc_en_rom_height_a_" + IntToStr(j) + ", sc_en_rom_height_b_" + IntToStr(j) + " : std_logic;");
//        memo->Lines->Add("   signal sc_rom_height_" + IntToStr(j) + " : std_logic_vector" + RAM.GetDownto(ramb_size[1] - 1, 0) + ";");
//        if(ramb_kol[i] > 1)
//          memo->Lines->Add("   signal sc_en_rom_height_" + IntToStr(j) + " : std_logic;");
      }
      if(options_proc.GetChip() < 2)
      {
        if(RAM.IsEnabledRamB("RAMB_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i])))
          continue;
        RAM.AddEnabledRamB("RAMB_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i]));
        if(!options_proc.GetChip())
        {
          memo->Lines->Add("   component RAMB4_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i]));
          memo->Lines->Add("     generic(INIT_00, INIT_01,  INIT_02,  INIT_03,  INIT_04,  INIT_05,  INIT_06,  INIT_07, INIT_08, INIT_09,  INIT_0A,  INIT_0B,  INIT_0C,  INIT_0D,  INIT_0E,  INIT_0F : bit_vector);");
          memo->Lines->Add("        port(DIA, DIB : in std_logic_vector" + RAM.GetDownto2(ramb_size[i] - 1, 0) + ";");
          memo->Lines->Add("             ENA, ENB, WEA, WEB, RSTA, RSTB, CLKA, CLKB: in std_logic; ");
          memo->Lines->Add("             ADDRA, ADDRB: in std_logic_vector" + RAM.GetDownto2(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ";");
          memo->Lines->Add("             DOA, DOB : out std_logic_vector" + RAM.GetDownto2(ramb_size[i] - 1, 0) + ");");
          memo->Lines->Add("   end component;");
        }
        else if(options_proc.GetChip() == 1)
        {
          int s = ramb_size[i] + ramb_size[i] / 8;
          memo->Lines->Add("   component RAMB16_S" + IntToStr(s) + "_S" + IntToStr(s));
          memo->Lines->Add("     generic(INIT_A, INIT_B : bit_vector;");
          memo->Lines->Add("             INIT_00, INIT_01,  INIT_02,  INIT_03,  INIT_04,  INIT_05,  INIT_06,  INIT_07, INIT_08, INIT_09,  INIT_0A,  INIT_0B,  INIT_0C,  INIT_0D,  INIT_0E,  INIT_0F : bit_vector;");
          memo->Lines->Add("             INIT_10, INIT_11,  INIT_12,  INIT_13,  INIT_14,  INIT_15,  INIT_16,  INIT_17, INIT_18, INIT_19,  INIT_1A,  INIT_1B,  INIT_1C,  INIT_1D,  INIT_1E,  INIT_1F : bit_vector;");
          memo->Lines->Add("             INIT_20, INIT_21,  INIT_22,  INIT_23,  INIT_24,  INIT_25,  INIT_26,  INIT_27, INIT_28, INIT_29,  INIT_2A,  INIT_2B,  INIT_2C,  INIT_2D,  INIT_2E,  INIT_2F : bit_vector;");
          memo->Lines->Add("             INIT_30, INIT_31,  INIT_32,  INIT_33,  INIT_34,  INIT_35,  INIT_36,  INIT_37, INIT_38, INIT_39,  INIT_3A,  INIT_3B,  INIT_3C,  INIT_3D,  INIT_3E,  INIT_3F : bit_vector;");
          if(ramb_size[i] >= 8)
            memo->Lines->Add("             INITP_00, INITP_01,  INITP_02,  INITP_03,  INITP_04,  INITP_05,  INITP_06,  INITP_07 : bit_vector;");
          memo->Lines->Add("             SRVAL_A, SRVAL_B : bit_vector := X\"0\";");
          memo->Lines->Add("             WRITE_MODE_A, WRITE_MODE_B : string := \"WRITE_FIRST\");");
          memo->Lines->Add("        port(DIA, DIB : in std_logic_vector" + RAM.GetDownto2(ramb_size[i] - 1, 0) + ";");
          if(ramb_size[i] >= 8)
            memo->Lines->Add("             DIPA, DIPB : in std_logic_vector(" + IntToStr(ramb_size[i]/8 - 1) + " downto 0);");
          memo->Lines->Add("             ENA, ENB, WEA, WEB, SSRA, SSRB, CLKA, CLKB: in std_logic; ");
          memo->Lines->Add("             ADDRA, ADDRB: in std_logic_vector" + RAM.GetDownto2(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ";");
          if(ramb_size[i] >= 8)
            memo->Lines->Add("             DOPA, DOPB : out std_logic_vector(" + IntToStr(ramb_size[i]/8 - 1) + " downto 0);");
          memo->Lines->Add("             DOA, DOB : out std_logic_vector" + RAM.GetDownto2(ramb_size[i] - 1, 0) + ");");
          memo->Lines->Add("   end component;");
        }
      }
    }
  }
  if((GetKolBitAddress() < RAM.GetShARAMB(ramb_size[0])) && ramb_kol[0])
    memo->Lines->Add("   signal sc_rom_addr_a_low, sc_rom_addr_b_low : std_logic_vector" + RAM.GetDownto2(RAM.GetShARAMB(ramb_size[0]) - 1, 0) + ";");
  if((GetKolBitAddress() < RAM.GetShARAMB(ramb_size[1])) && ramb_kol[1])
    memo->Lines->Add("   signal sc_rom_addr_a_height, sc_rom_addr_b_height : std_logic_vector" + RAM.GetDownto2(RAM.GetShARAMB(ramb_size[1]) - 1, 0) + ";");
  memo->Lines->Add("   signal sc_rom_a, sc_rom_b : std_logic_vector" + RAM.GetDownto2(kol_bit_data - 1, 0) + ";");
  memo->Lines->Add("   signal sc_rom : std_logic_vector" + RAM.GetDownto2(2*kol_bit_data - 1, 0) + ";");
  memo->Lines->Add("   signal sc_kod1 : std_logic_vector" + RAM.GetDownto2(GetKolBitKodOp() - 1, 0) + ";");
  memo->Lines->Add("   signal sc_kod2 : std_logic_vector" + RAM.GetDownto2(GetKolBitAddrressOp() - 1, 0) + ";");
}
//---------------------------------------------------------------------------
void CROM::PrintVHDLSignalsSpartan6(TMemo *memo)
{
  //ramb_size[0] = 32; ramb_size[1] = 0;
  PrintVHDLSignalsSpartan3(memo);
}
//---------------------------------------------------------------------------
void CROM::PrintVHDLSignalsSpartan3(TMemo *memo)
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
void CROM::PrintVHDLSignalsSpartan2(TMemo *memo)
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
 /* ramb_kol[0] = size / RAM.GetSizeRAMB(ramb_size[0]) + ((size % RAM.GetSizeRAMB(ramb_size[0]))? 1 : 0);
  ramb_kol[1] = (!ramb_size[1])? 0 : (size / RAM.GetSizeRAMB(ramb_size[1]) + ((size % RAM.GetSizeRAMB(ramb_size[1]))? 1 : 0));
  for(int i = 0; i < 2; i ++)
  {
    for(int j = 0; j < ramb_kol[i]; j ++)
    {
      if(!i)
      {
        memo->Lines->Add("   signal sc_rom_a_low_" + IntToStr(j) + ",sc_rom_b_low_" + IntToStr(j) + " : std_logic_vector" + RAM.GetDownto(ramb_size[0] - 1, 0) + ";");
        if(ramb_kol[i] > 1)
          memo->Lines->Add("   signal sc_en_rom_low_a_" + IntToStr(j) + ", sc_en_rom_low_b_" + IntToStr(j) + " : std_logic;");
      }
      else
      {
        memo->Lines->Add("   signal sc_rom_a_height_" + IntToStr(j) + ", sc_rom_b_height_" + IntToStr(j) + " : std_logic_vector" + RAM.GetDownto(ramb_size[1] - 1, 0) + ";");
        if(ramb_kol[i] > 1)
          memo->Lines->Add("   signal sc_en_rom_height_a_" + IntToStr(j) + ", sc_en_rom_height_b_" + IntToStr(j) + " : std_logic;");
      }
      if(RAM.IsEnabledRamB("RAMB4_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i])))
        continue;
      RAM.AddEnabledRamB("RAMB4_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i]));
      memo->Lines->Add("   component RAMB4_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i]));
      memo->Lines->Add("     generic(INIT_00, INIT_01,  INIT_02,  INIT_03,  INIT_04,  INIT_05,  INIT_06,  INIT_07, INIT_08, INIT_09,  INIT_0A,  INIT_0B,  INIT_0C,  INIT_0D,  INIT_0E,  INIT_0F : bit_vector);");
      memo->Lines->Add("      port (DIA, DIB : in std_logic_vector" + RAM.GetDownto(ramb_size[i] - 1, 0) + ";");
      memo->Lines->Add("            ENA, ENB, WEA, WEB, RSTA, RSTB, CLKA, CLKB: in std_logic; ");
      memo->Lines->Add("            ADDRA, ADDRB: in std_logic_vector" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ";");
      memo->Lines->Add("            DOA, DOB : out std_logic_vector" + RAM.GetDownto(ramb_size[i] - 1, 0) + ");");
      memo->Lines->Add("   end component;");
    }
  }
  if((GetKolBitAddress() < RAM.GetShARAMB(ramb_size[0])) && ramb_kol[0])
    memo->Lines->Add("   signal sc_rom_addr_a_low, sc_rom_addr_b_low : std_logic_vector" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[0]) - 1, 0) + ";");
  if((GetKolBitAddress() < RAM.GetShARAMB(ramb_size[1])) && ramb_kol[1])
    memo->Lines->Add("   signal sc_rom_addr_b_height, sc_rom_addr_b_height : std_logic_vector" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[1]) - 1, 0) + ";");
  memo->Lines->Add("   signal sc_rom_a, sc_rom_b : std_logic_vector" + RAM.GetDownto(kol_bit_data - 1, 0) + ";");
  memo->Lines->Add("   signal sc_rom : std_logic_vector" + RAM.GetDownto(2*kol_bit_data - 1, 0) + ";");
  memo->Lines->Add("   signal sc_kod1 : std_logic_vector" + RAM.GetDownto(GetKolBitKodOp() - 1, 0) + ";");
  memo->Lines->Add("   signal sc_kod2 : std_logic_vector" + RAM.GetDownto(GetKolBitAddrressOp() - 1, 0) + ";");
*/}
//---------------------------------------------------------------------------
void CROM::PrintVHDLProcess(TMemo *memo)
{
  if(!size)
  {
    memo->Lines->Add("   sc_kod1 <= (others => '0');");
    memo->Lines->Add("   sc_kod2 <= (others => '0');");
    return;
  }
  memo->Lines->Add("--команды нового процессора");
  RunRAMBHex();
  if((GetKolBitAddress() < RAM.GetShARAMB(ramb_size[0])) && ramb_kol[0])
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
      memo->Lines->Add("   sc_rom_addr_a_low <= (" + stack_return.GetChNULL(RAM.GetShARAMB(ramb_size[0]) - GetKolBitAddress() - kol_first_null) +
        " & sc_address_commands_next & " + stack_return.GetChNULL(kol_first_null) + ");");
      memo->Lines->Add("   sc_rom_addr_b_low <= (" + stack_return.GetChNULL(RAM.GetShARAMB(ramb_size[0]) - GetKolBitAddress() - kol_first_null) +
        " & sc_address_commands_next1 & " + stack_return.GetChNULL(kol_first_null) + ");");
    }
    else
    {
      memo->Lines->Add("   sc_rom_addr_a_low <= (" + stack_return.GetChNULL(RAM.GetShARAMB(ramb_size[0]) - GetKolBitAddress()) + " & sc_address_commands_next);");
      memo->Lines->Add("   sc_rom_addr_b_low <= (" + stack_return.GetChNULL(RAM.GetShARAMB(ramb_size[0]) - GetKolBitAddress()) + " & sc_address_commands_next1);");
    }
  }
  if((GetKolBitAddress() < RAM.GetShARAMB(ramb_size[1])) && ramb_kol[1])\
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
      memo->Lines->Add("   sc_rom_addr_a_height <= (" + stack_return.GetChNULL(RAM.GetShARAMB(ramb_size[1]) - GetKolBitAddress() - kol_first_null) +
        " & sc_address_commands_next & " + stack_return.GetChNULL(kol_first_null) + ");");
      memo->Lines->Add("   sc_rom_addr_b_height <= (" + stack_return.GetChNULL(RAM.GetShARAMB(ramb_size[1]) - GetKolBitAddress() - kol_first_null) +
        " & sc_address_commands_next & " + stack_return.GetChNULL(kol_first_null) + ");");
    }
    else
    {
      memo->Lines->Add("   sc_rom_addr_a_height <= (" + stack_return.GetChNULL(RAM.GetShARAMB(ramb_size[1]) - GetKolBitAddress()) + " & sc_address_commands_next);");
      memo->Lines->Add("   sc_rom_addr_b_height <= (" + stack_return.GetChNULL(RAM.GetShARAMB(ramb_size[1]) - GetKolBitAddress()) + " & sc_address_commands_next1);");
    }  
  }
  switch(options_proc.GetChip())
  {
    case 0: PrintVHDLProcessSpartan2(memo); break;
    case 1: PrintVHDLProcessSpartan3(memo); break;
    case 2: PrintVHDLProcessSpartan6(memo); break;
    default: return;
  }
  memo->Lines->Add("   sc_rom <= sc_rom_a & sc_rom_b;");
  memo->Lines->Add("   sc_kod1 <= sc_rom" + RAM.GetDownto(2*kol_bit_data - 1, 2*kol_bit_data - GetKolBitKodOp()) + ";");
  memo->Lines->Add("   sc_kod2 <= sc_rom" + RAM.GetDownto(GetKolBitAddrressOp() - 1, 0) + ";");
}
//---------------------------------------------------------------------------
void CROM::PrintVHDLProcessSpartan6(TMemo *memo)
{
  for(int i = 0; i < 2; i ++)
  {
    AnsiString stra = "", strb = "";
    bool first = true;
    for(int j = 0; j < ramb_kol[i]; j ++)
    {
      memo->Lines->Add("sc_rom_" + IntToStr(i) + "_" + IntToStr(j) + ":RAMB16BWER");
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
      memo->Lines->Add("       INIT_A => X\"000000000\", INIT_B => X\"000000000\",");
      memo->Lines->Add("       INIT_FILE=>\"NONE\",");
      memo->Lines->Add("       RSTTYPE=>\"ASYNC\",");
      memo->Lines->Add("       RST_PRIORITY_A=>\"SR\",");
      memo->Lines->Add("       RST_PRIORITY_B=>\"SR\",");
      memo->Lines->Add("       SIM_COLLISION_CHECK=>\"ALL\",");
      memo->Lines->Add("       SIM_DEVICE=>\"SPARTAN3ADSP\",");
      memo->Lines->Add("       SRVAL_A => X\"000000000\", SRVAL_B => X\"000000000\",");
      memo->Lines->Add("       WRITE_MODE_A=>\"WRITE_FIRST\",");
      memo->Lines->Add("       WRITE_MODE_B=>\"WRITE_FIRST\")");
      memo->Lines->Add("  port map(DIA => (others => '0'), DIB => (others => '0'),");
      memo->Lines->Add("           DIPA => (others => '0'), DIPB => (others => '0'),");
      if(ramb_kol[i] > 1)
        if(!i)
          memo->Lines->Add("           RSTA => sc_en_rom_low_a_" + IntToStr(j)+ ", RSTB => sc_en_rom_low_b_" + IntToStr(j)+ ",");
        else
          memo->Lines->Add("           RSTA => sc_en_rom_height_a_" + IntToStr(j)+ ", RSTB => sc_en_rom_height_b_" + IntToStr(j)+ ",");
      else
        memo->Lines->Add("           RSTA => '0', RSTB => '0',");

      memo->Lines->Add("           REGCEA => '0', REGCEB => '0',");
      memo->Lines->Add("           WEA => \"0000\", WEB => \"0000\", ENA => '1', ENB => '1', CLKA => clk, CLKB => clk,");
      AnsiString str_addr_a, str_addr_b;
      if(GetKolBitAddress() >= RAM.GetShARAMB(ramb_size[i]))
      {
          if(RAM.GetShARAMB(ramb_size[i]) == 14)
          {
            str_addr_a = "sc_address_commands_next" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0);
            str_addr_b = "sc_address_commands_next1" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0);
          }
          else
          {
            str_addr_a = "(" + stack_return.GetChNULL(14 - RAM.GetShARAMB(ramb_size[i])) + " & sc_address_commands_next" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0);
            str_addr_b = "(" + stack_return.GetChNULL(14 - RAM.GetShARAMB(ramb_size[i])) + " & sc_address_commands_next1" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0);
          }
       // memo->Lines->Add("           ADDRA => sc_address_commands_next" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ", ADDRB => sc_address_commands_next1" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ",");
      }
      else
      {
        if(!i)    // "(" + stack_return.GetChNULL(14 - GetShARAMB(ramb_size[i])) + " &
        {
          if(RAM.GetShARAMB(ramb_size[i]) == 14)
          {
            str_addr_a = "sc_rom_addr_a_low";
            str_addr_b = "sc_rom_addr_b_low";
          }
          else
          {
            str_addr_a = "(" + stack_return.GetChNULL(14 - RAM.GetShARAMB(ramb_size[i])) + " & sc_rom_addr_a_low)";
            str_addr_b = "(" + stack_return.GetChNULL(14 - RAM.GetShARAMB(ramb_size[i])) + " & sc_rom_addr_b_low)";
          }
          //memo->Lines->Add("           ADDRA => sc_rom_addr_a_low, ADDRB => sc_rom_addr_b_low,");
        }
        else
        {
          if(RAM.GetShARAMB(ramb_size[i]) == 14)
          {
            str_addr_a = "sc_rom_addr_a_height";
            str_addr_b = "sc_rom_addr_b_height";
          }
          else
          {
            str_addr_a = "(" + stack_return.GetChNULL(14 - RAM.GetShARAMB(ramb_size[i])) + " & sc_rom_addr_a_height)";
            str_addr_b = "(" + stack_return.GetChNULL(14 - RAM.GetShARAMB(ramb_size[i])) + " & sc_rom_addr_b_height)";
          }
//          memo->Lines->Add("           ADDRA => sc_rom_addr_a_height, ADDRB => sc_rom_addr_b_height,");
        }
      }  
      memo->Lines->Add("           ADDRA => " + str_addr_a + ", ADDRB => " + str_addr_b + ",");
      if(!i)
      {
        memo->Lines->Add("           DOA => sc_rom_a_low_spartan6_" + IntToStr(j) + ", DOB => sc_rom_b_low_spartan6_" + IntToStr(j) + ");");
        memo->Lines->Add("   sc_rom_a_low_" + IntToStr(j) + " <= sc_rom_a_low_spartan6_" + IntToStr(j) + RAM.GetDownto(ramb_size[0] - 1, 0) + ";");
        memo->Lines->Add("   sc_rom_b_low_" + IntToStr(j) + " <= sc_rom_b_low_spartan6_" + IntToStr(j) + RAM.GetDownto(ramb_size[0] - 1, 0) + ";");
        if(!first) { stra += " or "; strb += " or ";}
        if(kol_bit_data >= ramb_size[0])
        {
          stra += ("sc_rom_a_low_" + IntToStr(j));
          strb += ("sc_rom_b_low_" + IntToStr(j));
        }
        else
        {
          stra += ("sc_rom_a_low_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - 1, 0));
          strb += ("sc_rom_b_low_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - 1, 0));
        }
        first = false;
      }
      else
      {
        memo->Lines->Add("           DOA => sc_rom_a_height_spartan6_" + IntToStr(j) + ", DOB => sc_rom_b_height_spartan6_" + IntToStr(j) + ");");
        memo->Lines->Add("   sc_rom_a_height_" + IntToStr(j) + " <= sc_rom_a_height_spartan6_" + IntToStr(j) + RAM.GetDownto(ramb_size[1] - 1, 0) + ";");
        memo->Lines->Add("   sc_rom_b_height_" + IntToStr(j) + " <= sc_rom_b_height_spartan6_" + IntToStr(j) + RAM.GetDownto(ramb_size[1] - 1, 0) + ";");
        if(!first) { stra += " or "; strb += " or ";}
        if(kol_bit_data >= (ramb_size[0] + ramb_size[1]))
        {
          stra += ("sc_rom_a_height_" + IntToStr(j));
          strb += ("sc_rom_b_height_" + IntToStr(j));
        }
        else
        {
          stra += ("sc_rom_a_height_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - ramb_size[0] - 1, 0));
          strb += ("sc_rom_b_height_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - ramb_size[0] - 1, 0));
        }
        first = false;
      }
      if(ramb_kol[i] > 1)
        if(!i)
          if((GetKolBitAddress() - RAM.GetShARAMB(ramb_size[0])) > 1)
          {
            memo->Lines->Add("   sc_en_rom_low_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[0])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[0])) + " else '1';");
            memo->Lines->Add("   sc_en_rom_low_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[0])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[0])) + " else '1';");
          }
          else
          {
            memo->Lines->Add("   sc_en_rom_low_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
            memo->Lines->Add("   sc_en_rom_low_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
          }
        else
          if((GetKolBitAddress() - RAM.GetShARAMB(ramb_size[1])) > 1)
          {
            memo->Lines->Add("   sc_en_rom_height_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[1])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[1])) + " else '1';");
            memo->Lines->Add("   sc_en_rom_height_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1(" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[1])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[1])) + " else '1';");
          }
          else
          {
            memo->Lines->Add("   sc_en_rom_height_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
            memo->Lines->Add("   sc_en_rom_height_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
          }
    }
    if(!i)
      if(kol_bit_data >= ramb_size[0])
      {
        memo->Lines->Add("   sc_rom_a" + RAM.GetDownto2(ramb_size[0] - 1, 0) + " <= " + stra + ";");
        memo->Lines->Add("   sc_rom_b" + RAM.GetDownto2(ramb_size[0] - 1, 0) + " <= " + strb + ";");
      }
      else
      {
        memo->Lines->Add("   sc_rom_a <= " + stra + ";");
        memo->Lines->Add("   sc_rom_b <= " + strb + ";");
      }
    else
      if(stra != "")
      {
        memo->Lines->Add("   sc_rom_a" + RAM.GetDownto2(kol_bit_data - 1, ramb_size[0]) + " <= " + stra + ";");
        memo->Lines->Add("   sc_rom_b" + RAM.GetDownto2(kol_bit_data - 1, ramb_size[0]) + " <= " + strb + ";");
      }
  }
}
//---------------------------------------------------------------------------
void CROM::PrintVHDLProcessSpartan3(TMemo *memo)
{
  for(int i = 0; i < 2; i ++)
  {
    AnsiString stra = "", strb = "";
    bool first = true;
    for(int j = 0; j < ramb_kol[i]; j ++)
    {
      int s = ramb_size[i] + ramb_size[i] / 8;
      memo->Lines->Add("sc_rom_" + IntToStr(i) + "_" + IntToStr(j) + ":RAMB16_S" + IntToStr(s) + "_S" + IntToStr(s));
      if(ramb_size[i] < 8)
        memo->Lines->Add("   generic map(INIT_A => X\"0\", INIT_B => X\"0\",");
      else if(ramb_size[i] == 8)
        memo->Lines->Add("   generic map(INIT_A => X\"000\", INIT_B => X\"000\",");
      else if(ramb_size[i] == 16)
        memo->Lines->Add("   generic map(INIT_A => X\"00000\", INIT_B => X\"00000\",");
      else
        memo->Lines->Add("   generic map(INIT_A => X\"000000000\", INIT_B => X\"000000000\",");
      if(s >= 8)
        for(int k = 0; k < 8; k ++)
          memo->Lines->Add("       INITP_" + IntToHex(k, 2) + " => X\"0000000000000000000000000000000000000000000000000000000000000000\",");
      for(int k = 0; k < 64; k ++)
        memo->Lines->Add("       INIT_" + IntToHex(k, 2) + " => X\"" + ranb_hex[i][j][k] + "\"" + ((k != 63)? "," : ")"));
      memo->Lines->Add("  port map(DIA => (others => '0'), DIB => (others => '0'),");
      if(ramb_size[i] >= 8)
        memo->Lines->Add("           DIPA => (others => '0'), DIPB => (others => '0'),");
      if(ramb_kol[i] > 1)
        if(!i)
          memo->Lines->Add("           SSRA => sc_en_rom_low_a_" + IntToStr(j)+ ", SSRB => sc_en_rom_low_b_" + IntToStr(j)+ ",");
        else
          memo->Lines->Add("           SSRA => sc_en_rom_height_a_" + IntToStr(j)+ ", SSRB => sc_en_rom_height_b_" + IntToStr(j)+ ",");
      else
        memo->Lines->Add("           SSRA => '0', SSRB => '0',");

      memo->Lines->Add("           WEA => '0', WEB => '0', ENA => '1', ENB => '1', CLKA => clk, CLKB => clk,");
      if(GetKolBitAddress() >= RAM.GetShARAMB(ramb_size[i]))
        memo->Lines->Add("           ADDRA => sc_address_commands_next" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ", ADDRB => sc_address_commands_next1" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ",");
      else
        if(!i)
          memo->Lines->Add("           ADDRA => sc_rom_addr_a_low, ADDRB => sc_rom_addr_b_low,");
        else
          memo->Lines->Add("           ADDRA => sc_rom_addr_a_height, ADDRB => sc_rom_addr_b_height,");
      if(!i)
      {
        memo->Lines->Add("           DOA => sc_rom_a_low_" + IntToStr(j) + ", DOB => sc_rom_b_low_" + IntToStr(j) + ");");
        if(!first) { stra += " or "; strb += " or ";}
        if(kol_bit_data >= ramb_size[0])
        {
          stra += ("sc_rom_a_low_" + IntToStr(j));
          strb += ("sc_rom_b_low_" + IntToStr(j));
        }
        else
        {
          stra += ("sc_rom_a_low_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - 1, 0));
          strb += ("sc_rom_b_low_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - 1, 0));
        }
        first = false;
      }
      else
      {
        memo->Lines->Add("           DOA => sc_rom_a_height_" + IntToStr(j) + ", DOB => sc_rom_b_height_" + IntToStr(j) + ");");
        if(!first) { stra += " or "; strb += " or ";}
        if(kol_bit_data >= (ramb_size[0] + ramb_size[1]))
        {
          stra += ("sc_rom_a_height_" + IntToStr(j));
          strb += ("sc_rom_b_height_" + IntToStr(j));
        }
        else
        {
          stra += ("sc_rom_a_height_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - ramb_size[0] - 1, 0));
          strb += ("sc_rom_b_height_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - ramb_size[0] - 1, 0));
        }
        first = false;
      }
      if(ramb_kol[i] > 1)
        if(!i)
          if((GetKolBitAddress() - RAM.GetShARAMB(ramb_size[0])) > 1)
          {
            memo->Lines->Add("   sc_en_rom_low_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[0])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[0])) + " else '1';");
            memo->Lines->Add("   sc_en_rom_low_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[0])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[0])) + " else '1';");
          }
          else
          {
            memo->Lines->Add("   sc_en_rom_low_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
            memo->Lines->Add("   sc_en_rom_low_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
          }
        else
          if((GetKolBitAddress() - RAM.GetShARAMB(ramb_size[1])) > 1)
          {
            memo->Lines->Add("   sc_en_rom_height_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[1])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[1])) + " else '1';");
            memo->Lines->Add("   sc_en_rom_height_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1(" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[1])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[1])) + " else '1';");
          }
          else
          {
            memo->Lines->Add("   sc_en_rom_height_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
            memo->Lines->Add("   sc_en_rom_height_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
          }
    }
    if(!i)
      if(kol_bit_data >= ramb_size[0])
      {
        memo->Lines->Add("   sc_rom_a" + RAM.GetDownto2(ramb_size[0] - 1, 0) + " <= " + stra + ";");
        memo->Lines->Add("   sc_rom_b" + RAM.GetDownto2(ramb_size[0] - 1, 0) + " <= " + strb + ";");
      }
      else
      {
        memo->Lines->Add("   sc_rom_a <= " + stra + ";");
        memo->Lines->Add("   sc_rom_b <= " + strb + ";");
      }
    else
      if(stra != "")
      {
        memo->Lines->Add("   sc_rom_a" + RAM.GetDownto2(kol_bit_data - 1, ramb_size[0]) + " <= " + stra + ";");
        memo->Lines->Add("   sc_rom_b" + RAM.GetDownto2(kol_bit_data - 1, ramb_size[0]) + " <= " + strb + ";");
      }
  }
}
//---------------------------------------------------------------------------
void CROM::PrintVHDLProcessSpartan2(TMemo *memo)
{
  for(int i = 0; i < 2; i ++)
  {
    AnsiString stra = "", strb = "";
    bool first = true;
    for(int j = 0; j < ramb_kol[i]; j ++)
    {
      memo->Lines->Add("sc_rom_" + IntToStr(i) + "_" + IntToStr(j) + ":RAMB4_S" + IntToStr(ramb_size[i]) + "_S" + IntToStr(ramb_size[i]));
      memo->Lines->Add("   generic map(");
      for(int k = 0; k < 16; k ++)
        memo->Lines->Add("       INIT_0" + IntToHex(k, 1) + " => X\"" + ranb_hex[i][j][k] + "\"" + ((k != 15)? "," : ")"));
      memo->Lines->Add("  port map(DIA => (others => '0'), DIB => (others => '0'),");
      if(ramb_kol[i] > 1)
        if(!i)
          memo->Lines->Add("           RSTA => sc_en_rom_low_a_" + IntToStr(j)+ ", RSTB => sc_en_rom_low_b_" + IntToStr(j)+ ",");
        else
          memo->Lines->Add("           RSTA => sc_en_rom_height_a_" + IntToStr(j)+ ", RSTB => sc_en_rom_height_b_" + IntToStr(j)+ ",");
      else
        memo->Lines->Add("           RSTA => '0', RSTB => '0',");

      memo->Lines->Add("           WEA => '0', WEB => '0', ENA => '1', ENB => '1', CLKA => clk, CLKB => clk,");
      if(GetKolBitAddress() >= RAM.GetShARAMB(ramb_size[i]))
        memo->Lines->Add("           ADDRA => sc_address_commands_next" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ", ADDRB => sc_address_commands_next1" + RAM.GetDownto(RAM.GetShARAMB(ramb_size[i]) - 1, 0) + ",");
      else
        if(!i)
          memo->Lines->Add("           ADDRA => sc_rom_addr_a_low, ADDRB => sc_rom_addr_b_low,");
        else
          memo->Lines->Add("           ADDRA => sc_rom_addr_a_height, ADDRB => sc_rom_addr_b_height,");
      if(!i)
      {
        memo->Lines->Add("           DOA => sc_rom_a_low_" + IntToStr(j) + ", DOB => sc_rom_b_low_" + IntToStr(j) + ");");
        if(!first) { stra += " or "; strb += " or ";}
        if(kol_bit_data >= ramb_size[0])
        {
          stra += ("sc_rom_a_low_" + IntToStr(j));
          strb += ("sc_rom_b_low_" + IntToStr(j));
        }
        else
        {
          stra += ("sc_rom_a_low_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - 1, 0));
          strb += ("sc_rom_b_low_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - 1, 0));
        }
        first = false;
      }
      else
      {
        memo->Lines->Add("           DOA => sc_rom_a_height_" + IntToStr(j) + ", DOB => sc_rom_b_height_" + IntToStr(j) + ");");
        if(!first) { stra += " or "; strb += " or ";}
        if(kol_bit_data >= (ramb_size[0] + ramb_size[1]))
        {
          stra += ("sc_rom_a_height_" + IntToStr(j));
          strb += ("sc_rom_b_height_" + IntToStr(j));
        }
        else
        {
          stra += ("sc_rom_a_height_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - ramb_size[0] - 1, 0));
          strb += ("sc_rom_b_height_" + IntToStr(j) + RAM.GetDownto(kol_bit_data - ramb_size[0] - 1, 0));
        }
        first = false;
      }
      if(ramb_kol[i] > 1)
        if(!i)
          if((GetKolBitAddress() - RAM.GetShARAMB(ramb_size[0])) > 1)
          {
            memo->Lines->Add("   sc_en_rom_low_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[0])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[0])) + " else '1';");
            memo->Lines->Add("   sc_en_rom_low_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[0])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[0])) + " else '1';");
          }
          else
          {
            memo->Lines->Add("   sc_en_rom_low_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
            memo->Lines->Add("   sc_en_rom_low_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
          }
        else
          if((GetKolBitAddress() - RAM.GetShARAMB(ramb_size[1])) > 1)
          {
            memo->Lines->Add("   sc_en_rom_height_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[1])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[1])) + " else '1';");
            memo->Lines->Add("   sc_en_rom_height_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1(" + RAM.GetDownto(GetKolBitAddress() - 1, RAM.GetShARAMB(ramb_size[1])) +
                             " = " + RAM.GetNumberBin(j , GetKolBitAddress() - RAM.GetShARAMB(ramb_size[1])) + " else '1';");
          }
          else
          {
            memo->Lines->Add("   sc_en_rom_height_a_" + IntToStr(j) + " <= '0' when sc_address_commands_next(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
            memo->Lines->Add("   sc_en_rom_height_b_" + IntToStr(j) + " <= '0' when sc_address_commands_next1(" + IntToStr(GetKolBitAddress() - 1) + ") = " + RAM.GetNumberBin(j , 1) + " else '1';");
          }
    }
    if(!i)
      if(kol_bit_data >= ramb_size[0])
      {
        memo->Lines->Add("   sc_rom_a" + RAM.GetDownto2(ramb_size[0] - 1, 0) + " <= " + stra + ";");
        memo->Lines->Add("   sc_rom_b" + RAM.GetDownto2(ramb_size[0] - 1, 0) + " <= " + strb + ";");
      }
      else
      {
        memo->Lines->Add("   sc_rom_a <= " + stra + ";");
        memo->Lines->Add("   sc_rom_b <= " + strb + ";");
      }
    else
      if(stra != "")
      {
        memo->Lines->Add("   sc_rom_a" + RAM.GetDownto2(kol_bit_data - 1, ramb_size[0]) + " <= " + stra + ";");
        memo->Lines->Add("   sc_rom_b" + RAM.GetDownto2(kol_bit_data - 1, ramb_size[0]) + " <= " + strb + ";");
      }
  }
}
//---------------------------------------------------------------------------
void CROM::RunRAMBHex()
{
  int kol = 0;
  unsigned kod1, kod2, kod;
  unsigned maska1 = 0xFFFFFFFF >> (32 - k1);
  unsigned maska2 = 0xFFFFFFFF >> (32 - k2);
  unsigned maska3 = (0xFFFFFFFF >> (32 - kol_bit_data)) << kol_bit_data;
  unsigned maska4 = 0xFFFFFFFF >> (32 - kol_bit_data);
  unsigned ch1, ch2;
  int size_real = ramb_size[0] + ramb_size[1];
  AnsiString strb[2]; strb[0] = strb[1] = "";
  AnsiString strh[2]; strh[0] = strh[1] = "";
  for(int i = 0; i < size; i += kol)
  {
    for(vector <CFunction>:: iterator p = functions.begin(); p < functions.end(); p ++)
      if(p->Run(i, kod1, kod2, kol))
      {
        kod1 = maska1 & kod1;
        kod2 = (kol> 1)? (maska2 & kod2) : 0;
        kod = kod1 << (2*kol_bit_data - k1);
        kod = kod + kod2;
        ch1 = (kod & maska3) >> kol_bit_data;
        ch2 =  kod & maska4;
        break;
      }
    if(kol)
      for(int j = 0; j < size_real; j ++)
        if(j < ramb_size[0])
          strb[0] += (((ch1 >> j) & 0x00000001)? "1" : "0");
        else
          strb[1] += (((ch1 >> j) & 0x00000001)? "1" : "0");
    if(kol == 2)
      for(int j = 0; j < size_real; j ++)
        if(j < ramb_size[0])
          strb[0] += (((ch2 >> j) & 0x00000001)? "1" : "0");
        else
          strb[1] += (((ch2 >> j) & 0x00000001)? "1" : "0");
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
    int end_k = 16;
    if(options_proc.GetChip() == 1)
      end_k = 64;
    else if(options_proc.GetChip() == 2)
      end_k = 64;
    int re = strh[i].Length();
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
          int qwe = strh[i].Length();
          ranb_hex[i][j][k] = add + ranb_hex[i][j][k];
        }
      }  
  }
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
