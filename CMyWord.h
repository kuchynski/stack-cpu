//---------------------------------------------------------------------------
#ifndef CMyWordH
#define CMyWordH

#include "CWord.h"
#include <vector>
#include "CCommandString.h"

using namespace std;
//---------------------------------------------------------------------------
class CMyWord : public CWord
{
  private:
    bool in_line;
    vector <CCommandString> str;
//    vector <AnsiString> str;
    vector <CCommandForth> kod;
    int g_kol_metok;

    vector<int> is_metka;
    void AddKod(CCommandForth k);

    int RunIF(int i);
    int RunDO(int i);
    int RunBEGIN(int i);
    void RunMyWord(int i);
  public:
    CMyWord(AnsiString n, vector <CCommandString> s);
    ~CMyWord() {is_metka.clear();}

    void AddUse();
    bool GetInLine() {return in_line;}
    void SetKod();
    int GetKolKod();
    int GetKolCommandsForth() {return kod.size();}
    vector <CCommandForth> GetAllKod() {return kod;}
};
//---------------------------------------------------------------------------
#endif
