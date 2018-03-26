//---------------------------------------------------------------------------
#ifndef CLinkerH
#define CLinkerH

#include <ComCtrls.hpp>
#include "COptions.h"
#include "CDictionary.h"
#include "CROM.h"
#include "CRAM.h"
#include "CPorts.h"
#include "CStackData.h"
#include "CALU.h"
#include "CYY.h"
#include "CCommandString.h"
//---------------------------------------------------------------------------
class CLinker
{
  private:
    TRichEdit *edit_error;

//    AnsiString str;
    vector <CCommandString> str_mail;
    vector <CMyWord> words;
    unsigned k1, k2;
    unsigned size_shina_data;

    void RunWords();
    void RunRAM();
    void RunEnabledIRQ();
    void RunROM();
    bool RunOtchet();
  public:
    CLinker(){}
    ~CLinker() {Clear();}

    void Clear();
    void AddMailWord(vector <CCommandString> s);
    bool Run(COptionsProc &options_proc);

    unsigned GetSizeShinaData() {return size_shina_data;}

    void SetEditError(TRichEdit *er){ edit_error = er;}
};
extern CLinker linker;
//---------------------------------------------------------------------------
#endif
