//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("PackageAlt.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("DBGridAlt.cpp");
USEPACKAGE("Vcldb50.bpi");
USEUNIT("EditAlt.cpp");
USEUNIT("MonthPicker.cpp");
USEPACKAGE("DAC50.bpi");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
    return 1;
}
//---------------------------------------------------------------------------
