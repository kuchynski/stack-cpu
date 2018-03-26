//---------------------------------------------------------------------------
#ifndef CTimersH
#define CTimersH

#include <Grids.hpp>
#include <vcl.h>
#include <vector>
#include "CDiv.h"
#include "CPorts.h"
#include "CIRQ.h"

using namespace std;
//---------------------------------------------------------------------------
class CTimers
{
  private:
    struct STimer
    {
      AnsiString name;
      AnsiString frequency;
      unsigned size;
      AnsiString irq;
      bool state;

      STimer(AnsiString n, AnsiString f, unsigned s, AnsiString ir):
            name(n), frequency(f), size(s), irq(ir), state(false) {}
    };

    vector <STimer> modules;
    TStringGrid *grid;
  public:
    ~CTimers() {Clear();}

    void Clear() {modules.clear();}
    void Reset();

    void AddTimer(AnsiString n, AnsiString f, unsigned s, AnsiString ir);

    void NewTakt(AnsiString name_frequency);
    void NewDataPort(AnsiString name_port, unsigned d);

    void PrintAll(TStringGrid *sg);
    void PrintValue();

    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
};
extern CTimers timers;
//---------------------------------------------------------------------------
#endif
