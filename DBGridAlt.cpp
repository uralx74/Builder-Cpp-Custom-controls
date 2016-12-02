
#include <vcl.h>
#pragma hdrstop

#include "DBGridAlt.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
static inline void ValidCtrCheck(TDBGridAlt *)
{
    new TDBGridAlt(NULL);
}

//---------------------------------------------------------------------------
namespace Dbgridalt
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TDBGridAlt)};
         RegisterComponents("AltCtrl", classes, 0);
    }
}

/*
 TDBGridAltFilterItem class
*/

__fastcall TDBGridAltFilterItem::TDBGridAltFilterItem() :
    _active(true),
    _text(""),
    _paramValue(""),
    _textResult(""),
    _empty(true)
{
    //TReplaceFlags _replaceFlags = TReplaceFlags() << rfReplaceAll << rfIgnoreCase;
    _replaceFlags = TReplaceFlags() << rfReplaceAll;
}

TDBGridAltFilterItem::~TDBGridAltFilterItem()
{
}

void TDBGridAltFilterItem::setActive(bool active)
{
    _active = active;
}


String TDBGridAltFilterItem::getText() const
{
    return _textResult;
}

void TDBGridAltFilterItem::setParamValue(const String& paramValue)
{
    _paramValue = paramValue;
    _empty = (_paramValue == "" || _text == "");
    _textResult = StringReplace(_text, ":param", _paramValue, _replaceFlags);
}

void TDBGridAltFilterItem::setText(const String& text)
{
    _text = text;

    // чтобы задать первоначальное значение _textResult
    //setParamValue("");
}

/*
 TDBGridAltFilter class
*/


/* Добавляет элемент в фильтр
*/
void TDBGridAltFilter::add(const String& filterName, const String& filterStr)
{
    (*_items)[filterName].setText(filterStr);
}

/* Удаляет элемент фильтра по имени элемента
*/
void TDBGridAltFilter::remove(const String& filterName)
{
    _items->erase(filterName);
}

/* Очищает фильтр
*/
void TDBGridAltFilter::clear()
{
    _items->clear();
}

void TDBGridAltFilter::setActive(const String& filterName, bool active)
{
    (*_items)[filterName].setActive(active);
}

void TDBGridAltFilter::setValue(const String& filterName, const String& value)
{
    /*TDBGridAltFilterItem *item;
    std::pair<const AnsiString, TDBGridAltFilterItem> *it = _items->find(filterName);
    if ( it != _items.end() )
    {
        item->setParamValue(value);
    } */
    (*_items)[filterName].setParamValue(value);
}


//---------------------------------------------------------------------------
//
__fastcall TDBGridAltFilter::TDBGridAltFilter()
{
    _items = new std::map<String, TDBGridAltFilterItem>;
}


//---------------------------------------------------------------------------
//
__fastcall TDBGridAltFilter::~TDBGridAltFilter()
{
    _items->clear();
    delete _items;
}

//------------------------------------------------------------------------------
// Объединяет вектор подстрок в одну строку используя соединитель
String TDBGridAltFilter::getFilterString(const String &glue) const
{
	String a = "";

    for (std::map<String, TDBGridAltFilterItem>::iterator it = _items->begin(); it != _items->end(); it++)
    {
        if ( it->second.isActive() && !it->second.isEmpty())   // если фильтр активен
        {
            a += it->second.getText() + glue;
        }
    }

    a = a.SubString(1, a.Length() - glue.Length());

 	return a;
}

/*
 TDBGridAlt class
*/

//---------------------------------------------------------------------------
//
__fastcall TDBGridAlt::TDBGridAlt(TComponent* Owner)
    : TDBGrid(Owner),
    FHideKeyField(true),
    FColumnAutosize(true),
    FFiltered(false)

    //FFiltered(false)
{
    FOddRowColor = static_cast<TColor>(RGB(250,250,250));
    FEvenRowColor = clWhite;
    FSortColumnColor = static_cast<TColor>(RGB(255,240,210));
    Options = TDBGridOptions() /*<< dgRowSelect*/ << dgTitles << dgRowLines << dgColLines << dgColumnResize << dgEditing << dgConfirmDelete << dgCancelOnExit << dgIndicator << dgTabs;
    FCheckedFont = new TFont();
    FCheckedFont->Color = clRed;
    FCheckedFont->Style = TFontStyles();


    //Create(this);
    //FHideKeyField = true;
    //FColumnAutosize = true;
    //FKeyFieldIndex = 1;
    //Filter = new TDBGridAltFilter();
}

