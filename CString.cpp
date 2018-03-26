//---------------------------------------------------------------------------
#pragma hdrstop

#include "CString.h"
//---------------------------------------------------------------------------
CString::CString(const char *new_str)
{
  int size = strlen(new_str);

 // if(size > 255)
 //   size = 255;

  str = new char[size+1];
  strncpy(str, new_str, size);
  str[size] = '\0';
}
//---------------------------------------------------------------------------
CString::CString(int n)
{
  int size = 0, n2 = abs(n);
  do{
    n2 /= 10;
    size ++;
  }while(n2);
  size += (n < 0)? 1 : 0;
  str = new char[size + 1];

  char i = (n2!=n)? 1 : 0, ch;
  str[0] = '-';
  do{
    str[i++] = (char)(n2 % 10) - 0x30;
    n2 /= 10;
  }while(n2);
  str[size] = '\0';
}
//---------------------------------------------------------------------------
CString::CString(const CString &s)
{
  str = new char[strlen(s.str)+1];
  strcpy(str, s.str);
  str[strlen(s.str)] = '\0';
}
//---------------------------------------------------------------------------
CString::CString(FILE *f)
{
  int size;
  str = NULL;

  if(fread(&size, sizeof(size), 1, f) != 1)
    throw CError(CError::eeFile);
//    throw this;
  str = new char[size+1];
  int w;
  w = fread(str, size+1, 1, f);
  if(w!=1)
    throw CError(CError::eeFile);
  str[size] = '\0';
}
//-------------------------------------------------------------
CString::CString (int n, int size)
{
  char Mas[5];
  int i, s = 0;

  do{
    Mas[s] = n % 10;
    s ++;
    n /= 10;
  }while(n);
  Mas[s] = 0;
  str = new char[size + 1];
  str[size] = '\0';
  for(s = size - 1, i = 0; s >= 0; s --, i ++)
    str[i] = Mas[s] + 0x30;
}
//---------------------------------------------------------------------------
CString::~CString()
{
  if(str)
  {
    delete [] str;
    str = NULL;
  }  
}
//---------------------------------------------------------------------------
CString CString::operator+=(const CString &ks)
{
  char *str_tmp = str;
  str = new char[strlen(str_tmp) + strlen(ks.str) + 1];
  strcpy(str, str_tmp);
  strcpy(str + strlen(str_tmp), ks.str);
  str[strlen(str_tmp) + strlen(ks.str)] = '\0';
  delete []str_tmp;
  return *this;
}
//---------------------------------------------------------------------------
CString operator+(const CString &ks1, const CString &ks2)
{
  return CString(ks1) += ks2;
}
//---------------------------------------------------------------------------
CString CString::operator=(const CString &ks)
{
  if(this != &ks)
  {
    delete []str;
    str = new char[strlen(ks.str) + 1];
    strcpy(str, ks.str);
  }
  return *this;
}
//---------------------------------------------------------------------------
void CString::SaveToFile(FILE *f) const
{
  int size = strlen(str);

  if(fwrite(&size, sizeof(size), 1, f) != 1)
    throw(CError(CError::eeFile));
  if(fwrite(str, size+1, 1, f) != 1)
    throw(CError(CError::eeFile));
}
//---------------------------------------------------------------------------
AnsiString CString::AnsiString() const
{
  return AnsiString::AnsiString(str);
}
//-------------------------------------------------------------
int CString::GetInt(void)
{
  int ret = 0;

  for(int i = 0; str[i]; i ++)
    if((str[i] >= 0x30) && (str[i] <= 0x39))
      ret = ret*10 + (int)(str[i] - 0x30);
    else
      return 0;  
  return ret;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
     
