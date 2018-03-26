//----------------------------------------------------------------------------
#ifndef UPeripheryH
#define UPeripheryH
//----------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <Messages.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>

#include "COptions.h"
#include <Buttons.hpp>
//----------------------------------------------------------------------------
class TPeripheryForm : public TForm
{
__published:
        TButton *ButOk;
        TButton *ButCancel;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
        TBitBtn *ButAddUart;
        TBitBtn *ButSubUart;
        TStringGrid *StringGridUart;
        TBitBtn *BuSubTimer;
        TBitBtn *ButAddTimer;
        TStringGrid *StringGridTimer;
        void __fastcall ButAddUartClick(TObject *Sender);
        void __fastcall ButSubUartClick(TObject *Sender);
        void __fastcall StringGridUartGetEditMask(TObject *Sender,
          int ACol, int ARow, AnsiString &Value);
        void __fastcall StringGridUartSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall ButOkClick(TObject *Sender);
        void __fastcall StringGridUartSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
        void __fastcall ButAddTimerClick(TObject *Sender);
        void __fastcall BuSubTimerClick(TObject *Sender);
        void __fastcall StringGridTimerSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect);
        void __fastcall StringGridTimerSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
        void __fastcall StringGridTimerGetEditMask(TObject *Sender,
          int ACol, int ARow, AnsiString &Value);
private:
        bool result;
public:
	virtual __fastcall TPeripheryForm(TComponent *Owner);
        bool Run(CTextOfGrid &tog_uart, CTextOfGrid &tog_timer);
        TStringGrid* GetStringGridUart() {return StringGridUart;}
        TStringGrid* GetStringGridTimer() {return StringGridTimer;}
};
//----------------------------------------------------------------------------
extern TPeripheryForm *PeripheryForm;
//----------------------------------------------------------------------------
#endif
