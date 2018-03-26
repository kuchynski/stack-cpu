//---------------------------------------------------------------------------
#ifndef CString_H
#define CString_H
#include <String.h>
#include <vcl.h>

#include <stdio.h>
#include "CError.h"
//---------------------------------------------------------------------------
class CString
{
  private:
    char *str;
  public:
    CString(const char *new_str = "");
    CString(int n);
    CString(const CString &s);
    CString(int n, int size);
    CString(FILE *f);
    ~CString();
    CString operator=(const CString &ks);
    bool operator==(const CString &ks)const  {return strcmp(str, ks.str) == 0;}
    bool operator!=(const CString &ks)const  {return !(*this == ks);}
    bool operator>(const CString &ks)const  {return strcmp(str, ks.str) > 0;}
    bool operator<(const CString &ks)const  {return strcmp(str, ks.str) < 0;}
    bool operator<=(const CString &ks)const  {return !(*this > ks);}
    bool operator>=(const CString &ks)const  {return !(*this < ks);}
    CString operator+=(const CString &ks);
    void SaveToFile(FILE *f)const;
    AnsiString AnsiString() const;
    int GetInt (void);
};
CString operator+(const CString &ks1, const CString &ks2);
//---------------------------------------------------------------------------
#endif
