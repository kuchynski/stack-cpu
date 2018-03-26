//---------------------------------------------------------------------------
#pragma hdrstop
#include "CColors.h"

#define color_def clBlack
#define color_comment clRed
#define color_constant clBlue
#define color_standart clGreen
#define color_chycle clTeal
//---------------------------------------------------------------------------
void CColors::SetPosition()
{
  pos_old1 = edit_text1->SelStart;
  pos_old2 = edit_text2->SelStart;
  pos_old3 = edit_text3->SelStart;
  pos_old4 = edit_text4->SelStart;
}
//---------------------------------------------------------------------------
void CColors::PressKey(char key)
{   return;
  int pos = edit_text1->SelStart;
//  edit_text->Enabled = false;
  if(pos_old1 < pos)
    Run(1, pos_old1, pos);
  else if(pos_old1 > pos)
    Run(1, pos, pos_old1);
  pos_old1 = pos;
//  edit_text->Enabled = true;
  pos = edit_text2->SelStart;
  if(pos_old2 < pos)
    Run(2, pos_old2, pos);
  else if(pos_old2 > pos)
    Run(2, pos, pos_old2);
  pos_old2 = pos;
  pos = edit_text3->SelStart;
  if(pos_old3 < pos)
    Run(3, pos_old3, pos);
  else if(pos_old3 > pos)
    Run(3, pos, pos_old3);
  pos_old3 = pos;
  pos = edit_text4->SelStart;
  if(pos_old4 < pos)
    Run(4, pos_old4, pos);
  else if(pos_old4 > pos)
    Run(4, pos, pos_old4);
  pos_old4 = pos;
}
//---------------------------------------------------------------------------
void CColors::Run(int ns, int b, int e)
{  return;
  edit_text = edit_text1;
  if(ns == 2) edit_text = edit_text2;
  else if(ns == 3) edit_text = edit_text3;
  else if(ns == 4) edit_text = edit_text4;
  strcpy(str_vydelen, edit_text->Text.c_str());
  str_vydelen[edit_text->Text.Length()] = 0;
  if(!e && strlen(str_vydelen))
    e = strlen(str_vydelen) - 1;
//  edit_text->SelStart = begin;
//  edit_text->SelLength = edit_text->Text.Length();
//  edit_text->SelAttributes->Assign(edit_text->DefAttributes);
//  edit_text->SelLength = 0;
  int pos = edit_text->SelStart;
  int begin = GetBeginWord(b-1) + 1;
  end = GetEndWord(e);
  if(end >= strlen(str_vydelen))
    end = strlen(str_vydelen) - 1;
  for(int uk = begin; uk <= end; uk++ ) /////////////////
  {
    switch(str_vydelen[uk])
    {
      case ')':
        SetAttributesColor(uk, uk+1, color_comment);
        break;
      case ' ': case '\n' : case '\r' :
        break;
      case '('://---начало коментария
//        RunComments(uk, '(');
        SetAttributesColor(uk, uk+1, color_comment);
        break;
      case '\\'://---начало коментария
     //   RunComments(uk, '\\');
        SetAttributesColor(uk, uk+1, color_comment);
        break;
      case ':'://---начало определения
      case ';'://---конец определения
        SetAttributesColor(uk, uk+1, color_chycle);
        break;
      default:
        //AnsiString word =
        RunWord(uk);
        break;
    }
  }
  edit_text->SelStart = pos;
 // edit_text->SelAttributes->Assign(edit_text->DefAttributes);
}
//---------------------------------------------------------------------------
int CColors::GetBeginWord(int k)
{
  for(int uk = k; uk; uk--)
  {
    if(IsRazdelitel2(str_vydelen[uk]))
      return uk;
  }
  return 0;
}
//---------------------------------------------------------------------------
int CColors::GetEndWord(int k)
{
  int uk;
  for(uk = k; str_vydelen[uk]; uk++)
  {
    if(IsRazdelitel2(str_vydelen[uk]))
      return uk;
  }
  return uk - 1;
}
//---------------------------------------------------------------------------
void CColors::RunComments(int &uk, char begin)
{
  int begin_comment = uk;
  if(begin == '(')
    while((str_vydelen[uk] != ')') && (uk <= end))
      uk++;
  else
    while((str_vydelen[uk] != '\n') && (uk <= end))
      uk++;
  SetAttributesColor(begin_comment, uk+1, color_comment);
}
//---------------------------------------------------------------------------
inline AnsiString CColors::RunWord(int &uk)
{                                                // = true? если нужно выделять
  int begin_word = uk;
  while(!IsRazdelitel(str_vydelen[uk]))
    uk++;
  AnsiString word = GetText(begin_word, uk);
  uk--;

  if(dictionary.ServiceWord(word))
  {
    SetAttributesColor(begin_word, uk+1, color_chycle);
    return word;
  }
  switch(dictionary.WhatIs2(word))
  {
    case twStandart:
      SetAttributesColor(begin_word, uk+1, color_standart);
      break;
    case twMy:
    case twConst:
    case twVariable:
    case twPort:
      SetAttributesColor(begin_word, uk+1, color_def);
      break;
    default:
      if(dictionary.IsConst(word))
      {
        SetAttributesColor(begin_word, uk+1, color_constant);
        break;
      }
      else
        SetAttributesColor(begin_word, uk+1, color_def);
  }
  return word;
}
//---------------------------------------------------------------------------
inline void CColors::SetAttributesColor(int begin, int end, TColor color)
{
  edit_text->SelStart = begin;
  edit_text->SelLength = end - begin;
  edit_text->SelAttributes->Color = color;
  edit_text->SelLength = 0;
}
//---------------------------------------------------------------------------
inline AnsiString CColors::GetText(int begin, int end)
{
  AnsiString ret;
  for(int i = begin; i < end && str_vydelen[i]; i ++)
    ret += str_vydelen[i];
  return ret;
}
//---------------------------------------------------------------------------
int CColors::GetNumberStr(int pos)
{
  edit_text->SelStart = pos;
  return edit_text->CaretPos.y + 1;
}
//---------------------------------------------------------------------------
inline bool CColors::IsRazdelitel(char s)
{
  return (s == ' ' || s == '(' || s == ')' || s == '\\' || s == '\n' ||
          s == '\r'|| s == ':' || s == ';' || !s);
}
//---------------------------------------------------------------------------
inline bool CColors::IsRazdelitel2(char s)
{
  return (s == ' ' || s == '\n' || s == '\r' || !s);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
