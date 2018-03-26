//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
                                
#include "USoproch.h"
//---------------------------------------------------------------------
#pragma link "CSPIN"
#pragma resource "*.dfm"
TSoprochForm *SoprochForm;
//--------------------------------------------------------------------- 
__fastcall TSoprochForm::TSoprochForm(TComponent* AOwner)
	: TForm(AOwner)
{
  AnsiString str_grid_filer[2] = {"Имя", "Частота"};
  for(int i = 0; i < StringGridFilter->ColCount; i ++)
    StringGridFilter->Cells[i][0] = str_grid_filer[i];//пишем заголовок таблицы
  AnsiString str_grid_delit[2] = {"Имя", "Пример применения: делим 4 на 2"};
  for(int i = 0; i < StringGridDelit->ColCount; i ++)
    StringGridDelit->Cells[i][0] = str_grid_delit[i];//пишем заголовок таблицы
}
//---------------------------------------------------------------------------
bool TSoprochForm::Run(CTextOfGrid &tog_filter, CTextOfGrid &tog_delit)
{
  result = false;
  tog_filter.PaintOnStringGrid(StringGridFilter);
  tog_delit.PaintOnStringGrid(StringGridDelit);
  filters_new = filters;
  filters_new.RunAll();
  if(StringGridFilter->Row)
    PrintFilter(StringGridFilter->Row-1);
  ShowModal();
  if(result)
  {
    tog_filter = CTextOfGrid("Фильтры1", StringGridFilter);
    filters = filters_new;
    tog_delit = CTextOfGrid("Делители2", StringGridDelit);
  }
  return result;
}
//---------------------------------------------------------------------
void __fastcall TSoprochForm::OKBtnClick(TObject *Sender)
{
  if(StringGridFilter->Row)
    SetFilter(StringGridFilter->Row-1);
  if(StringGridFilter->RowCount == 1)
    filters_new.Clear();
  result = true;
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::CancelBtnClick(TObject *Sender)
{
  result = false;
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::ButAddClick(TObject *Sender)
{
  StringGridFilter->RowCount =  StringGridFilter->RowCount + 1;
  StringGridFilter->FixedRows =  1;
  int k = StringGridFilter->RowCount-1;
  while(k > StringGridFilter->Row)
  {
    for(int i = 0; i < StringGridFilter->ColCount; i ++)
      StringGridFilter->Cells[i][k] =  StringGridFilter->Cells[i][k-1];
    k --;
  }
  for(int i = 0; i < StringGridFilter->ColCount; i ++)
    StringGridFilter->Cells[i][StringGridFilter->Row] =  "";
  if(StringGridFilter->RowCount == 2)
    filters_new.Add(0 ,SFilter());
  else
    filters_new.Add(StringGridFilter->Row-1 ,SFilter());
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::ButSubClick(TObject *Sender)
{
  if(StringGridFilter->Row < 1)
    return;
  int k = StringGridFilter->Row;
  while(k < (StringGridFilter->RowCount-1))
  {
    for(int i = 0; i < StringGridFilter->ColCount; i ++)
      StringGridFilter->Cells[i][k] = StringGridFilter->Cells[i][k+1];
    k ++;
  }
  filters_new.Sub(StringGridFilter->Row-1);
  StringGridFilter->RowCount = StringGridFilter->RowCount - 1;
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::StringGridFilterSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  int n = StringGridFilter->Row;
  if(!ARow)
    return;
  switch(ACol)
  {
    case 0: case 1: StringGridFilter->Options << goEditing; break;
    default: StringGridFilter->Options >> goEditing; break;
  }
  if(StringGridFilter->Row != ARow)
  {
    if(StringGridFilter->Row)
      SetFilter(StringGridFilter->Row-1);
    if(ARow)
      PrintFilter(ARow-1);
  }
}
//---------------------------------------------------------------------------
void TSoprochForm::PrintFilter(int n)
{
  SFilter sf = filters_new.GetFilter(n);
//  CSpinEditMantisa->Value = options_proc.GetaRazradData();
  CSpinEditMantisa->Value = sf.mantisa;
  CSpinEditPor->Value = sf.por;
  CSpinEditKol->Value = sf.kol_coof;
  EditFd->Text = IntToStr(sf.fd);
  EditFr1->Text = IntToStr(sf.fr[0]);
  EditA1->Text = IntToStr(sf.a[0]);
  EditSm->Text = IntToStr(sf.sm);
  EditFr2->Text = IntToStr(sf.fr[1]);
  EditA2->Text = IntToStr(sf.a[1]);
  EditFr3->Text = IntToStr(sf.fr[2]);
  EditA3->Text = IntToStr(sf.a[2]);
  EditB->Text = sf.GetCoofB();
  EditA->Text = sf.GetCoofA();

  filters_new.Run(n);
  PaintFilter(n);
}
//---------------------------------------------------------------------------
void TSoprochForm::SetFilter(int n)
{
  SFilter sf = filters_new.GetFilter(n);
  sf.mantisa = CSpinEditMantisa->Value;
  sf.por = CSpinEditPor->Value;
  sf.kol_coof = CSpinEditKol->Value;

  AnsiString str = StringGridFilter->Cells[0][n+1];
  strcpy(sf.name, str.c_str());
  str = StringGridFilter->Cells[1][n+1];
  strcpy(sf.frequency, str.c_str());
  GetInt(sf.fd, EditFd->Text, "");
  GetInt(sf.fr[0], EditFr1->Text, "");
  GetInt(sf.a[0], EditA1->Text, "");
  GetInt(sf.sm, EditSm->Text, "");
  GetInt(sf.fr[1], EditFr2->Text, "");
  GetInt(sf.a[1], EditA2->Text, "");
  GetInt(sf.fr[2], EditFr3->Text, "");
  GetInt(sf.a[2], EditA3->Text, "");
  GetDouble(sf.kol_coof, sf.coof_b, EditB->Text, "");
  GetDouble(sf.kol_coof, sf.coof_a, EditA->Text, "");
  filters_new.Set(n, sf);
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::ButRunFilterClick(TObject *Sender)
{
  if(StringGridFilter->Row)
  {
    SetFilter(StringGridFilter->Row-1);
    PrintFilter(StringGridFilter->Row-1);
  }
}
//---------------------------------------------------------------------------
bool TSoprochForm::GetInt(int &c, AnsiString str, AnsiString err)
{
  AnsiString str2 = "";
  for(int i = 1; i <= str.Length(); i ++)
    if((str[i] >= '0') && (str[i] <= '9'))
      str2 += str[i];
  c = StrToIntDef(str2, -1);
  if(c <= 0)
  {
    if(err != "")
      MessageBox(0, "Ошибка...", err.c_str(), MB_ICONSTOP | MB_TASKMODAL | MB_OK);
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
bool TSoprochForm::GetDouble(int kol, double *c, AnsiString str, AnsiString err)
{
  str += " ";
  int uk = 0;
  int as = str.Length();
  AnsiString str2 = "";
  int i = 1;
  while((str[i] == ' ') && (i <= str.Length())) i++;
  for(; i <= str.Length(); i ++)
  {
    if(str[i] != ' ')
    {
      if(str[i] == ',')
        str2 += '.';
      else
        str2 += str[i];
    }
    else
    {
      if(str2 == "")
        continue;
      c[uk++] = atof(str2.c_str());
      str2 = "";
    }
  }
  if(uk != kol)
  {
    if(err != "")
      MessageBox(0, "Ошибка...", err.c_str(), MB_ICONSTOP | MB_TASKMODAL | MB_OK);
    for(int j = uk; j < kol; j ++)
      c[j] = 1;
    return false;
  }
  return true;
}
 //---------------------------------------------------------------------------
void __fastcall TSoprochForm::TrackBar1Change(TObject *Sender)
{
  Label16->Caption = TrackBar1->Position;
  if(StringGridFilter->Row)
    PaintFilter(StringGridFilter->Row-1);
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::ScrollBar1Change(TObject *Sender)
{
  if(StringGridFilter->Row)
    PaintFilter(StringGridFilter->Row-1);
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::TrackBar2Change(TObject *Sender)
{
  if(TrackBar2->Position == 0)
    Label20->Caption = "1";
  else if(TrackBar2->Position > 0)
    Label20->Caption = TrackBar2->Position;
  else
    Label20->Caption = "1/" + IntToStr(abs(TrackBar2->Position));
  if(StringGridFilter->Row)
    PaintFilter(StringGridFilter->Row-1);
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::ButInClick(TObject *Sender)
{
  if(StringGridFilter->Row)
    PaintFilter(StringGridFilter->Row-1, 1);
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::ButMidleClick(TObject *Sender)
{
  if(StringGridFilter->Row)
    PaintFilter(StringGridFilter->Row-1, 2);
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::ButOutClick(TObject *Sender)
{
  if(StringGridFilter->Row)
    PaintFilter(StringGridFilter->Row-1, 3);
}
//---------------------------------------------------------------------------
void TSoprochForm::PaintFilter(int n, int s)
{
  filters_new.SetBegin(n, ScrollBar1->Position);
  filters_new.SetSz(n, TrackBar1->Position);
  filters_new.SetSz2(n, TrackBar2->Position);
  filters_new.Paint(n, image, s);
  filters_new.PaintFFT(n, imagefft, s);
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::ButAddDelitClick(TObject *Sender)
{
  StringGridDelit->RowCount =  StringGridDelit->RowCount + 1;
  StringGridDelit->FixedRows =  1;
  int k = StringGridDelit->RowCount-1;
  while(k > StringGridDelit->Row)
  {
    for(int i = 0; i < StringGridDelit->ColCount; i ++)
      StringGridDelit->Cells[i][k] =  StringGridDelit->Cells[i][k-1];
    k --;
  }
  for(int i = 0; i < StringGridDelit->ColCount; i ++)
    StringGridDelit->Cells[i][StringGridDelit->Row] =  "";
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::ButSubDelitClick(TObject *Sender)
{
  if(StringGridDelit->Row < 1)
    return;
  int k = StringGridDelit->Row;
  while(k < (StringGridDelit->RowCount-1))
  {
    for(int i = 0; i < StringGridDelit->ColCount; i ++)
      StringGridDelit->Cells[i][k] = StringGridDelit->Cells[i][k+1];
    k ++;
  }
  StringGridDelit->RowCount = StringGridDelit->RowCount - 1;
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::StringGridDelitSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  int n = StringGridDelit->Row;
  if(!ARow)
    return;
  switch(ACol)
  {
    case 0: StringGridDelit->Options << goEditing; break;
    default: StringGridFilter->Options >> goEditing; break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TSoprochForm::StringGridDelitSetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
  if(ACol < 1)
  {
    if(Value == "")
      StringGridDelit->Cells[2][ARow] = "";
    else
    {
      AnsiString n = StringGridDelit->Cells[0][StringGridDelit->Row];
      AnsiString str = "4 " + n + "1 ! 2 " + n + "2 ! ";
      int size = options_proc.GetaRazradData() / 5;
      str += (IntToStr(size) + " BEGIN DUP 1 - UNTIL DROP " + n + " @");
      StringGridDelit->Cells[1][ARow] = str;
    }  
  }
}
//---------------------------------------------------------------------------


