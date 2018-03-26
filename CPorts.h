//---------------------------------------------------------------------------
#ifndef CPortsH
#define CPortsH

#include "CPort.h"
#include "CROM.h"
#include <Grids.hpp>
#include <vector>
#include "CUart.h"
#include "CTimers.h"
#include "CALU.h"
#include "CLinker.h"

using namespace std;
//---------------------------------------------------------------------------
class CPorts
{
  private:
    vector <CPort> data;
    unsigned kod_address;
    unsigned sm_address;
    int kol_irq, kol_out, kol_in;

    TStringGrid *grid_debug_ports;
    int Norma(int ch);
    AnsiString GetAddressBin(unsigned a);
  public:
    CPorts(): grid_debug_ports(NULL){}
    ~CPorts(){ Clear();}

    void Clear() {data.clear();}

    unsigned GetSize() {return data.size();}

    void AddData(CPort d);
    void DeletePort(AnsiString name);
    void AddUse(AnsiString name);
    bool IsUse(AnsiString name);

    bool GetData(unsigned address, int &ch);
    bool GetDataIn(AnsiString name, unsigned &ch);
    bool GetDataOut(AnsiString name, unsigned &ch);
    unsigned GetData(unsigned address);
    void SetData(unsigned address, unsigned d);
    void SetDataIn(unsigned address, unsigned d);
    void SetDataIn(AnsiString name, unsigned ch);
    void Reset();
    AnsiString GetDataName(unsigned address);

    void PrintAll(TStringGrid *sg);
    void PrintAllInputs(TStringGrid *sg);
    void PrintValue();
    bool IsPort(AnsiString n);

    void SetKod(unsigned k, unsigned sm) {kod_address = k; sm_address = sm;}
    CCommandForth GetKod(AnsiString name);

    void PrintVHDLPort(TMemo *memo);
    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);

    AnsiString GetAddressBin(AnsiString n);
};
extern CPorts ports;
//---------------------------------------------------------------------------
#endif
