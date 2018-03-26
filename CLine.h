//---------------------------------------------------------------------------
#ifndef CLineH
#define CLineH

#include <stdio.h>
#include <vector>
using namespace std;
#include "CDictionary.h"
#include <ComCtrls.hpp>
#define max 200
//---------------------------------------------------------------------------
class CLine
{
  public:
        struct SFont
        {
          TColor color;
          TFontStyles style;
          SFont(const TColor &c, const TFontStyles &fs): color(c), style(fs) {}
          SFont(): color(clBlack), style() {}
        };
        static SFont font_text;
        static SFont font_number;
        static SFont font_coment;
        static SFont font_coment2;
        static SFont font_word;
        static SFont font_my;
        static TColor color_font;
        static SFont font_chycle;
        static TColor color_invert_font;
        static SFont font_invert_text;
        static int font_size;
        CLine();
        CLine(FILE *f);
        CLine(AnsiString str);
        ~CLine(){};
        void Paint(TPaintBox *image, const int &y, const int &begin, const int &sm_x, bool &ret, int b_vyd, int end_vyd = 0);
        void SaveToFile(FILE *f);
        void Overwrite(const unsigned &pos, const char &ch);
        void Insert(const unsigned &pos, const char &ch);
        void Clear(const unsigned &pos);
        int GetEndLine();
        int GetBeginLine();
        AnsiString Copy(int begin, int end = max);
        AnsiString Cut(int begin, int end = max);
        AnsiString Paste(int begin, AnsiString str, bool insert);
        int GetMax()const {return max;}
        bool IfOnlySpaceLate(int begin);
        AnsiString GetText();
  private:
        char text[max];
        SFont font[max];
        void Clear();

        AnsiString Copy(int begin, int end, bool cut);
        void SetFont(bool &ret);
        void Set(int begin, int end, SFont f);
        void RunWord(int &uk);
        inline bool IsRazdelitel(char s);
};                                                        //  fsBold, fsItalic, fsUnderline, fsStrikeOut
CLine::SFont CLine::font_text(clGreen, TFontStyles());
CLine::SFont CLine::font_number(clBlue, TFontStyles() << fsItalic);
CLine::SFont CLine::font_coment(clSilver, TFontStyles() << fsStrikeOut);
CLine::SFont CLine::font_coment2(clRed, TFontStyles());
CLine::SFont CLine::font_word(clBlack, TFontStyles() << fsBold);
CLine::SFont CLine::font_my(clBlack, TFontStyles());
TColor CLine::color_font(clWhite);
CLine::SFont CLine::font_chycle(clBlack, TFontStyles()<< fsBold);
TColor CLine::color_invert_font(clBlue);
CLine::SFont CLine::font_invert_text(clWhite, TFontStyles());
int CLine::font_size = 12;
//---------------------------------------------------------------------------
#endif
