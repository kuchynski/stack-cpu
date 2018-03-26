//---------------------------------------------------------------------------
#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <sysutils.hpp>
#include <windows.hpp>
#include <messages.hpp>
#include <sysutils.hpp>
#include <classes.hpp>
#include <graphics.hpp>
#include <controls.hpp>
#include <forms.hpp>
#include <dialogs.hpp>
#include <stdctrls.hpp>
#include <buttons.hpp>
#include <extctrls.hpp>
#include <menus.hpp>
#include <Classes.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>

#include "UOptionsProcForm.h"
#include "UPortsForm.h"
#include "CDictionary.h"

#include "CCompiler.h"
#include "CLinker.h"
#include "Debug.h"
#include "UInputsForm.h"
#include "UPeriphery.h"
#include "UDiv.h"
#include "AboutForm.h"
#include "AboutForm1.h"
#include "URam.h"
#include "CVHDL.h"
#include "URunForm.h"
#include "UPrint.h"
#include "USoproch.h"
#include "UModules.h"
#include "CEdit.h"
#include <AppEvnts.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:   
	TMainMenu *MainMenu;
	TMenuItem *FileNewItem;
	TMenuItem *FileOpenItem;
	TMenuItem *FileSaveItem;
	TMenuItem *FileSaveAsItem;
	TMenuItem *FilePrintItem;
	TMenuItem *FileExitItem;
        TMenuItem *HelpWrite;
        TMenuItem *HelpDebug;
        TMenuItem *HelpForth;
	TMenuItem *HelpAboutItem;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TPrintDialog *PrintDialog;
        TMenuItem *OptionsMenu;
        TPanel *Panel1;
        TSplitter *Splitter1;
        TMenuItem *ProcessorItem;
        TMenuItem *PortsItem;
        TMenuItem *PeripheryItem;
        TBitBtn *ButNew;
        TBitBtn *ButLoad;
        TBitBtn *ButSave;
        TBitBtn *ButPrint;
        TMenuItem *RunMenu;
        TBitBtn *ButRun;
        TMenuItem *CompileLinkerItem;
        TPageControl *PageControl1;
        TTabSheet *TabSheetText;
        TTabSheet *TabSheetDebug2;
        TPanel *Panel10;
        TPanel *PanelLeftDebug;
        TStringGrid *grid_debug_Text2;
        TPanel *Panel4;
        TPanel *Panel2;
        TPanel *PanelPosText;
        TPanel *Panel17;
        TRichEdit *EditError;
        TSplitter *Splitter2;
        TMenuItem *DivItem;
        TTabSheet *TabSheetVHDL;
        TMemo *MemoVhdl;
        TMenuItem *HelpMain;
        TMenuItem *HelpVHDL;
        TPageControl *PageControl2;
        TSplitter *Splitter5;
        TTabSheet *TabSheetRAM;
        TTabSheet *TabSheetRAMPer;
        TPanel *PanelRightDebug;
        TPanel *Panel8;
        TSplitter *Splitter4;
        TPanel *Panel5;
        TStringGrid *string_grid_RAM;
        TPanel *Panel7;
        TLabel *Label1;
        TBitBtn *ButAddRAM;
        TBitBtn *ButSubRAM;
        TTabSheet *TabSheetDebug1;
        TPanel *Panel3;
        TBitBtn *ButAllCommands1;
        TBitBtn *ButNextWord1;
        TBitBtn *ButNextComFort1;
        TBitBtn *ButSetCommand1;
        TBitBtn *ButStopPoint1;
        TBitBtn *ButDebugTo1;
        TBitBtn *ButDebugStop1;
        TBitBtn *ButInput1;
        TPageControl *PageControl3;
        TTabSheet *TabSheetRAMD;
        TTabSheet *TabSheetPorts;
        TStringGrid *grid_debug_ports;
        TPageControl *PageControl4;
        TTabSheet *TabSheetUART;
        TTabSheet *TabSheetIRQ;
        TStringGrid *grid_debug_irq;
        TStringGrid *grid_debug_uarts;
        TTabSheet *TabSheetTimers;
        TStringGrid *grid_debug_timers;
        TPanel *Panel15;
        TPanel *Panel16;
        TLabel *Label3;
        TLabel *Label4;
        TSpeedButton *ButStackDataSS;
        TStringGrid *grid_stack_data;
        TStringGrid *grid_stack_return;
        TSplitter *Splitter3;
        TPanel *Panel9;
        TStringGrid *grid_debug_RAM;
        TSpeedButton *ButRAMSS;
        TBitBtn *ButTakt;
        TMenuItem *Soproch;
        TMenuItem *ModulsItem;
        TPanel *Panel6;
        TPanel *Panel11;
        TSplitter *Splitter6;
        TStringGrid *StringGridEdits;
        TPanel *Panel12;
        TBitBtn *ButAddPage;
        TBitBtn *BuSubPage;
        TSplitter *Splitter7;
        TPanel *Panel14;
        TApplicationEvents *ApplicationEvents1;
        TScrollBar *ScrollBarY;
        TScrollBar *ScrollBarX;
        TPanel *Panel13;
        TPaintBox *image;
        TTimer *Timer1;
        TPanel *Panel19;
        TPanel *Panel18;
        TStringGrid *StringGridEditsD;
        TSplitter *Splitter8;
        TPaintBox *imaged;
	void __fastcall FileNew(TObject *Sender);
	void __fastcall FileOpen(TObject *Sender);
	void __fastcall FileSave(TObject *Sender);
	void __fastcall FileSaveAs(TObject *Sender);
	void __fastcall FilePrint(TObject *Sender);
	void __fastcall FileExit(TObject *Sender);
	void __fastcall HelpContents(TObject *Sender);
	void __fastcall HelpSearch(TObject *Sender);
	void __fastcall HelpHowToUse(TObject *Sender);
        void __fastcall ProcessorItemClick(TObject *Sender);
        void __fastcall string_grid_RAMSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall ButAddRAMClick(TObject *Sender);
        void __fastcall ButSubRAMClick(TObject *Sender);
        void __fastcall PortsItemClick(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall CompileLinkerItemClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ButRAMSSClick(TObject *Sender);
        void __fastcall ButDebugStop2Click(TObject *Sender);
        void __fastcall ButInput2Click(TObject *Sender);
        void __fastcall PeripheryItemClick(TObject *Sender);
        void __fastcall DivItemClick(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall ButAllCommands2Click(TObject *Sender);
        void __fastcall ButStackDataSSClick(TObject *Sender);
        void __fastcall HelpAboutItemClick(TObject *Sender);
        void __fastcall ButTaktClick(TObject *Sender);
        void __fastcall ButNextWord1Click(TObject *Sender);
        void __fastcall ButNextComFort1Click(TObject *Sender);
        void __fastcall ButSetCommand1Click(TObject *Sender);
        void __fastcall ButStopPoint1Click(TObject *Sender);
        void __fastcall ButDebugTo1Click(TObject *Sender);
        void __fastcall HelpMainClick(TObject *Sender);
        void __fastcall SoprochClick(TObject *Sender);
        void __fastcall ModulsItemClick(TObject *Sender);
        void __fastcall imageClick(TObject *Sender);
        void __fastcall imageMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall imageMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall imageMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall imagePaint(TObject *Sender);
        void __fastcall ButAddPageClick(TObject *Sender);
        void __fastcall ScrollBarXChange(TObject *Sender);
        void __fastcall ApplicationEvents1ShortCut(TWMKey &Msg,
          bool &Handled);
        void __fastcall BuSubPageClick(TObject *Sender);
        void __fastcall Panel13Enter(TObject *Sender);
        void __fastcall Panel13Exit(TObject *Sender);
        void __fastcall StringGridEditsSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormMouseWheelDown(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
        void __fastcall FormMouseWheelUp(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
        void __fastcall ScrollBarYChange(TObject *Sender);
        void __fastcall StringGridEditsDblClick(TObject *Sender);
private:        // private user declarations
        vector <CEdit> edits;
        unsigned uk;
  //      COptionsProc options_proc;
        CTextOfGrid options_div;
        CTextOfGrid options_ports;
        CTextOfGrid options_RAM;
        CTextOfGrid options_uart;
        CTextOfGrid options_timer;
        CTextOfGrid options_filter;
        CTextOfGrid options_delit;
        CTextOfGrid options_modul;
        CTextOfGrid options_text;
        AnsiString file_name;
        CVHDL vhdl;
        bool edit_text_in_work;
        bool soproch_enabled;

        CCompiler compiler;
        Debug *debug;
        int number_takt;

        void SetSave(bool s = false);
        void SaveTextOfProrammToFile(FILE *f);
        void LoadTextOfProrammFromFile(FILE *f);

        void SetAttributesColor(int begin, int end, TColor color);
        void SetAttributesStyle(int begin, int end, TFontStyle style);

        void SetVydelen();

        void RunDebug();
        void StopDebug();

        void __fastcall FromDebug(TMessage& message);
        void PrintTakt(int t);

public:         // public user declarations
	virtual __fastcall TMainForm(TComponent* Owner);
        BEGIN_MESSAGE_MAP
          MESSAGE_HANDLER(WM_USER, TMessage, FromDebug);
        END_MESSAGE_MAP(TComponent)
};
//---------------------------------------------------------------------------
extern TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
