//---------------------------------------------------------------------------
#ifndef CYYH
#define CYYH

#include "CWord.h"
#include "CROM.h"
#include "CRAM.h"
#include "CStackData.h"
#include "CALU.h"
#include "CPorts.h"
#include "CIRQ.h"
#include "CDiv.h"
#include "CUart.h"
#include "CTimers.h"
#include "CLinker.h"
#include "CFilter.h"
//#include <controls.hpp>
//---------------------------------------------------------------------------
class CYY
{
  private:
    vector <CStandartWord> commands;
    unsigned st_commands;
    AnsiString command, command2;
    unsigned kod_op;
    unsigned address;
    int shina_data;
    int alu_data;
    int work_irq;
    bool print_state;

    AnsiString WhatTheCommand(unsigned c);
    void RunStCommands();
    void RunStackReturn();
    void RunStackData();
    void RunShinaData();
    void RunRAMAndPorts();
  public:
    CYY(){}

    void Reset();

    void SetCommands(vector <CStandartWord> c);
    int GetKolCommands() {return commands.size();}

    bool Run();
    unsigned GetStCommands() {return st_commands;}
    void SetStCommands(unsigned st);
    AnsiString GetCommand(bool real = true);

    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);

    AnsiString GetCommandKodBin(AnsiString c);
};
extern CYY YY;
//---------------------------------------------------------------------------
#endif
