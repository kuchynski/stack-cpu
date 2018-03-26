//---------------------------------------------------------------------------
#ifndef CPortH
#define CPortH

#include <vcl.h>
//---------------------------------------------------------------------------
class CPort
{
  private:
    unsigned in;
    unsigned out;
    bool enabled_in;
    bool enabled_out, enabled_out_irq;
    AnsiString name;
    bool internal;
    int kol_use;

    int size;

    unsigned Norma(unsigned ch);

  public:
    CPort(AnsiString n, int s, bool ei, bool eo, bool eoi, bool in);
    unsigned GetIn();
    unsigned GetOut();
    bool GetEnabledIn() { return enabled_in;}
    bool GetEnabledOut() { return enabled_out;}
    bool GetEnabledOutIrq() { return enabled_out_irq;}
    bool SetIn(unsigned n);
    bool SetOut(unsigned n);
    void AddUse() {kol_use++;}
    int GetUse() {return kol_use;}
    void Reset() {in = 0; out = 0;}
    AnsiString GetName(){return name;}
    int GetSize(){return size;}
    bool IsInternal() {return internal;}
};
//---------------------------------------------------------------------------
#endif
