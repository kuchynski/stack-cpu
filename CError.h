#ifndef CError_H
#define CError_H
//---------------------------------------------------------------------------
class CError
{
  public:
    enum EError {eeFile, eeCom, eeDefault};
    CError(EError e = eeDefault): error(e) {};
  private:
    EError error;};
//---------------------------------------------------------------------------
#endif

