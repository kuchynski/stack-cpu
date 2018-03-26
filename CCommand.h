//---------------------------------------------------------------------------
#ifndef CCommandH
#define CCommandH

#include <vcl.h>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
class CCommand
{
  private:
    AnsiString name;
    int kol_kod;
    unsigned kod1, kod2;
    unsigned address;

    vector<int> metka;
  public:
    CCommand(AnsiString n, unsigned k);
    CCommand():kod1(0xffffffff), kol_kod(0xffffffff){}
    CCommand(AnsiString n, unsigned k1, unsigned k2);
    ~CCommand() {metka.clear();}

    int GetKolKod() {return kol_kod;}

    void SetAddress(unsigned a) {address = a;}
    unsigned GetAddress() {return address;}
    bool SetKod2(unsigned k2);
    void SetName(AnsiString n) {name = n;}
    AnsiString GetName(int kol_hex);

    bool SetCall(unsigned k2);
    bool SetJMP(int m, unsigned k2);

    bool Run(unsigned a, unsigned &k1, unsigned &k2, int &kol);
    AnsiString GetNameCommand(unsigned a);

    bool IsMyCommand(unsigned c){return kod1 == c;}

    void SetMetka(vector<int> m){ metka = m;}
    vector<int> GetMetka() {return metka;}


    unsigned GetKod1() {return kod1;}
};
//---------------------------------------------------------------------------
#endif