//---------------------------------------------------------------------------
//
__fastcall TDBGridAlt::~TDBGridAlt()
{
    delete FCheckedFont;
    ItemsFlg.clear();
}

//---------------------------------------------------------------------------
// Событие при открытии и закрытии DataSet
void __fastcall TDBGridAlt::LinkActive(bool Value)
{
    //ShowMessage( "Link Active");

    FHideKeyField = true;
    //FKeyFieldIndex = 1;
    //FColumnAutosize = true;
    SortType = 0; // Без сортировки
    SortColumnIndex = -1;
    ItemsFlg.clear();

    if (Value) {

        ItemsFlg.reserve(DataSource->DataSet->RecordCount+1); // +1 - жертвуем 1 байтом так как нумерация rownum с 1
        for(int i=0; i <= DataSource->DataSet->RecordCount; i++) // Заполняем false - все пункты по умолчанию не выделены
        {
            ItemsFlg.push_back(false);
            //DataSource->DataSet->Fields->FieldByNumber(1)->Tag = i;
            //DataSource->DataSet->Next();
        }

        DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->Visible = !FHideKeyField;
        if (FColumnAutosize)
        {
            setAutosize();
        }

        DataSource->DataSet->Filtered = FFiltered;
        refreshFilter();

        //ShowMessage( IntToStr(DataSource->DataSet->RecordCount) + " + " + IntToStr(ItemsFlg.size()) + " + " + IntToStr(ItemsFlg[0]));
    } else
    {
        for (int i = 0; i < Columns->Count; i++)
        {
            Columns->Items[i]->Title->Font->Style = Columns->Items[i]->Title->Font->Style >> fsBold;
        }
    }

    TDBGrid::LinkActive(Value);
}

/*void __fastcall TDBGridAlt::DataChanged(void)
{
}*/

/*
//---------------------------------------------------------------------------
// При создании компонента
void __fastcall TDBGridAlt::Loaded()
{
    ShowMessage("Loaded");
} */

//---------------------------------------------------------------------------
// Двойной щелчок на строке
void __fastcall TDBGridAlt::DblClick()
{
    if (DataSource == NULL || DataSource->DataSet == NULL)
    {
        return;
    }

    TPoint pt;
    pt = Mouse->CursorPos;
    pt = ScreenToClient (pt);

    int RowHeight = RowHeights[0];



    if (pt.y > RowHeight  )     // Если двойной щелчек не по заголовку
    {
        invertCheck(DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger);
        Refresh();
    }
    else      // Если двойной щелчек по заголовку
    {
    }
}

//---------------------------------------------------------------------------
// Сортировка по щелчку на заголовке DBGrid
void __fastcall TDBGridAlt::KeyDown(Word &Key, Classes::TShiftState Shift)
{
    //ShowMessage( "key");
	switch (Key) {
	case VK_SPACE:
    	// Помечаем выделенную строку
        invertCheck(DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger);
    	//DblClick();
        break;
    case VK_INSERT:
    	// Помечаем выделенную строку
    	//DblClick();
        invertCheck(DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger);
        // Перемещаем курсор на следующую строку в Гриде
        TDBGrid::KeyDown(VK_DOWN, Shift);
        //DataSource->DataSet->Next();
        break;
    /*case VK_SHIFT:
        {
            //_selStartBookmark = DataSource->DataSet->Bookmark;
            //DataSource->DataSet->FreeBookmark();
            //DataSource->DataSet->GotoBookmark();
            _selStartBookmark = DataSource->DataSet->GetBookmark();
            break;
        } */
    default:
        TDBGrid::KeyDown(Key, Shift);
        break;
    }
    Refresh();
}

//---------------------------------------------------------------------------
// Прокрутка колесиком мыши
void __fastcall TDBGridAlt::MouseWheelHandler(TMessage &Msg)
{
    TShiftState Shift;
    KeyDown((short) Msg.WParamHi == WHEEL_DELTA ? VK_UP : VK_DOWN, Shift);
}

