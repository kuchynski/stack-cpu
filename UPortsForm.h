//----------------------------------------------------------------------------
#ifndef UPortsFormH
#define UPortsFormH
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
#include <Grids.hpp>
#include <Buttons.hpp>

#include "COptions.h"
//----------------------------------------------------------------------------
class TPortsForm : public TForm
{
__published:
        TButton *ButtonOk;
        TButton *ButtonCancel;
        TBitBtn *ButAdd;
        TBitBtn *ButSubPort;
        TStringGrid *StringGridPorts;
        void __fastcall StringGridPortsSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect);
        void __fastcall ButAddClick(TObject *Sender);
        void __fastcall ButSubPortClick(TObject *Sender);
        void __fastcall ButtonOkClick(TObject *Sender);
        void __fastcall StringGridPortsGetEditMask(TObject *Sender,
          int ACol, int ARow, AnsiString &Value);
        void __fastcall StringGridPortsSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
private:
        bool result;
public:
	virtual __fastcall TPortsForm(TComponent *Owner);
        bool Run(CTextOfGrid &op);
        TStringGrid* GetStringGridPorts() {return StringGridPorts;}
};
//----------------------------------------------------------------------------
extern TPortsForm *PortsForm;
//----------------------------------------------------------------------------
#endif
