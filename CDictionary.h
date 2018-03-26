//---------------------------------------------------------------------------
#ifndef CDictionaryH
#define CDictionaryH

#include <vector>
#include <Math.hpp>
#include "CMyWord.h"
#include "CVariableWord.h"
#include "CPorts.h"

#include "CIRQ.h"
using namespace std;
//---------------------------------------------------------------------------
class CDictionary
{
  private:
    vector <CStandartWord> standart_words;
    vector <CMyWord> my_words;
    vector <CVariableWord> my_variables;

  public:
    CDictionary();
    ~CDictionary();
    void New();

    TypeOfWord WhatIs(AnsiString name);
    TypeOfWord WhatIs2(AnsiString name);
    void ClearMyWords();
    void ClearMyVariables();
    bool AddMyWord(AnsiString name, vector <CCommandString> str);
    bool AddVariableWord(AnsiString n, TypeOfWord t, bool fg, int kol, int *value = NULL);
    bool AddPort(AnsiString n, int size, bool in, bool out, bool out_irq, bool internal);
    bool IsConst(AnsiString name);
    bool GetConstValue(AnsiString name, int &n);
    bool GetConstValueHex(AnsiString name, unsigned &n);

    CWord* GetWord(AnsiString name);
    CStandartWord* GetStandartWord(AnsiString name);
    CCommandForth GetKod(AnsiString name);
    void AddUse(AnsiString name);
    int RunTaskKodOp();
    vector <CStandartWord> GetStandartWords() {return standart_words;}
    vector <CMyWord> GetMyWords() {return my_words;}
    vector <CVariableWord> GetMyVariables() {return my_variables;}
    bool ServiceWord(AnsiString word);

    AnsiString GetCommandKodBin(AnsiString c);
    int GetKolUse(AnsiString c);
};
extern CDictionary dictionary;
//---------------------------------------------------------------------------
#endif
