//---------------------------------------------------------------------------
#ifndef CVHDLH
#define CVHDLH

#include <controls.hpp>
#include "CRam.h"
#include "CRom.h"
#include "CStackData.h"
#include "CUart.h"
#include "CTimers.h"
#include "CPorts.h"
#include "CALU.h"
#include "CYY.h"
#include "CFilter.h"
#include "CDelit.h"
#include "CModuls.h"
//---------------------------------------------------------------------------
class CVHDL
{
  private:
    TMemo *memo;
  public:
    void Run(AnsiString name, TMemo *m);
};
extern CVHDL vhdl;
//---------------------------------------------------------------------------
#endif
