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
    page_str = "�������� "+IntToStr(i + 1); page = i + 1;
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
      case '('://---������ ����������
        while((str_vydelen[uk] != ')') && (str_vydelen[uk])) uk++;
        break;
      case '\\'://---������ ����������
        while((str_vydelen[uk] != '\n') && (str_vydelen[uk])) uk++;
        break;
      case ':'://---������ �����������
        RunDefinition(uk);
        break;
      case ';'://---����� �����������
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "';' ������ ��������� ���������, ��� �� �������");
        break;
      case ')'://---����� ����������
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "')' ������ ��������� �����������, ��� �� �������");
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
            AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "'" + word + "' - ��� �������� �� �� ���� �����.");
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
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "���� �� � ���� - ';'");
        break;
      case '(':
        while((str_vydelen[uk] != ')') && (str_vydelen[uk])) uk++;
        break;
      case '\\':       
        while((str_vydelen[uk] != '\n') && (str_vydelen[uk])) uk++;
        break;
      case ')'://---����� ����������
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "')' ������ ��������� �����������, ��� �� �������");
        break;
      case ' ': case '\n' : case '\r' : case '.' : case ',' : case '{' : case '}' :
        break;
      default:
        AnsiString w = RunWord(uk, ok);
        if(w == "IF") Add(str_ret, RunIF(uk));
        else if(w == "BEGIN") Add(str_ret, RunBEGIN(uk));
        else if(w == "DO") Add(str_ret, RunDO(uk));
        else if((w == "LOOP") || (w == "+LOOP") || (w == "WHILE") || (w == "REPEAT") || (w == "UNTIL") || (w == "LEAVE"))
          AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "'" + w + "' - ��� �������� �� �� ���� �����.");
        else if(w == "ELSE")
        {
          if(enabled_else)
            AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "'ELSE' �� ������ ���� ������ ��� 'IF'");
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
  AddLineToEditError("(��) "+page_str, GetNumberStr(begin), "� ���� ������ ���� 'IF', �� ��� ��� 'THEN'?");
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
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "���� �� � ���� - ';'");
        break;
      case '(':
        while((str_vydelen[uk] != ')') && (str_vydelen[uk])) uk++;
        break;
      case '\\':
        while((str_vydelen[uk] != '\n') && (str_vydelen[uk])) uk++;
        break;
      case ')'://---����� ����������
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "')' ������ ��������� �����������, ��� �� �������");
        break;
      case ' ': case '\n' : case '\r' : case '.' : case ',' : case '{' : case '}' :
        break;
      default:
        AnsiString w = RunWord(uk, ok);
        if(w == "IF") Add(str_ret, RunIF(uk));
        else if(w == "BEGIN") Add(str_ret, RunBEGIN(uk));
        else if(w == "DO") Add(str_ret, RunDO(uk));
        else if((w == "LOOP") || (w == "ELSE") || (w == "+LOOP") || (w == "LEAVE"))
          AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "'" + w + "' - ��� �������� �� �� ���� �����.");
        else if(w == "WHILE")
        {
          if(enabled_while)
            AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "'WHILE' �� ������ ���� ������ ��� 'BEGIN'");
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
            AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "� ����� 'BEGIN ... UNTIL' ��� ����� 'WHILE'");
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
  AddLineToEditError("(��) "+page_str, GetNumberStr(begin), "� ���� ������ ���� 'BEGIN', �� ��� ��� 'REPEAT' ��� 'UNTIL'?");
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
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "���� �� � ���� - ';'");
        break;
      case '(':
        while((str_vydelen[uk] != ')') && (str_vydelen[uk])) uk++;
        break;
      case '\\':
        while((str_vydelen[uk] != '\n') && (str_vydelen[uk])) uk++;
        break;
      case ')'://---����� ����������
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "')' ������ ��������� �����������, ��� �� �������");
        break;
      case ' ': case '\n' : case '\r' : case '.' : case ',' : case '{' : case '}' :
        break;
      default:
        AnsiString w = RunWord(uk, ok);
        if(w == "IF") Add(str_ret, RunIF(uk));
        else if(w == "BEGIN") Add(str_ret, RunBEGIN(uk));
        else if(w == "DO") Add(str_ret, RunDO(uk));
        else if((w == "THEN") || (w == "ELSE") || (w == "WHILE") || (w == "REPEAT") || (w == "UNTIL"))
          AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "'" + w + "' - ��� �������� �� �� ���� �����.");
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
  AddLineToEditError("(��) "+page_str, GetNumberStr(begin), "� ���� ������ ���� 'DO', �� ��� ��� 'LOOP' ��� '+LOOP'?");
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
      case ';'://---����� �����������
        end = true;
        uk --;
        if(new_word != "")
        {
          if(error)
            AddLineToEditError("(��) "+page_str, GetNumberStr(begin_new_word),
            "��-�� ������������ ������ ����� '" +  new_word + "' �� �������� � ������. �� ����������");
          else if(!dictionary.AddMyWord(new_word, str_new_word))//��������� � �������
              AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "����� � ������ '" + new_word + "' ������� �� ���������");
        }
        break;
      case '(':
        while((str_vydelen[uk] != ')') && (str_vydelen[uk])) uk++;
        break;
      case '\\':
        while((str_vydelen[uk] != '\n') && (str_vydelen[uk])) uk++;
        break;
      case ':'://---������ �����������
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "':' ������ ������ ���������, �� �������� ����������");
        break;
      case ')'://---����� ����������
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "')' ������ ��������� �����������, ��� �� �������");
        break;
      case ' ': case '\n' : case '\r' : case '.' : case ',' : case '{' : case '}' :
        break;
      default:
        if(new_word == "")//������ �����
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
              AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "'" + w + "' - ��� �������� �� �� ���� �����.");
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
         AddLineToEditError("(��) "+page_str, GetNumberStr(begin_new_word),
         "��-�� ������������ ������ ����� '" +  new_word + "' �� ���������");
      else
          AddLineToEditError("(��) "+page_str, GetNumberStr(begin_new_word), "��� ����� ����������� ����� '" + new_word + "'");
    else
      AddLineToEditError("(��) "+page_str, GetNumberStr(begin_new_word), "��� ����� ����������� �����.. (����� ����� ��� �� ���)");
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
{                                                // = true? ���� ����� ��������
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
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "����� ���� � ������ '" + word + "' ���������� � �������. ����������� ������!");
//=      SetAttributesColor(begin_word, uk+1, color_standart);
//=      SetAttributesStyle(begin_word, uk+1, style_standart);
      ok = vydelen;
      break;
    case twMy:
      if(!vydelen)
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "����� ���� � ������ '"  + word + "' ��� � �������");
      ok = vydelen;
      break;
    case twConst:
    case twVariable:
      if(!vydelen)
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "C��������� ������ ������ '" + word + "'");
//=      SetAttributesColor(begin_word, uk+1, color_variable);
      ok = vydelen;
      break;
    case twPort:
      if(!vydelen)
        AddLineToEditError("(��) "+page_str,  GetNumberStr(uk), "���� ����� ���� '" + word + "'");
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
        AddLineToEditError("(��) "+page_str, GetNumberStr(uk), "����� '"  + word + "' ����������� � �������");
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
      AddLineToEditError("������", i, "����������� ����� '" + name + "' �� ����� 32");
      continue;
    }
    if(!size && port_out != "")
    {
      AddLineToEditError("������", i, "�������� ������� ����� '" + name + "' �� ����� �������� �� ������.");
      continue;
    }
    if(!IsOkName(name))
    {
      AddLineToEditError("������", i, "������ ������� ���� � ������ '"  + name + "' ('�', '�' ���� � '�'!)");
      continue;
    }
    int irq_priority = 0;
    for(int i = 1; i <= port_irq_priority.Length(); i ++)
      if(port_irq_priority[i] != ' ')
        irq_priority = irq_priority * 10 + (port_irq_priority[i] - '0');

    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("������", i, "���������� ����� ���� � ������ '" + name + "'");
      continue;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("������", i, "����� ���� � ������ '" + name + "' ���������� � �������. �������� ���.");
        continue;
      case twMy:
        AddLineToEditError("������", i, "����� ���� � ������ '"  + name + "' ��� � �������");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("������", i, "������ ������ '" + name + "' ��� ����������");
        continue;
      case twPort:
        AddLineToEditError("������", i, "���� '" + name + "' ��� ����������");
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
      AddLineToEditError("UART", i, "����������� ����� '" + name + "' ������ ���� �� 1 �� ����������� ������ ����������");
      ok = false;
    }
    if(!IsOkName(name))
    {
      AddLineToEditError("UART", i, "������ ������� UART � ������ '"  + name + "' ('�', '�' ���� � '�'!)");
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
      AddLineToEditError("UART", i, "��� �������: '" + frequency + "' ��� '" + name + "'");
      ok = false;
    }
    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("UART", i, "���������� ����� ���� � ������ '" + name + "'");
      ok = false;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("UART", i, "����� ���� � ������ '" + name + "' ���������� � �������. �������� ���.");
        continue;
      case twMy:
        AddLineToEditError("UART", i, "����� ���� � ������ '"  + name + "' ��� � �������");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("UART", i, "������ ������ '" + name + "' ��� ����������");
        continue;
      case twPort:
        AddLineToEditError("UART", i, "���� '" + name + "' ��� ����������");
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
    AddLineToEditError("�����������", 0, "������ ������� ������ � ����������� ����������");
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
      AddLineToEditError("�����������", i, "������ ������� ������ � ������ '"  + name);
      ok = false;
    }
    if(!div_frequency.IsFrequency(frequency))
    {
      AddLineToEditError("�����������", i, "��� �������: '" + frequency + "' ��� '" + name + "'");
      ok = false;
    }
    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("�����������", i, "���������� ����� ���� � ������ '" + name + "'");
      ok = false;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("�����������", i, "����� ���� � ������ '" + name + "' ���������� � �������. �������� ���.");
        continue;
      case twMy:
        AddLineToEditError("�����������", i, "����� ���� � ������ '"  + name + "' ��� � �������");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("�����������", i, "������ ������ '" + name + "' ��� ����������");
        continue;
      case twPort:
        AddLineToEditError("�����������", i, "���� '" + name + "' ��� ����������");
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
      AddLineToEditError("������", i, "����������� ����� '" + name + "' ������ ���� �� 1 �� ����������� ������ ����������");
      ok = false;
    }
    if(!IsOkName(name))
    {
      AddLineToEditError("������", i, "������ ������� timer � ������ '"  + name + "' ('�', '�' ���� � '�'!)");
      ok = false;
    }
    int irq_int_priority = 0;
    for(int i = 1; i <= irq_priority.Length(); i ++)
      if(irq_priority[i] != ' ')
        irq_int_priority = irq_int_priority * 10 + (irq_priority[i] - '0');

    if(!div_frequency.IsFrequency(frequency))
    {
      AddLineToEditError("������", i, "��� �������: '" + frequency + "' ��� '" + name + "'");
      ok = false;
    }
    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("������", i, "���������� ����� ���� � ������ '" + name + "'");
      ok = false;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("������", i, "����� ���� � ������ '" + name + "' ���������� � �������. �������� ���.");
        continue;
      case twMy:
        AddLineToEditError("������", i, "����� ���� � ������ '"  + name + "' ��� � �������");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("������", i, "������ ������ '" + name + "' ��� ����������");
        continue;
      case twPort:
        AddLineToEditError("������", i, "���� '" + name + "' ��� ����������");
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
      AddLineToEditError("�����������", i, "����������� �������� '" + name + "' ������ ���� �� 2 �� ����������� ������ ����������");
      ok = false;
    }*/
    if(!IsOkName(name))
    {
      AddLineToEditError("�����������", i, "������ ������� �������� � ������ '"  + name + "' ('�', '�' ���� � '�'!)");
      ok = false;
    }
    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("�����������", i, "���������� ����� ���� � ������ '" + name + "'");
      ok = false;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("�����������", i, "����� ���� � ������ '" + name + "' ���������� � �������. �������� ���.");
        continue;
      case twMy:
        AddLineToEditError("�����������", i, "����� ���� � ������ '"  + name + "' ��� � �������");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("�����������", i, "������ ������ '" + name + "' ��� ����������");
        continue;
      case twPort:
        AddLineToEditError("�����������", i, "���� '" + name + "' ��� ����������");
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
      AddLineToEditError("���", i, "������ ������� ������ ������ � ������ '"  + name + "' ('�', '�' ���� � '�'!)");
      continue;
    }
    if(dictionary.ServiceWord(name))
    {
      AddLineToEditError("���", i, "���������� ����� ���� � ������ '" + name + "'");
      continue;
    }
    TypeOfWord t = dictionary.WhatIs(name);
    switch(t)
    {
      case twStandart:
        AddLineToEditError("���", i, "����� ���� � ������ '" + name + "' ���������� � �������. �������� ���.");
        continue;
      case twMy:
        AddLineToEditError("���", i, "����� ���� � ������ '"  + name + "' ��� � �������");
        continue;
      case twConst:
      case twVariable:
        AddLineToEditError("���", i, "������ ������ '" + name + "' ��� ����������");
        continue;
      case twPort:
        AddLineToEditError("���", i, "���� ����� ���� '" + name + "'");
        continue;
      default:   ;
    }
    if(!dictionary.GetConstValue(kol, n))
    {
      AddLineToEditError("���", i, "'" + kol + "' - ��� �� ������ ����� ��������� '����������' ������ ������ '" + name + "'");
      continue;
    }
    if(n < 1)
    {
      AddLineToEditError("���", i, "������ ������� " + IntToStr(n) + " ����� ������ (������� �� ������)");
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
        AddLineToEditError("���", i, "'" + init + "' - ��� ��� ������ ����������� �������� ������ ������ '" + name + "'");
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
  edit_error->Lines->Add("������� � " + name + ", ������ " + IntToStr(line) + " : " + text);
  result_compiler = false;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
