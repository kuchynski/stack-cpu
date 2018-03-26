//----------------------------------------------------------------------------
#ifndef UPrintH
#define UPrintH
//----------------------------------------------------------------------------
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include "CEdit.h"
//----------------------------------------------------------------------------
class TPrintForm : public TForm
{
__published:
	TButton *CancelBtn;
	TBevel *Bevel1;
        TPrinterSetupDialog *PrinterSetupDialog1;
        TButton *ButSetup;
        TButton *ButPrint1;
        TButton *Button3;
        void __fastcall ButSetupClick(TObject *Sender);
        void __fastcall ButPrint1Click(TObject *Sender);
private:
        vector <CEdit> edits;
        TStringGrid *grid_RAM;
public:
	virtual __fastcall TPrintForm(TComponent* AOwner);
        void Set(vector <CEdit> e, TStringGrid *gr);
};
//----------------------------------------------------------------------------
extern PACKAGE TPrintForm *PrintForm;
//----------------------------------------------------------------------------
#endif    
