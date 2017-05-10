//---------------------------------------------------------------------------

#include <basepch.h>

#pragma hdrstop

#include "DataSetFilter.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TDataSetFilter *)
{
    new TDataSetFilter(NULL);
}

/**/
namespace Datasetfilter
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TDataSetFilter)};
         RegisterComponents("AltCtrl", classes, 0);
    }  
}

TParamValue::TParamValue() :
    value(""),
    defaultValue("")
{
    //
}

/**/
__fastcall TSingleFilterItem::TSingleFilterItem() :
    _active(true),
    _text(""),
    //_paramValue(""),
    _textResult(""),
    _empty(true)

{
    _replaceFlags = TReplaceFlags() << rfReplaceAll;
}

/*
*/
TSingleFilterItem::~TSingleFilterItem()
{
}

/*
*/
void TSingleFilterItem::setActive(bool active)
{
    _active = active;
}

/* ���������� �������������� ����� �������
*/
String TSingleFilterItem::getText() const
{
    return _textResult;
}

/* ��������� �������� � ������
*/
void TSingleFilterItem::addParameter(const String& paramName, const String& paramValue)
{
    _param[paramName].defaultValue = paramValue;
    _param[paramName].value = paramValue;
}


/* ������ �������� ��-��������� ���������, ���� �������� �� ��� �������� �����, ��������� ���
*/
void TSingleFilterItem::setParamDefaultValue(const String& paramName, Variant defaultValue)
{
    _param[paramName].defaultValue = defaultValue;
    setParamValue(paramName, defaultValue);
}

/* ������ �������� ���������, ���� �������� �� ��� �������� �����, ��������� ���
*/
void TSingleFilterItem::setParamValue(const String& paramName, Variant paramValue)
{
    _param[paramName].value = paramValue;

    // ���� ��������� ������
    if (_text == "")
    {
        _empty = true;
        return;
    }

    // ���� ����������
    _empty = isEmptyParams();

    if (_empty)
    {
        _textResult = "";
    }
    else
    {
        refreshFilterText();
    }
}

/* ���������, ���� �� �������� � ��������� ������ */
bool TSingleFilterItem::isParamExists(const String& paramName)
{
    TFilterParamType::iterator param = _param.find(paramName);
    return param != _param.end();
}

/* ��������� ������ ���������� �� �������
   ��������� true ���� ���� ���� �������� �� �����
*/
bool TSingleFilterItem::isEmptyParams()
{
    for (TFilterParamType::iterator it = _param.begin(); it != _param.end(); it++)
    {
        if ( VarToStr(it->second.value) == "" )
        {
            return true;
        }
    }

    return false;
}

/* ��������� �������� �������� ������ */
void TSingleFilterItem::refreshFilterText()
{
    _textResult = _text;
    // ��������� �����
    for ( TFilterParamType::iterator it = _param.begin(); it != _param.end(); it++ )
    {
        /*if (VarIsEmpty(it->second))
        {
            ???
        } */
        _textResult = StringReplace(_textResult, ":" + it->first, VarToStr(it->second.value), _replaceFlags);
    }
}


/**/
Variant TSingleFilterItem::getParamValue(const String& paramName)
{
    return _param[paramName].value;
}

/* ���������� �������� ���������� �������� ����������
*/
void TSingleFilterItem::resetParamValue(const String& paramName)
{
    _param[paramName].value = _param[paramName].defaultValue;
}

/* ���������� �������� ���������� �������� ����������
*/
void TSingleFilterItem::resetParamValue()
{
    for (TFilterParamType::iterator it = _param.begin(); it != _param.end(); it++)
    {
        setParamValue(it->first, it->second.defaultValue);
        //setParamValue(it->first, VT_EMPTY);
    }
}

/*
*/
void TSingleFilterItem::setText(const String& text)
{
    _text = text;
    // ����� ������ �������������� �������� _textResult
    //setParamValue("");
}

