//---------------------------------------------------------------------------
#ifndef CFunctionH
#define CFunctionH

#include <vcl.h>
#include "CCommandForth.h"
//---------------------------------------------------------------------------
class CFunction
{
  private:
    AnsiString name;
    vector <CCommandForth> commands;
    unsigned address;
  public:
    CFunction(){}
    CFunction(AnsiString n, vector <CCommandForth> c);
    ~CFunction(){ commands.clear();}

    void Clear() {commands.clear();}

    int GetKolCommandsForth() {return commands.size();}
    int GetKolCommands();
    int GetKolKod(int i = -1);

    void AddCommandForth(CCommandForth c);
    AnsiString GetCommandForthName(int i);
    AnsiString GetCommandName2(int i, int kol_hex);

    unsigned SetAddress(unsigned a);
    unsigned GetAddress();
    AnsiString GetName() {return name;}

    void SetName(AnsiString n) {name = n;}

    bool SetCall(AnsiString n, unsigned k2);
    void SetJMP();

    bool Run(unsigned a, unsigned &k1, unsigned &k2, int &kol);
    AnsiString GetNameCommand(unsigned a);
    CCommandForth* GetCommandFort(unsigned a);

    int GetLine(int i);
    int GetPage(int i);
    int GetLineReal(int i);
};
//---------------------------------------------------------------------------
#endif
