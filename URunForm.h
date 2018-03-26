//----------------------------------------------------------------------------
#ifndef URunFormH
#define URunFormH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TRunForm : public TForm
{
__published:
private:
public:
	virtual __fastcall TRunForm(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TRunForm *RunForm;
//----------------------------------------------------------------------------
#endif    
