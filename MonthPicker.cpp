/* MonthPicker.cpp file
 */

#include <vcl.h>
#pragma hdrstop

#include "MonthPicker.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
static inline void ValidCtrCheck(TMonthPicker *)
{
    new TMonthPicker(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMonthPicker::TMonthPicker(TComponent* Owner)
    : TPanel(Owner)
{
    ShortDateFormat = "dd.mm.yyyy";
    DateSeparator = '.';
    this->Caption = " ";
    Create(this);
    SetDate(Now());
    FEnabled = true;
}

//---------------------------------------------------------------------------
namespace Monthpicker
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TMonthPicker)};
         RegisterComponents("AltCtrl", classes, 0);
    }
}

//---------------------------------------------------------------------------
//
__fastcall TMonthPicker::~TMonthPicker(void)
{
}

//---------------------------------------------------------------------------
// Создание элементов управления
void __fastcall TMonthPicker::Create(TWinControl *Owner)
{
    int left = 0;
    int top = 0;
    static const int colors[5] = {RGB(53,194,255), RGB(171,220,24), RGB(255,210,64), RGB(200,170,120), RGB(53,194,255)};
    int color;

    this->Width = 156;
    this->Height = 115;

    edit = new TEdit(Owner);
    edit->Parent = this;
    edit->Width = 50;
    edit->Height = 22;
    edit->Top = 1;
    edit->Left = this->Width / 2 - edit->Width / 2;
    edit->MaxLength = 4;

    edit->OnKeyPress = EditKeyPress;
    edit->OnChange = EditChange;

    button1 = new TSpeedButton(Owner);
    button1->Parent = this;
    button1->Width = 23;
    button1->Top = 1;
    button1->Left = edit->Left - button1->Width - 1;
    button1->Tag = -1;
    button1->OnClick = SpeedButtonClick;
    button1->Caption = "<";
    button1->Flat = true;
    button1->BringToFront();

    button2 = new TSpeedButton(Owner);
    button2->Parent = this;
    button2->Width = 23;
    button2->Top = 1;
    button2->Left = edit->Left + edit->Width + 1;
    button2->Tag = 1;
    button2->OnClick = SpeedButtonClick;
    button2->Caption = ">";
    button2->Flat = true;

    shape = new TShape(Owner);
    shape->Parent = this;
    shape->Width = this->Width - 2;
    shape->Height = this->Height - edit->Height - 4;
    shape->Top = edit->Height + 3;
    shape->Left = 1;
    shape->Brush->Style = bsClear;
    shape->Pen->Style = psSolid;
    shape->Pen->Color = clSilver;
    shape->Pen->Width = 1;

    for(int i=0; i<=11; i++)
    {
        if (i%4 == 0)
        {
            top += 28;
            left = 4;
        }

        TSpeedButton* button = new TSpeedButton(Owner);
        button->Caption = AnsiUpperCase(ShortMonthNames[i][1])+ShortMonthNames[i].SubString(2, ShortMonthNames[i].Length()-1);
        button->Parent = this;
        button->Name = "Label_text" + String(i);
        button->Width = 28;
        button->Left = left;
        button->Top = top;
        button->Font->Size = 8;
        button->GroupIndex = 1;
        button->Flat = true;
        button->OnClick = MonthButtonClick;
        button->Tag = i;
        btnMonthsList[i] = button;

        color = colors[(i+1)/3];

        TLabel* l_c = new TLabel(Owner);
        l_c->Name = "Label_color" + String(i);
        l_c->Parent = this;
        l_c->Width = 20;
        l_c->Height = 2;
        l_c->Left = button->Left + button->Width / 2 - l_c->Width / 2;
        l_c->Top = button->Top+button->Height+1;
        l_c->Color = static_cast<TColor>(color);
        labelColorLines[i] = l_c;

        left += 40;
    }           
}

//---------------------------------------------------------------------------
// Проверка нижней границы даты
void __fastcall TMonthPicker::SetMinDate(const TDateTime& dt)
{
    unsigned short year, month, day;
    dt.DecodeDate(&year, &month, &day);
    dt = EncodeDate(year, month, 1);

    if (!CheckBoundMax(dt))
    {
        throw Exception("Min date must be less than the max date");
    }

    FMinDate = dt;
    FixDate();
    UpdateControl();
}

