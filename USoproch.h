//----------------------------------------------------------------------------
#ifndef USoprochH
#define USoprochH
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
#include <ComCtrls.hpp>
#include "CSPIN.h"
#include <Grids.hpp>

#include "COptions.h"
#include "CFilter.h"
//----------------------------------------------------------------------------
class TSoprochForm : public TForm
{
__published:
	TButton *OKBtn;
	TButton *CancelBtn;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TPanel *PanelFilter;
        TPageControl *PageControl2;
        TTabSheet *TabSheet3;
        TLabel *Label2;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label14;
        TGroupBox *GroupBox1;
        TLabel *Label3;
        TLabel *Label4;
        TCSpinEdit *CSpinEditMantisa;
        TCSpinEdit *CSpinEditPor;
        TEdit *EditB;
        TEdit *EditA;
        TGroupBox *GroupBox2;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TEdit *EditFd;
        TEdit *EditFr1;
        TEdit *EditFr2;
        TEdit *EditFr3;
        TEdit *EditA3;
        TEdit *EditA2;
        TEdit *EditA1;
        TEdit *EditSm;
        TCSpinEdit *CSpinEditKol;
        TTabSheet *TabSheet4;
        TPanel *Panel2;
        TScrollBar *ScrollBar1;
        TPageControl *PageControl3;
        TTabSheet *TabSheet5;
        TImage *image;
        TLabel *Label15;
        TLabel *Label16;
        TTrackBar *TrackBar1;
        TTabSheet *TabSheet6;
        TImage *imagefft;
        TLabel *Label17;
        TLabel *Label20;
        TTrackBar *TrackBar2;
        TStringGrid *StringGridFilter;
        TBitBtn *ButAdd;
        TBitBtn *ButSub;
        TBitBtn *ButRunFilter;
        TBitBtn *ButIn;
        TBitBtn *ButMidle;
        TBitBtn *ButOut;
        TLabel *Label1;
        TBitBtn *ButAddDelit;
        TBitBtn *ButSubDelit;
        TStringGrid *StringGridDelit;
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall CancelBtnClick(TObject *Sender);
        void __fastcall ButAddClick(TObject *Sender);
        void __fastcall ButSubClick(TObject *Sender);
        void __fastcall StringGridFilterSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect);
        void __fastcall ButRunFilterClick(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall ScrollBar1Change(TObject *Sender);
        void __fastcall TrackBar2Change(TObject *Sender);
        void __fastcall ButMidleClick(TObject *Sender);
        void __fastcall ButInClick(TObject *Sender);
        void __fastcall ButOutClick(TObject *Sender);
        void __fastcall ButAddDelitClick(TObject *Sender);
        void __fastcall ButSubDelitClick(TObject *Sender);
        void __fastcall StringGridDelitSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect);
        void __fastcall StringGridDelitSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
private:
        CFilters filters_new;
        bool result;
        void PrintFilter(int n);
        void SetFilter(int n);
        bool GetInt(int &c, AnsiString str, AnsiString err = "");
        bool GetDouble(int kol, double *c, AnsiString str, AnsiString err = "");
        void PaintFilter(int n,int s = 0);
public:
	virtual __fastcall TSoprochForm(TComponent* AOwner);
        bool Run(CTextOfGrid &tog_filter, CTextOfGrid &tog_delit);
        TStringGrid* GetStringGridFilter() {return StringGridFilter;}
        TStringGrid* GetStringGridDelit() {return StringGridDelit;}
};
//----------------------------------------------------------------------------
extern PACKAGE TSoprochForm *SoprochForm;
//----------------------------------------------------------------------------
#endif    
