//---------------------------------------------------------------------------
#ifndef CIRQH
#define CIRQH

#include <vcl.h>
#include <vector>
#include <Grids.hpp>
#include "CROM.h"
#include "CDictionary.h"

using namespace std;
//---------------------------------------------------------------------------
class CIRQ
{
  private:
    vector<AnsiString> name;
    vector<bool> state;
    vector<bool> run;
    vector<unsigned> address;
    vector<int> priority;
    TStringGrid *grid_irq;

    int GetPriorityRun();
  public:
    CIRQ() {grid_irq = NULL;}
    ~CIRQ() {Clear();}

    void Clear() {name.clear(); state.clear(); address.clear(); priority.clear(); run.clear();}
    int GetSize() {return name.size();}
    int GetSizeLow();
    int GetSizeHigh();
    void AddIRQ(AnsiString n, int p);
    bool GetState(AnsiString n);
    void SetState(AnsiString n, bool s);
    bool GetState(int n);
    void SetState(int n, bool s);
    unsigned GetAddress(AnsiString n);
    unsigned SetAddress(AnsiString n, unsigned a);
    void DeleteIRQ(AnsiString n);
    bool IsIRQ(AnsiString n);
    void Run();
    void AddUseToFunctions();
    int NewIRQ(unsigned &a, bool real);
    void ResetIRQ(unsigned a);

    void Reset();

    void PrintAll(TStringGrid *sg);
    void PrintAll2(TStringGrid *sg);
    void PrintValue();

    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
};
extern CIRQ IRQ;
//---------------------------------------------------------------------------
#endif
