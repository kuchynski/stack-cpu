//---------------------------------------------------------------------------
#pragma hdrstop

#include "CCompiler.h"
#define color_comment clRed
#define color_variable clBlue
#define color_opred clGreen
#define color_port clNavy
#define color_standart clGreen
#define style_error clItalic
#define style_do fsBold
#define style_standart fsBold
#define style_opred fsBold
//---------------------------------------------------------------------------
CCompiler::CCompiler()
{
}
//---------------------------------------------------------------------------
void CCompiler::Clear()
{
  dictionary.New();
  result_compiler = true;
}
//---------------------------------------------------------------------------
bool CCompiler::RunEditText(vector <CEdit> &e)
{
  Clear();
  linker.Clear();
  RunGridVariable();
  RunGridPorts();
  RunGridUart();
  RunGridTimer();
  RunGridDelit();
  RunFilters();
  RunModuls();
  if(IRQ.GetSizeHigh())
    dictionary.AddPort("irq_en", 1, true, true, false, true);
  if(IRQ.GetSizeLow())
    dictionary.AddPort("irq_en_low", 1, true, true, false, true);
  str_mail_word.erase(str_mail_word.begin(), str_mail_word.end());
  bool res = true;
  for(int i = 0; i < e.size(); i ++)
  {
    page_str = "странице "+IntToStr(i + 1); page = i + 1;
    if(!RunEditTextInternal(e[i].GetText()))
      res = false;
  }
  linker.AddMailWord(str_mail_word);
  str_mail_word.erase(str_mail_word.begin(), str_mail_word.end());
  return res;
}
//---------------------------------------------------------------------------
bool CCompiler::RunEditTextInternal(AnsiString &str)
{
//  edit_text = et;

  strcpy(str_vydelen, str.c_str());
  int s = str.Length();
  str_vydelen[str.Length()] = 0;

  bool ok;
  for(int uk = 0; uk < s; uk++ ) /////////////////
  {
    uk = uk;
    switch(str_vydelen[uk])
    {
      case '('://---начало коментария
        while((str_vydelen[uk] != ')') && (str_vydelen[uk])) uk++;
        break;
      case '\\'://---начало коментария
        while((str_vydelen[uk] != '\n') && (str_vydelen[uk])) uk++;
        break;
      case ':'://---начало определения
        RunDefinition(uk);
        break;
      case ';'://---конец определения
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "';' Нельзя закончить опредение, его не начиная");
        break;
      case ')'://---конец коментария
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "')' Нельзя закончить комментарий, его не начиная");
        //SetAttributesColor(uk, uk+1, color_error);
        break;
      case ' ': case '\n' : case '\r' : case '.' : case ',' : case '{' : case '}' :
        break;
      default:
        AnsiString word = RunWord(uk, ok);
        if(ok)
        {
          if(word == "IF") Add(str_mail_word, RunIF(uk));
          else if(word == "BEGIN") Add(str_mail_word, RunBEGIN(uk));
          else if(word == "DO") Add(str_mail_word, RunDO(uk));
          else if((word == "LOOP") || (word == "+LOOP") || (word == "WHILE") || (word == "REPEAT") || (word == "ELSE") || (word == "THEN") || (word == "LEAVE"))
            AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "'" + word + "' - это словечко не на своём месте.");
          else
            str_mail_word.push_back(CCommandString(word, page, GetNumberStr(uk)));
        }
        break;
    }
  }
  return result_compiler;
}
//---------------------------------------------------------------------------
int CCompiler::GetNumberStr(int pos)
{
  return pos / max + 1;
}
//---------------------------------------------------------------------------
void CCompiler::RunComments(int &uk, char begin)
{
  int begin_comment = uk;
  if(begin == '(')
    while((str_vydelen[uk] != ')') && (str_vydelen[uk]))
      uk++;
  else
    while((str_vydelen[uk] != '\n') && (str_vydelen[uk]))
      uk++;
//=  SetAttributesColor(begin_comment, uk+1, color_comment);
}
//---------------------------------------------------------------------------
vector <CCommandString> CCompiler::RunIF(int &uk)
{
  vector <CCommandString> str_ret;
  str_ret.push_back(CCommandString("IF", page, GetNumberStr(uk)));
  bool ok;
  bool enabled_else = false;
  int begin = uk;
  for(uk++; str_vydelen[uk]; uk++ )
  {
    switch(str_vydelen[uk])
    {
      case ':': case ';':
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "Явно не в тему - ';'");
        break;
      case '(':
        while((str_vydelen[uk] != ')') && (str_vydelen[uk])) uk++;
        break;
      case '\\':       
        while((str_vydelen[uk] != '\n') && (str_vydelen[uk])) uk++;
        break;
      case ')'://---конец коментария
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "')' Нельзя закончить комментарий, его не начиная");
        break;
      case ' ': case '\n' : case '\r' : case '.' : case ',' : case '{' : case '}' :
        break;
      default:
        AnsiString w = RunWord(uk, ok);
        if(w == "IF") Add(str_ret, RunIF(uk));
        else if(w == "BEGIN") Add(str_ret, RunBEGIN(uk));
        else if(w == "DO") Add(str_ret, RunDO(uk));
        else if((w == "LOOP") || (w == "+LOOP") || (w == "WHILE") || (w == "REPEAT") || (w == "UNTIL") || (w == "LEAVE"))
          AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "'" + w + "' - это словечко не на своём месте.");
        else if(w == "ELSE")
        {
          if(enabled_else)
            AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "'ELSE' не должно быть больше чем 'IF'");
          else
          {
            enabled_else = true;
            str_ret.push_back(CCommandString(w, page, GetNumberStr(uk)));
          }  
        }
        else if(w == "THEN")
        {
          str_ret.push_back(CCommandString(w, page, GetNumberStr(uk)));
          return str_ret;
        }
        else
          str_ret.push_back(CCommandString(w, page, GetNumberStr(uk)));
        break;
    }
  }    
  AddLineToEditError("(на) "+page_str, GetNumberStr(begin), "В этой строке есть 'IF', но где его 'THEN'?");
  str_ret.erase(str_ret.begin(), str_ret.end());
  return str_ret;
}
//---------------------------------------------------------------------------
vector <CCommandString> CCompiler::RunBEGIN(int &uk)
{
  vector <CCommandString> str_ret;
  str_ret.push_back(CCommandString("BEGIN", page, GetNumberStr(uk)));
  bool ok;
  bool enabled_while = false;
  int begin = uk;
  for(uk++; str_vydelen[uk]; uk++ )
  {
    switch(str_vydelen[uk])
    {
      case ':': case ';':
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "Явно не в тему - ';'");
        break;
      case '(':
        while((str_vydelen[uk] != ')') && (str_vydelen[uk])) uk++;
        break;
      case '\\':
        while((str_vydelen[uk] != '\n') && (str_vydelen[uk])) uk++;
        break;
      case ')'://---конец коментария
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "')' Нельзя закончить комментарий, его не начиная");
        break;
      case ' ': case '\n' : case '\r' : case '.' : case ',' : case '{' : case '}' :
        break;
      default:
        AnsiString w = RunWord(uk, ok);
        if(w == "IF") Add(str_ret, RunIF(uk));
        else if(w == "BEGIN") Add(str_ret, RunBEGIN(uk));
        else if(w == "DO") Add(str_ret, RunDO(uk));
        else if((w == "LOOP") || (w == "ELSE") || (w == "+LOOP") || (w == "LEAVE"))
          AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "'" + w + "' - это словечко не на своём месте.");
        else if(w == "WHILE")
        {
          if(enabled_while)
            AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "'WHILE' не должно быть больше чем 'BEGIN'");
          else
          {
            enabled_while = true;
            str_ret.push_back(CCommandString(w, page, GetNumberStr(uk)));
          }
        }
        else if(w == "REPEAT")
        {
          str_ret.push_back(CCommandString(w, page, GetNumberStr(uk)));
          return str_ret;
        }
        else if(w == "UNTIL")
        {
          if(enabled_while)
            AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "В цикле 'BEGIN ... UNTIL' нет места 'WHILE'");
          else
          {
            str_ret.push_back(CCommandString(w, page, GetNumberStr(uk)));
            return str_ret;
          }
        }
        else
          str_ret.push_back(CCommandString(w, page, GetNumberStr(uk)));
        break;
    }
  }
  AddLineToEditError("(на) "+page_str, GetNumberStr(begin), "В этой строке есть 'BEGIN', но где его 'REPEAT' или 'UNTIL'?");
  str_ret.erase(str_ret.begin(), str_ret.end());
  return str_ret;
}
//---------------------------------------------------------------------------
vector <CCommandString> CCompiler::RunDO(int &uk)
{
  vector <CCommandString> str_ret;
  str_ret.push_back(CCommandString("BEGIN", page, GetNumberStr(uk)));
  bool ok;
  int begin = uk;
  for(uk++; str_vydelen[uk]; uk++ )
  {
    switch(str_vydelen[uk])
    {
      case ':': case ';':
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "Явно не в тему - ';'");
        break;
      case '(':
        while((str_vydelen[uk] != ')') && (str_vydelen[uk])) uk++;
        break;
      case '\\':
        while((str_vydelen[uk] != '\n') && (str_vydelen[uk])) uk++;
        break;
      case ')'://---конец коментария
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "')' Нельзя закончить комментарий, его не начиная");
        break;
      case ' ': case '\n' : case '\r' : case '.' : case ',' : case '{' : case '}' :
        break;
      default:
        AnsiString w = RunWord(uk, ok);
        if(w == "IF") Add(str_ret, RunIF(uk));
        else if(w == "BEGIN") Add(str_ret, RunBEGIN(uk));
        else if(w == "DO") Add(str_ret, RunDO(uk));
        else if((w == "THEN") || (w == "ELSE") || (w == "WHILE") || (w == "REPEAT") || (w == "UNTIL"))
          AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "'" + w + "' - это словечко не на своём месте.");
        else if((w == "LOOP") || (w == "+LOOP"))
        {
          str_ret.push_back(CCommandString(w, page, GetNumberStr(uk)));
          return str_ret;
        }
        else
          str_ret.push_back(CCommandString(w, page, GetNumberStr(uk)));
        break;
    }
  }    
  AddLineToEditError("(на) "+page_str, GetNumberStr(begin), "В этой строке есть 'DO', но где его 'LOOP' или '+LOOP'?");
  str_ret.erase(str_ret.begin(), str_ret.end());
  return str_ret;
}
//---------------------------------------------------------------------------
void CCompiler::RunDefinition(int &uk)
{
  bool end = false;
  bool ok;
  int begin_new_word = -1;
  AnsiString new_word;
  vector <CCommandString> str_new_word;
//  AnsiString str_new_word;
  bool error = false;
  begin_new_word = uk;
  for(uk++; str_vydelen[uk] && !end; uk++ )
  {
    switch(str_vydelen[uk])
    {
      case ';'://---конец определения
        end = true;
        uk --;
        if(new_word != "")
        {
          if(error)
            AddLineToEditError("(на) "+page_str, GetNumberStr(begin_new_word),
            "Из-за вышеуказаных ошибок слово '" +  new_word + "' не занесено в ловарь. Не понимаюююю");
          else if(!dictionary.AddMyWord(new_word, str_new_word))//добовляем в словарь
              AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "Слово с именем '" + new_word + "' Словарь НЕ принимает");
        }
        break;
      case '(':
        while((str_vydelen[uk] != ')') && (str_vydelen[uk])) uk++;
        break;
      case '\\':
        while((str_vydelen[uk] != '\n') && (str_vydelen[uk])) uk++;
        break;
      case ':'://---начало определения
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "':' Нельзя начать опредение, не закончив предыдущее");
        break;
      case ')'://---конец коментария
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "')' Нельзя закончить комментарий, его не начиная");
        break;
      case ' ': case '\n' : case '\r' : case '.' : case ',' : case '{' : case '}' :
        break;
      default:
        if(new_word == "")//первое слово
        {
          begin_new_word = uk;
          new_word = RunWord(uk, ok, false);
        }
        else
        {
          AnsiString w = RunWord(uk, ok);
          if(!ok) error = true;
          else
          {
            if(w == "IF") Add(str_new_word, RunIF(uk));
            else if(w == "BEGIN") Add(str_new_word, RunBEGIN(uk));
            else if(w == "DO") Add(str_new_word, RunDO(uk));
            else if((w == "LOOP") || (w == "+LOOP") || (w == "WHILE") || (w == "REPEAT") || (w == "ELSE") || (w == "THEN") || (w == "LEAVE"))
              AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "'" + w + "' - это словечко не на своём месте.");
            else
              str_new_word.push_back(CCommandString(w, page, GetNumberStr(uk)));
            break;
          }  
        }  
    }
  }
  if(!end)
    if(new_word != "")
      if(error)
         AddLineToEditError("(на) "+page_str, GetNumberStr(begin_new_word),
         "Из-за вышеуказаных ошибок слово '" +  new_word + "' не оределено");
      else
          AddLineToEditError("(на) "+page_str, GetNumberStr(begin_new_word), "Нет конца определения слова '" + new_word + "'");
    else
      AddLineToEditError("(на) "+page_str, GetNumberStr(begin_new_word), "Нет конца определения слова.. (имени слова так же нет)");
  str_new_word.erase(str_new_word.begin(), str_new_word.end());
}
//---------------------------------------------------------------------------
void CCompiler::Add(vector <CCommandString> &a, vector <CCommandString> b)
{
  for(vector<CCommandString>:: iterator p = b.begin(); p < b.end(); p++)
    a.push_back(*p);
}
//---------------------------------------------------------------------------
inline AnsiString CCompiler::RunWord(int &uk, bool &ok, bool vydelen)
{                                                // = true? если нужно выделять
  int begin_word = uk;
  while(!IsRazdelitel(str_vydelen[uk]))
    uk++;
  AnsiString word = GetText(begin_word, uk);
  uk--;

  if(dictionary.ServiceWord(word) && vydelen)
  {
    ok = vydelen;
    return word;
  }
  switch(dictionary.WhatIs(word))
  {
    case twStandart:
      if(!vydelen)
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "Слово ФОРТ с именем '" + word + "' изночально в словаре. Определяйте другие!");
//=      SetAttributesColor(begin_word, uk+1, color_standart);
//=      SetAttributesStyle(begin_word, uk+1, style_standart);
      ok = vydelen;
      break;
    case twMy:
      if(!vydelen)
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "Слово ФОРТ с именем '"  + word + "' уже в словаре");
      ok = vydelen;
      break;
    case twConst:
    case twVariable:
      if(!vydelen)
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "Cуществует ячейка памяти '" + word + "'");
//=      SetAttributesColor(begin_word, uk+1, color_variable);
      ok = vydelen;
      break;
    case twPort:
      if(!vydelen)
        AddLineToEditError("(на) "+page_str,  GetNumberStr(uk), "Есть такой порт '" + word + "'");