//---------------------------------------------------------------------------
// Сортировка по щелчку на заголовке DBGrid
void __fastcall TDBGridAlt::TitleClick(TColumn *Column)
{
    //ShowMessage( "title " + ((TOraQuery*)DataSource->DataSet)->IndexFieldNames);

    // Сортируем по выбранному полю + по полю Num
    // Выделение заголовка
    if (DataSource == NULL || DataSource->DataSet == NULL)
    {
        return;
    }
    
    if (SortColumnIndex == Column->Index)
    {
        if (SortType == 2)
        {
            SortType = 0;
            SortColumnIndex = -1;
            Column->Title->Font->Style = Column->Title->Font->Style >> fsBold;
        }
        else
        {
            SortType++;
            //Column->Title->Font->Style = Column->Title->Font->Style << fsBold;
        }
    } else {
        if (SortColumnIndex >= 0)
        {
            Columns->Items[SortColumnIndex]->Title->Font->Style = Columns->Items[SortColumnIndex]->Title->Font->Style >> fsBold;
        }
        Column->Title->Font->Style = Column->Title->Font->Style << fsBold;
        SortColumnIndex = Column->Index;
        SortType = 1;
    }

    AnsiString sSort;

    switch(SortType)
    {
    case 0:
        ((TOraQuery*)DataSource->DataSet)->IndexFieldNames = DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->DisplayName;//Column->FieldName;
        break;
    case 1:
        sSort = " ASC CIS";
        ((TOraQuery*)DataSource->DataSet)->IndexFieldNames = Column->FieldName + sSort;
        break;
    case 2:
        sSort = " DESC CIS";
        ((TOraQuery*)DataSource->DataSet)->IndexFieldNames = Column->FieldName + sSort;
        break;
    }

    DataSource->DataSet->First();
    Refresh();
}

//---------------------------------------------------------------------------
// Прорисовка
void __fastcall TDBGridAlt::DrawColumnCell(const TRect &Rect, int DataCol, TColumn* Column, TGridDrawState State)
{
    Canvas->Lock();    // Блокирум канвас перед рисованием

	// Расскрашиваем нечетные строки
	if ( (DataSource->DataSet->RecNo % 2 ) == 0)
    {
    	Canvas->Brush->Color = FOddRowColor;
    }
    else
    {
    	Canvas->Brush->Color = FEvenRowColor;
    }

    // Расскрашиваем столбец, по которому проведена сортировка
    if (SortType != 0 && Column->Index == SortColumnIndex)
    {
        Canvas->Brush->Color = FSortColumnColor;
    }

    // Оставляем выделение текущей позиции курсора
    //enum  { gdSelected, gdFocused, gdFixed };
	//if  (State.Contains(gdSelected))
    //static int curSelRecNo;
	if  (State.Contains(gdSelected))
    {
        Canvas->Brush->Color= clHighlight;
        Canvas->Font->Color = clHighlightText;
        //curSelRecNo = DataSource->DataSet->RecNo;
	}
    /*if (DataSource->DataSet->RecNo == curSelRecNo)
    {
        Canvas->Brush->Color= clHighlight;
        Canvas->Font->Color = clHighlightText;
    }*/

    //DataSource->DataSet->RecNo
    // Выделяем цветом помеченные строки
    if (ItemsFlg[DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger])
    {
        Canvas->Font = FCheckedFont;
    }
    Canvas->Unlock();    // Освобождаем канвас

	DefaultDrawColumnCell(Rect, DataCol, Column, State);
}

/*int __fastcall TDBGridAlt::test_01()
{
    return SelectedIndex;
}*/

//---------------------------------------------------------------------------
// Параметр Спрятать ключевое поле
void __fastcall TDBGridAlt::setVisibleKeyField(bool VisibleFlg)
{
    FHideKeyField = VisibleFlg;
}

//---------------------------------------------------------------------------
//
void __fastcall TDBGridAlt::setColumnAutosize(bool ColumnAutosizeFlg)
{
    FColumnAutosize = ColumnAutosizeFlg;
    if (Columns->Count > 0 && ColumnAutosizeFlg)
    {
        setAutosize();
    }
}

/*//---------------------------------------------------------------------------
// Параметр Индекс ключевого поля
void __fastcall TDBGridAlt::SetKeyFieldIndex(int Index)
{
    FKeyFieldIndex = Index;
} */

/*//---------------------------------------------------------------------------
// Параметр Индекс ключевого поля
void __fastcall TDBGridAlt::SetKeyFieldName(AnsiString Name)
{
    FKeyFieldName = Name;
}*/

