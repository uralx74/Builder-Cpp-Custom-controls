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

class TDBGridAltFilter
{
private:

public:
    __fastcall TDBGridAltFilter();
    __fastcall TDBGridAltFilter::~TDBGridAltFilter();
    void Add(String FilterName, String FilterStr);
    void Delete(String FilterName);
    void Clear();
    String GetFilterString(const String &glue = "");

    std::map<String, String>* Items;

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
    //virtual void __fastcall Loaded();     // при создании компонента
    void __fastcall SetVisibleKeyField(bool VisibleFlg);
    void __fastcall SetColumnAutosize(bool ColumnAutosizeFlg);
    //void __fastcall SetKeyFieldIndex(int Index);
    //void __fastcall SetKeyFieldName(AnsiString Name);
    void __fastcall SetFiltered(bool Value);
    int __fastcall GetCheckedCount();
    void __fastcall InvertCheck(int Index);
    //void __fastcall AddFilter(int Index);
    //void __fastcall ClearFilter(int Index);

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
    typedef void __fastcall (__closure *TOnDataEvent)(TObject* Sender,AnsiString &Buffer); 
    TOnDataEvent fOnData;

protected:
    BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(WM_MOUSEWHEEL, TMessage, MouseWheelHandler)
    END_MESSAGE_MAP(TDBGrid)


public:
    __fastcall TDBGridAlt(TComponent* Owner);
    __fastcall TDBGridAlt::~TDBGridAlt();
    bool __fastcall IsChecked(int RowNum);
    void __fastcall CheckAll();
    void __fastcall CheckFiltered();
    void __fastcall UncheckAll();
    void __fastcall SetAutosize();
    TVirtualTable* __fastcall GetChecked();

    TDBGridAltFilter* Filter;


__published:
   __property String FileName  = { read=FFileName, write=FFileName };
   __property TColor OddRowColor  = { read=FOddRowColor, write=FOddRowColor};
   __property TColor EvenRowColor  = { read=FEvenRowColor, write=FEvenRowColor};
   __property TColor SortColumnColor  = { read=FSortColumnColor, write=FSortColumnColor};
   __property bool HideKeyField  = { read=FHideKeyField, write=SetVisibleKeyField, default = true};
   __property bool ColumnAutosize  = { read=FColumnAutosize, write=SetColumnAutosize, default = true};
   //__property int KeyFieldIndex  = { read=FKeyFieldIndex, write=SetKeyFieldIndex, default = 1};
   __property AnsiString KeyFieldName  = { read=FKeyFieldName, write=FKeyFieldName};
   __property TFont* CheckedFont = { read=FCheckedFont, write=FCheckedFont};
   __property int CheckedCount = { read=GetCheckedCount};
   __property TNotifyEvent OnChangeCheck = {read=FOnChangeCheck, write=FOnChangeCheck};
   __property TOnDataEvent OnData = { read=fOnData, write=fOnData };
   __property bool Filtered = {read = FFiltered, write = SetFiltered, default = false};


};
//---------------------------------------------------------------------------
#endif

