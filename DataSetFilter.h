//---------------------------------------------------------------------------
/*
  Class for convinient work with Filter property in most populare DataSets.
  @created: 2016-01-16
  @author: vs.ovchinnikov
*/

#ifndef DataSetFilterH
#define DataSetFilterH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <vector>
#include <map>
#include <Db.hpp>

/*class ParamItem
{
public:
    String value;
    String defaultValue;
};*/

class TDataSetFilterItem;

typedef std::map<String, Variant> TFilterParamType;
typedef std::map<String, TDataSetFilterItem> TFilterItemType;

class TDataSetFilterItem
{
public:
    __fastcall TDataSetFilterItem();
    ~TDataSetFilterItem();
    void setActive(bool active);
    bool isActive() const { return _active; };
    bool isEmpty() const { return _empty; };
    String getText() const;
    void addParameter(const String& paramName, const String& paramValue="");
    void setText(const String& text);
    void resetParamValue(); // Для всех параметров
    void setParamValue(const String& paramName, Variant paramValue);
    Variant getParamValue(const String& paramName);
    bool isParamExists(const String& paramName);

private:
    void refreshFilterText();   // Обновляет итоговое значение строки
    bool isEmptyParams();       // Проверяет, является ли полностью заданным список параметров
    bool _active;   // Если фильтр включен пользователем
    bool _empty;    // Если значение фильтра пустое (=="")
    String _text;        // Исходный текст фильтра с полями-параметрами (:<имя параметра>)
    String _textResult; // текст после установки значения
    TReplaceFlags _replaceFlags;
    TFilterParamType _param;
    //std::map<String, ParamItem> _param;
};


class TDataSetFilter;
typedef void __fastcall (__closure *TOnChangeEvent)(TDataSetFilter* Sender, String filterName);

//---------------------------------------------------------------------------
class PACKAGE TDataSetFilter : public TComponent
{
private:
    /*Notification*/
	//Classes::TNotifyEvent FOnChange;
    //void __fastcall Change();
    TOnChangeEvent FOnChange;
    virtual void __fastcall DoOnChange(TDataSetFilter* Sender, String filterName = "");

    /* Main storage*/
    TFilterItemType* _items;

    /* Processing */
    String FGlue;

    /* Refresh */
    bool _controlsDisabled;
    String _beforeDisableControls;
    bool _beginUpdateSwitcher;

    /* DataSet */
    void __fastcall SetDataset(TDataSet* dataSet);
    TDataSet* FDataSet;
    void* _dataSetPos;
    String _filterStrBeforeLocking;
    //int _dataSetPos;


public:
    __fastcall TDataSetFilter(TComponent* Owner);
    __fastcall ~TDataSetFilter();

    /* Refresh */
    void __fastcall BeginUpdate();
    void __fastcall EndUpdate();
    void __fastcall DisableControls();
    void __fastcall EnableControls();
    void __fastcall Refresh();

    void __fastcall LockDataSetPos();
    void __fastcall UnlockDataSetPos();



    /*Filling the Main storage*/
    TDataSetFilterItem* add(const String& filterName, const String& filterStr);
    //void add(void* filterCtrl, const String& filterStr);
    void remove(const String& filterName);
    void clear();

    /**/
    String getFilterString() const;
    void setActive(const String& filterName, bool active);
    void setValue(const String& filterName, const String& paramName, Variant paramValue);
    Variant getValue(const String& filterName, const String& paramName) const;
    void clearValue(const String& filterName, const String& paramName);
    //void clearValue(const String& filterName);
    void clearAllValues();

    /* Testing */
    bool isFilterExists(const String& filterName, const String& paramName = "");

__published:
	//__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};
	__property TOnChangeEvent OnChange = {read=FOnChange, write=FOnChange};
    __property String Glue = {read = FGlue, write = FGlue};
    __property TDataSet* DataSet = {read = FDataSet, write = SetDataset};

};

//---------------------------------------------------------------------------
#endif
 