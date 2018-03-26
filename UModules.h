//----------------------------------------------------------------------------
#ifndef UModulesH
#define UModulesH
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
#include <Grids.hpp>

#include "COptions.h"
#include "CModuls.h"
//----------------------------------------------------------------------------
class TModulForm : public TForm
{
__published:
        TPanel *Panel3;
        TMemo *Memo2;
        TStringGrid *grid_entity;
        TMemo *Memo7;
        TPanel *Panel4;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TMemo *Memo8;
        TPanel *Panel1;
        TButton *OKBtn;
        TButton *CancelBtn;
        TPanel *Panel2;
        TStringGrid *StringGridModul;
        TBitBtn *ButAdd;
        TBitBtn *ButSub;
        TPanel *Panel5;
        TMemo *memo_arch;
        TMemo *memo_signals;
        TMemo *Memo5;
        TMemo *Memo6;
        TMemo *Memo3;
        void __fastcall ButAddClick(TObject *Sender);
        void __fastcall ButSubClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall CancelBtnClick(TObject *Sender);
        void __fastcall StringGridModulSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect);
        void __fastcall grid_entitySelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall grid_entityGetEditMask(TObject *Sender, int ACol,
          int ARow, AnsiString &Value);
        void __fastcall grid_entitySetEditText(TObject *Sender, int ACol,
          int ARow, const AnsiString Value);
        void __fastcall StringGridModulSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
private:	// User declarations
        CModuls moduls_new;
        bool result;
        void PrintModul(int n);
        void SetModul(int n);
public:		// User declarations
	virtual __fastcall TModulForm(TComponent* AOwner);
        bool Run(CTextOfGrid &tog_modul);
        TStringGrid* GetStringGridModul() {return StringGridModul;}
};
//----------------------------------------------------------------------------
extern PACKAGE TModulForm *ModulForm;
//----------------------------------------------------------------------------
#endif    
