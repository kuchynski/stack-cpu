//---------------------------------------------------------------------------
#pragma hdrstop

#include "CPort.h"
//---------------------------------------------------------------------------
CPort::CPort(AnsiString n, int s, bool ei, bool eo, bool eoi, bool in): name(n), size(s),
       enabled_in(ei), enabled_out(eo), in(0), out(0), enabled_out_irq(eoi), internal(in), kol_use(0) {}
//---------------------------------------------------------------------------
unsigned CPort::GetIn()
{
  return enabled_in? in : GetOut();
}
//---------------------------------------------------------------------------
unsigned CPort::GetOut()
{
  return enabled_out? out : 0;
}
//---------------------------------------------------------------------------
bool CPort::SetIn(unsigned n)
{
  in = Norma(n);
  return enabled_in;
}
//---------------------------------------------------------------------------
bool CPort::SetOut(unsigned n)
{
  out = Norma(n);
  return enabled_out;
}
//---------------------------------------------------------------------------
unsigned CPort::Norma(unsigned ch)
{
  unsigned maska = 0xFFFFFFFF >> (32 - size);
  return ch & maska;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
