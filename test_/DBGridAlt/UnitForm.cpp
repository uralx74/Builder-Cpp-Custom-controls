//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "Ora"
#pragma link "DBGridAlt"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    TestSession->Connect();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    TestQuery->Open();
}


void __fastcall TForm1::GetSumClick(TObject *Sender)
{
    TSumResult checkedResult = DBGridAlt1->getSum("", "CHECK_DATA = 1", false);
}
//---------------------------------------------------------------------------

