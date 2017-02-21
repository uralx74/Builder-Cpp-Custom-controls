//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditAlt.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
static inline void ValidCtrCheck(TEditAlt *)
{
    new TEditAlt(NULL);
}

//---------------------------------------------------------------------------
__fastcall TEditAlt::TEditAlt(TComponent* Owner)
    : TEdit(Owner),
    _changeFromCode(false)
{
    this->SetParent((TWinControl*) Owner);
    //this->Owner = this;
    this->Text = "";
    _textOld = Text;
}

//---------------------------------------------------------------------------
namespace Editalt
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TEditAlt)};
         RegisterComponents("AltCtrl", classes, 0);
    }
}
//---------------------------------------------------------------------------


/**/
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

/**/
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

    if (_changeFromCode)
    {
        _changeFromCode = false;
        return;
    }

   if (IsNumber(Text, FUseDot, FUseSign))
   {
        _textOld = Text;
        _selStartOld = SelStart;
        TEdit::OnChange(this);
    }
    else
    {
        _changeFromCode = true;
        Text = _textOld;
        SelStart = _selStartOld;

        /*TNotifyEvent event = this->OnChange;
        this->OnChange = NULL;

        try
        {
            Text = TextOld;
            SelStart = SelStartOld;
        }
        __finally
        {
            this->OnChange = event;
        }    */
        Beep();
    }


}

//---------------------------------------------------------------------------
//
void __fastcall TEditAlt::KeyPress(char &Key)
{
    _selStartOld = this->SelStart;
}

//---------------------------------------------------------------------------
//
bool __fastcall TEditAlt::IsNumber(String Value, bool bFloat, bool bSign)
{
    int n = Value.Length();
    bool bSignExist = false;
    int iStart = 1;

    if (bSign && n > 0)    // Знак + или - могут быть только вначале
    {
        bSignExist = Value[1] == '+' || Value[1] == '-';
    }

    if (bSignExist)    // Если был знак + или -, значит начинаем со второго символа
    {
        iStart = 2;
    }

    bool nDotExist = false;
    for (int i = iStart; i <= n; i++)   // Цикл по символам
    {
        if (!isdigit(Value[i]) )
        {
            if (bFloat && Value[i] == DecimalSeparator)
            {
                if (nDotExist)       // Если ранее уже была .
                {
                    return false;
                }
                else
                {
                    nDotExist = true;
                }
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

/* Возвращает признак задано ли не нулевое значение */
bool __fastcall TEditAlt::IsNull()
{
    return Text == "";
}

/* Возвращает значение */
int __fastcall TEditAlt::GetValue()
{
    if (FAllowNull && Text == "")
    {
        return 0;
    }
    else
    {
        return StrToInt(Text);
    }
}

/* Очищает значение */
void __fastcall TEditAlt::ClearValue()
{
    if (FAllowNull)
    {
        Text = "";
    }
}

/* Задает значение */
void __fastcall TEditAlt::SetValue(int value)
{
    Text = IntToStr(value);
}