//---------------------------------------------------------------------------
// Проверить, является ли строка отмеченой
bool __fastcall TDBGridAlt::isChecked(int RowNum)
{
    return ItemsFlg[RowNum];
}

//---------------------------------------------------------------------------
// Пометить все строки
void __fastcall TDBGridAlt::checkAll()
{
    for(std::vector<bool>::iterator it = ItemsFlg.begin(); it != ItemsFlg.end(); ++it)
    {
        (*it) = true;
    }
    Refresh();

    if (FOnChangeCheck != NULL)
    {
        FOnChangeCheck(this);
    }
}
//---------------------------------------------------------------------------
// Снять пометку со всех строк
void __fastcall TDBGridAlt::uncheckAll()
{
    for(std::vector<bool>::iterator it = ItemsFlg.begin(); it != ItemsFlg.end(); ++it)
    {
        (*it) = false;
    }

    Refresh();
    if (FOnChangeCheck != NULL)
    {
        FOnChangeCheck(this);
    }
}

//---------------------------------------------------------------------------
// Инверсия выделения пункта
void __fastcall TDBGridAlt::invertCheck(int Index)
{
    ItemsFlg[Index] = !ItemsFlg[Index];
    if (FOnChangeCheck != NULL)
    {
        FOnChangeCheck(this);
    }
}

//---------------------------------------------------------------------------
// Подсчет количества отмеченных строк
int __fastcall TDBGridAlt::getCheckedCount()
{
    int Count = 0;
    for(std::vector<bool>::size_type i = 1; i < ItemsFlg.size(); i++)
    {
        if (ItemsFlg[i])
        {
            Count++;
        }
    }
    return Count;
}

//---------------------------------------------------------------------------
// Пометить все отфильтрованные строки
void __fastcall TDBGridAlt::checkFiltered()
{
    //AnsiString bookmark = DataSource->DataSet->Bookmark;
    int N = DataSource->DataSet->RecNo;
    DataSource->DataSet->DisableControls();

    DataSource->DataSet->First();
    while(!DataSource->DataSet->Eof)
    {
        ItemsFlg[DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger] = true;
        DataSource->DataSet->Next();
    }

    DataSource->DataSet->RecNo = N;
    //DataSource->DataSet->Bookmark = bookmark;
    DataSource->DataSet->EnableControls();
    Refresh();

    if (FOnChangeCheck != NULL)
    {
        FOnChangeCheck(this);
    }
}

/* Задает текущий фильтр
*/
void __fastcall TDBGridAlt::assignFilter(TDBGridAltFilter* filter)
{
    _filter = filter;
}

void __fastcall TDBGridAlt::clearFilter()
{
    _filter = NULL;
    DataSource->DataSet->Filter = "";
}


/* Обновляет строку фильтра в dataset
*/
void __fastcall TDBGridAlt::refreshFilter()
{
    DataSource->DataSet->Filter = _filter->getFilterString(" AND ");
    if (FOnChangeFilter != NULL)
    {
        FOnChangeFilter(this);
    }

}

/* Установка ширины столбцов
*/
void __fastcall TDBGridAlt::setAutosize()
{
    int ScrollWidth = GetSystemMetrics(SM_CXVSCROLL);       // Ширина вертикального скроллбара

    // Подсчитываем ширину видимых столбцов
    int AllWidth = 0;
    int VisibleColumnsCount = 0;        // Это значение будем использовать для подсчета ширины границы между столбцами
    for (int i = 0; i < Columns->Count; i++)
    {
        if (Columns->Items[i]->Visible)
        {
            AllWidth += Columns->Items[i]->Width;
            VisibleColumnsCount++;
        }
    }

    float k = float(ClientWidth - ScrollWidth - VisibleColumnsCount)/AllWidth;  // Коэффициент

    for (int i = 0; i < Columns->Count; i++)
    {
        if (Columns->Items[i]->Visible)
        {
            //int r1 = Columns->Items[i]->Width;
            Columns->Items[i]->Width = ceil(Columns->Items[i]->Width * k);
        }
    }
}




/***************************************************************
****************************************************************
****************************************************************/

