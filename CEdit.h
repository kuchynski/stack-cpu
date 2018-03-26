//---------------------------------------------------------------------------
#ifndef CEditH
#define CEditH

#include <Clipbrd.hpp>
#include <vector>
using namespace std;
#include "CLine.h"
#define maxctrlz 10
//---------------------------------------------------------------------------
class CEdit
{
  public:
        CEdit(TPaintBox *i, TScrollBar *sbx,  TScrollBar *sby);
        CEdit(FILE *f, TPaintBox *i, TScrollBar *sbx,  TScrollBar *sby);
        ~CEdit();
        void LoadFromFile(FILE *f);
        void SaveToFile(FILE *f);
        void SetImage(TPaintBox *i, TScrollBar *sbx,  TScrollBar *sby, bool init = false);
        void NewTakt();
        void BeginVydelen();
        void OnMouseUp(int x, int y);
        void OnMouseDown(int x, int y);
        void OnMouseMove(int x, int y);
        void KeyPress(char &Key, bool control);
        void SetActiv(const bool a);
        bool GetActiv()const {return activ;}
        void NewScrollX();
        void NewScrollY();
        void Paint();
        void SetShift(bool sh);
        AnsiString GetText();
        void MouseScroll(int n);
  private:
        int sbx, sby;
        TPaintBox *image;
        TScrollBar *scroll_bar_x, *scroll_bar_y;
        TPanel *panel_coordinate;
        int old_cursor_x, old_cursor_y;
        vector <CLine> text, text2[maxctrlz];
        int X, Y, XCtrlZ[maxctrlz], YCtrlZ[maxctrlz]; //позиции в text
        bool cursor;
        bool activ;
        bool insert;
        unsigned vyd_x[2], vyd_y[2];
        bool mouse_down;
        bool shift, vydelen;
        int mouse_scroll;

        bool PaintScroll();
        void PaintCursor(bool t = false);
        void PaintCoordinate();
        int Getx(int pos);
        int Gety(int line);
        int GetPos(int x);
        int GetLine(int y);
        void SetVydelen();
        bool ControlBeginirgs();
        void CEdit::PaintSymbol(int x, const char &ch);
        void SetMinXY(int &minx, int &maxx, int &miny, int &maxy);
        void CtrlC();
        void CtrlV();
        void CtrlX(const bool copy = true);
        void AddStr(AnsiString str, bool sm = false);
        void SubStr();
        void ForCtrlZ();
        void CtrlZ();
        AnsiString Print(AnsiString str);
};
//---------------------------------------------------------------------------
#endif
