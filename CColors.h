//---------------------------------------------------------------------------
#ifndef CColorsH
#define CColorsH

#include <ComCtrls.hpp>

#include "CUart.h"
//---------------------------------------------------------------------------
class CColors
{
  private:
    TRichEdit *edit_text1, *edit_text2, *edit_text3, *edit_text4;
    TRichEdit *edit_text;
    char str_vydelen[100000];
    int pos_old1, pos_old2, pos_old3, pos_old4;
    int end;

    void SetAttributesColor(int begin, int end, TColor color);
    bool IsRazdelitel(char s);
    bool IsRazdelitel2(char s);
    AnsiString GetText(int begin, int end);
    void RunComments(int &uk, char begin);
    AnsiString RunWord(int &uk);

    int GetBeginWord(int k);
    int GetEndWord(int k);
    int GetNumberStr(int pos);
  public:
    CColors() : edit_text1(NULL), edit_text2(NULL), edit_text3(NULL), edit_text4(NULL),
                pos_old1(0), pos_old2(0), pos_old3(0), pos_old4(0) {}
    ~CColors() {}

    void SetPosition();
    void SetEditText(TRichEdit *et1, TRichEdit *et2, TRichEdit *et3, TRichEdit *et4)
    { edit_text1 = et1; edit_text2 = et2; edit_text3 = et3; edit_text4 = et4; }
    void PressKey(char key);
    void Run(int ns, int b, int e = 0);
};
//---------------------------------------------------------------------------
#endif
 