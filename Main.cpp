//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "MainDebug.h"
//---------------------------------------------------------------------------
#pragma link "CSPIN"
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner), file_name("��� �����.sp"), options_ports("�����1", 7),
          options_uart("Uarts1", 9), options_timer("�������1", 5), options_filter("�������1", 2), options_div("��������1", 5),
          options_RAM("���1", 5), options_delit("��������2", 2), options_modul("������1", 1), options_text("����1", 1)
{
  SetSave(true);
  Caption = "�������� ���������� - " + ChangeFileExt(file_name, "");
  SaveDialog->InitialDir = GetCurrentDir();
  OpenDialog->InitialDir = GetCurrentDir();
//  EditTextIRQSelectionChange(Owner);
//  EditTextOpredSelectionChange(Owner);
  AnsiString str_grid_RAM[5] = {"�", "���", "�����", "���", "��������"};
  for(int i = 0; i < string_grid_RAM->ColCount; i ++)
    string_grid_RAM->Cells[i][0] = str_grid_RAM[i];//����� ��������� �������
  AnsiString str_grid_debug_Text2[5] = {"", "�����", "�����", "������� ����", "������� ����������"};
  for(int i = 0; i < grid_debug_Text2->ColCount; i ++)
    grid_debug_Text2->Cells[i][0] = str_grid_debug_Text2[i];//����� ��������� ������� 1
  AnsiString str_grid_debug_Text1[2] = {"", ""};
//  for(int i = 0; i < grid_debug_Text11->ColCount; i ++)
//    grid_debug_Text11->Cells[i][0] = str_grid_debug_Text1[i];//����� ��������� ������� 2
  AnsiString str_grid_debug_RAM[3] = {"�����", "���", "��������"};
  for(int i = 0; i < grid_debug_RAM->ColCount; i ++)
    grid_debug_RAM->Cells[i][0] = str_grid_debug_RAM[i];//����� ��������� �������
  AnsiString str_grid_debug_ports[4] = {"�����", "���", "����", "�����"};
  for(int i = 0; i < grid_debug_ports->ColCount; i ++)
    grid_debug_ports->Cells[i][0] = str_grid_debug_ports[i];//����� ��������� �������
  AnsiString str_grid_debug_uarts[2] = {"���", "����."};
  for(int i = 0; i < grid_debug_uarts->ColCount; i ++)
    grid_debug_uarts->Cells[i][0] = str_grid_debug_uarts[i];//����� ��������� �������
  AnsiString str_grid_debug_irq[3] = {"���", "���������", "�������."};
  for(int i = 0; i < grid_debug_irq->ColCount; i ++)
    grid_debug_irq->Cells[i][0] = str_grid_debug_irq[i];//����� ��������� �������
  AnsiString str_stack[2] = {"�", "��������"};
  for(int i = 0; i < grid_stack_data->ColCount; i ++)
    grid_stack_data->Cells[i][0] = str_stack[i];//����� ��������� �������
  for(int i = 0; i < grid_stack_return->ColCount; i ++)
    grid_stack_return->Cells[i][0] = str_stack[i];//����� ��������� �������
  AnsiString str_timers[2] = {"���", "����."};
  for(int i = 0; i < grid_debug_timers->ColCount; i ++)
    grid_debug_timers->Cells[i][0] = str_timers[i];//����� ��������� �������

  compiler.SetEditError(EditError);
  debug = NULL;
  StopDebug();
  linker.SetEditError(EditError);
  EditError->Height = 0;
  compiler.SetStringGridRAM(string_grid_RAM);
  edit_text_in_work = true;
  edits.push_back(CEdit(image, ScrollBarX, ScrollBarY));
  uk = 0;
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FileNew(TObject *Sender)
{
  edit_text_in_work = false;
m1:if(ButSave->Enabled)
    switch(MessageBox(0, "���������� �� ���������. ���������?", ChangeFileExt(file_name, "").c_str(),
           MB_ICONQUESTION | MB_YESNOCANCEL))
    {
      case IDYES: FileSave(Sender);
                  goto m1;
      case IDCANCEL: return;
    }
  EditError->Clear();
  MemoVhdl->Clear();
  ROM.Clear();
  ROM.PrintAll(grid_debug_Text2);
  StopDebug();
  SetSave(true);
  file_name = "��� �����.sp";
  Caption = "�������� ���������� - " + ChangeFileExt(file_name, "");
  options_proc = COptionsProc();
  options_div = CTextOfGrid("��������1", 5);
  options_ports = CTextOfGrid("�����1", 7);
  options_uart = CTextOfGrid("Uarts1", 9);
  options_timer = CTextOfGrid("�������1", 5);
  options_RAM = CTextOfGrid("���1", 5);
  options_filter = CTextOfGrid("�������1", 2);
  options_delit = CTextOfGrid("��������2", 2);
  options_modul = CTextOfGrid("������1", 1);
  options_text = CTextOfGrid("����1", 1);
  options_RAM.PaintOnStringGrid(string_grid_RAM);
  options_text.PaintOnStringGrid(StringGridEdits);
  edit_text_in_work = true;
  filters.Clear();
  moduls.Clear();
  edits.clear();
  edits.push_back(CEdit(image, ScrollBarX, ScrollBarY));
  uk = 0;
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FileOpen(TObject *Sender)
{
  edit_text_in_work = false;
m1:if(ButSave->Enabled)
    switch(MessageBox(0, "���������� �� ���������. ���������?", ChangeFileExt(file_name, "").c_str(),
           MB_ICONQUESTION | MB_YESNOCANCEL))
    {
      case IDYES: FileSave(Sender);
                  goto m1;
      case IDCANCEL: return;
    }
  if (OpenDialog->Execute())
  {
    EditError->Clear();
    MemoVhdl->Clear();
    ROM.Clear();
    ROM.PrintAll(grid_debug_Text2);
    StopDebug();
    FILE *f = fopen(OpenDialog->FileName.c_str(), "rb");
    if(f)
    {
      options_proc = COptionsProc(f);
      CString str = CString(f);
      DivForm->SetInputDiv(str.AnsiString());
      options_div = CTextOfGrid("��������1", 5, f);
      options_ports = CTextOfGrid("�����1", 7, f);
      options_uart = CTextOfGrid("Uarts1", 9, f);
      options_timer = CTextOfGrid("�������1", 5, f);
      options_RAM = CTextOfGrid("���1", 5, f);
      options_RAM.PaintOnStringGrid(string_grid_RAM);
      options_div.PaintOnStringGrid(DivForm->GetStringGridPorts());
      options_ports.PaintOnStringGrid(PortsForm->GetStringGridPorts());
      options_uart.PaintOnStringGrid(PeripheryForm->GetStringGridUart());
      options_timer.PaintOnStringGrid(PeripheryForm->GetStringGridTimer());
      LoadTextOfProrammFromFile(f);
      fread(&soproch_enabled, sizeof(soproch_enabled), 1, f);
      options_filter = CTextOfGrid("�������1", 2, f);
      options_filter.PaintOnStringGrid(SoprochForm->GetStringGridFilter());
      options_delit = CTextOfGrid("��������2", 2, f);
      options_delit.PaintOnStringGrid(SoprochForm->GetStringGridDelit());
      filters.LoadFromFile(f);
      options_modul = CTextOfGrid("������1", 1, f);
      options_modul.PaintOnStringGrid(ModulForm->GetStringGridModul());
      moduls.LoadFromFile(f);
      fclose(f);
      file_name = OpenDialog->FileName;
      file_name = ChangeFileExt(file_name, ".sp");
      Caption = "�������� ���������� - " + ExtractFileName(ChangeFileExt(file_name, ""));
//      edit_text_in_work = true;
//      compiler.RunEditTextInWork();
      edit_text_in_work = true;
      SetSave(true);
//      CompileLinkerItemClick(Sender);
    }
    else
      MessageBox(0, "�� ���� ������� ����.", "�������� �����", MB_ICONSTOP | MB_TASKMODAL | MB_OK);
  }
  if(!edits.size())
    edits.push_back(CEdit(image, ScrollBarX, ScrollBarY));
  uk = 0;
  edits[uk].Paint();
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FileSave(TObject *Sender)
{
  if(file_name == "��� �����.sp")
  {
    FileSaveAs(Sender);
    return;
  }
  FILE *f = fopen(file_name.c_str(), "wb");
  if(f)
  {
    options_proc.SaveToFile(f);
    CString str = CString(DivForm->GetInputDiv().c_str());
    str.SaveToFile(f);
    options_div.SaveToFile(f);
    options_ports.SaveToFile(f);
    options_uart.SaveToFile(f);
    options_timer.SaveToFile(f);
    options_RAM = CTextOfGrid("���1", string_grid_RAM);
    options_RAM.SaveToFile(f);
    SaveTextOfProrammToFile(f);
    fwrite(&soproch_enabled, sizeof(soproch_enabled), 1, f);
    options_filter.SaveToFile(f);
    options_delit.SaveToFile(f);
    filters.SaveToFile(f);
    options_modul.SaveToFile(f);
    moduls.SaveToFile(f);
    fclose(f);
    Caption = "�������� ���������� - " + ExtractFileName(ChangeFileExt(file_name, ""));
    edit_text_in_work = false;
//    compiler.RunEditTextInWork();
    edit_text_in_work = true;
   // if(PageControl1->ActivePage == TabSheetText)
//      if(PageControl5->ActivePage == TabSheet1)
 //       EditTextMain->SetFocus();
//      else if(PageControl5->ActivePage == TabSheet2)
//        EditTextIRQ->SetFocus();
//      else EditTextOpred->SetFocus();
    SetSave(true);
  }
  else
    MessageBox(0, "�� ���� ��������� ����������.", "����������", MB_ICONSTOP | MB_TASKMODAL | MB_OK);
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FileSaveAs(TObject *Sender)
{
  if(SaveDialog->Execute())
  {
    if(SaveDialog->FileName == "")
      return;
    file_name = SaveDialog->FileName;
    file_name = ChangeFileExt(file_name, ".sp");
    FileSave(Sender);
  }
}
//----------------------------------------------------------------------------
void TMainForm::SaveTextOfProrammToFile(FILE *f)
{
  options_text = CTextOfGrid("����1", StringGridEdits, false);
  options_text.SaveToFile(f);
  int size = edits.size();
  fwrite(&size, sizeof(size), 1, f);
  for(int i = 0; i < size; i ++)
    edits[i].SaveToFile(f);
}
//----------------------------------------------------------------------------
void TMainForm::LoadTextOfProrammFromFile(FILE *f)
{
  options_text = CTextOfGrid("����1", 1, f);
  options_text.PaintOnStringGrid(StringGridEdits, false);
  edits.clear();
  int size;
  if(fread(&size, sizeof(size), 1, f) != 1)
    return;
//  StringGridEdits->RowCount = size;
//  StringGridEdits->Row = 0;
  uk = 0;
  for(int i = 0; i < size; i ++)
    edits.push_back(CEdit(f, image, ScrollBarX, ScrollBarY));
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FilePrint(TObject *Sender)
{
  PrintForm->Set(edits, string_grid_RAM);
  PrintForm->ShowModal();
  return;
  if (PrintDialog->Execute())
  {
//    EditTextOpred->Print(Caption);
//    EditTextIRQ->Print(Caption);
//    EditTextMain1->Print(Caption);
//    EditTextMain2->Print(Caption);
//    EditTextMain3->Print(Caption);
//    EditTextMain4->Print(Caption);
  	//  "�������� ���������� - " + ChangeFileExt(file_name, "");
  }
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
m1:if(ButSave->Enabled)
    switch(MessageBox(0, "���������� �� ���������. ���������?", ChangeFileExt(file_name, "").c_str(),
           MB_ICONQUESTION | MB_YESNOCANCEL))
    {
      case IDYES: FileSave(Sender);
                  goto m1;
      case IDCANCEL: CanClose = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  StopDebug();
  edits.clear();
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FileExit(TObject *Sender)
{
  Close();
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::HelpContents(TObject *Sender)
{
  Application->HelpCommand(HELP_CONTENTS, 0);
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::HelpSearch(TObject *Sender)
{
  Application->HelpCommand(HELP_PARTIALKEY, Longint(""));
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::HelpHowToUse(TObject *Sender)
{
  Application->HelpCommand(HELP_HELPONHELP, 0);
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::string_grid_RAMSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  if(!ARow)
    return;
  if(ACol != 3)
    string_grid_RAM->Options << goEditing;
  else
  {
    string_grid_RAM->Options >> goEditing;
    if(string_grid_RAM->Cells[ACol][ARow] != "variable")
      string_grid_RAM->Cells[ACol][ARow] = "variable";
    else
      string_grid_RAM->Cells[ACol][ARow] = "const";
  }
  SetSave(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButAddRAMClick(TObject *Sender)
{
  string_grid_RAM->RowCount =  string_grid_RAM->RowCount + 1;
  string_grid_RAM->FixedRows =  1;
  int k = string_grid_RAM->RowCount-1;
  while(k > string_grid_RAM->Row)
  {
    for(int i = 1; i < string_grid_RAM->ColCount; i ++)
      string_grid_RAM->Cells[i][k] =  string_grid_RAM->Cells[i][k-1];
    k --;
  }
  string_grid_RAM->Cells[1][string_grid_RAM->Row] =  "����������";
  string_grid_RAM->Cells[2][string_grid_RAM->Row] =  "1";
  string_grid_RAM->Cells[3][string_grid_RAM->Row] =  "variable";
  string_grid_RAM->Cells[4][string_grid_RAM->Row] =  "0";
  SetSave(false);
  for(int i = 1; i < string_grid_RAM->RowCount; i ++)
    string_grid_RAM->Cells[0][i] =  IntToStr(i);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButSubRAMClick(TObject *Sender)
{
  if(string_grid_RAM->Row < 1)
    return;
  int k = string_grid_RAM->Row;
  while(k < (string_grid_RAM->RowCount-1))
  {
    for(int i = 1; i < string_grid_RAM->ColCount; i ++)
      string_grid_RAM->Cells[i][k] =  string_grid_RAM->Cells[i][k+1];
    k ++;
  }
  string_grid_RAM->RowCount =  string_grid_RAM->RowCount - 1;
  SetSave(false);
  for(int i = 1; i < string_grid_RAM->RowCount; i ++)
    string_grid_RAM->Cells[0][i] =  IntToStr(i);
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::ProcessorItemClick(TObject *Sender)
{
  if(OptionsProcForm->Run(options_proc))
    SetSave(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::PortsItemClick(TObject *Sender)
{
  if(PortsForm->Run(options_ports))
    SetSave(false);
}
//---------------------------------------------------------------------------
void TMainForm::SetSave(bool s)
{
//  MessageBox(0, "�� ���� ��������� ����������.", "����������", MB_ICONSTOP | MB_TASKMODAL | MB_OK);
  ButSave->Enabled = !s;
  FileSaveItem->Enabled = !s;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CompileLinkerItemClick(TObject *Sender)
{
  RunForm->Show();
//  Sleep(3000);
  if(EditError->Height < 100)
    EditError->Height = 100;
  EditError->Clear();
  MemoVhdl->Clear();
  StopDebug();
  compiler.SetStringGridPorts(PortsForm->GetStringGridPorts());
  compiler.SetStringGridUart(PeripheryForm->GetStringGridUart());
  compiler.SetStringGridTimer(PeripheryForm->GetStringGridTimer());
  compiler.SetStringGridDelit(SoprochForm->GetStringGridDelit());

  if(!DivForm->Linker())
  {
    RunForm->Hide();
    return;
  }  
  edit_text_in_work = false;
  if(!compiler.RunEditText(edits))
    EditError->Lines->Add("\n������ ������� �������!");
  else
  {
    EditError->Lines->Add("\n���������� ��������� ok\n");
    if(linker.Run(options_proc))
    {
      EditError->Lines->Add("\n�������� ���������� ok");
      RunDebug();
      vhdl.Run(ExtractFileName(ChangeFileExt(file_name, "")), MemoVhdl);
      EditError->Lines->Add("\n������� ��� ������� �����������...");
      ROM.PrintAll(grid_debug_Text2); 
    }
    else
      EditError->Lines->Add("\n���� ������...");
  }
  edit_text_in_work = true;
  RunForm->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::PeripheryItemClick(TObject *Sender)
{
  if(PeripheryForm->Run(options_uart, options_timer))
    SetSave(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SoprochClick(TObject *Sender)
{
  if(SoprochForm->Run(options_filter, options_delit))
    SetSave(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DivItemClick(TObject *Sender)
{
  if(DivForm->Run(options_div))
    SetSave(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ModulsItemClick(TObject *Sender)
{
  if(ModulForm->Run(options_modul))
    SetSave(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::imageClick(TObject *Sender)
{
  Panel13->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::imageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Button == Controls::mbLeft)
    edits[uk].OnMouseDown(X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::imageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  edits[uk].OnMouseMove(X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::imageMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Button == Controls::mbLeft)
    edits[uk].OnMouseUp(X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::imagePaint(TObject *Sender)
{
  edits[uk].Paint();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButAddPageClick(TObject *Sender)
{
  edits.insert(&edits[StringGridEdits->Row], CEdit(image, ScrollBarX, ScrollBarY));
  StringGridEdits->RowCount ++;
  for(int k = StringGridEdits->RowCount-1; k > StringGridEdits->Row; k --)
    StringGridEdits->Cells[0][k] =  StringGridEdits->Cells[0][k-1];
  StringGridEdits->Cells[0][StringGridEdits->Row] = "";
  StringGridEdits->Row = StringGridEdits->Row;
  edits[uk+1].SetActiv(false);
  edits[uk].Paint();
  SetSave(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BuSubPageClick(TObject *Sender)
{
  if(edits.size() > 1)
  {
    edits.erase(&edits[StringGridEdits->Row]);
    for(int i = StringGridEdits->Row; i < StringGridEdits->RowCount; i++)
      StringGridEdits->Cells[0][i] = StringGridEdits->Cells[0][i+1] ;
//    if(StringGridEdits->Row > edits.size())
  //    uk =
    StringGridEdits->RowCount = edits.size();
    edits[uk].Paint();
    SetSave(false);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::StringGridEditsSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  edits[uk].SetActiv(false);
  uk = ARow;
  edits[uk].Paint();
  StringGridEdits->Options >> goEditing;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::StringGridEditsDblClick(TObject *Sender)
{
  StringGridEdits->Options << goEditing;
  SetSave(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ScrollBarXChange(TObject *Sender)
{
  edits[uk].NewScrollX();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ScrollBarYChange(TObject *Sender)
{
  edits[uk].NewScrollY();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ApplicationEvents1ShortCut(TWMKey &Msg,
      bool &Handled)
{
  if(edits[uk].GetActiv())
  {
    switch(Msg.CharCode)
    {
      case VK_LEFT: case VK_RIGHT: case VK_UP: case VK_DOWN:
      case VK_INSERT:
      case VK_PRIOR: case VK_NEXT:
      case VK_HOME: case VK_END:
        edits[uk].KeyPress(Msg.CharCode, true);
        Handled = true; break;
      case VK_TAB: case VK_DELETE:
        edits[uk].KeyPress(Msg.CharCode, true);
        SetSave();
        Handled = true; break;
      default: break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Panel13Enter(TObject *Sender)
{
  edits[uk].SetActiv(true);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Panel13Exit(TObject *Sender)
{
  edits[uk].SetActiv(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Timer1Timer(TObject *Sender)
{
  edits[uk].NewTakt();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormKeyPress(TObject *Sender, char &Key)
{
  if(edits[uk].GetActiv())
  {
    edits[uk].KeyPress(Key, false);
    SetSave();
  }  
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if(edits[uk].GetActiv())
  {
    if(Key == VK_SHIFT)
    {
      edits[uk].SetShift(true);
      edits[uk].BeginVydelen();
    }
    else
      edits[uk].SetShift(false);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(edits[uk].GetActiv() && Key == VK_SHIFT)
    edits[uk].SetShift(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormMouseWheelDown(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
  if(edits[uk].GetActiv())
    edits[uk].MouseScroll(1);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormMouseWheelUp(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
  if(edits[uk].GetActiv())
    edits[uk].MouseScroll(-1);
}
//---------------------------------------------------------------------------