//=      SetAttributesColor(begin_word, uk+1, color_port);
      ok = vydelen;
      break;
    //case twDelit:
    //  ok = vydelen;
//      word = delits.GetText
  //    break;
    default:
      if(dictionary.IsConst(word))
      {
        int value;
        dictionary.GetConstValue(word, value);
        if(vydelen)
          ok = dictionary.AddVariableWord(word, twConst, false, 1, &value);
        else
          ok = false;
//=        SetAttributesColor(begin_word, uk+1, color_variable);
        break;
      }
      ok = false;
      if(vydelen)
        AddLineToEditError("(на) "+page_str, GetNumberStr(uk), "Слово '"  + word + "' отсутствует в словаре");
  }
  return word;
}
//---------------------------------------------------------------------------
inline bool CCompiler::IsRazdelitel(char s)
{
  return (s == ' ' || s == '(' || s == ')' || s == '\\' || s == '\n' ||
          s == '\r'|| s == ':' || s == ';' || s == '.' || s == ',' || s == '{' || s == '}' || !s);
}
//---------------------------------------------------------------------------
inline bool CCompiler::IsRazdelitel2(char s)
{
  return (s == ' ' || s == '\n' || s == '\r' || s == '.' || s == ',' || s == '{' || s == '}' || !s);
}
//---------------------------------------------------------------------------
inline AnsiString CCompiler::GetText(int begin, int end)
{
  AnsiString ret;
  for(int i = begin; i < end && str_vydelen[i]; i ++)
    ret += str_vydelen[i];
  return ret;
}
//---------------------------------------------------------------------------
inline void CCompiler::SetAttributesColor(int begin, int end, TColor color)
{
  edit_text->SelStart = begin;
  edit_text->SelLength = end - begin;
  edit_text->SelAttributes->Color = color;
  edit_text->SelLength = 0;
}
//---------------------------------------------------------------------------
inline void CCompiler::SetAttributesStyle(int begin, int end, TFontStyle style)
{
  edit_text->SelStart = begin;
  edit_text->SelLength = end - begin;
  edit_text->SelAttributes->Style = edit_text->SelAttributes->Style << style;
  edit_text->SelLength = 0;
  //{ fsBold, fsItalic, fsUnderline, fsStrikeOut };
}
//---------------------------------------------------------------------------
void CCompiler::RunGridPorts()
{
  IRQ.Clear();
  ports.Clear();
  for(int i = 1; i < string_grid_ports->RowCount; i++)
  {
    AnsiString name = string_grid_ports->Cells[0][i];
    AnsiString size_str  = string_grid_ports->Cells[1][i];
    AnsiString port_in = string_grid_ports->Cells[2][i];
    AnsiString port_irq = string_grid_ports->Cells[3][i];
    AnsiString port_irq_priority = string_grid_ports->Cells[4][i];
    AnsiString port_out = string_grid_ports->Cells[5][i];
    AnsiString port_out_irq = string_grid_ports->Cells[6][i];

    bool ok_name = false;
    for(int i = 1; i <= name.Length(); i ++)
      if(&name[i] != " ")
        ok_name = true;
    if(!ok_name)
      continue;
    unsigned size = 0;
    for(int i = 1; i <= size_str.Length(); i ++)
      if(size_str[i] != ' ')
        size = size * 10 + (size_str[i] - '0');
    if(size > 32)
    {
      AddLineToEditError("портах", i, "Разрядность порта '" + name + "' не более 32");
      continue;
    }
    if(!size && port_out != "")
    {
      AddLineToEditError("портах", i, "Выходной регистр порта '" + name + "' не может состоять из ничего.");
      continue;
    }
    if(!IsOkName(name))
    {
      AddLineToEditError("портах", i, "Нельзя создать порт с именем '"  + name + "' ('Ж', 'Ш' пишы с 'ы'!)");
      continue;
    }
    int irq_priority = 0;
    for(int i = 1; i <= port_irq_priority.Length(); i ++)
      if(port_irq_priority[i] != ' ')
        irq_priority = irq_priority * 10 + (port_irq_priority[i] - '0');

    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("портах", i, "Существует слово ФОРТ с именем '" + name + "'");
      continue;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("портах", i, "Слово ФОРТ с именем '" + name + "' изночально в словаре. Измените имя.");
        continue;
      case twMy:
        AddLineToEditError("портах", i, "Слово ФОРТ с именем '"  + name + "' уже в словаре");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("портах", i, "Ячейка памяти '" + name + "' уже существует");
        continue;
      case twPort:
        AddLineToEditError("портах", i, "Порт '" + name + "' уже существует");
        continue;
      default:
        dictionary.AddPort(name, size, port_in != "", port_out != "", port_out_irq != "", false);
        if(port_irq != "")
          IRQ.AddIRQ(port_irq, irq_priority);
    }
  }
}
//---------------------------------------------------------------------------
void CCompiler::RunGridUart()
{
  uarts.Clear();
  for(int i = 1; i < string_grid_uart->RowCount; i++)
  {
    AnsiString name = string_grid_uart->Cells[0][i];
    AnsiString size_str = string_grid_uart->Cells[1][i];
    AnsiString frequency = string_grid_uart->Cells[2][i];
    AnsiString port_in = string_grid_uart->Cells[3][i];
    AnsiString port_irq_in = string_grid_uart->Cells[4][i];
    AnsiString port_irq_in_priority = string_grid_uart->Cells[5][i];
    AnsiString port_out = string_grid_uart->Cells[6][i];
    AnsiString port_irq_out = string_grid_uart->Cells[7][i];
    AnsiString port_irq_out_priority = string_grid_uart->Cells[8][i];

    bool ok_name = false;
    bool ok = true;
    for(int i = 1; i <= name.Length(); i ++)
      if(&name[i] != " ")
        ok_name = true;
    if(!ok_name)
      continue;
    unsigned size = 0;
    for(int i = 1; i <= size_str.Length(); i ++)
      if(size_str[i] != ' ')
        size = size * 10 + (size_str[i] - '0');
    if((size > (options_proc.GetaRazradData() + (options_proc.GetZnakData()? 1 :0))) || (!size))
    {
      AddLineToEditError("UART", i, "Разрядность порта '" + name + "' должна быть от 1 до разрядности данных процессора");
      ok = false;
    }
    if(!IsOkName(name))
    {
      AddLineToEditError("UART", i, "Нельзя создать UART с именем '"  + name + "' ('Ж', 'Ш' пишы с 'ы'!)");
      ok = false;
    }
    int irq_in_priority = 0;
    for(int i = 1; i <= port_irq_in_priority.Length(); i ++)
      if(port_irq_in_priority[i] != ' ')
        irq_in_priority = irq_in_priority * 10 + (port_irq_in_priority[i] - '0');
    int irq_out_priority = 0;
    for(int i = 1; i <= port_irq_out_priority.Length(); i ++)
      if(port_irq_out_priority[i] != ' ')
        irq_out_priority = irq_out_priority * 10 + (port_irq_out_priority[i] - '0');

    if(!div_frequency.IsFrequency(frequency))
    {
      AddLineToEditError("UART", i, "Нет частоты: '" + frequency + "' для '" + name + "'");
      ok = false;
    }
    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("UART", i, "Существует слово ФОРТ с именем '" + name + "'");
      ok = false;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("UART", i, "Слово ФОРТ с именем '" + name + "' изночально в словаре. Измените имя.");
        continue;
      case twMy:
        AddLineToEditError("UART", i, "Слово ФОРТ с именем '"  + name + "' уже в словаре");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("UART", i, "Ячейка памяти '" + name + "' уже существует");
        continue;
      case twPort:
        AddLineToEditError("UART", i, "Порт '" + name + "' уже существует");
        continue;
      default:
        if(!ok)
          continue;
        if((port_in != "") || (port_out != ""))
        {
          dictionary.AddPort(name, size, port_in != "", port_out != "", false, true);
          if(port_irq_in != "")
            IRQ.AddIRQ(port_irq_in, irq_in_priority);
          if(port_irq_out != "")
          {
            IRQ.AddIRQ(port_irq_out, irq_out_priority);
            div_frequency.Add4Div(frequency);
          }
          uarts.AddUart(name, frequency, size, port_irq_in, port_irq_out);
        }
    }
  }
}
//---------------------------------------------------------------------------
void CCompiler::RunFilters()
{
  if(!filters.GetSize())
    return;
  if(!options_proc.GetZnakData())
  {
    AddLineToEditError("Сопроцессор", 0, "Нельзя создать Фильтр в беззнаковом процессоре");
    return;
  }
  for(int i = 0; i < filters.GetSize(); i++)
  {
    SFilter filter = filters.GetFilter(i);
    AnsiString name = filter.name;
    AnsiString size_str = filter.mantisa;
    AnsiString frequency = filter.frequency;

    bool ok_name = false;
    bool ok = true;
    for(int i = 1; i <= name.Length(); i ++)
      if(&name[i] != " ")
        ok_name = true;
    if(!ok_name)
      continue;
    unsigned size = 0;
    for(int i = 1; i <= size_str.Length(); i ++)
      if(size_str[i] != ' ')
        size = size * 10 + (size_str[i] - '0');
//    if((size > (options_proc.GetaRazradData() + (options_proc.GetZnakData()? 1 :0))) || (!size))
    if(!IsOkName(name))
    {
      AddLineToEditError("Сопроцессор", i, "Нельзя создать Фильтр с именем '"  + name);
      ok = false;
    }
    if(!div_frequency.IsFrequency(frequency))
    {
      AddLineToEditError("Сопроцессор", i, "Нет частоты: '" + frequency + "' для '" + name + "'");
      ok = false;
    }
    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("Сопроцессор", i, "Существует слово ФОРТ с именем '" + name + "'");
      ok = false;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("Сопроцессор", i, "Слово ФОРТ с именем '" + name + "' изночально в словаре. Измените имя.");
        continue;
      case twMy:
        AddLineToEditError("Сопроцессор", i, "Слово ФОРТ с именем '"  + name + "' уже в словаре");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("Сопроцессор", i, "Ячейка памяти '" + name + "' уже существует");
        continue;
      case twPort:
        AddLineToEditError("Сопроцессор", i, "Порт '" + name + "' уже существует");
        continue;
      default:
        if(!ok)
          continue;
      dictionary.AddPort(name, options_proc.GetaRazradData(), true, true, true, true);
    }
  }
}
//---------------------------------------------------------------------------
void CCompiler::RunModuls()
{
}
//---------------------------------------------------------------------------
void CCompiler::RunGridTimer()
{
  timers.Clear();
  for(int i = 1; i < string_grid_timer->RowCount; i++)
  {
    AnsiString name = string_grid_timer->Cells[0][i];
    AnsiString size_str = string_grid_timer->Cells[1][i];
    AnsiString frequency = string_grid_timer->Cells[2][i];
    AnsiString irq = string_grid_timer->Cells[3][i];
    AnsiString irq_priority = string_grid_timer->Cells[4][i];

    bool ok_name = false;
    bool ok = true;
    for(int i = 1; i <= name.Length(); i ++)
      if(&name[i] != " ")
        ok_name = true;
    if(!ok_name)
      continue;
    unsigned size = 0;
    for(int i = 1; i <= size_str.Length(); i ++)
      if(size_str[i] != ' ')
        size = size * 10 + (size_str[i] - '0');
    if((size > (options_proc.GetaRazradData() + (options_proc.GetZnakData()? 1 :0))) || (!size))
    {
      AddLineToEditError("Таймер", i, "Разрядность порта '" + name + "' должна быть от 1 до разрядности данных процессора");
      ok = false;
    }
    if(!IsOkName(name))
    {
      AddLineToEditError("Таймер", i, "Нельзя создать timer с именем '"  + name + "' ('Ж', 'Ш' пишы с 'ы'!)");
      ok = false;
    }
    int irq_int_priority = 0;
    for(int i = 1; i <= irq_priority.Length(); i ++)
      if(irq_priority[i] != ' ')
        irq_int_priority = irq_int_priority * 10 + (irq_priority[i] - '0');

    if(!div_frequency.IsFrequency(frequency))
    {
      AddLineToEditError("Таймер", i, "Нет частоты: '" + frequency + "' для '" + name + "'");
      ok = false;
    }
    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("Таймер", i, "Существует слово ФОРТ с именем '" + name + "'");
      ok = false;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("Таймер", i, "Слово ФОРТ с именем '" + name + "' изночально в словаре. Измените имя.");
        continue;
      case twMy:
        AddLineToEditError("Таймер", i, "Слово ФОРТ с именем '"  + name + "' уже в словаре");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("Таймер", i, "Ячейка памяти '" + name + "' уже существует");
        continue;
      case twPort:
        AddLineToEditError("Таймер", i, "Порт '" + name + "' уже существует");
        continue;
      default:
        if(!ok)
          continue;
        dictionary.AddPort(name, size, true, true, false, true);
        IRQ.AddIRQ(irq, irq_int_priority);
        timers.AddTimer(name, frequency, size, irq);
    }
  }
}
//---------------------------------------------------------------------------
void CCompiler::RunGridDelit()
{
  delits.Clear();
  for(int i = 1; i < string_grid_delit->RowCount; i++)
  {
    AnsiString name = string_grid_delit->Cells[0][i];
    AnsiString size_str = string_grid_delit->Cells[1][i];

    bool ok_name = false;
    bool ok = true;
    for(int i = 1; i <= name.Length(); i ++)
      if(&name[i] != " ")
        ok_name = true;
    if(!ok_name)
      continue;
/*    unsigned size = 0;
    for(int i = 1; i <= size_str.Length(); i ++)
      if(size_str[i] != ' ')
        size = size * 10 + (size_str[i] - '0');
    if((size > (options_proc.GetaRazradData() + (options_proc.GetZnakData()? 1 :0))) || (size < 2))
    {
      AddLineToEditError("Сопроцессор", i, "Разрядность делителя '" + name + "' должна быть от 2 до разрядности данных процессора");
      ok = false;
    }*/
    if(!IsOkName(name))
    {
      AddLineToEditError("Сопроцессор", i, "Нельзя создать делитель с именем '"  + name + "' ('Ж', 'Ш' пишы с 'ы'!)");
      ok = false;
    }
    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("Сопроцессор", i, "Существует слово ФОРТ с именем '" + name + "'");
      ok = false;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("Сопроцессор", i, "Слово ФОРТ с именем '" + name + "' изночально в словаре. Измените имя.");
        continue;
      case twMy:
        AddLineToEditError("Сопроцессор", i, "Слово ФОРТ с именем '"  + name + "' уже в словаре");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("Сопроцессор", i, "Ячейка памяти '" + name + "' уже существует");
        continue;
      case twPort:
        AddLineToEditError("Сопроцессор", i, "Порт '" + name + "' уже существует");
        continue;
      default:
        if(!ok)
          continue;
        dictionary.AddPort(name+"1", options_proc.GetaRazradData(), false, true, false, true);
        dictionary.AddPort(name+"2", options_proc.GetaRazradData(), false, true, true, true);
        dictionary.AddPort(name, options_proc.GetaRazradData(), true, false, false, true);
        delits.AddDelit(name);
    }
  }
}
//---------------------------------------------------------------------------
void CCompiler::RunGridVariable()
{
  for(int i = 1; i < string_grid_RAM->RowCount; i++)
  {
    AnsiString name = string_grid_RAM->Cells[1][i];
    AnsiString kol  = string_grid_RAM->Cells[2][i];
    AnsiString type = string_grid_RAM->Cells[3][i];
    AnsiString init = string_grid_RAM->Cells[4][i];
    int n;
    if(!IsOkName(name))
    {
      AddLineToEditError("ОЗУ", i, "Нельзя создать ячейку памяти с именем '"  + name + "' ('Ж', 'Ш' пишы с 'ы'!)");
      continue;
    }
    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("ОЗУ", i, "Существует слово ФОРТ с именем '" + name + "'");
      continue;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("ОЗУ", i, "Слово ФОРТ с именем '" + name + "' изночально в словаре. Измените имя.");
        continue;
      case twMy:
        AddLineToEditError("ОЗУ", i, "Слово ФОРТ с именем '"  + name + "' уже в словаре");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("ОЗУ", i, "Ячейка памяти '" + name + "' уже существует");
        continue;
      case twPort:
        AddLineToEditError("ОЗУ", i, "Есть такой порт '" + name + "'");
        continue;
      default:   ;
    }
    if(!dictionary.GetConstValue(kol, n))
    {
      AddLineToEditError("ОЗУ", i, "'" + kol + "' - Где вы видели такое измерение 'Количества' ячейки памяти '" + name + "'");
      continue;
    }
    if(n < 1)
    {
      AddLineToEditError("ОЗУ", i, "Нельзя создать " + IntToStr(n) + " ячеек памяти (столько не бывает)");
      continue;
    }
    int value[10000], uk = 0;
    int c;
    char str[100000];
    strcpy(str, init.c_str());
    bool res = true;
    for(int i = 0; i < init.Length(); i ++)
    {
      while(((str[i] == ' ') || (str[i] == ',')) && str[i]) i++;
      if(!str[i]) break;
      AnsiString v;
      while((str[i] != ' ') && (str[i] != ',') && (str[i] > 0x10) && str[i])
        v += str[i++];
      if(!dictionary.GetConstValue(v, c))
      {
        AddLineToEditError("ОЗУ", i, "'" + init + "' - Вот вам пример непонятного значения ячейки памяти '" + name + "'");
        res = false; break;
      }
      value[uk++] = c;
    }
    if(res)
    {
      for(int i = uk; i < kol; i ++)
        value[i] = (uk)? value[uk-1] : 0;
      dictionary.AddVariableWord(name, (type == "const")? twConst : twVariable,
                                 true, n, value);
    }
  }
}
//---------------------------------------------------------------------------
bool CCompiler::IsOkName(AnsiString name)
{
  if(name == "")
    return false;
  for(int i = 1; i < name.Length(); i ++)
    if(IsRazdelitel(name[i]))
      return false;
  return !dictionary.IsConst(name);
}
//---------------------------------------------------------------------------
void CCompiler::AddLineToEditError(AnsiString name, int line, AnsiString text)
{
  edit_error->Lines->Add("Обшибка в " + name + ", строка " + IntToStr(line) + " : " + text);
  result_compiler = false;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
