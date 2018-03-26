//---------------------------------------------------------------------------
#pragma hdrstop
#pragma package(smart_init)
#include "CDictionary.h"

CDictionary dictionary;
//---------------------------------------------------------------------------
CDictionary::CDictionary()
{
  New();
}
//---------------------------------------------------------------------------
CDictionary::~CDictionary()
{
  standart_words.erase(standart_words.begin(), standart_words.end());
  ClearMyWords();
  ClearMyVariables();
}
//---------------------------------------------------------------------------
void CDictionary::New()
{
  standart_words.erase(standart_words.begin(), standart_words.end());

  standart_words.push_back(CStandartWord("+", 1));
  standart_words.push_back(CStandartWord("-", 1));
  standart_words.push_back(CStandartWord("*", 1));
  standart_words.push_back(CStandartWord("1+", 1));
  standart_words.push_back(CStandartWord("1-", 1));
  standart_words.push_back(CStandartWord("2+", 1));
  standart_words.push_back(CStandartWord("2-", 1));
  standart_words.push_back(CStandartWord("NOT", 1));
  standart_words.push_back(CStandartWord("OR", 1));
  standart_words.push_back(CStandartWord("AND", 1));
  standart_words.push_back(CStandartWord("XOR", 1));
  standart_words.push_back(CStandartWord("SWAP", 1));
  standart_words.push_back(CStandartWord("DUP", 1));
  standart_words.push_back(CStandartWord("OVER", 1));
  standart_words.push_back(CStandartWord("ROT", 1));
  standart_words.push_back(CStandartWord("DROP", 1));
  standart_words.push_back(CStandartWord("=", 1));
  standart_words.push_back(CStandartWord("<>", 1));
  standart_words.push_back(CStandartWord("<", 1));
  standart_words.push_back(CStandartWord(">", 1));
  standart_words.push_back(CStandartWord(">=", 1));
  standart_words.push_back(CStandartWord("<=", 1));
  standart_words.push_back(CStandartWord("0=", 1));
  standart_words.push_back(CStandartWord("0<", 1));
  standart_words.push_back(CStandartWord("0>", 1));
  standart_words.push_back(CStandartWord("MIN", 1));
  standart_words.push_back(CStandartWord("MAX", 1));
  standart_words.push_back(CStandartWord("2*", 1));
  standart_words.push_back(CStandartWord("2/", 1));
  standart_words.push_back(CStandartWord("ABS", 1));
  standart_words.push_back(CStandartWord("NEGATE", 1));
  standart_words.push_back(CStandartWord("!", 1));
  standart_words.push_back(CStandartWord("@", 1));
//  standart_words.push_back(CStandartWord("INSERT", 1));
  standart_words.push_back(CStandartWord(">R", 1));
  standart_words.push_back(CStandartWord("R>", 1));
  standart_words.push_back(CStandartWord("R@", 1));
  standart_words.push_back(CStandartWord("QUIT", 1));
  standart_words.push_back(CStandartWord("ADDRESS", 2, false));
  standart_words.push_back(CStandartWord("CONSTANT", 2, false));
  standart_words.push_back(CStandartWord("JMP", 2, false));
  standart_words.push_back(CStandartWord("JZ", 2, false));
  standart_words.push_back(CStandartWord("CALL", 2, false));
  standart_words.push_back(CStandartWord("RETURN", 1, false));
  standart_words.push_back(CStandartWord("NOP", 1, false));
//  standart_words.push_back(CStandartWord("/BEGIN", 1, false));
//  standart_words.push_back(CStandartWord("/END", 1, false));
  ClearMyWords();
  ClearMyVariables();
}
//---------------------------------------------------------------------------
TypeOfWord CDictionary::WhatIs(AnsiString name)
{
  for(vector<CStandartWord>:: iterator p = standart_words.begin();
      p < standart_words.end(); p++)
    if(p->IsThe(name))
      return twStandart;
  for(vector<CMyWord>:: iterator p = my_words.begin();
      p < my_words.end(); p++)
    if(p->IsThe(name))
      return twMy;
  for(vector<CVariableWord>:: iterator p = my_variables.begin();
      p < my_variables.end(); p++)
    if(p->IsThe(name))
      return p->GetType();
  if(ServiceWord(name))
    return twServiceWord;
  if(ports.IsPort(name))
    return twPort;  
  return twDontNo;
}
//---------------------------------------------------------------------------
TypeOfWord CDictionary::WhatIs2(AnsiString name)
{
  for(vector<CStandartWord>:: iterator p = standart_words.begin();
      p < standart_words.end(); p++)
    if(p->IsThe(name))
      return twStandart;
  if(ServiceWord(name))
    return twServiceWord;
  return twDontNo;
}
//---------------------------------------------------------------------------
void CDictionary::ClearMyWords()
{
  my_words.erase(my_words.begin(), my_words.end());
}
//---------------------------------------------------------------------------
void CDictionary::ClearMyVariables()
{
  my_variables.erase(my_variables.begin(), my_variables.end());
}
//---------------------------------------------------------------------------
bool CDictionary::AddMyWord(AnsiString name, vector <CCommandString> str)
{
  if(WhatIs(name) != twDontNo)
    return false;
  if(IsConst(name))//константа
    return false;
  my_words.push_back(CMyWord(name, str));
  return true;
}
//---------------------------------------------------------------------------
bool CDictionary::AddVariableWord(AnsiString n, TypeOfWord t, bool fg, int kol, int *value)
{
  if(WhatIs(n) != twDontNo)
    return false;
  my_variables.push_back(CVariableWord(n, t, fg, kol , value));
  return true;
}
//---------------------------------------------------------------------------
bool CDictionary::AddPort(AnsiString n, int size, bool in, bool out, bool out_irq, bool internal)
{
  if((WhatIs(n) != twDontNo) || (!in && !out))
    return false;
  ports.AddData(CPort(n, size, in, out, out_irq, internal));
  return true;
}
//---------------------------------------------------------------------------
bool CDictionary::IsConst(AnsiString name)
{
  if(name == "")
    return false;
  if((name[1] < '0' || name[1] > '9') && (name[1] != '-') && (name[1] != '+'))
    return false;
  unsigned ss = 10;
  if(name.Length() > 1)
    if((name[2] == 'x') && (name[1] == '0'))
      ss = 16;
  int begin = (ss == 10)? 2 : 3;
  for(int i = begin; i <= name.Length(); i++)
    if(ss == 10)
    {
      if(name[i] < '0' || name[i] > '9')
        return false;
    }
    else
      if((name[i] < '0' || name[i] > '9') && (name[i] < 'a' || name[i] > 'f') && (name[i] < 'A' || name[i] > 'F'))
        return false;
  return true;
}
//---------------------------------------------------------------------------
bool CDictionary::GetConstValue(AnsiString name, int &n)
{
  if(name == "")
    return false;
  n = 0;
  bool minus = false;
  int i = 1;
  if(name[1] == '-')
  {
    minus = true;
    i ++;
  }
  unsigned ss = 10;
  if(name.Length() > 1)
    if((name[2] == 'x') && (name[1] == '0'))
    {
      ss = 16;
      i += 2;
    }  
  for(; i <= name.Length(); i++)
    if(ss == 10)
    {
      if(name[i] < '0' || name[i] > '9')
        return false;
      else
        n = n*10 + (name[i] - 0x30);
    }
    else
    {
      if((name[i] >= '0') && (name[i] <= '9'))
        n = n * 16 + (name[i] - '0');
      else if((name[i] >= 'a') && (name[i] <= 'f'))
        n = n * 16 + (name[i] - 'a' + 10);
      else if((name[i] >= 'A') && (name[i] <= 'F'))
        n = n * 16 + (name[i] - 'A' + 10);
      else
        return false;
    }
  n = (minus? -n : n);
  return true;
}
//---------------------------------------------------------------------------
bool CDictionary::GetConstValueHex(AnsiString name, unsigned &n)
{
  if(name == "")
    return false;
  n = 0;
  for(int i = 1; i <= name.Length(); i++)
    if((name[i] < '0' || name[i] > '9') && (name[i] < 'A' || name[i] > 'F') && (name[i] < 'a' || name[i] > 'f'))
      return false;
    else
    {
      if(name[i] >= '0' && name[i] <= '9')
        n = n*0x10 + (name[i] - '0');
      else if(name[i] >= 'A' && name[i] <= 'F')
        n = n*0x10 + (10 + name[i] - 'A' );
      else if(name[i] >= 'a' && name[i] <= 'f')
        n = n*0x10 + (10 + name[i] - 'a' );
    }
  return true;
}
//---------------------------------------------------------------------------
CWord* CDictionary::GetWord(AnsiString name)
{
  for(vector<CStandartWord>:: iterator p = standart_words.begin(); p < standart_words.end(); p++)
    if(p->IsThe(name))
      return static_cast<CWord*>(p);
  for(vector<CMyWord>:: iterator p = my_words.begin(); p < my_words.end(); p++)
    if(p->IsThe(name))
      return static_cast<CWord*>(p);
  for(vector<CVariableWord>:: iterator p = my_variables.begin(); p < my_variables.end(); p++)
    if(p->IsThe(name))
      return static_cast<CWord*>(p);
  return static_cast<CWord*>(NULL);
}
//---------------------------------------------------------------------------
CStandartWord* CDictionary::GetStandartWord(AnsiString name)
{
  for(vector<CStandartWord>:: iterator p = standart_words.begin(); p < standart_words.end(); p++)
    if(p->IsThe(name, true))
      return p;
  return static_cast<CStandartWord*>(NULL);
}
//---------------------------------------------------------------------------
CCommandForth CDictionary::GetKod(AnsiString name)
{
  for(vector<CStandartWord>:: iterator p = standart_words.begin(); p < standart_words.end(); p++)
    if(p->IsThe(name, true))
      return p->GetKod();
  for(vector<CMyWord>:: iterator p = my_words.begin(); p < my_words.end(); p++)
    if(p->IsThe(name))
      return p->GetKod();
  for(vector<CVariableWord>:: iterator p = my_variables.begin(); p < my_variables.end(); p++)
    if(p->IsThe(name))
      return p->GetKod();
  if(ports.IsPort(name))
    return ports.GetKod(name);
  return CCommandForth();
}
//---------------------------------------------------------------------------
void CDictionary::AddUse(AnsiString name)
{
  if(WhatIs(name) == twServiceWord)
  {
    CStandartWord* w = NULL;
    if((name == "WHILE") || (name == "UNTIL") || (name == "IF"))
    {
      w = GetStandartWord("JZ"); if(w) w->AddUse();
      w = GetStandartWord("JMP"); if(w) w->AddUse();
      return;
    }
/*    else if(name == "/")
    {
      w = GetStandartWord("/BEGIN");  if(w) w->AddUse();
      w = GetStandartWord("/END");  if(w) w->AddUse();
      w = GetStandartWord("NOP");  if(w) w->AddUse();
    }*/
    else if((name == "REPEAT") || (name == "ELSE") || (name == "LEAVE"))
      w = GetStandartWord("JMP");
    else if(name == "LOOP")
      w = GetStandartWord("1+");
    else if(name == "+LOOP")
      w = GetStandartWord("+");
    if(w) w->AddUse();
    if((name == "LOOP") || (name == "+LOOP"))
    {
      w = GetStandartWord("R>"); if(w) w->AddUse();
      w = GetStandartWord(">R"); if(w) w->AddUse();
      w = GetStandartWord("SWAP"); if(w) w->AddUse();
      w = GetStandartWord("OVER"); if(w) w->AddUse();
      w = GetStandartWord("<"); if(w) w->AddUse();
      w = GetStandartWord("JZ"); if(w) w->AddUse();
      w = GetStandartWord("DROP"); if(w) w->AddUse();
    }
    return;
  }
  CWord* w = GetWord(name);
  if(w)
    w->AddUse();
  else
    ports.AddUse(name);  
}
//---------------------------------------------------------------------------
AnsiString CDictionary::GetCommandKodBin(AnsiString c)
{
  unsigned kod;
  CStandartWord* w = NULL;
  w = GetStandartWord(c);
  if(w)
  {
    CCommandForth k = w->GetKod();
    kod = k.GetKod();
    AnsiString ret = "";
    for(int i = 0; i < ROM.GetKolBitKodOp(); i ++, kod /= 2)
      ret = ((kod % 2)? "1" : "0") + ret;
    return "\"" + ret + "\"";
  }
  return " ошибка программы (извините..)";
}
//---------------------------------------------------------------------------
int CDictionary::GetKolUse(AnsiString c)
{
  for(vector<CStandartWord>:: iterator p = standart_words.begin(); p < standart_words.end(); p++)
    if(p->IsThe(c, true))
      return p->GetKolUse();
  return 0;    
}
//---------------------------------------------------------------------------
int CDictionary::RunTaskKodOp()
{
  unsigned kod = 0;
  for(vector<CStandartWord>:: iterator p = standart_words.begin(); p < standart_words.end(); p++)
    if(p->IsThe("QUIT"))
     { p->AddUse(); p->SetKod(kod); kod++;}
  for(vector<CStandartWord>:: iterator p = standart_words.begin(); p < standart_words.end(); p++)
  {
    if(p->IsThe("CALL", true)) { p->AddUse(); p->SetKod(kod++);}
    if(p->IsThe("RETURN", true)) { p->AddUse(); p->SetKod(kod++);}
    if(p->IsThe("NOP", true)) { p->AddUse(); p->SetKod(kod++);}
  }

  unsigned st_address = 0;  //RAM
  bool move = false, address = false;
  unsigned kod_move, kod_address;
  for(vector<CVariableWord>:: iterator p = my_variables.begin(); p < my_variables.end(); p++)
    if(p->GetKolUse())
    {
      if(p->GetType() == twConst)
      {
        if(!move)
        {
          kod_move = kod++;
          for(vector<CStandartWord>:: iterator p = standart_words.begin(); p < standart_words.end(); p++)
           if(p->IsThe("CONSTANT", true))
           {
             p->SetKod(kod_move);
             p->AddUse();
             break;
           }
          move = true;
        }
        p->SetKod(kod_move, st_address);//CONSTANT
      }
      else
      {
        if(!address)
        {
          kod_address = kod++;
          for(vector<CStandartWord>:: iterator p = standart_words.begin(); p < standart_words.end(); p++)
           if(p->IsThe("ADDRESS", true))
           {
             p->SetKod(kod_address);
             p->AddUse();
             break;
           }
          address = true;
        }
        p->SetKod(kod_address, st_address);  //ADDRESS
      }
      st_address += p->GetKolCells();
    }
  if(address)
    ports.SetKod(kod_address, st_address);
  else
    if(ports.GetSize())
      for(vector<CStandartWord>:: iterator p = standart_words.begin(); p < standart_words.end(); p++)
        if(p->IsThe("ADDRESS", true))
        {
          p->SetKod(kod);
          p->AddUse();
          ports.SetKod(kod++, st_address);
          break;
        }
                        //COMMAND
  for(vector<CStandartWord>:: iterator p = standart_words.begin(); p < standart_words.end(); p++)
    if(p->GetKolUse() && !p->IsThe("QUIT", true) &&
      !p->IsThe("ADDRESS", true) && !p->IsThe("CONSTANT", true) &&
      !p->IsThe("CALL", true) && !p->IsThe("RETURN", true) && !p->IsThe("NOP", true))
      p->SetKod(kod++);

  for(vector<CMyWord>:: iterator p = my_words.begin(); p < my_words.end(); p++)
    p->SetKod();

  if(kod)
    for(int i = 1; i < 33; i++)
      if(IntPower(2, i) >= kod)
      {
        kod = i;
        break;
      }
  return kod;
}
//---------------------------------------------------------------------------
bool CDictionary::ServiceWord(AnsiString word)
{
  AnsiString service_words[11] = {"IF", "THEN", "ELSE", "DO", "LOOP", "+LOOP",
                                  "WHILE", "BEGIN", "UNTIL", "LEAVE", "REPEAT"};
  for(int i = 0; i < 11; i++)
    if(service_words[i] == word)
      return true;
  return false;
}
//---------------------------------------------------------------------------