// Сумма по столбцу всего
double __fastcall TDBGridAlt::getSum(const String& fieldName, bool checked, bool filtered)
{
    //int topRow = this->TopRow;
    //void* bookmark = DataSource->DataSet->GetBookmark();
    //AnsiString bookmark = DataSource->DataSet->Bookmark;
    //int MyActiveRec = DataSource->DataSet->ActiveRecord;

    int N = DataSource->DataSet->RecNo;
    DataSource->DataSet->DisableControls();

    bool wasFiltered = DataSource->DataSet->Filtered;
    DataSource->DataSet->Filtered = filtered;  // Если нужно получить все записи

    DataSource->DataSet->First();

    double sum = 0;
    if (checked)
    {
        while(!DataSource->DataSet->Eof)
        {
            if (ItemsFlg[DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger])
            {
                sum += DataSource->DataSet->FieldByName(fieldName)->AsFloat;
            }
            DataSource->DataSet->Next();
        }
    }
    else
    {
        while(!DataSource->DataSet->Eof)
        {
            sum += DataSource->DataSet->FieldByName(fieldName)->AsFloat;
            DataSource->DataSet->Next();
        }
    }

    DataSource->DataSet->Filtered = wasFiltered;
    DataSource->DataSet->RecNo = N;
    DataSource->DataSet->EnableControls();


    //DataSource->DataSet->Bookmark = bookmark;
    //DataSource->DataSet->CheckBrowseMode();
    //DataSource->DataSet->GotoBookmark(bookmark);
    //DataSource->DataSet->BeforeScroll();
    //DataSource->DataSet->InternalGotoBookmark(bookmark);
    //DataSource->DataSet->
    //DataSource->DataSet->Resync(TResyncMode()<<rmExact);
    //DataSource->DataSet->FreeBookmark(bookmark);
    //this->TopRow = topRow;
    //ScrollData();

    return sum;
}

// Сумма по столбцу с учетом фильтра
// Сумма по столбцу с учетом выделенных

//---------------------------------------------------------------------------
//
void __fastcall TDBGridAlt::setFiltered(bool Value)
{
    if (FFiltered != Value || DataSource->DataSet->Filtered != Value)
    {
        FFiltered = Value;

        if ((DataSource == NULL) || (DataSource->DataSet == NULL))
        {
            return;
        }

        if (Value && _filter)
        {
            refreshFilter();
        }
        DataSource->DataSet->Filtered = Value;

        /*if (Value && Filter->Items->size() > 0)
        {
            refreshFilter();
        }
        else
        {
            DataSource->DataSet->Filter = "";
            //Value = false;
            //FFiltered = false;
        }*/
    }


/*   if dgeLocalFilter in OptionsEx then
      DataSource.DataSet.Filtered := Value
    else begin
      if not Value then
        if DataLink.DataSet is TCustomDADataSet then
          TCustomDADataSet(DataSource.DataSet).FilterSQL := '';
    end;
  end   */

}


//---------------------------------------------------------------------------
//
//TVirtualTable* __fastcall TDBGridAlt::GetChecked()
//{
    //TOraDataSet* result = new TOraDataSet("");
    //TTable* result = new DataTable("");

/*    TVirtualTable* TableResult = new TVirtualTable(NULL);

    for (int i = 0; i < DataSource->DataSet->Fields->Count; i++) {
        TableResult->AddField(DataSource->DataSet->Fields->FieldByNumber(i)->Name, DataSource->DataSet->Fields->FieldByNumber(i)->DataType, DataSource->DataSet->Fields->FieldByNumber(i)->DataSize, false);
    }
    TableResult


    while (!DataSource->DataSet->Eof) {

    }
 */

    /*TableResult->AddField("N_DOG", ftLargeint, 0, false);
    TableResult->AddField("CLIC_SCHET", ftLargeint, 0, false);
    TableResult->AddField("G_NUM",ftInteger, 0, false);
    TableResult->AddField("NABONENTID", ftLargeint, 0, false);
    TableResult->AddField("CFIO",ftString, 40, false);
    TableResult->AddField("ULITSA",ftString, 25, false);
    TableResult->AddField("NDOM", ftInteger, 0, false);
    TableResult->AddField("CDOM", ftString, 20, false);
    TableResult->AddField("NKVARTIRA", ftInteger, 0, false);
    TableResult->AddField("CKVARTIRA", ftString, 20, false);
    TableResult->AddField("NOTE", ftString, 80, false);
    TableResult->AddField("DATEKP", ftDateTime, 0, false);
    TableResult->AddField("KPDAY", ftInteger, 0, false);
    TableResult->AddField("KPNITE" ,ftInteger, 0, false);
    TableResult->AddField("nvidrasx" ,ftInteger, 0, false); */


//}
