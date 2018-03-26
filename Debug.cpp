//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Debug.h"
#pragma package(smart_init)
#include "Main.h"
//---------------------------------------------------------------------------
__fastcall Debug::Debug(bool CreateSuspended, vector <CEdit> e, TStringGrid *sg)
        : TThread(CreateSuspended), edits(e), StringGrid(sg)
{
}
//---------------------------------------------------------------------------
void __fastcall Debug::Execute()
{
  prev_line = 0;
  prev_page = 0;
  visiable_page = 1;
  visiable_line = 0;
  kol_call = 0;
  if_chek_line = if_chek_page = 0;

  PrintState();
  while(!Terminated)// П о т о к '''''''''''''''''''''''''''''''
  {
    if(kind_of_debug == kdNon)
      continue;
    switch(kind_of_debug)
    {
      case kdToNextLineIn:
        if(!RunCommand())
          break;
        kol_call = 0;
        if((prev_line != ROM.GetCurLineReal()) || (prev_page != ROM.GetCurPage()) || IsStopLine())
          kind_of_debug = kdNon;
        break;
      case kdToNextLine:
        if((YY.GetCommand(false) == "CALL") || (YY.GetCommand(false) == "IRQ"))
          kol_call ++;
        if((YY.GetCommand(false) == "RETURN") && kol_call)
          kol_call --;
        if(!RunCommand())  break;
        if( (((prev_line != ROM.GetCurLineReal()) || (prev_page != ROM.GetCurPage())) && !kol_call)
        || IsStopLine())
        {
          kol_call = 0;
          kind_of_debug = kdNon;
        }
        break;
      case kdToCheck:
        if((YY.GetCommand(false) == "CALL") || (YY.GetCommand(false) == "IRQ"))
          kol_call ++;
        if((YY.GetCommand(false) == "RETURN") && kol_call)
          kol_call --;
        if(!RunCommand())
          break;
        if( (((if_chek_line != ROM.GetCurLineReal()) || (if_chek_page != ROM.GetCurPage())) && !kol_call))
        {
          if_chek_line = if_chek_page = 0;
        }
        prev_line = ROM.GetCurLineReal();
        prev_page = ROM.GetCurPage();
        if(((prev_line == stop_line) && (prev_page == stop_page)) || IsStopLine())
          kind_of_debug = kdNon;
        break;
    }
    if(kind_of_debug == kdNon)//прорисовка
    {
      stop_line = stop_page = kol_call = 0;
      PrintState();
      SendMessage(MainForm->Handle, WM_USER, fdNextCommands, YY.GetStCommands());
    }
  }
}
//---------------------------------------------------------------------------
bool Debug::RunCommand()
{
  if(!YY.Run())
  {
    Terminate();
    MessageBox(0, "ПЗУ не даёт новой команды. Всё...", "Процессор", MB_ICONSTOP | MB_TASKMODAL | MB_OK);
    PostMessage(MainForm->Handle, WM_USER, fdBadCommand, YY.GetStCommands());
    return false;
  }
  SendMessage(MainForm->Handle, WM_USER, fdOneTakt, 0);
  return true;
}
//---------------------------------------------------------------------------
void Debug::Stop()
{
  PrintState();
  kol_call = 0;
}
//---------------------------------------------------------------------------
void Debug::PrintState()
{
  TStringGrid *g = NULL;
//  switch(visiable_page)
//  {
//    case 1 : g = grid_debug_Text11; break;
//    case 2 : g = grid_debug_Text12; break;
//    case 3 : g = grid_debug_Text13; break;
//    case 4 : g = grid_debug_Text14; break;
//    default : g = NULL; break;
//  }
  if(g)
 // if(g->RowCount < ROM.GetCurLineReal())
  {
    if(g->Cells[0][visiable_line] == ">ЗДЕСЬ")
      g->Cells[0][visiable_line] = ">стоп<";
    else if(g->Cells[0][visiable_line] == "ЗДЕСЬ")
      g->Cells[0][visiable_line] = "";
  }
  prev_line = ROM.GetCurLineReal();
  prev_page = ROM.GetCurPage();
  visiable_line = ROM.GetCurLineReal();
  visiable_page = ROM.GetCurPage();
/*  switch(visiable_page)
  {
    case 1 : g = grid_debug_Text11; PageControl->ActivePageIndex = 0; break;
    case 2 : g = grid_debug_Text12; PageControl->ActivePageIndex = 1; break;
    case 3 : g = grid_debug_Text13; PageControl->ActivePageIndex = 2; break;
    case 4 : g = grid_debug_Text14; PageControl->ActivePageIndex = 3; break;
    default : return;
  }*/
//  if(g->RowCount < ROM.GetCurLineReal())
  {
    g->Row = visiable_line;
    if(g->Cells[0][visiable_line] == "")
      g->Cells[0][visiable_line] = "ЗДЕСЬ";
    else if(g->Cells[0][visiable_line] == ">стоп<")
      g->Cells[0][visiable_line] = ">ЗДЕСЬ";
  }
  if_chek_line = visiable_line;
  if_chek_page = visiable_page;
}
//---------------------------------------------------------------------------
bool Debug::IsStopLine()
{
  switch(ROM.GetCurPage())
  {
    case 1 :
      if((if_chek_line == ROM.GetCurLineReal()) && (if_chek_page == 1))
        return false;
//      if((grid_debug_Text11->Cells[0][ROM.GetCurLineReal()] != ">стоп<") && (grid_debug_Text11->Cells[0][ROM.GetCurLineReal()] != ">ЗДЕСЬ"))
  //      return false;
      if_chek_page = 1; if_chek_line = ROM.GetCurLineReal();
      return true;
    case 2 :
      if((if_chek_line == ROM.GetCurLineReal()) && (if_chek_page == 2))
        return false;
//      if((grid_debug_Text12->Cells[0][ROM.GetCurLineReal()] != ">стоп<") && (grid_debug_Text12->Cells[0][ROM.GetCurLineReal()] != ">ЗДЕСЬ"))
  //      return false;
      if_chek_page = 2; if_chek_line = ROM.GetCurLineReal();
      return true;
//      if_chek_page = 2; if_chek_line = ROM.GetCurLineReal();
  //    return (grid_debug_Text12->Cells[0][ROM.GetCurLineReal()] == ">стоп<") || (grid_debug_Text12->Cells[0][ROM.GetCurLineReal()] == ">ЗДЕСЬ");
    case 3 :
      if((if_chek_line == ROM.GetCurLineReal()) && (if_chek_page == 3))
        return false;
//      if((grid_debug_Text13->Cells[0][ROM.GetCurLineReal()] != ">стоп<") && (grid_debug_Text13->Cells[0][ROM.GetCurLineReal()] != ">ЗДЕСЬ"))
  //      return false;
      if_chek_page = 3; if_chek_line = ROM.GetCurLineReal();
      return true;
    case 4 :
      if((if_chek_line == ROM.GetCurLineReal()) && (if_chek_page == 4))
        return false;
//      if((grid_debug_Text14->Cells[0][ROM.GetCurLineReal()] != ">стоп<") && (grid_debug_Text14->Cells[0][ROM.GetCurLineReal()] != ">ЗДЕСЬ"))
  //      return false;
      if_chek_page = 4; if_chek_line = ROM.GetCurLineReal();
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------
