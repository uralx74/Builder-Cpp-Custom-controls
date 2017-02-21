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


//TDataLink
//virtual void __fastcall SetActiveRecord(int Value);



namespace DBGridAltCheckTypes
{

enum CheckType
{
    CT_UNDEFINED = 0,
    CT_CHECKED,
    CT_UNCHECKED
};
};


//class TGridDataLink* FDataLink;
/*class TGridDataLinkAlt : public TGridDataLink // Db::TDataLink
{
};  */

typedef std::pair<unsigned int, double> TSumResult;     // Тип результата функции Sum

//---------------------------------------------------------------------------
class PACKAGE TDBGridAlt : public TDBGrid
{
private:
    //TFieldDataLink* FDataLink;
    //TDataChangeEvent FOnDataChange;
    //typedef void __fastcall (__closure *TOnDataEvent)(TObject* Sender,AnsiString &Buffer);
    //DYNAMIC void __fastcall ColExit();
    //DYNAMIC void __fastcall OnEnter();
    //DYNAMIC void __fastcall RowMoved(int FromIndex, int ToIndex);

    DYNAMIC void __fastcall DblClick();
    DYNAMIC void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
    DYNAMIC void __fastcall TitleClick(TColumn *Column);
    DYNAMIC void __fastcall DrawColumnCell(const TRect &Rect, int DataCol, TColumn* Column, TGridDrawState State);
    DYNAMIC void __fastcall MouseWheelHandler(TMessage &Msg);
    virtual void __fastcall LinkActive(bool Value);      //http://bendelasoft.com/_delphi/ehlib4/DBLookupGridsEh.hpp


    String MergeStr(const String& s1, const String& s2, const String &glue = "");
    //TVirtualTable* __fastcall getChecked();
    int __fastcall getRecordCountChecked();
    int __fastcall getRecordCountFiltered();

    /* Display */
    void __fastcall ScrollActiveToRow(int ARow);

    TOraQuery* _dataSet;
    TGridDataLink* FDataLink;

    /* Rows Drawing */
    TColor FOddRowColor;
    TColor FEvenRowColor;
    TColor FSortColumnColor;
    TFont* FCheckedFont;
    TFont* FEditableFont;

    /* Filtering */
    //void __fastcall setFiltered(bool Value);
    //bool FFiltered;
    //TDBGridAltFilter* _filter;

    /* Sorting */
    void __fastcall SetDefaultSortFieldName(String value);
    String FDefaultSortFieldName;
    int _sortColumnIndex;    // Индекс колонки, по которой ведется сортировка
    int SortType;           // 0 - без, 1 - ASC, 2 - DESC

    /* Checking */
    void __fastcall setAllowChecked(bool allowChecked);
    void __fastcall SetCheckDataFieldName(const String value);
    bool FAllowChecked;
    String _checkDataFieldName;  // Имя поля для хранения данных о пометке (ftInteger)

    /* Statistics */
    int _recordCount;

    /* Events */
    TNotifyEvent FOnChangeCheck;
    TNotifyEvent FOnChangeFilter;

    /* Other benefits */
    void __fastcall setColumnAutosize(bool ColumnAutosizeFlg);
    bool FColumnAutosize;   //
    bool FAllowManualAppend;  // Предотвратить ручное добавление записей


protected:
    BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(WM_MOUSEWHEEL, TMessage, MouseWheelHandler)
    END_MESSAGE_MAP(TDBGrid)
    void __fastcall setCheck(int index, bool value);    // Устанавливает выделение по индексу
    //virtual void __fastcall SetFilterText(const AnsiString Value);
    //virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	//virtual void __fastcall Scroll(int Distance);

public:
    __fastcall TDBGridAlt(TComponent* Owner);
    virtual __fastcall TDBGridAlt::~TDBGridAlt();
    bool __fastcall isChecked();
    void __fastcall setCheck(bool value = true);
    void __fastcall setCheckAll(bool value);
    void __fastcall setCheckFiltered(bool value);
    void __fastcall setAutosize();
    void __fastcall invertCheck();
    void __fastcall refreshFilter();
    void __fastcall removeFilter();
    //double __fastcall getSum(const String& fieldName, bool checked = false, bool filtered = false);
    double __fastcall getCount(const String& fieldName, bool checked = false, bool filtered = false);
    TSumResult __fastcall getSum(const String& fieldName, const String& expression, bool filtered = false);


__published:
   __property String DefaultSortFieldName  = { read=FDefaultSortFieldName, write=SetDefaultSortFieldName};
   __property String CheckDataFieldName  = { read=_checkDataFieldName, write=SetCheckDataFieldName};
   __property TColor OddRowColor  = { read=FOddRowColor, write=FOddRowColor};
   __property TColor EvenRowColor  = { read=FEvenRowColor, write=FEvenRowColor};
   __property TColor SortColumnColor  = { read=FSortColumnColor, write=FSortColumnColor};
   __property bool AllowChecked  = { read=FAllowChecked, write=setAllowChecked, default = true};
   //__property bool HideKeyField  = { read=FHideKeyField, write=setVisibleKeyField, default = true};
   //__property AnsiString KeyFieldName  = { read=FKeyFieldName, write=FKeyFieldName};
   __property bool ColumnAutosize  = { read=FColumnAutosize, write=setColumnAutosize, default = true};
   __property bool AllowManualAppend  = { read=FAllowManualAppend, write=FAllowManualAppend};
   __property TFont* CheckedFont = { read=FCheckedFont, write=FCheckedFont};
   __property TFont* EditableFont = { read=FEditableFont, write=FEditableFont};
   __property int recordCountChecked = { read=getRecordCountChecked};
   __property int recordCountFiltered = { read=getRecordCountFiltered};
   __property int recordCount = { read=_recordCount};
   __property TNotifyEvent OnChangeCheck = {read=FOnChangeCheck, write=FOnChangeCheck};
   __property TNotifyEvent OnChangeFilter = {read=FOnChangeFilter, write=FOnChangeFilter};

};
//---------------------------------------------------------------------------
#endif

