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

/**/
__fastcall TDataSetFilterItem::TDataSetFilterItem() :
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
TDataSetFilterItem::~TDataSetFilterItem()
{
}

/*
*/
void TDataSetFilterItem::setActive(bool active)
{
    _active = active;
}

/* Возвращает результирующий текст фильтра
*/
String TDataSetFilterItem::getText() const
{
    return _textResult;
}

/* Добавляет параметр в список
*/
void TDataSetFilterItem::addParameter(const String& paramName, const String& paramValue)
{
    _param[paramName] = paramValue;
}

/* Задает значение параметра, если параметр не был добавлен ранее, добавляет его
*/
void TDataSetFilterItem::setParamValue(const String& paramName, Variant paramValue)
{
    _param[paramName] = paramValue;

    // Тест исходного текста
    if (_text == "")
    {
        _empty = true;
        return;
    }

    // Тест параметров
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

/* Проверяет, есть ли параметр с указанным именем */
bool TDataSetFilterItem::isParamExists(const String& paramName)
{
    TFilterParamType::iterator param = _param.find(paramName);

    //Variant k = param->second;

    //bool t = param == _param.end();
    return param != _param.end();
}


/* Тестирует список параметров на пустоту
   возращает true если хоть один параметр не задан
*/
bool TDataSetFilterItem::isEmptyParams()
{
    for (TFilterParamType::iterator it = _param.begin(); it != _param.end(); it++)
    {
        if ( VarToStr(it->second) == "" )
        {
            return true;
        }
    }

    return false;
}

/* Обновляет итоговое значение строки */
void TDataSetFilterItem::refreshFilterText()
{
    _textResult = _text;
    // Обновляем текст
    for ( TFilterParamType::iterator it = _param.begin(); it != _param.end(); it++ )
    {
        /*if (VarIsEmpty(it->second))
        {
            ???
        } */
        _textResult = StringReplace(_textResult, ":" + it->first, VarToStr(it->second), _replaceFlags);
    }
}


/**/
Variant TDataSetFilterItem::getParamValue(const String& paramName)
{
    //String s = _param[paramName];
    return _param[paramName];
}

/*
*/
void TDataSetFilterItem::resetParamValue()
{
    for (TFilterParamType::iterator it = _param.begin(); it != _param.end(); it++)
    {
        setParamValue(it->first, VT_EMPTY);
    }
}

/*
*/
void TDataSetFilterItem::setText(const String& text)
{
    _text = text;
    // чтобы задать первоначальное значение _textResult
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
    _items = new TFilterItemType;
}

/*
*/
__fastcall TDataSetFilter::~TDataSetFilter()
{
    _items->clear();
    delete _items;
    _items = NULL;
}


/* Добавляет элемент в фильтр
*/
TDataSetFilterItem* TDataSetFilter::add(const String& filterName, const String& filterStr)
{
    (*_items)[filterName].setText(filterStr);

    DoOnChange(this, filterName);

    return &(*_items)[filterName];
}

/* Добавляет элемент в фильтр
*/
/*void TDBGridAltFilter::add(void* filterCtrl, const String& filterStr)
{
    //std::stringstream ss;
    //ss << filterCtrl;
    //std::string name = ss.str();

    String s = IntToStr(filterCtrl);
    add(s, filterStr);
}*/


/* Удаляет элемент фильтра по имени элемента
*/
void TDataSetFilter::remove(const String& filterName)
{
    _items->erase(filterName);
    DoOnChange(this);
}

/* Очищает фильтр
*/
void TDataSetFilter::clear()
{
    _items->clear();
    DoOnChange(this);
}

/* Меняет свойство Active фильтра
*/
void TDataSetFilter::setActive(const String& filterName, bool active)
{
    (*_items)[filterName].setActive(active);
    DoOnChange(this, filterName);
}

/* Задает значение элемента фильтра
*/
void TDataSetFilter::setValue(const String& filterName, const String& paramName, Variant paramValue)
{
    /*TDBGridAltFilterItem *item;
    std::pair<const AnsiString, TDBGridAltFilterItem> *it = _items->find(filterName);
    if ( it != _items.end() )
    {
        item->setParamValue(value);
    } */
    //(*_items)[filterName].setParamValue(":" + paramName, paramValue);
    (*_items)[filterName].setParamValue(paramName, paramValue);
    DoOnChange(this, filterName);
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
 Замена нескольких значений
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


/* Обнуляет текущее значение фильтра (всех подфильтров или выбранного по имени)
*/
void TDataSetFilter::clearValue(const String& filterName, const String& paramName)
{
    (*_items)[filterName].setParamValue(paramName, "");
    DoOnChange(this, filterName);

}

/*void TDBGridAltFilter::clearValue(const String& filterName)
{
    for(...)
    {
        (*_items)[filterName].setParamValue(paramName, "");
    }
}*/


/* Очищает значение всех параметров фильтра
*/
void TDataSetFilter::clearAllValues()
{
    // Цикл по под-фильтрам
    for (TFilterItemType::iterator it = _items->begin(); it != _items->end(); it++)
    {
        if ( it->second.isActive() && !it->second.isEmpty())   // если фильтр активен
        {

            it->second.resetParamValue();
        }
    }
    DoOnChange(this);
}

/* Проверяет существует ли фильтр или параметр с заданным именим */
bool TDataSetFilter::isFilterExists(const String& filterName, const String& paramName)
{
    TFilterItemType::iterator item = _items->find(filterName);

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
        //item->second.getParamValue()
        //return (*_items)[filterName].getParamValue(":" + paramName);
    }

    /*TDataSetFilterItem* item = &(*_items)[filterName];
    if ((*_items)[filterName] == _items->end())
    {
    }
    else
    {
        //_items[filterName]
    } */
}



/* Объединяет вектор подстрок в одну строку используя соединитель
*/
String TDataSetFilter::getFilterString() const
{
	String a = "";

    for (TFilterItemType::iterator it = _items->begin(); it != _items->end(); it++)
    {
        if ( it->second.isActive() && !it->second.isEmpty())   // если фильтр активен
        {
            a += it->second.getText() + FGlue;
        }
    }

    a = a.SubString(1, a.Length() - FGlue.Length());

 	return a;
}


/* Удаляет присоединенный фильтр
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

/* Блокирует отображение изменений значения фильтра
   Блокирует отправку сообщений об изменении значений фильтра
 */
void __fastcall TDataSetFilter::DisableControls()
{
    _controlsDisabled = true;
    //if (FDataSet != NULL && FDataSet->Active)
    //{
    //    _dataSetPos = FDataSet->RecNo;
    //}
}

/* Снимает блокировку отображения изменений значения фильтра
   Снимает блокировку отправки сообщений об изменении значений фильтра
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

/* Предотвращает изменение dataSeta
Возможно в дальнейшем удалить*/
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
    // возможно реализовать проверку _beforeDisableControls == _afterDisableControls
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