//---------------------------------------------------------------------------
// Проверка верхней границы даты
void __fastcall TMonthPicker::SetMaxDate(const TDateTime& dt)
{
    unsigned short year, month, day;
    dt.DecodeDate(&year, &month, &day);
    dt = IncMonth(EncodeDate(year, month, 1), 1)-1;

    if (!CheckBoundMin(dt))
    {
        throw Exception("Max date must be greater than the min date");
    }

    FMaxDate = dt;
    FixDate();
    UpdateControl();
}

//---------------------------------------------------------------------------
// Проверка выхода даты за верхнюю и нижнюю границы
bool __fastcall TMonthPicker::CheckBounds(const TDateTime& dt) const
{
    return CheckBoundMin(dt) && CheckBoundMax(dt);
}

//---------------------------------------------------------------------------
// Проверка выхода за нижнюю границу
inline bool __fastcall TMonthPicker::CheckBoundMin(const TDateTime& dt) const
{
    return (int(FMinDate) == 0 || dt >= FMinDate);
}

//---------------------------------------------------------------------------
// Проверка выхода за верхнюю границу
inline bool __fastcall TMonthPicker::CheckBoundMax(const TDateTime& dt) const
{
    return (int(FMaxDate) == 0 || dt <= FMaxDate);
}

//---------------------------------------------------------------------------
//  Коррекция даты
void __fastcall TMonthPicker::FixDate()
{
    TDateTime seldate = GetDate();
    // Коррекция выбранного месяца
    if (!CheckBoundMax(seldate))
    {
        SetDate(FMaxDate);
    }
    else if (!CheckBoundMin(seldate))
    {
        SetDate(FMinDate);
    }
}

//---------------------------------------------------------------------------
//  Коррекция даты
void __fastcall TMonthPicker::FixMonth()
{
    TDateTime seldate = GetDate();
    // Коррекция выбранного месяца
    if (!CheckBoundMax(seldate))
    {
        SetMonth(FMaxDate);
    }
    else if (!CheckBoundMin(seldate))
    {
        SetMonth(FMinDate);
    }
}

//---------------------------------------------------------------------------
// Обновление элементов управления
void __fastcall TMonthPicker::UpdateControl()
{
    TDateTime curyear = EncodeDate(FYear, 1, 1);
    //TDateTime seldate = GetDate();

    // Возможность смены года
    button1->Enabled = int(FMinDate) == 0 || FMinDate < curyear;
    button2->Enabled = int(FMaxDate) == 0 || FMaxDate > IncMonth(curyear,12);
    edit->Enabled = button1->Enabled || button2->Enabled;

    // Доступные месяцы
    TDateTime tmpdt;
    for (unsigned short i=0; i<12; i++)
    {
        tmpdt = IncMonth(curyear,i);
        btnMonthsList[i]->Enabled = CheckBounds(tmpdt);
    }
}


//---------------------------------------------------------------------------
// Установка свойства Enabled
void __fastcall TMonthPicker::SetFEnabled(bool Value)
{
    FEnabled = Value;
    button1->Enabled = Value;
    button2->Enabled = Value;
    for (unsigned short i=0; i<12; i++) {
        btnMonthsList[i]->Enabled = Value;
    }
    btnMonthsList[this->FMonth]->Enabled = true;
}

//---------------------------------------------------------------------------
// Установка года
void __fastcall TMonthPicker::SetYear(unsigned short year)
{
    edit->OnChange = NULL;

    if (year > 0 && CheckBoundMin(EncodeDate(year, 12, 31)) && CheckBoundMax(EncodeDate(year, 1, 1)))
    {
        this->FYear = year;
        edit->Text = year;
    }
    else
    {
        edit->Text = this->FYear;
    }

    // Коррекция выбранного месяца
    FixMonth();

    edit->OnChange = EditChange;
    if (FOnChange != NULL)
    {
        FOnChange(this);
    }

    UpdateControl();
}

