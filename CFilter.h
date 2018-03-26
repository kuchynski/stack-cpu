//---------------------------------------------------------------------------
#ifndef CFilterH
#define CFilterH

#include "CDataFilter.h"
#include <vector>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include "CPorts.h"
#include <stdio.h>
#include <math.h>
using namespace std;

struct SFilter
{
  char name[100];
  char frequency[100];
  long mantisa, por;
  int kol_coof;
  double coof_a[100], coof_b[100];
  int fd, fr[3], a[3], sm;
  double xfft1[4096], yfft1[4096];
  double xfft2[4096], yfft2[4096];
  double xfft3[4096], yfft3[4096];
  int data_in[10000], data_midle[10000], data_out[10000];
  CDataFilter coof_bd[100], coof_ad[100];
  int center, begin, sz, sz2;
  SFilter();
  SFilter(AnsiString n, AnsiString f, long m, long p, int kc,
          double *ca, double *cb, int fd_, int *fr_, int *a_, int sm_);
  AnsiString GetCoofA();
  AnsiString GetCoofB();
};
//---------------------------------------------------------------------------
class CFilters
{
  private:
    vector <SFilter> modules;
    void Paint(int n, TImage *image, int *data, TColor color, int cen);
    AnsiString DownTo(int end, int begin);
    AnsiString GetNumberBin(int n, int kol_bit);

    void FFT(int n, int *in, double *x, double *y);
    void RunFFT(int n);
    void RunFFT2(int n);
    void PaintFFT(int n, TImage *image, double *x, double *y, TColor color);
  public:
    ~CFilters();
    void Clear();
    void SetFilters(const vector <SFilter> &m) {modules = m;}

    bool LoadFromFile(FILE *f);
    bool SaveToFile(FILE *f);
    bool Sub(int n);
    bool Add(int n, SFilter &f);
    bool Set(int n, SFilter &f);
    SFilter GetFilter(int n);

    void Paint(int n, TImage *image, int s = 0);
    void PaintFFT(int n, TImage *image, int s = 0);
    void SetBegin(int n, int b);
    void SetSz(int n, int b);
    void SetSz2(int n, int b);
    void Run(int n);
    void RunAll();
    void PrintVHDLSignals(TMemo *memo);
    void PrintVHDLProcess(TMemo *memo);
    int GetSize() { return modules.size();}
};
extern CFilters filters;
//---------------------------------------------------------------------------
#endif
