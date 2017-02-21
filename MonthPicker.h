/* Компонент TMonthPicker
 * Автор: В.С.Овчинников
 * 2016 г.
 * e-mail.: x74@list.ru
 *
 * Описание:
 * Данный компонент позволяет пользователю выбрать год и месяц, без возможности
 * выбора числа месяца.
 * Свойства:
 * TDateTime Date - Воз./уст. выбранную дату (без учета числа месяца)
 * TDateTime MinDate - Ограничить выбор минимальной датой
 * TDateTime MaxDate - Ограничить выбор максимальной датой
 * Word Month - Воз./уст. выбранный месяц
 * Word Year - Воз./уст. выбранный год
 * Word GetLastDay - Возвращает последний день в выбранном месяце

 Примечание:
 1. Неполность реализована работа с Day

 */

#ifndef MONTNH_PICKER_H
#define MONTNH_PICKER_H

#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>

class PACKAGE TMonthPicker : public TPanel
{
private:
    TPanel* panel;      // Панель, на которой размещаются остальные компоненты
    TEdit* edit;        // Поле Год
    TSpeedButton* button1;  // Кнопка декремента года
    TSpeedButton* button2;  // Кнопка инкремента года
    TShape* shape;          // Полоска между элеметами выбора года и месяцами
    TSpeedButton* btnMonthsList[12];    // Кнопки месяцев
    TLabel* labelColorLines[12];        // Цветные лейблы месяцев
    void __fastcall MonthButtonClick(TObject *Sender);
    void __fastcall SpeedButtonClick(TObject *Sender);
    void __fastcall EditKeyPress(TObject *Sender, char &Key);
    void __fastcall EditChange(TObject *Sender);
    TNotifyEvent FOnChange;

    bool __fastcall IsNumber(const String& str) const;
    bool __fastcall CheckBounds(const TDateTime& dt) const;
    inline bool __fastcall CheckBoundMin(const TDateTime& dt) const;
    inline bool __fastcall CheckBoundMax(const TDateTime& dt) const;

    void __fastcall SetFEnabled(bool Value);
    void __fastcall refresh();
    void __fastcall FixDate();
    void __fastcall FixMonth();
    void __fastcall sendOnChange();


    bool FEnabled;
    Word FDay;
    Word FMonth; 
    Word FYear;
    TDateTime FMinDate;
    TDateTime FMaxDate;

    Word _updateLockedCounter;
    void lockUpdate();
    void unlockUpdate();
    bool isUpdateLocked();

__published:

    __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
    __property bool Enabled = {read = FEnabled, write = SetFEnabled, default=true};
    __property TDateTime Date = {read = GetDate, write = SetDate};
    __property Word Day = {read = GetDay, write = SetDay};
    __property Word Month = {read = GetMonth, write = SetMonth};
    __property Word Year = {read = GetYear, write = SetYear};
    //__property Word LastDay = {read = GetLastDay};
    __property TDateTime MinDate = {read=FMinDate, write=SetMinDate};
    __property TDateTime MaxDate = {read=FMaxDate, write=SetMaxDate};

public:
    __fastcall TMonthPicker(TComponent* Owner);
    __fastcall virtual ~TMonthPicker(void);
    void __fastcall Create(TWinControl *Owner);
    void __fastcall SetYear(Word year);
    void __fastcall SetYear(const TDateTime& dt);
    void __fastcall SetMonth(Word month);
    void __fastcall SetMonth(const TDateTime& dt);
    void __fastcall SetDay(Word day);

    void __fastcall SetDate(TDateTime dt);
    TDateTime __fastcall GetDate() const;
    String __fastcall GetDate(const String& format) const;
    Word __fastcall GetYear();
    Word __fastcall GetMonth();
    Word __fastcall GetDay();
    Word __fastcall GetLastDay();
    Word __fastcall GetCurrentDay();
    void __fastcall SetMinDate(const TDateTime& dt);
    void __fastcall SetMaxDate(const TDateTime& dt);

};

//---------------------------------------------------------------------------
#endif  // MONTNH_PICKER_H
 