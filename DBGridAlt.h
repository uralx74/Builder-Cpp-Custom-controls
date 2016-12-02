//---------------------------------------------------------------------------

#ifndef DBGridAltH
#define DBGridAltH
//---------------------------------------------------------------------------
#include "VirtualTable.hpp"
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <vector>
#include <map>
#include "Ora.hpp"
#include "math.h"

class TDBGridAltFilterItem
{
public:
    __fastcall TDBGridAltFilterItem();
    ~TDBGridAltFilterItem();
    void setActive(bool active);
    bool isActive() const { return _active; };
    bool isEmpty() const { return _empty; };
    String getText() const;
    void setText(const String& text);
    void setParamValue(const String& paramValue);


private:
    bool _active;   // Если фильтр включен пользователем
    bool _empty;    // Если значение фильтра пустое (=="")
    String _text;
    String _paramValue;
    String _textResult; // текст после установки значения
    TReplaceFlags _replaceFlags;
    //String leftText;
    //String middleText;  // value
    //String rightText;

private:
};

class TDBGridAltFilter
{
private:
    std::map<String, TDBGridAltFilterItem>* _items;

public:
    __fastcall TDBGridAltFilter();
    __fastcall ~TDBGridAltFilter();
    void add(const String& filterName, const String& filterStr);
    void remove(const String& filterName);
    void clear();
    String getFilterString(const String &glue = "") const;
    void setActive(const String& filterName, bool active);
    void setValue(const String& filterName, const String& value);



};


//---------------------------------------------------------------------------
class PACKAGE TDBGridAlt : public TDBGrid
{
private:
    DYNAMIC void __fastcall DblClick();
    DYNAMIC void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
    DYNAMIC void __fastcall TitleClick(TColumn *Column);
    DYNAMIC void __fastcall DrawColumnCell(const TRect &Rect, int DataCol, TColumn* Column, TGridDrawState State);
    DYNAMIC void __fastcall MouseWheelHandler(TMessage &Msg);
    virtual void __fastcall LinkActive(bool Value);      //http://bendelasoft.com/_delphi/ehlib4/DBLookupGridsEh.hpp
    //virtual void __fastcall DataChanged(void);

    //virtual void __fastcall Loaded();     // при создании компонента
    void __fastcall setVisibleKeyField(bool VisibleFlg);
    void __fastcall setColumnAutosize(bool ColumnAutosizeFlg);
    //void __fastcall SetKeyFieldIndex(int Index);
    //void __fastcall SetKeyFieldName(AnsiString Name);
    void __fastcall setFiltered(bool Value);
    //void __fastcall ClearFilter(int Index);
    TVirtualTable* __fastcall getChecked();
    //AnsiString _selStartBookmark;
    //void* _selStartBookmark;

    String FFileName;
    TColor FOddRowColor;
    TColor FEvenRowColor;
    TColor FSortColumnColor;
    TFont* FCheckedFont;

    std::vector<bool> ItemsFlg;



    int SortColumnIndex;    // Индекс колонки, по которой ведется сортировка
    int SortType;           // 0 - без, 1 - ASC, 2 - DESC
    bool FHideKeyField;     //
    bool FColumnAutosize;   //
    bool FFiltered;
    //int FKeyFieldIndex;     //
    AnsiString FKeyFieldName; // Имя уникального поля - значение должно быть integer
    TNotifyEvent FOnChangeCheck;
    TNotifyEvent FOnChangeFilter;
    TDataChangeEvent FOnDataChange;

    typedef void __fastcall (__closure *TOnDataEvent)(TObject* Sender,AnsiString &Buffer);
    //TOnDataEvent fOnData;

    TDBGridAltFilter* _filter;

    TPanel* panel;      // Панель, на которой размещаются остальные компоненты
    TEdit* edit;        // Поле Год

    


protected:
    BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(WM_MOUSEWHEEL, TMessage, MouseWheelHandler)
    END_MESSAGE_MAP(TDBGrid)


public:
    __fastcall TDBGridAlt(TComponent* Owner);
    virtual __fastcall TDBGridAlt::~TDBGridAlt();
    bool __fastcall isChecked(int RowNum);
    void __fastcall checkAll();
    void __fastcall uncheckAll();
    void __fastcall checkFiltered();
    void __fastcall refreshFilter();
    void __fastcall setAutosize();
    int __fastcall getCheckedCount();
    void __fastcall invertCheck(int Index);
    void __fastcall assignFilter(TDBGridAltFilter* filter);
    void __fastcall clearFilter();
    double __fastcall getSum(const String& fieldName, bool checked = false, bool filtered = false);
    //int __fastcall test_01();




__published:
   __property String FileName  = { read=FFileName, write=FFileName };
   __property TColor OddRowColor  = { read=FOddRowColor, write=FOddRowColor};
   __property TColor EvenRowColor  = { read=FEvenRowColor, write=FEvenRowColor};
   __property TColor SortColumnColor  = { read=FSortColumnColor, write=FSortColumnColor};
   __property bool HideKeyField  = { read=FHideKeyField, write=setVisibleKeyField, default = true};
   __property bool ColumnAutosize  = { read=FColumnAutosize, write=setColumnAutosize, default = true};
   //__property int KeyFieldIndex  = { read=FKeyFieldIndex, write=SetKeyFieldIndex, default = 1};
   __property AnsiString KeyFieldName  = { read=FKeyFieldName, write=FKeyFieldName};
   __property TFont* CheckedFont = { read=FCheckedFont, write=FCheckedFont};
   __property int CheckedCount = { read=getCheckedCount};
   __property TNotifyEvent OnChangeCheck = {read=FOnChangeCheck, write=FOnChangeCheck};
   __property TNotifyEvent OnChangeFilter = {read=FOnChangeFilter, write=FOnChangeFilter};
   //__property TOnDataEvent OnData = { read=fOnData, write=fOnData };
   //__property TDataChangeEvent OnDataChange = {read=FOnDataChange, write=FOnDataChange};

   __property bool Filtered = {read = FFiltered, write = setFiltered, default = false};

};
//---------------------------------------------------------------------------
#endif