//---------------------------------------------------------------------------
// Установить месяц short
void __fastcall TMonthPicker::SetMonth(unsigned short month)
{
    //btnMonthsList[this->pMonth]->Font->Style = btnMonthsList[this->pMonth]->Font->Style >> fsBold;
    this->FMonth = month-1;
    //btnMonthsList[this->pMonth]->Font->Style = btnMonthsList[this->pMonth]->Font->Style << fsBold;
    btnMonthsList[this->FMonth]->Down = true;
    if (FOnChange != NULL)
    {
        FOnChange(this);
    }
}

//---------------------------------------------------------------------------
// Установить месяц из TDateTime
void __fastcall TMonthPicker::SetMonth(const TDateTime& dt)
{
    unsigned short year, month, day;
    dt.DecodeDate(&year, &month, &day);
    SetMonth(month - 1);
}

//---------------------------------------------------------------------------
// Установить год из TDateTime
void __fastcall TMonthPicker::SetYear(const TDateTime& dt)
{
    unsigned short year, month, day;
    dt.DecodeDate(&year, &month, &day);
    SetYear(year);
}

//---------------------------------------------------------------------------
// Установить дату из TDateTime
void __fastcall TMonthPicker::SetDate(const TDateTime& dt)
{
    unsigned short year, month, day;
    dt.DecodeDate(&year, &month, &day);
    SetMonth(month);
    SetYear(year);
}

//---------------------------------------------------------------------------
// Вернуть дату в TDateTime
TDateTime __fastcall TMonthPicker::GetDate() const
{
    return EncodeDate(this->FYear, this->FMonth + 1, 1);
}

//---------------------------------------------------------------------------
// Вернуть дату в String
String __fastcall TMonthPicker::GetDate(const String& format) const
{
    return FormatDateTime(format, GetDate());
}

//---------------------------------------------------------------------------
// Вернуть параметр Year
unsigned short  __fastcall TMonthPicker::GetYear()
{
    return this->FYear;
}

//---------------------------------------------------------------------------
// Вернуть параметр Month
unsigned short __fastcall TMonthPicker::GetMonth()
{
    return this->FMonth + 1;
}

//---------------------------------------------------------------------------
// Вернуть параметр LastDay (последний день месяца)
unsigned short __fastcall TMonthPicker::GetLastDay()
{
    return MonthDays[IsLeapYear(this->FYear)][this->FMonth];
}

//---------------------------------------------------------------------------
// Обработчик кнопок выбора месяца
void __fastcall TMonthPicker::MonthButtonClick(TObject *Sender)
{
    TSpeedButton* b = ((TSpeedButton*) Sender);
    SetMonth(b->Tag + 1);
}

//---------------------------------------------------------------------------
// Обработчик кнопок < и > для инкримента/декремента поля ГОД
void __fastcall TMonthPicker::SpeedButtonClick(TObject *Sender)
{
    SetYear(FYear + ((TSpeedButton*)Sender)->Tag);
}

//---------------------------------------------------------------------------
// Обработчик KeyPress в поле ГОД
void __fastcall TMonthPicker::EditKeyPress(TObject *Sender, char &Key)
{
    if ((Key >= '0') && (Key <= '9') || Key == VK_BACK)
    {
        // цифры и 'Key == '-'' - на всякий случай для отрицательных чисел,
        //по надобности можно убрать.
    }
    else if (Key == 8)
    {
        // <-
    }
    else
    {
        Key = 0;
    }

    edit->SelLength = 1;
}

//---------------------------------------------------------------------------
// Обработчик Ctrl-V в поле ГОД
void __fastcall TMonthPicker::EditChange(TObject *Sender)
{
    if (!IsNumber(edit->Text))
    {
        edit->Text = FYear;
    }
    else
    {
        SetYear(StrToInt(edit->Text));
    }
    edit->SelLength = 1;
}

//------------------------------------------------------------------------------
// Проверяет является ли строка целым числом
bool __fastcall TMonthPicker::IsNumber(const String& str) const
{
    int l = str.Length();
    if ( l <= 0)
    {
        return false;
    }

    for (int i = 1; i <= l; i++)
    {
        char a = str[i];
        if (a == '1' || a== '2' || a == '3' || a == '4' || a== '5' ||
            a== '6' || a == '7' || a == '8' || a== '9' || a== '0')
        {
            continue;
        }
        return false;
    }
    return true;
}

