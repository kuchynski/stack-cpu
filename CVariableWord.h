//---------------------------------------------------------------------------
#ifndef CVariableWordH
#define CVariableWordH

#include <vector>
#include "CWord.h"

using namespace std;
//---------------------------------------------------------------------------
class CVariableWord : public CWord
{
  private:
    vector <int> value;
    bool from_grid;
    unsigned address;
  public:
    CVariableWord(AnsiString n, TypeOfWord t, bool fg, int kol, int *v = NULL);

    bool IsFromGrid() {return from_grid;}
    int GetKolCells() {return value.size();}
    void SetKod(unsigned k1, unsigned k2 = 0);
    unsigned SetAddress(unsigned a);
    unsigned GetAddress(){ return address;}
    bool GetValue(unsigned a, int &v);
    bool SetValue(unsigned a, int v);
};
//---------------------------------------------------------------------------
#endif
