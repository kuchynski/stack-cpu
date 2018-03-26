//---------------------------------------------------------------------------
#ifndef CDelitH
#define CDelitH

#include <Grids.hpp>
#include <vcl.h>
#include <vector>
#include "CDiv.h"
#include "CPorts.h"
#include "CIRQ.h"

using namespace std;
//---------------------------------------------------------------------------
class CDelits
{
  private:
    struct SDelit
    {
      AnsiString name;
   //   unsigned size;
      bool state;
  //    SDelit(AnsiString n, unsigned s): name(n), size(s), state(false) {}
      SDelit(AnsiString n): name(n), state(false) {}
    };
    vector <SDelit> modules;
    TStringGrid *grid;
  public:
    ~CDelits() {Clear();}

    void Clear() {modules.clear();}
    void Reset();

    void AddDelit(AnsiString n);
//    void AddDelit(AnsiString n, unsigned s);

    void NewTakt(AnsiString name_frequency);
    void NewDataPort(AnsiString name_port, unsigned d);

    void PrintAll(TStringGrid *sg);
    void PrintValue();

    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
};
extern CDelits delits;
//---------------------------------------------------------------------------
#endif
