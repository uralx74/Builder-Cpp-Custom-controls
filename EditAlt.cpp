//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditAlt.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TEditAlt *)
{
    new TEditAlt(NULL);
}
//---------------------------------------------------------------------------
__fastcall TEditAlt::TEditAlt(TComponent* Owner)
    : TEdit(Owner)
{
    this->SetParent((TWinControl*) Owner);
    this->Text = "0";
    TextOld = Text;
}
//---------------------------------------------------------------------------
namespace Editalt
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TEditAlt)};
         RegisterComponents("Alt", classes, 0);
    }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
void __fastcall TEditAlt::SetDataType(TEditDataType EditDataType)
{
    switch (EditDataType) {
    case edtInteger:
        FUseDot = false;
        FUseSign = true;
        break;
    case edtUnsignedInteger:
        FUseDot = false;
        FUseSign = false;
        break;
    case edtFloat:
        FUseDot = true;
        FUseSign = true;
        break;
    case edtUnsignedFloat:
        FUseDot = true;
        FUseSign = false;
        break;
    }
    FEditDataType = EditDataType;
}

//---------------------------------------------------------------------------
//
void __fastcall TEditAlt::Change()
{
   /*if (IsNumber(Text, FUseDot, FUseSign)) {
        TextOld = Text;
        try {
            SelStartOld = SelStart;
        } catch (...) {}
    } else {
        TNotifyEvent event = this->OnChange;
        this->OnChange = NULL;
        try {
            this->Text = TextOld;
            this->SelStart = SelStartOld;
        }
        __finally {
            this->OnChange = event;
        }
        Beep();
    } */


   if (IsNumber(Text, FUseDot, FUseSign)) {
        TextOld = Text;
        SelStartOld = SelStart;
    } else {
        TNotifyEvent event = this->OnChange;
        this->OnChange = NULL;
        try {
            Text = TextOld;
            SelStart = SelStartOld;
        }
        __finally {
            this->OnChange = event;
        }
        Beep();
    }

}

//---------------------------------------------------------------------------
//
void __fastcall TEditAlt::KeyPress(char &Key)
{
    SelStartOld = this->SelStart;
}

//---------------------------------------------------------------------------
//
bool __fastcall TEditAlt::IsNumber(String Value, bool bFloat, bool bSign)
{
    int n = Value.Length();
    bool bSignExist = false;
    int iStart = 1;

    if (bSign && n > 0) {   // Знак + или - могут быть только вначале
        bSignExist = Value[1] == '+' || Value[1] == '-';
    }

    if (bSignExist) {   // Если был знак + или -, значит начинаем со второго символа
        iStart = 2;
    }

    bool nDotExist = false;
    for (int i = iStart; i <= n; i++)   // Цикл по символам
    {
        if (!isdigit(Value[i]) ) {
            if (bFloat && Value[i] == DecimalSeparator) {
                if (nDotExist)       // Если ранее уже была .
                    return false;
                else
                    nDotExist = true;
            } else {
                return false;
            }
        }
    }
    return true;
}

int __fastcall TEditAlt::GetValue()
{
    return StrToInt(Text);
}

__fastcall TEditAlt::SetValue(int value)
{
    Text = IntToStr(value);
}
