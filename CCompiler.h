//---------------------------------------------------------------------------
#ifndef CCompilerH
#define CCompilerH

#include <ComCtrls.hpp>
#include <Grids.hpp>      

#include "CLinker.h"
#include "CUart.h"
#include "CFilter.h"
#include "CTimers.h"
#include "CDelit.h"
#include "CModuls.h"
#include "CEdit.h"
//---------------------------------------------------------------------------
class CCompiler
{
  private:
    TRichEdit *edit_text;
    TRichEdit *edit_error;
    TStringGrid *string_grid_RAM;
    TStringGrid *string_grid_ports;
    TStringGrid *string_grid_uart;
    TStringGrid *string_grid_timer;
    TStringGrid *string_grid_delit;
    char str_vydelen[100000];
    bool result_compiler;
    AnsiString page_str;
    int page;
    vector <CCommandString> str_mail_word;

    void SetAttributesColor(int begin, int end, TColor color);
    void SetAttributesStyle(int begin, int end, TFontStyle style);
    bool IsRazdelitel(char s);
    bool IsRazdelitel2(char s);
    AnsiString GetText(int begin, int end);
    void RunComments(int &uk, char begin);
    void RunDefinition(int &uk);
    vector <CCommandString> RunIF(int &uk);
    vector <CCommandString> RunBEGIN(int &uk);
    vector <CCommandString> RunDO(int &uk);
    AnsiString RunWord(int &uk, bool &ok, bool vydelen = true);
    bool IsOkName(AnsiString name);

    int GetNumberStr(int pos);

    void AddLineToEditError(AnsiString name, int line, AnsiString text);
    
    void RunGridVariable();
    void RunGridPorts();
    void RunGridUart();
    void RunGridTimer();
    void RunGridDelit();
    void RunFilters();
    void RunModuls();
    void Add(vector <CCommandString> &a, vector <CCommandString> b);
    bool RunEditTextInternal(AnsiString &str);
  public:
    CCompiler();
    ~CCompiler() {}
    void Clear();
    void SetEditError(TRichEdit *er){ edit_error = er;}
    void SetStringGridRAM(TStringGrid *sgR){ string_grid_RAM = sgR;}
    void SetStringGridPorts(TStringGrid *sgp){ string_grid_ports = sgp;}
    void SetStringGridUart(TStringGrid *sgu){ string_grid_uart = sgu;}
    void SetStringGridTimer(TStringGrid *sgt){ string_grid_timer = sgt;}
    void SetStringGridDelit(TStringGrid *sgt){ string_grid_delit = sgt;}
    bool RunEditText(vector <CEdit> &e);
    void RunEditTextInWork();
};
//---------------------------------------------------------------------------
#endif
