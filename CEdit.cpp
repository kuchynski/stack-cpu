//---------------------------------------------------------------------------
#pragma hdrstop

#include "CEdit.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CEdit::CEdit(TPaintBox *i, TScrollBar *sbx,  TScrollBar *sby) :
  insert(true), X(0), Y(0), mouse_down(false), shift(false), vydelen(false), activ(false)
{
  CLine::font_size = 10;
  text.push_back(CLine());
  for(int i = 0; i < maxctrlz; i ++)
  {
    text2[i] = text;
    XCtrlZ[i] = X;
    YCtrlZ[i] = Y;
  }
  SetImage(i, sbx, sby);
  mouse_scroll = 0;
  sbx = 0;
  sby = 0;
}
//---------------------------------------------------------------------------
CEdit::CEdit(FILE *f, TPaintBox *i, TScrollBar *sbx,  TScrollBar *sby) : 
  insert(true), X(0), Y(0), mouse_down(false), shift(false), vydelen(false), activ(false)
{
  CLine::font_size = 10;
  text.push_back(CLine());
  for(int i = 0; i < maxctrlz; i ++)
  {
    text2[i] = text;
    XCtrlZ[i] = X;
    YCtrlZ[i] = Y;
  }
  SetImage(i, sbx, sby);
  LoadFromFile(f);
  mouse_scroll = 0;
}
//---------------------------------------------------------------------------
CEdit::~CEdit()
{
  text.clear();
  for(int i = 0; i < maxctrlz; i ++)
    text2[i].clear();
}
//---------------------------------------------------------------------------
AnsiString CEdit::GetText()
{
  AnsiString ret = "";
  for(int i = 0; i < text.size(); i ++)
    ret += (text[i].GetText() + '\n');
  return ret;
}
//---------------------------------------------------------------------------
void CEdit::LoadFromFile(FILE *f)
{
  text.clear();
  int size;
  if(fread(&size, sizeof(size), 1, f) != 1)
    return;
  for(int i = 0; i < size; i ++)
    text.push_back(CLine(f));
  X = Y = 0;  
  for(int i = 0; i < maxctrlz; i ++)
  {
    text2[i] = text;
    XCtrlZ[i] = X;
    YCtrlZ[i] = Y;
  }
  mouse_scroll = 0;
  sbx = 0;
  sby = 0;
}
//---------------------------------------------------------------------------
void CEdit::SaveToFile(FILE *f)
{
  int size = text.size();
  if(fwrite(&size, sizeof(size), 1, f) != 1)
    return;
  for(int i = 0; i < size; i ++)
    text[i].SaveToFile(f);
}
//---------------------------------------------------------------------------
void CEdit::SetImage(TPaintBox *i, TScrollBar *sbx,  TScrollBar *sby, bool init)
{
  image = i;
  scroll_bar_x = sbx;
  scroll_bar_y = sby;
  if(init)
  {
    scroll_bar_x->Max = max;
    scroll_bar_y->Max = text.size() - 1;
    scroll_bar_x->Position = 0;
    scroll_bar_y->Position = 0;
  }  
}
//---------------------------------------------------------------------------
void CEdit::NewTakt()
{
  if(!activ)
    cursor = false;
  else
    cursor = !cursor;
  PaintCursor();
}
//---------------------------------------------------------------------------
void CEdit::SetActiv(const bool a)
{
  activ = a;
  cursor = a;
  NewTakt();
}
//---------------------------------------------------------------------------
void CEdit::PaintCursor(bool t)
{
  if(t)
    cursor = true;
  int x, y;
  x = Getx(X);
  y = Gety(Y);
  if(old_cursor_x >= 0)
  {
    if(vydelen)
    {
      int minY, maxY, minX, maxX;
      SetMinXY(minX, maxX, minY, maxY);
      int miny = Gety(minY), maxy = Gety(maxY), minx = Getx(minX), maxx = Getx(maxX);
      if((old_cursor_y > miny && old_cursor_y < maxy) ||
         (old_cursor_y == miny && old_cursor_y == maxy && old_cursor_x >= minx && old_cursor_x <= maxx) ||
         (old_cursor_y == miny && old_cursor_y < maxy && old_cursor_x > minx) ||
         (old_cursor_y > miny && old_cursor_y == maxy && old_cursor_x < maxx))
        image->Canvas->Pen->Color = CLine::color_invert_font;
      else
        image->Canvas->Pen->Color = CLine::color_font;
    }
    else
      image->Canvas->Pen->Color = CLine::color_font;
    image->Canvas->MoveTo(old_cursor_x-1, old_cursor_y+2);
    image->Canvas->LineTo(old_cursor_x-1, old_cursor_y + CLine::font_size+4);
    image->Canvas->MoveTo(old_cursor_x, old_cursor_y+2);
    image->Canvas->LineTo(old_cursor_x, old_cursor_y + CLine::font_size+4);
    old_cursor_x = -1;
  }
  if(cursor)
  {
    image->Canvas->Pen->Color = clBlack;
    old_cursor_x = x;
    old_cursor_y = y;
  }
  else
    image->Canvas->Pen->Color = CLine::color_font;
  image->Canvas->MoveTo(x-1, y+2);
  image->Canvas->LineTo(x-1, y + CLine::font_size+4);
  image->Canvas->MoveTo(x, y+2);
  image->Canvas->LineTo(x, y + CLine::font_size+4);
}
//---------------------------------------------------------------------------
void CEdit::PaintCoordinate()
{
  image->Canvas->Font->Name = "Times New Roman";
  image->Canvas->Font->Size = CLine::font_size;
  image->Canvas->Font->Style = TFontStyles();
  image->Canvas->Brush->Color = CLine::color_font;  //очищаем
  image->Canvas->FillRect(Rect(0, image->Height - 2*CLine::font_size+1, image->Width, image->Height));
  scroll_bar_y->Max = text.size() - 1;
  image->Canvas->Font->Color = clBlack;
  image->Canvas->Brush->Color = CLine::color_font;
  image->Canvas->TextOut(1, image->Height - 1.5*CLine::font_size, IntToStr(X+1) + ":" + IntToStr(Y+1));
  if(insert)
    image->Canvas->TextOut(50, image->Height - 1.5*CLine::font_size, "Insert     ");
  else
    image->Canvas->TextOut(50, image->Height - 1.5*CLine::font_size, "Overwrite");
  image->Canvas->Pen->Color = clBlack;
  image->Canvas->MoveTo(0, image->Height - 1.5*CLine::font_size);
  image->Canvas->LineTo(image->Width-1, image->Height - 1.5*CLine::font_size);
  image->Canvas->LineTo(image->Width-1, image->Height-1);
  image->Canvas->LineTo(0, image->Height-1);
  image->Canvas->LineTo(0, image->Height - 2*CLine::font_size+5);
}
//---------------------------------------------------------------------------
bool CEdit::PaintScroll()
{
  scroll_bar_y->Max = text.size() - 1;
  int kol_x = (image->Width - (CLine::font_size-1)*3) / (CLine::font_size - 1);
  int kol_y = (image->Height - 1.5*CLine::font_size) / (CLine::font_size + 4);

  bool ret = false;
  if(X < scroll_bar_x->Position)
  { scroll_bar_x->Position = X; ret = true;}
  if(X >= (scroll_bar_x->Position + kol_x))
  { scroll_bar_x->Position = X - kol_x + 1; ret = true;}
  if(Y < scroll_bar_y->Position)
  { scroll_bar_y->Position = Y; ret = true;}
  if(Y >= (scroll_bar_y->Position + kol_y))
  { scroll_bar_y->Position = Y - kol_y + 1; ret = true;}
  return ret;
}
//---------------------------------------------------------------------------
void CEdit::NewScrollX()
{
  if(scroll_bar_x->Position != sbx)
  {
    sbx = scroll_bar_x->Position;
    Paint();
  }
}
//---------------------------------------------------------------------------
void CEdit::NewScrollY()
{
  if(scroll_bar_y->Position != sby)
  {
    sby = scroll_bar_y->Position;
    Paint();
  }
}
//---------------------------------------------------------------------------
void CEdit::SetMinXY(int &minx, int &maxx, int &miny, int &maxy)
{
  miny = (vyd_y[0] < vyd_y[1])? vyd_y[0] : vyd_y[1];
  maxy = (vyd_y[0] < vyd_y[1])? vyd_y[1] : vyd_y[0];
  if(miny == maxy)
  {
    minx = (vyd_x[0] < vyd_x[1])? vyd_x[0] : vyd_x[1];
    maxx = (vyd_x[0] < vyd_x[1])? vyd_x[1] : vyd_x[0];
  }
  else if(miny < maxy)
  {
    minx = (miny < vyd_y[1])? vyd_x[0] : vyd_x[1];
    maxx = (miny < vyd_y[1])? vyd_x[1] : vyd_x[0];
  }
}
//---------------------------------------------------------------------------
void CEdit::Paint()
{
  PaintScroll();
  if(scroll_bar_x->Position != sbx)
    scroll_bar_x->Position = sbx;
  if(scroll_bar_y->Position != sby)
    scroll_bar_y->Position = sby;
  PaintCoordinate();
  image->Canvas->Brush->Color = CLine::color_font;  //очищаем
  image->Canvas->Font->Name = "Courier New";
  image->Canvas->Font->Size = CLine::font_size;
  image->Canvas->Pen->Color = clBlack;
//  int begin = 0;
//  end = text.size() - 1;
//  int e = (end > -1)? end : text.size() - 1;
  int y, last;
  int miny, maxy, minx, maxx;
  SetMinXY(minx, maxx, miny, maxy);
  bool ret = false;
  for(int i = 0; i < text.size(); i ++)
  {
    y = Gety(i);
    if(y > image->Height-2*CLine::font_size)
      break;
    if(y < 0)
      continue;
    int b_vyd = 0;
    int end_vyd = 0;
    if(vydelen)
    {
      if(i > miny && i < maxy)
      { end_vyd = text[i].GetMax();}
      else if(i == miny && i < maxy)
      { b_vyd = minx; end_vyd = text[i].GetMax();}
      else if(i > miny && i == maxy)
      { b_vyd = 0; end_vyd = maxx;}
      else if(i == miny && i == maxy)
      { b_vyd = minx; end_vyd = maxx;}
    }
    text[i].Paint(image, y, scroll_bar_x->Position, 0, ret, b_vyd, end_vyd);
    last = i;
  }
  last = text.size()-1;
//  if(y < (image->Height-1) && end != -1)
  if(y < (image->Height-2*CLine::font_size))
  {
    image->Canvas->Brush->Color = CLine::color_font;  //очищаем
    image->Canvas->FillRect(Rect(0, Gety(last+1), image->Width, image->Height -  1.5*CLine::font_size));
  }
}
//---------------------------------------------------------------------------
int CEdit::Getx(int pos)
{
  return (pos - scroll_bar_x->Position) * (CLine::font_size - 1) + 1;
}
//---------------------------------------------------------------------------
int CEdit::Gety(int line)
{
  return (line - scroll_bar_y->Position) * (CLine::font_size + 6) ;
}
//---------------------------------------------------------------------------
int CEdit::GetPos(int x)
{
  int ret = (x - 1) / (CLine::font_size - 1);
  if(ret >= 0)
    return ret + scroll_bar_x->Position;
  return 0;
}
//---------------------------------------------------------------------------
int CEdit::GetLine(int y)
{
  int ret = y / (CLine::font_size + 6) + scroll_bar_y->Position;
  if(ret < 0) ret = 0;
  if(ret >= text.size()) ret = text.size() - 1;
  return ret;
}
//---------------------------------------------------------------------------
void CEdit::SetShift(bool sh)
{
  shift = sh;
}
//---------------------------------------------------------------------------
void CEdit::BeginVydelen()
{
  vyd_x[0] = X;
  vyd_y[0] = Y;
  vyd_x[1] = X;
  vyd_y[1] = Y;
}
//---------------------------------------------------------------------------
void CEdit::SetVydelen()
{
  if(shift || mouse_down)
  {
    vyd_x[1] = X;
    vyd_y[1] = Y;
    vydelen = true;
  }
  else 
    vydelen = false;
  Paint();
}
//---------------------------------------------------------------------------
void CEdit::OnMouseDown(int x, int y)
{
  X = GetPos(x+(CLine::font_size-1)*0.5);
  Y = GetLine(y);
  mouse_down = true;
  if(!shift)
    BeginVydelen();
  Paint();
}
//---------------------------------------------------------------------------
void CEdit::OnMouseUp(int x, int y)
{
  mouse_down = false;
}
//---------------------------------------------------------------------------
void CEdit::OnMouseMove(int x, int y)
{
  if(mouse_down)
  {
    X = GetPos(x);
    Y = GetLine(y);
    if(vyd_x[1] != X || vyd_y[1] != Y)
      SetVydelen();
  }
}
//---------------------------------------------------------------------------
void CEdit::MouseScroll(int n)
{
  mouse_scroll += n;
  if(mouse_scroll == 10)
  {
    scroll_bar_y->Position ++;
     mouse_scroll = 0;
  }
  else if(mouse_scroll == -10)
  {
    scroll_bar_y->Position --;
     mouse_scroll = 0;
  }
}
//---------------------------------------------------------------------------
bool CEdit::ControlBeginirgs()
{
  int x = Getx(X) - (CLine::font_size-1)*3 + 1;
  int y = Gety(Y);
  bool ret = false;
  if(x <= 0)
  { scroll_bar_x->Position += x / (CLine::font_size - 1) - 1; ret = true;}
  else if(x >= image->Width)
  { scroll_bar_x->Position += (x / (CLine::font_size - 1))*2/3; ret = true;}
  if(y < 0)
  { scroll_bar_y->Position += y / (CLine::font_size + 4); ret = true;}
  else if(y >= (image->Height - (CLine::font_size + 4) - 1.5*CLine::font_size))
  { scroll_bar_y->Position += (((double)y-image->Height + 1.5*CLine::font_size) / ((double)CLine::font_size + 4.0) + 1.5); ret = true;}
  return ret;
}
//---------------------------------------------------------------------------
void CEdit::CtrlC()
{
  if(!vydelen)
    return;
  AnsiString str;
  int miny, maxy, minx, maxx;
  SetMinXY(minx, maxx, miny, maxy);
  if(miny == maxy)
    str = text[miny].Copy(minx, maxx);
  else
  {
    str = text[miny].Copy(minx) + "\n";
    for(int i = miny + 1; i < maxy; i ++)
      str += text[i].Copy(0) + "\n";
    str += text[maxy].Copy(0, maxx);
  }
  Clipboard()->SetTextBuf(str.c_str());
}
//---------------------------------------------------------------------------
void CEdit::CtrlV()
{
  AnsiString GetStr(AnsiString txt, int &i);
  char txt[100000];
  int n = Clipboard()->GetTextBuf(txt, 100000);
  AnsiString str = txt;
  if(n < 1)
    return;
  int i = 1;
  str += text[Y].Paste(X, GetStr(str, i), insert);
  while(i <= str.Length())
  {
    AnsiString str2 = GetStr(str, ++i);
    if(str2 != "")
      AddStr(str2);
  }
}
AnsiString GetStr(AnsiString txt, int &i)
{
  AnsiString ret = "";
  int n = txt.Length();
  for(; i <= n; i ++)
  {
    ret += txt[i];
    if((txt[i] == '\n') || (txt[i] == '\r'))
    {
      if(i != n)
        if(txt[i] == '\r')
          i ++;
      break;
    }
  }
  return ret;
}
//---------------------------------------------------------------------------
void CEdit::CtrlX(const bool copy)
{
  if(!vydelen)
    return;
  AnsiString str;
  int b = -1, e = -1;
  int miny, maxy, minx, maxx;
  SetMinXY(minx, maxx, miny, maxy);
  if(miny == maxy)
    str = text[miny].Cut(minx, maxx);
  else
  {
    str = text[miny].Cut(minx) + "\n";
    if(minx)
      b = miny + 1;
    else
    {
      b = miny;
      e = 0;
    }
    for(int i = miny + 1; i < maxy; i ++, e = 0)
      str += text[i].Cut(0) + "\n";
    str += text[maxy].Cut(0, maxx);
  }
  if(b != -1 && e != -1)
    for(int i = b; i < maxy; i ++)
      text.erase(&text[b]);
  Y = miny;
  X = minx;
  if(copy)
    Clipboard()->SetTextBuf(str.c_str());
}
//---------------------------------------------------------------------------
void CEdit::AddStr(AnsiString str, bool sm)
{
  if(sm)
  {
    int s = text[Y].GetBeginLine();
    for(int i = 0; i < s; i ++)
      str = " " + str;
    X = s;
  }
  else
    X = 0;
  if(insert)
  {
    if(Y < (text.size()-1))
      text.insert(&text[Y+1], CLine(str));
    else
      text.push_back(CLine(str));
  }
  else
    if(Y >= (text.size()-1))
      text.push_back(CLine(str));
  Y ++;
}
//---------------------------------------------------------------------------
void CEdit::SubStr()
{
  if(Y < 1 && text.size() < 2)
    return;
  ForCtrlZ();
  text[Y].Paste(X, text[Y+1].Copy(0), true);
  text.erase(&text[Y+1]);
  if(Y >=  text.size())
    Y = text.size() - 1;
}
//---------------------------------------------------------------------------
AnsiString CEdit::Print(AnsiString str)
{
  if(vydelen)
    CtrlX(false);
  return text[Y].Paste(X, str, insert);
}
//---------------------------------------------------------------------------
void CEdit::ForCtrlZ()
{
  for(int i = maxctrlz - 1; i ; i --)
  {
    text2[i] = text2[i - 1];
    XCtrlZ[i] = XCtrlZ[i - 1];
    YCtrlZ[i] = YCtrlZ[i - 1];
  }
  text2[0] = text;
  XCtrlZ[0] = X;
  YCtrlZ[0] = Y;
}
//---------------------------------------------------------------------------
void CEdit::CtrlZ()
{
  vector <CLine> text3;
  int x3, y3;
  text3 = text;
  x3 = X;
  y3 = Y;
  text = text2[0];
  X = XCtrlZ[0];
  Y = YCtrlZ[0];
  for(int i = 1; i < maxctrlz; i ++)
  {
    text2[i - 1] = text2[i];
    XCtrlZ[i - 1] = XCtrlZ[i];
    YCtrlZ[i - 1] = YCtrlZ[i];
  }
  text2[maxctrlz-1] = text3;
  XCtrlZ[maxctrlz - 1] = x3;
  YCtrlZ[maxctrlz - 1] = y3;
  text3.clear();
}
//---------------------------------------------------------------------------
void CEdit::KeyPress(char &Key, bool control)
{
  int old_X = X;
  int old_Y = Y;
  if(control)
  {
    switch(Key)
    {
      case VK_DELETE:
           if(!vydelen)
           {
             if(!text[Y].IfOnlySpaceLate(X))
             {
               ForCtrlZ();
               text[Y].Clear(X);
             }
             else
               SubStr();
           }
           else
           {
             ForCtrlZ();
             CtrlX(false);
           }
           break;
      case VK_TAB: if(X < (max-8)) { ForCtrlZ();
           Print("        "); X +=8;}
           break;
      case VK_INSERT: insert = !insert; break;
      case VK_LEFT: if(X > 0) X --;
           break;
      case VK_RIGHT: if(X < (max-1)) X ++;
           break;
      case VK_UP: if(Y > 0) Y --;
           break;
      case VK_DOWN: if(Y < (text.size()-1)) Y ++;
           break;
      case VK_HOME: if(X > 0) X = 0;
           break;
      case VK_END: X = text[Y].GetEndLine()+1;
           break;
      case VK_PRIOR:
           Y -= (image->Height / (CLine::font_size + 4));
           if(Y < 0) Y = 0;
           break;
      case VK_NEXT:
           Y += (image->Height / (CLine::font_size + 4));
           if(Y > text.size()) Y = text.size() - 1;
           break;
      default: break;
    }
  }
  else
  {
    unsigned char key = Key;
    switch(key)
    {
      case VK_BACK: if(X > 0)
           {
             ForCtrlZ();
             text[Y].Clear(--X);
           }
           break;
      case VK_RETURN:
           if(insert)
           {
             ForCtrlZ();
             AddStr(text[Y].Cut(X), true);
           }
           else if(Y < (text.size()-1))
             { Y ++; X = text[Y].GetBeginLine();}
           break;
      case 0x03 : CtrlC();
           return;
      case 0x16 : ForCtrlZ(); CtrlV();
           break;
      case 0x18 : ForCtrlZ(); CtrlX();
           break;
      case 0x1A : CtrlZ();
           break;
      default:
           if(key >= ' ' && key <= 255 && X < max - 1)
           {
             ForCtrlZ();
             Print(AnsiString(Key)); X++;
           }
    }
  }
  if((old_X != X) || (old_Y != Y))
    ControlBeginirgs();
  SetVydelen();
  PaintCursor(true);
}
//---------------------------------------------------------------------------


