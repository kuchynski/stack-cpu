//---------------------------------------------------------------------------
#pragma hdrstop

#include "CVHDL.h"

CVHDL vhdl;
//---------------------------------------------------------------------------
void CVHDL::Run(AnsiString name, TMemo *m)
{
  memo = m;
  memo->Clear();
  memo->Lines->Add("--от Кучинского Андрея");
  memo->Lines->Add("library IEEE;");
  memo->Lines->Add("use IEEE.STD_LOGIC_1164.ALL;");
  memo->Lines->Add("use IEEE.STD_LOGIC_ARITH.ALL;");
  memo->Lines->Add("use IEEE.STD_LOGIC_UNSIGNED.ALL;");
  memo->Lines->Add("library unisim;");
  memo->Lines->Add("use unisim.vcomponents.all;");
  memo->Lines->Add("");
  memo->Lines->Add("entity " + name + " is");
  memo->Lines->Add("    Port (");
  ports.PrintVHDLPort(memo);
  uarts.PrintVHDLPort(memo);
  div_frequency.PrintVHDLPort(memo);
  memo->Lines->Add("           clk : in std_logic;");
  memo->Lines->Add("           rst : in std_logic);");
  memo->Lines->Add("end " + name + ";");
  memo->Lines->Add("");
  memo->Lines->Add("architecture arch of " + name + " is");//architecture
  YY.PrintVHDLSignals(memo);
  ports.PrintVHDLSignals(memo);
  RAM.PrintVHDLSignals(memo);
  ROM.PrintVHDLSignals(memo);
  ALU.PrintVHDLSignals(memo);
  div_frequency.PrintVHDLSignals(memo);
  timers.PrintVHDLSignals(memo);
  uarts.PrintVHDLSignals(memo);
  delits.PrintVHDLSignals(memo);
  filters.PrintVHDLSignals(memo);
//  moduls.PrintVHDLSignals(memo);
  stack_data.PrintVHDLSignals(memo);
  stack_return.PrintVHDLSignals(memo);
  IRQ.PrintVHDLSignals(memo);
  memo->Lines->Add("begin"); //begin
  YY.PrintVHDLProcess(memo);
  ports.PrintVHDLProcess(memo);
  RAM.PrintVHDLProcess(memo);
  ROM.PrintVHDLProcess(memo);
  ALU.PrintVHDLProcess(memo);
  div_frequency.PrintVHDLProcess(memo);
  timers.PrintVHDLProcess(memo);
  uarts.PrintVHDLProcess(memo);
  delits.PrintVHDLProcess(memo);
  filters.PrintVHDLProcess(memo);
//  moduls.PrintVHDLProcess(memo);
  stack_data.PrintVHDLProcess(memo);
  stack_return.PrintVHDLProcess(memo);
  IRQ.PrintVHDLProcess(memo);
  memo->Lines->Add("end arch;"); //end
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
