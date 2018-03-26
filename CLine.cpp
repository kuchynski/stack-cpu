//---------------------------------------------------------------------------
#pragma hdrstop

#include "CLine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
extern CDictionary dictionary;
//---------------------------------------------------------------------------
CLine::CLine()
{
  Clear();
}
//---------------------------------------------------------------------------
CLine::CLine(FILE *f)
{
  fread(text, sizeof(text), 1, f);
}
//---------------------------------------------------------------------------
CLine::CLine(AnsiString str)
{
  int i = 0;
  int n = str.Length();
  for(; i < max && i < str.Length(); i++)
    if((str[i+1] != '\n') && (str[i+1] != '\r'))
      text[i] = str[i+1];
    else
      break;  
  for(; i < max; i++)
    text[i] = VK_SPACE;
}
//---------------------------------------------------------------------------
void CLine::Clear()
{
  for(int i = 0; i < max; i++)
    text[i] = VK_SPACE;
}
//---------------------------------------------------------------------------
AnsiString CLine::GetText()
{
  AnsiString ret = "";
  for(int i = 0; i < max; i ++)
    ret += AnsiString(text[i]);
  return ret;
}
//---------------------------------------------------------------------------
void CLine::Insert(const unsigned &pos, const char &ch)
{
  if(pos >= max)
    return;
  for(int i = max - 1; i > pos; i --)
    text[i] = text[i-1];
  text[pos] = ch;
}
//---------------------------------------------------------------------------
void CLine::Overwrite(const unsigned &pos, const char &ch)
{
  if(pos < max)
    text[pos] = ch;
}
//---------------------------------------------------------------------------
void CLine::Clear(const unsigned &pos)
{
  if(pos >= max)
    return;
  for(int i = pos; i < (max-2); i ++)
    text[i] = text[i+1];
  text[max-1] = VK_SPACE;
}
//---------------------------------------------------------------------------
void CLine::SaveToFile(FILE *f)
{
  fwrite(text, sizeof(text), 1, f);
}
//---------------------------------------------------------------------------
int CLine::GetEndLine()
{
  for(int i = max - 1; i >= 0; i --)
    if(text[i] != VK_SPACE)
      return i;
  return -1;
}
//---------------------------------------------------------------------------
int CLine::GetBeginLine()
{
  for(int i = 0; i < max; i ++)
    if(text[i] != VK_SPACE)
      return i;
  return 0;
}
//---------------------------------------------------------------------------
void CLine::SetFont(bool &ret)
{
  for(int i = 0; i < max; i++)
    font[i] = font_text;
  for(int i = 0; i < max; i++)
  {
    if(ret)
    {
      font[i] = font_coment;
      if(text[i] != ')')
        continue;
    }
    switch(text[i])
    {
      case ')':
        font[i] = font_coment;
        ret = false;
        break;
      case ' ': case '\n' : case '\r' :
        break;
      case '{': case '}' : case '.' : case ',' :
        font[i] = font_chycle;
        break;
      case '('://---начало коментария
        font[i] = font_coment;
        ret = true;
        break;
      case '\\'://---начало коментария
        if(i < max - 1)
          if(text[i+1] == '\\')
          {
            Set(i, max, font_coment2);
            i = max;
            break;
          }
        Set(i, max, font_coment);
        i = max;
        break;
      case ':'://---начало определения
      case ';'://---конец определения
        font[i] = font_chycle;
        break;
      default:
        RunWord(i);
        break;
    }
  }
}
//---------------------------------------------------------------------------
void CLine::Set(int begin, int end, SFont f)
{
  for(int i = begin; i < end; i ++)
    font[i] = f;
}
//---------------------------------------------------------------------------
inline bool CLine::IsRazdelitel(char s)
{
  return (s == ' ' || s == '(' || s == ')' || s == '\\' || s == '\n' ||
          s == '\r'|| s == ':' || s == ';' || s == '.' || s == ',' || s == '{' || s == '}' || !s);
}
//---------------------------------------------------------------------------
void CLine::RunWord(int &uk)
{
  int begin_word = uk;
  AnsiString word = "";
  while(!IsRazdelitel(text[uk]) && (uk < max))
  {
    word += AnsiString(text[uk]);
    uk++;
  }
  if(word == "")
    return;
  uk--;
  if(dictionary.ServiceWord(word))
  {
    Set(begin_word, uk+1, font_chycle);
    return;
  }
  switch(dictionary.WhatIs(word))
  {
    case twStandart:
      Set(begin_word, uk+1, font_word);
      break;
    case twMy:
  //  case twConst:
    case twVariable:
    case twPort:
      Set(begin_word, uk+1, font_my);
      break;
    default:
      if(dictionary.IsConst(word))
      {
        Set(begin_word, uk+1, font_number);
        break;
      }
      else  {}
        Set(begin_word, uk+1, font_text);
  }
}
//---------------------------------------------------------------------------
void CLine::Paint(TPaintBox *image, const int &y, const int &begin, const int &sm_x, bool &ret, int b_vyd, int end_vyd)
{
  SetFont(ret);
  double step_x = font_size - 1;
  int x, max_x = image->Width;
  for(int i = begin; i < max; i++)
  {
    x = sm_x + (i-begin)*step_x+1;
    if(x > max_x)
      break;
    if((i < b_vyd) || (i >= end_vyd))
    {
      image->Canvas->Brush->Color = color_font;
      image->Canvas->Pen->Color = color_font;
      image->Canvas->Font->Style = font[i].style;
      image->Canvas->Font->Color = font[i].color;
    }
    else
    {
      image->Canvas->Font->Style = font_invert_text.style;
      image->Canvas->Font->Color = font_invert_text.color;
      image->Canvas->Brush->Color = color_invert_font;
      image->Canvas->Pen->Color = color_invert_font;
    }
    image->Canvas->TextOut(x, y, text[i]);
    image->Canvas->MoveTo(x-1, y);
    image->Canvas->LineTo(x-1, y + CLine::font_size + 6);
  }
}
//---------------------------------------------------------------------------
AnsiString CLine::Copy(int begin, int end, bool cut)
{
  if(begin < 0) begin = 0;
  if(end > max) end = max;
  int size = end - begin;
  AnsiString ret;
  int i = begin;
  int tmp = i + size;
  for(; i < end; i++, tmp ++)
  {
    ret += text[i];
    if(cut)
      text[i] = (tmp < (max-1))? text[tmp] : VK_SPACE;
  }
  if(cut)
    for(; i < (max-1); i++, tmp ++)
      text[i] = (tmp < (max-1))? text[tmp] : VK_SPACE;
  return ret;
}
//---------------------------------------------------------------------------
AnsiString CLine::Copy(int begin, int end)
{
  return Copy(begin, end, false);
}
//---------------------------------------------------------------------------
AnsiString CLine::Cut(int begin, int end)
{
  return Copy(begin, end, true);
}
//---------------------------------------------------------------------------
AnsiString CLine::Paste(int begin, AnsiString str, bool insert)
{
  AnsiString ret = "";
  if(begin < 0) begin = 0;
  if(begin > (max-1)) return ret;
  int j = str.Length();
  int end = -1;
  for(int i = 1; i <= str.Length(); i++)
    if((str[i] == '\n') || (str[i] == '\r'))
    { end = i - 1; break;}
  if(insert)
  {
    if(end == -1)
    {
      end = j;
      for(int i = max - end - 1; i >= begin; i --)
        text[i+end] = text[i];
    }
    else
      for(int i = begin; i < max; i ++)
        ret += AnsiString(text[i]);
  }
  int k = 1;
  if(end == -1)
    end = j;
  for(int i = begin; (i < max) && (k <= end); i ++, k++)
    text[i] = str[k];
  return ret;
}
//---------------------------------------------------------------------------
bool CLine::IfOnlySpaceLate(int begin)
{
  for(int i = begin; i < max; i++)
    if(text[i] != VK_SPACE)
      return false;
  return true;
}
//---------------------------------------------------------------------------
