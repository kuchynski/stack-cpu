//---------------------------------------------------------------------------
#ifndef CWordH
#define CWordH
#include <vcl.h>

#include "CCommandForth.h"
//---------------------------------------------------------------------------
enum TypeOfWord {twStandart, twMy, twConst, twVariable, twPort, twServiceWord, twDontNo};
class CWord
{
  private:
    TypeOfWord type;
    int kol_use;
  protected:
    AnsiString name;
    CCommandForth kod;

  public:
    CWord(AnsiString n, TypeOfWord t);
    virtual bool IsThe(AnsiString n) { return name == n;}
    virtual void AddUse() {kol_use ++;}
    int GetKolUse() {return kol_use;}
    TypeOfWord GetType() {return type;}
    AnsiString GetName() {return name;}
    virtual void SetKod(unsigned k1, unsigned k2 = 0) {kod = CCommandForth(name, k1, k2);}
    virtual CCommandForth GetKod() {return kod;}
};
//---------------------------------------------------------------------------
class CStandartWord : public CWord
{
  private:
    int kol_kod;
    bool realy;
    unsigned address;
  public:
    CStandartWord(AnsiString n, int kd, bool re = true);
    bool IsThe(AnsiString n, bool r = false);

    void SetKod(unsigned k1, unsigned k2 = 0);
    void SetAddress(unsigned a) {address = a;}

    bool IsMyCommand(unsigned c){return kod.IsMyCommand(c);}
};
//---------------------------------------------------------------------------
#endif
