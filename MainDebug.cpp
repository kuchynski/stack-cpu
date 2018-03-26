//---------------------------------------------------------------------------
#pragma hdrstop

#include "Main.h"
#include "MainDebug.h"
//---------------------------------------------------------------------------
void TMainForm::RunDebug()
{
  return;
  StopDebug();
  ROM.SetState(0);
  ButTakt->Visible = true;
  ButDebugStop1->Visible = false;
  ButDebugTo1->Visible = true;
  ButNextWord1->Visible = true;
  ButNextComFort1->Visible = true;
  ButSetCommand1->Visible = true;
  ButInput1->Visible = true;
  ButStopPoint1->Visible = true;
  ButAllCommands1->Visible = true;
  ROM.PrintAll(grid_debug_Text2);
  RAM.PrintAll(grid_debug_RAM);
  IRQ.PrintAll(grid_debug_irq);
  ports.PrintAll(grid_debug_ports);
  uarts.PrintAll(grid_debug_uarts);
  timers.PrintAll(grid_debug_timers);
  stack_data.PrintAll(grid_stack_data);
  stack_return.PrintAll(grid_stack_return);
  debug = new class Debug(true, edits, StringGridEditsD);
  debug->kind_of_debug = kdNon;
  PrintTakt(0);
  debug->PrintState();
}
//---------------------------------------------------------------------------
void TMainForm::StopDebug()
{
  ButTakt->Visible = false;
  ButDebugStop1->Visible = false;
  ButDebugTo1->Visible = false;
  ButNextWord1->Visible = false;
  ButNextComFort1->Visible = false;
  ButSetCommand1->Visible = false;
  ButInput1->Visible = false;
  ButStopPoint1->Visible = false;
  ButAllCommands1->Visible =false;
  YY.Reset();
  if(!debug)
    return;
  debug->Terminate();
  debug->Resume();
  debug->WaitFor();
  delete debug;
  debug = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButRAMSSClick(TObject *Sender)
{
  ButRAMSS->Caption = (ButRAMSS->Caption == "dec")? "hex" : "dec";
  RAM.PrintValue((ButRAMSS->Caption == "dec")? 10 : 16);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButStackDataSSClick(TObject *Sender)
{
  ButStackDataSS->Caption = (ButStackDataSS->Caption == "dec")? "hex" : "dec";
  stack_data.PrintValue((ButStackDataSS->Caption == "dec")? 10 : 16);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButNextWord1Click(TObject *Sender)
{
  debug->Stop();
  debug->kind_of_debug = kdToNextLine;
  debug->Resume();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButNextComFort1Click(TObject *Sender)
{
  debug->Stop();
  debug->kind_of_debug = kdToNextLineIn;
  debug->Resume();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButDebugTo1Click(TObject *Sender)
{
 // debug->Stop();
 /* if(PageControl6->ActivePageIndex == 0)
    debug->SetStopLine(1, grid_debug_Text11->Row);
  else if(PageControl6->ActivePageIndex == 1)
    debug->SetStopLine(2, grid_debug_Text12->Row);
  else if(PageControl6->ActivePageIndex == 2)
    debug->SetStopLine(3, grid_debug_Text13->Row);
  else if(PageControl6->ActivePageIndex == 3)
    debug->SetStopLine(4, grid_debug_Text14->Row);
  else return;
//  debug->row_check = grid_debug_Text11->Row;
  debug->kind_of_debug = kdToCheck;
  ButDebugStop1->Visible = true;
  ButDebugTo1->Visible = false;
  ButNextWord1->Visible = false;
  ButNextComFort1->Visible = false;
  ButSetCommand1->Visible = false;
  ButInput1->Visible = false;
  ButStopPoint1->Visible = false;
  ButAllCommands1->Visible = false;
  debug->Resume(); */
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButSetCommand1Click(TObject *Sender)
{
/*  TStringGrid *g;
  if(PageControl6->ActivePageIndex == 0)
    g = grid_debug_Text11;
  else if(PageControl6->ActivePageIndex == 1)
    g = grid_debug_Text12;
  else if(PageControl6->ActivePageIndex == 2)
    g = grid_debug_Text13;
  else if(PageControl6->ActivePageIndex == 3)
    g = grid_debug_Text14;
  unsigned new_address = ROM.GetAddressInLine(PageControl6->ActivePageIndex + 1, g->Row);
  YY.SetStCommands(new_address);
 //debug->cur_line = grid_debug_Text11->Row;
  debug->PrintState();  */
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButDebugStop2Click(TObject *Sender)
{
//  ButDebugStop1->Visible = false;
  ButDebugTo1->Visible = true;
  ButNextWord1->Visible = true;
  ButNextComFort1->Visible = true;
  ButSetCommand1->Visible = true;
  ButInput1->Visible = true;
  ButStopPoint1->Visible = true;
  ButAllCommands1->Visible = true;
  debug->kind_of_debug = kdNon;
  debug->Stop();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButAllCommands2Click(TObject *Sender)
{
  debug->row_check = 0;
  debug->kind_of_debug = kdToCheck;
  ButDebugStop1->Visible = true;
  ButDebugTo1->Visible = false;
  ButNextWord1->Visible = false;
  ButNextComFort1->Visible = false;
  ButSetCommand1->Visible = false;
  ButInput1->Visible = false;
  ButStopPoint1->Visible = false;
  ButAllCommands1->Visible = false;
  debug->Resume();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButStopPoint1Click(TObject *Sender)
{
 /* TStringGrid *g;
  if(PageControl6->ActivePageIndex == 0)
    g = grid_debug_Text11;
  else if(PageControl6->ActivePageIndex == 1)
    g = grid_debug_Text12;
  else if(PageControl6->ActivePageIndex == 2)
    g = grid_debug_Text13;
  else if(PageControl6->ActivePageIndex == 3)
    g = grid_debug_Text14;
  else return;
  if(g->Cells[0][g->Row] == ">ÇÄÅÑÜ")
    g->Cells[0][g->Row] = "ÇÄÅÑÜ";
  else if(g->Cells[0][g->Row] == "ÇÄÅÑÜ")
    g->Cells[0][g->Row] = ">ÇÄÅÑÜ";
  else if(g->Cells[0][g->Row] == ">ñòîï<")
    g->Cells[0][g->Row] = "";
  else if(g->Cells[0][g->Row] == "")
    g->Cells[0][g->Row] = ">ñòîï<";   */
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  switch(Key)
  {
    case VK_F4:  if(ButDebugTo1->Visible) ButDebugTo1->Click(); break;
    case VK_F7:  if(ButNextComFort1->Visible) ButNextComFort1->Click(); break;
    case VK_F8:  if(ButNextWord1->Visible) ButNextWord1->Click(); break;
    case VK_F2:  if(ButDebugStop1->Visible) ButDebugStop1->Click(); break;
    case VK_F12: if(ButInput1->Visible) ButInput1->Click(); break;
    case VK_F5:  if(ButStopPoint1->Visible) ButStopPoint1->Click(); break;
    case VK_F6:  if(ButAllCommands1->Visible) ButAllCommands1->Click(); break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButTaktClick(TObject *Sender)
{
  PrintTakt(0);
}
//---------------------------------------------------------------------------
void TMainForm::PrintTakt(int t)
{
  number_takt = t;
  double sec = (double)number_takt/(double)DivForm->GetInputDivInt();
  ButTakt->Caption = IntToStr(number_takt) + " òàêò   " + FloatToStrF(sec , ffFixed, 9, 9) + " ñ";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FromDebug(TMessage& message)
{
  switch(message.WParam)
  {
    case fdBadCommand:
      StopDebug();
      break;
    case fdOneTakt:
      PrintTakt(number_takt+1);
      break;
    case fdNextCommands:
      debug->Suspend();
      debug->kind_of_debug = kdNon;
//      ButDebugStop1->Visible = false;
      ButDebugTo1->Visible = true;
      ButNextWord1->Visible = true;
      ButNextComFort1->Visible = true;
      ButSetCommand1->Visible = true;
      ButInput1->Visible = true;
      ButStopPoint1->Visible = true;
      ButAllCommands1->Visible = true;
      break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButInput2Click(TObject *Sender)
{
  InputForm->Run();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::HelpAboutItemClick(TObject *Sender)
{
  AboutBox->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::HelpMainClick(TObject *Sender)
{
  AboutBox1->ShowModal();
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