/*
 TDBGridAltFilter class
 @created: 2016-01-16
 @author: vs.ovchinnikov
*/


__fastcall TDataSetFilter::TDataSetFilter(TComponent* Owner)
    : TComponent(Owner),
    _beginUpdateSwitcher(false),
    _controlsDisabled(false),
    FGlue(" AND "),
    FDataSet(NULL)
{
    _items = new TSingleFilterList;
}

/*
*/
__fastcall TDataSetFilter::~TDataSetFilter()
{
    _items->clear();
    delete _items;
    _items = NULL;
}


/* ��������� ������� � ������
*/
TSingleFilterItem* TDataSetFilter::add(const String& filterName, const String& filterStr)
{
    (*_items)[filterName].setText(filterStr);

    DoOnChange(this, filterName);

    return &(*_items)[filterName];
}

/* ��������� ������� � ������
*/
/*void TDBGridAltFilter::add(void* filterCtrl, const String& filterStr)
{
    //std::stringstream ss;
    //ss << filterCtrl;
    //std::string name = ss.str();

    String s = IntToStr(filterCtrl);
    add(s, filterStr);
}*/


/* ������� ������� ������� �� ����� ��������
*/
void TDataSetFilter::remove(const String& filterName)
{
    _items->erase(filterName);
    DoOnChange(this);
}

/* ������� ������
*/
void TDataSetFilter::clear()
{
    _items->clear();
    DoOnChange(this);
}

/* ������ �������� Active �������
*/
void TDataSetFilter::setActive(const String& filterName, bool active)
{
    (*_items)[filterName].setActive(active);
    DoOnChange(this, filterName);
}

/* ������ �������� ��-��������� �������� �������
*/
void TDataSetFilter::setDefaultValue(const String& filterName, const String& paramName, Variant paramValue)
{
    if ( isFilterExists(filterName) )
    {
        (*_items)[filterName].setParamDefaultValue(paramName, paramValue);
        DoOnChange(this, filterName);
    }
}

/* ������ �������� �������� �������
*/
void TDataSetFilter::setValue(const String& filterName, const String& paramName, Variant paramValue)
{
    if ( isFilterExists(filterName) )
    {
        (*_items)[filterName].setParamValue(paramName, paramValue);
        DoOnChange(this, filterName);
    }
}

/**/
Variant TDataSetFilter::getValue(const String& filterName, const String& paramName) const
{
    if ( isFilterExists(filterName, paramName) )
    {
        return (*_items)[filterName].getParamValue(paramName);
    }
    else
    {
        return Variant();
    }
    //return (*_items)[filterName].getParamValue(":" + paramName);
}


/*
 ������ ���������� ��������
*/
/*void TDBGridAltFilter::setValue(const String& filterName, const String& value1, const String& value2)
{
    /*TDBGridAltFilterItem *item;
    std::pair<const AnsiString, TDBGridAltFilterItem> *it = _items->find(filterName);
    if ( it != _items.end() )
    {
        item->setParamValue(value);
    } */
    /*(*_items)[filterName].setParamValue("param1", value1);
    (*_items)[filterName].setParamValue("param2", value2);
}*/


/* �������� ������� �������� ������� (���� ����������� ��� ���������� �� �����)
*/
void TDataSetFilter::clearValue(const String& filterName, const String& paramName)
{
    (*_items)[filterName].resetParamValue(paramName);
    DoOnChange(this, filterName);

}

/*void TDBGridAltFilter::clearValue(const String& filterName)
{
    for(...)
    {
        (*_items)[filterName].setParamValue(paramName, "");
    }
}*/


/* ���������� �������� ���������� ���� �����������
*/
void TDataSetFilter::clearAllValues()
{
    // ���� �� ���-��������
    for (TSingleFilterListIterator it = _items->begin(); it != _items->end(); it++)
    {
        if ( it->second.isActive() && !it->second.isEmpty())   // ���� ������ �������
        {
            it->second.resetParamValue();
        }
    }
    DoOnChange(this);
}

