//---------------------------------------------------------------------------
#ifndef CCommandForthH
#define CCommandForthH

#include "CCommand.h"
//---------------------------------------------------------------------------
class CCommandForth
{
  private:
    AnsiString name;
    vector <CCommand> commands;
    unsigned address;
    int page;
    int line;
    int line_real;

  public:
    CCommandForth(AnsiString n, unsigned k, AnsiString nc = "");
    CCommandForth(){}
    ~CCommandForth(){ commands.clear();}
    CCommandForth(AnsiString n, unsigned k1, unsigned k2, AnsiString nc = "");

    void Clear() {commands.clear();}
    void AddCommand(CCommand c){commands.push_back(c);}
    void AddCommandForth(CCommandForth c);
    CCommandForth operator+=(const CCommandForth &c);

    int GetKolKod();
    int GetKolCommands() {return commands.size();}

    unsigned SetAddress(unsigned a);
    unsigned GetAddress();
    bool SetKod2(unsigned k2);
    void SetName(AnsiString n) {name = n;}
    AnsiString GetName() {return name;}
    AnsiString GetAllCommandsName(int kol_hex);

    bool SetCall(AnsiString n, unsigned k2);
    bool SetJMP(int m, unsigned k2);

    bool Run(unsigned a, unsigned &k1, unsigned &k2, int &kol);
    AnsiString GetNameCommand(unsigned a);

    bool IsMyCommand(unsigned c){return commands[0].IsMyCommand(c);}
    void SetMetka(vector<int> metka);
    vector<int> GetMetka(unsigned &a);

    unsigned GetKod() { if(commands.size()) return commands[0].GetKod1(); return 0;}

    void SetPageLine(int new_page, int new_line) {page = new_page, line = new_line;}
    int GetLine() const {return line;}
    int GetPage() const {return page;}
    void SetLineReal(int new_line_real) {line_real = new_line_real;}
    int GetLineReal(void) const {return line_real;}
};
//---------------------------------------------------------------------------
#endif
