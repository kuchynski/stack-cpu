//---------------------------------------------------------------------------
#pragma hdrstop
                                              
#include "CMyWord.h"
#include "CDictionary.h"
//---------------------------------------------------------------------------
CMyWord::CMyWord(AnsiString n, vector <CCommandString> s):
         CWord(n, twMy), in_line(true)
{
  str = s;
}
//---------------------------------------------------------------------------
void CMyWord::SetKod()
{
  if(!str.size())
    return;
  kod.clear();
  is_metka.clear();
  g_kol_metok = 0;
  for(int i = 0; i < str.size(); i++)
  {
    CCommandForth cf;
    AnsiString r = str[i].GetStr();
    if(r == "изменить_индикацию")
      r = "изменить_индикацию";
    switch(dictionary.WhatIs(str[i].GetStr()))
    {
      case twStandart: case twConst: case twVariable: case twPort:
        cf = dictionary.GetKod(str[i].GetStr());
        cf.SetPageLine(str[i].GetPage(), str[i].GetLine());
        cf.SetLineReal(str[i].GetLine());
        AddKod(cf);
        break;
      case twServiceWord:
        if(str[i].GetStr() == "IF")
          i = RunIF(i);
        else if(str[i].GetStr() == "DO")
          i = RunDO(i);
        else if(str[i].GetStr() == "BEGIN")
          i = RunBEGIN(i);
        break;
      case twMy:
        RunMyWord(i); break;
    }
  }
//  in_line = !((GetKolKod()  > 3) && (GetKolUse() > 1));
  if(IRQ.IsIRQ(name))
    in_line = false;
  else
    if(is_metka.size())
      in_line = ((GetKolKod()+1)*GetKolUse() - 3*GetKolUse() - (GetKolKod()-1)) < 0;
    else
      in_line = (GetKolKod()*GetKolUse() - 3*GetKolUse() - GetKolKod()) < 0;
  in_line = false;    
  if(!in_line)
  {
    CCommandForth cf;
    cf = dictionary.GetKod("RETURN");
    cf.SetPageLine(str[str.size()-1].GetPage(), str[str.size()-1].GetLine());
    cf.SetLineReal(str[str.size()-1].GetLine());
    AddKod(cf);
  }
  else if(is_metka.size())
  {
    CCommandForth cf;
    cf = dictionary.GetKod("NOP");
    cf.SetPageLine(str[str.size()-1].GetPage(), str[str.size()-1].GetLine());
    cf.SetLineReal(str[str.size()-1].GetLine());
    AddKod(cf);
  }
}
//---------------------------------------------------------------------------
int CMyWord::RunIF(int i)
{
  int metka = g_kol_metok++;
  CCommandForth c = dictionary.GetKod("JZ");
  c.SetName("IF");
  c.SetKod2(metka);
  c.SetPageLine(str[i].GetPage(), str[i].GetLine());
  c.SetLineReal(str[i].GetLine());
  AddKod(c);
  for(i++; i < str.size(); i++)
  {
    switch(dictionary.WhatIs(str[i].GetStr()))
    {
      case twStandart: case twConst: case twVariable: case twPort:
        c = dictionary.GetKod(str[i].GetStr());
        c.SetPageLine(str[i].GetPage(), str[i].GetLine());
        c.SetLineReal(str[i].GetLine());
        AddKod(c);
        break;
      case twServiceWord:
        if(str[i].GetStr() == "IF")
          i = RunIF(i);
        else if(str[i].GetStr() == "DO")
          i = RunDO(i);
        else if(str[i].GetStr() == "BEGIN")
          i = RunBEGIN(i);
        else if(str[i].GetStr() == "ELSE")
        {
          int tmp = metka;
          metka = g_kol_metok++;
          c = dictionary.GetKod("JMP");
          c.SetName("ELSE");
          c.SetKod2(metka);
          c.SetPageLine(str[i].GetPage(), str[i].GetLine());
          c.SetLineReal(str[i].GetLine());
          AddKod(c);
          is_metka.push_back(tmp);
        }
        else if(str[i].GetStr() == "THEN")
        {
          is_metka.push_back(metka);
          return i;
        }
        break;
      case twMy: RunMyWord(i); break;
    }
  }
  return i;
}
//---------------------------------------------------------------------------
int CMyWord::RunDO(int i)
{
  int metka_do = g_kol_metok++;
  int metka_loop = -1;
  CCommandForth c = dictionary.GetKod(">R");
  is_metka.push_back(metka_do); c.SetMetka(is_metka); is_metka.clear();
  c += dictionary.GetKod(">R");
  c.SetName("DO");
  c.SetPageLine(str[i].GetPage(), str[i].GetLine());
  c.SetLineReal(str[i].GetLine());
  AddKod(c);
  for(i++; i < str.size(); i++)
  {
    switch(dictionary.WhatIs(str[i].GetStr()))
    {
      case twStandart: case twConst: case twVariable: case twPort:
        c = dictionary.GetKod(str[i].GetStr());
        c.SetPageLine(str[i].GetPage(), str[i].GetLine());
        c.SetLineReal(str[i].GetLine());
        AddKod(c);
        break;
      case twServiceWord:
        if(str[i].GetStr() == "IF")
          i = RunIF(i);
        else if(str[i].GetStr() == "DO")
          i = RunDO(i);
        else if(str[i].GetStr() == "BEGIN")
          i = RunBEGIN(i);
        else if(str[i].GetStr() == "LEAVE")
        {
          metka_loop = g_kol_metok++;
          c = dictionary.GetKod("JMP");
          c.SetName("LEAVE");
          c.SetKod2(metka_loop);
          c.SetPageLine(str[i].GetPage(), str[i].GetLine());
          c.SetLineReal(str[i].GetLine());
          AddKod(c);
        }
        else if((str[i].GetStr() == "LOOP") || (str[i].GetStr() == "+LOOP"))
        {
          if(str[i].GetStr() == "LOOP")
          {
            c = dictionary.GetKod("1");
            c += dictionary.GetKod("R>");
          }
          else
            c = dictionary.GetKod("R>");
          c += dictionary.GetKod("SWAP");
          c += dictionary.GetKod("R>");
          c += dictionary.GetKod("+");
          c += dictionary.GetKod("OVER");
          c += dictionary.GetKod("OVER");
          c += dictionary.GetKod("<");
          CCommandForth c2 = dictionary.GetKod("JZ"); c2.SetKod2(metka_do);
          c += c2;
          c += dictionary.GetKod("DROP");
          c += dictionary.GetKod("DROP");
          c.SetName("LOOP");
          c.SetPageLine(str[i].GetPage(), str[i].GetLine());
          c.SetLineReal(str[i].GetLine());
          AddKod(c);
          if(metka_loop >= 0)
            is_metka.push_back(metka_loop);
          return i;
        }
        break;
      case twMy: RunMyWord(i); break;
    }
  }
  return i;
}
//---------------------------------------------------------------------------
int CMyWord::RunBEGIN(int i)
{
  int metka_begin = g_kol_metok++;
  int metka_repeat;
  CCommandForth c;
  is_metka.push_back(metka_begin);
  for(i++; i < str.size(); i++)
  {
    switch(dictionary.WhatIs(str[i].GetStr()))
    {
      case twStandart: case twConst: case twVariable: case twPort:
        c = dictionary.GetKod(str[i].GetStr());
        c.SetPageLine(str[i].GetPage(), str[i].GetLine());
        c.SetLineReal(str[i].GetLine());
        AddKod(c);
        break;
      case twServiceWord:
        if(str[i].GetStr() == "IF")
          i = RunIF(i);
        else if(str[i].GetStr() == "DO")
          i = RunDO(i);
        else if(str[i].GetStr() == "BEGIN")
          i = RunBEGIN(i);
        else if(str[i].GetStr() == "UNTIL")
        {
          c = dictionary.GetKod("JZ");
          c.SetName("UNTIL");
          c.SetKod2(metka_begin);
          c.SetPageLine(str[i].GetPage(), str[i].GetLine());
          c.SetLineReal(str[i].GetLine());
          AddKod(c);
          return i;
        }
        else if(str[i].GetStr() == "WHILE")
        {
          c = dictionary.GetKod("JZ");
          c.SetName("WHILE");
          metka_repeat = g_kol_metok++;
          c.SetKod2(metka_repeat);
          c.SetPageLine(str[i].GetPage(), str[i].GetLine());
          c.SetLineReal(str[i].GetLine());
          AddKod(c);
        }
        else if(str[i].GetStr() == "REPEAT")
        {
          c = dictionary.GetKod("JMP");
          c.SetName("REPEAT");
          c.SetKod2(metka_begin);
          c.SetPageLine(str[i].GetPage(), str[i].GetLine());
          c.SetLineReal(str[i].GetLine());
          AddKod(c);
          is_metka.push_back(metka_repeat);
          return i;
        }
        break;
      case twMy: RunMyWord(i); break;
    }
  }
  return i;
}
//---------------------------------------------------------------------------
void CMyWord::RunMyWord(int i)
{
  if(!static_cast<CMyWord*>(dictionary.GetWord(str[i].GetStr()))->GetKolKod()) //пустое слово
    return;
  if(!static_cast<CMyWord*>(dictionary.GetWord(str[i].GetStr()))->GetInLine())
  {
    CCommandForth c = dictionary.GetKod("CALL");
    c.SetName("CALL " + str[i].GetStr());
    c.SetPageLine(str[i].GetPage(), str[i].GetLine());
    c.SetLineReal(str[i].GetLine());
    AddKod(c);
  }
  else
  {
    vector <CCommandForth> internal_kod = static_cast<CMyWord*>(dictionary.GetWord(str[i].GetStr()))->GetAllKod();
    for(vector<CCommandForth>:: iterator k = internal_kod.begin(); k < internal_kod.end(); k++)
    {
      CCommandForth c = *k;
      c.SetLineReal(str[i].GetLine());
      AddKod(c);
    }
  }
}
//---------------------------------------------------------------------------
void CMyWord::AddUse()
{
  CWord::AddUse();
  for(vector<CCommandString>:: iterator p = str.begin(); p < str.end(); p++)
    dictionary.AddUse(p->GetStr());
}
//---------------------------------------------------------------------------
int CMyWord::GetKolKod()
{
  int ret = 0;
  for(vector <CCommandForth>:: iterator p = kod.begin(); p < kod.end(); p ++)
    ret += p->GetKolKod();
  return ret;
}
//---------------------------------------------------------------------------
void CMyWord::AddKod(CCommandForth k)
{
  if(is_metka.size())
  {
    k.SetMetka(is_metka);
    is_metka.clear();
  }
  kod.push_back(k);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)