/* ��������� ���������� �� ������ ��� �������� � �������� ������ */
bool TDataSetFilter::isFilterExists(const String& filterName, const String& paramName) const
{
    TSingleFilterListIterator item = _items->find(filterName);

    if ( item == _items->end() )
    {
        return false;
    }
    else
    {
        if (paramName == "")
        {
            return true;
        }
        else
        {
            return item->second.isParamExists(paramName);
        }
    }
}



/* ���������� ������ �������� � ���� ������ ��������� �����������
*/
String TDataSetFilter::getFilterString() const
{
	String a = "";

    for (TSingleFilterListIterator it = _items->begin(); it != _items->end(); it++)
    {
        if ( it->second.isActive() && !it->second.isEmpty())   // ���� ������ �������
        {
            a += it->second.getText() + FGlue;
        }
    }

    a = a.SubString(1, a.Length() - FGlue.Length());

 	return a;
}


/* ������� �������������� ������
*//*
void __fastcall TDBGridAlt::OffFilter()
{
    this->BeginUpdate();
    _dataSet->Filter = "";
    //((TOraQuery*)DataSource->DataSet)->FilterSQL = "";
    //(dynamic_cast<TOraQuery*>(DataSource->DataSet))->FilterSQL = "";
    this->EndUpdate();
}*/


/*void __fastcall TDataSetFilter::Change()
{
    if (FOnChange != NULL)
    {
        FOnChange(this);
    }
}*/

/**/
void __fastcall TDataSetFilter::DoOnChange(TDataSetFilter* Sender, String filterName)
{
    if (FDataSet != NULL && _beginUpdateSwitcher != true)
    {
        FDataSet->Filter = getFilterString();
    }

    if (FOnChange != NULL && _controlsDisabled == false)
    {
        FOnChange(this, filterName);
    }
}

/* ��������� ����������� ��������� �������� �������
   ��������� �������� ��������� �� ��������� �������� �������
 */
void __fastcall TDataSetFilter::DisableControls()
{
    _controlsDisabled = true;
    //if (FDataSet != NULL && FDataSet->Active)
    //{
    //    _dataSetPos = FDataSet->RecNo;
    //}
}

/* ������� ���������� ����������� ��������� �������� �������
   ������� ���������� �������� ��������� �� ��������� �������� �������
*/
void __fastcall TDataSetFilter::EnableControls()
{
    _controlsDisabled = false;
    //if (FDataSet != NULL && FDataSet->Active)
    //{
    //    FDataSet->RecNo = _dataSetPos;
    //}
    Refresh();
}

/* ������������� ��������� dataSeta
�������� � ���������� �������*/
void __fastcall TDataSetFilter::BeginUpdate()
{
    _beginUpdateSwitcher = true;
}

void __fastcall TDataSetFilter::EndUpdate()
{
    _beginUpdateSwitcher = false;
}


/**/
void __fastcall TDataSetFilter::Refresh()
{
    // �������� ����������� �������� _beforeDisableControls == _afterDisableControls
    DoOnChange(this, "");
}

/**/
void __fastcall TDataSetFilter::LockDataSetPos()
{
    if (FDataSet != NULL && FDataSet->Active )
    {
        FDataSet->DisableControls();
        _filterStrBeforeLocking = FDataSet->Filter;
        _dataSetPos = FDataSet->GetBookmark();
        //_dataSetPos = FDataSet->RecNo;
    }
}

/**/
void __fastcall TDataSetFilter::UnlockDataSetPos()
{
    if (FDataSet != NULL && FDataSet->Active )
    {
        FDataSet->Filter = _filterStrBeforeLocking;
        FDataSet->GotoBookmark(_dataSetPos);
        FDataSet->FreeBookmark(_dataSetPos);
        FDataSet->EnableControls();
    }
}

void __fastcall TDataSetFilter::SetDataset(TDataSet* dataSet)
{
    FDataSet = dataSet;
    Refresh();
}

