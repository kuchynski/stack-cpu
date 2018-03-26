//---------------------------------------------------------------------------
#ifndef CModulsH
#define CModulsH

#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include "CPorts.h"
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
struct SModul
{
  CString name;
  CTextOfGrid entity;
  CString signals;
  CString arch;
  SModul():entity ("Entity1", 4) {};
  SModul(FILE *f);
  SModul(AnsiString n, AnsiString f, long m, long p, int kc,
          double *ca, double *cb, int fd_, int *fr_, int *a_, int sm_);
  void SaveToFile(FILE *f);
};
//---------------------------------------------------------------------------
class CModuls
{
  private:
    vector <SModul> modules;
  public:  
    ~CModuls();
    void Clear();
    void SetModuls(const vector <SModul> &m) {modules = m;}
    SModul GetModul(int n);

    bool LoadFromFile(FILE *f);
    bool SaveToFile(FILE *f);
    bool Sub(int n);
    bool Add(int n, SModul &f);
    bool Set(int n, SModul &f);

    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
    int GetSize() { return modules.size();}
};
extern CModuls moduls;
//---------------------------------------------------------------------------
#endif
