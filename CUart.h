//---------------------------------------------------------------------------
#ifndef CUartH
#define CUartH

#include <Grids.hpp>
#include <vcl.h>
#include <vector>
#include "CDiv.h"
#include "CPorts.h"
#include "CIRQ.h"

using namespace std;
//---------------------------------------------------------------------------
class CUarts
{
  private:
    struct SUart
    {
      AnsiString name;
      AnsiString frequency;
      unsigned size;
      AnsiString port_irq_in;
      AnsiString port_irq_out;
      int state_out;

      SUart(AnsiString n, AnsiString f, unsigned s, AnsiString ir, AnsiString or):
            name(n), frequency(f), size(s), port_irq_in(ir),
            port_irq_out(or), state_out(-1) {}
    };

    vector <SUart> modules;
    bool is_in, is_out;
    TStringGrid *grid;
  public:
    ~CUarts() {Clear();}

    void Clear() {modules.clear();}
    void Reset();

    void AddUart(AnsiString n, AnsiString f, unsigned s, AnsiString ir, AnsiString or);

    void NewTakt(AnsiString name_frequency);
    void NewDataPort(AnsiString name_port);

    void PrintAll(TStringGrid *sg);
    void PrintValue();
    
    void PrintVHDLPort(TMemo *memo);
    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
};
extern CUarts uarts;
//---------------------------------------------------------------------------
#endif
