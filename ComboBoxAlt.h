//---------------------------------------------------------------------------

#ifndef ComboBoxAltH
#define ComboBoxAltH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <DB.hpp>
#include "DBAccess.hpp"
#include <vector.h>


/* Источник */
class TSourceItem
{
public:  
    TSourceItem(TDataSet* ds_, String DisplayField_, String ValueField_, bool bShowValue_)
        : ds(ds_), DisplayField(DisplayField_), ValueField(ValueField_), bShowValue(bShowValue_)
    {}

public:
    String DisplayField;
    String ValueField;
    TDataSet* ds;
    bool bShowValue;

};

/* Значение в списке*/
class TListValue
{
public:  
    TListValue(TDataSet* ds_, String value_, String text_)
        : ds(ds_), value(value_), text(text_)
    {}

public:
    String value;
    TDataSet* ds;
    String text;
};

// Список источников
typedef std::vector<TSourceItem> TSourceList;


//---------------------------------------------------------------------------
class PACKAGE TComboBoxAlt : public TComboBox
{
private:
    // 2017-10-05 void __fastcall AddItems(TDataSet* ds, String DisplayField, String ValueField); // Заполняет список не присоединяя источник
    void __fastcall AddItems(TSourceItem* SourceItem); // Заполняет список не присоединяя источник
    void __fastcall ClearItems(TDataSet* ds);       // Очищает список не отсоединяя источник
    TSourceList _sourceList;       // Список источников

protected:
    virtual void __fastcall DestroyWnd(void);
    TListValue* __fastcall FGetCurrentItem();                 // Возвращает значение
    String __fastcall FGetValue();                 // Возвращает значение
    String __fastcall FGetText();                 // Возвращает значение
    //String FSourceText;

public:
    __fastcall TComboBoxAlt(TComponent* Owner);
    __fastcall ~TComboBoxAlt();
    void __fastcall AddSource(TDataSet* ds, String DisplayField, String ValueField, bool bShowValue);   // Добавляет источник
    void __fastcall DeleteSource(TDataSet* ds); // Удаляет источник
    void __fastcall RefreshItems();             // Обновляет список в соответствии с источниками
	__property TListValue* CurrentItem = {read=FGetCurrentItem};
	__property TListValue* Value = {read=FGetValue};
	//__property TListValue* SourceText = {read=FGetText};
	//__property bool ShowValue = {read=FShowValue, write=FShowValue, default = true};

__published:
};
//---------------------------------------------------------------------------
#endif
