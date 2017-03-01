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
    : TEdit(Owner)
{
    this->WindowProc = MyWndProc;
    //this->SetParent((TWinControl*) Owner);

    // Notice:
    // in time when control is in csDesigning state then
    // not allowed use this->SelStart parameter
    // _allowChange = !ComponentState.Contains(csDesigning);

    _allowChange = false;
    this->Text = "0";
    _allowChange = true;

    _textOld = Text;

    //Perform(CM_TEXTCHANGED, 0, 0);
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


/* Перехватываем оконное сообщение */
void __fastcall TEditAlt::MyWndProc(Messages::TMessage &Message)
{
    switch (Message.Msg )
    {
    //case WM_SETTEXT:  // 12 = 0x000C
    //{
    //    break;
    //}
    /*case WM_CHAR:     // 258 = 0x0102
    {
        break;
    }
    case WM_PASTE:      // 770 = 0x0302
    {
        break;
    }*/
    case WM_CHAR:     // 258 = 0x0102
    {
        _selStartOld = this->SelStart;
        WndProc(Message);
    }

    case CM_TEXTCHANGED:
    {
        if ( ComponentState.Contains(csDesigning) )
        {                                            // Необходимо для обработки неправильных значений в режиме дизайна
            String textTmp = this->Text.Trim();
            if ( checkValue(textTmp) )
            {
                this->Text = textTmp;
            }
            else
            {
                this->Text = _textOld;
            }
        }
    }
    case CM_CHANGED:    // 45111 = 0xB037
    {
        if ( !_allowChange )
        {
            return;
        }

        String textTmp = this->Text.Trim();
        if ( checkValue(textTmp) )
        {
            _textOld = textTmp;
            _selStartOld = this->SelStart;

            _allowChange = false;
            this->Text = textTmp;
            _allowChange = true;

            this->SelStart = _selStartOld;
            this->SelLength = 0;

        }
        else
        {
            _allowChange = false;
            this->Text = _textOld;
            _allowChange = true;

            this->SelStart = _selStartOld;
            this->SelLength = 0;
            Beep(); 
        }

        break;
    }
    default:
    {
        WndProc(Message);
    }
    }
}

/*
*/
bool __fastcall TEditAlt::IsNumber(const String& Value, bool bFloat, bool bSign) const
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

bool TEditAlt::checkValue(const String& value) const
{
    return IsNumber(value, FUseDot, FUseSign);
}

