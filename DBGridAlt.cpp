#include <vcl.h>
#pragma hdrstop
#include "DBGridAlt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//#pragma link "DBAccess"
//#pragma link "Ora"
//#pragma link "MemDS"
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.

using namespace DBGridAltCheckTypes;

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
 TDBGridAlt class
*/

/*
*/
__fastcall TDBGridAlt::TDBGridAlt(TComponent* Owner)
    : TDBGrid(Owner),
    //FHideKeyField(true),
    FColumnAutosize(true),
    FDefaultSortFieldName("ROWNUM"),
    FAllowChecked(true),
    _recordCount(0),
    _checkDataFieldName("CHECK_DATA")

    //FFiltered(false)
{
    FOddRowColor = static_cast<TColor>(RGB(250,250,250));
    FEvenRowColor = clWhite;
    FSortColumnColor = static_cast<TColor>(RGB(255,240,210));
    Options = TDBGridOptions() /*<< dgRowSelect*/ << dgTitles << dgRowLines << dgColLines << dgColumnResize << dgEditing << dgConfirmDelete << dgCancelOnExit << dgIndicator << dgTabs;
    FCheckedFont = new TFont();
    FCheckedFont->Color = clRed;
    FCheckedFont->Style = TFontStyles();

    FEditableFont = new TFont();
    FEditableFont->Color = clBlack;
    FEditableFont->Style = TFontStyles();

    //_dataSet = (TVirtualTable*)DataSource->DataSet;
    //_dataSet = new TVirtualTable(this);

    //OldDataSource =
    //DataSource->DataSet = _dataSet;


    /*edit = new TEdit(Owner);
    edit->Parent = this;
    edit->Visible = false;*/
    //edit->Text = "at";

    //FDataLink = new TFieldDataLink();//>CreateDataLink();
    //FDataLink->OnDataChange = DataChange;   // this was here too
}

/*
*/
__fastcall TDBGridAlt::~TDBGridAlt()
{
    delete FCheckedFont;
    delete FEditableFont;
    /*if (_dataSet != NULL)
    {
        delete _dataSet;
    }*/
    //ItemsFlg.clear();
}

/*void __fastcall TDBGridAlt::Assign(TPersistent* Source)
{
    _dataSet->Assign(Source);
    _dataSet->Active = true;
    _dataSet->AddField(_checkDataFieldName, ftInteger, 0, false);

    //if ( _dataSet->FindField(_checkDataFieldName) == NULL)
    //{
    //    _dataSet->AddField(_checkDataFieldName, ftInteger, 0, false);
    //}
} */


/* Событие при открытии и закрытии DataSet
*/
void __fastcall TDBGridAlt::LinkActive(bool Value)
{
    //Columns->Items

    //ShowMessage( "Link Active");

    //TDataLink
    //FHideKeyField = true;
    //FKeyFieldIndex = 1;
    //FColumnAutosize = true;
    SortType = 0; // Без сортировки
    _sortColumnIndex = -1;

    if ( Value )
    {
        _dataSet = (TOraQuery*)DataSource->DataSet;

        bool old_filtered = _dataSet->Filtered;
        _dataSet->Filtered = false;
        _recordCount = _dataSet->RecordCount;
        _dataSet->Filtered = old_filtered;
        if ( FColumnAutosize )
        {
            setAutosize();
        }
    }
    else
    {
        _dataSet = NULL;
        _recordCount = 0;
        for (int i = 0; i < Columns->Count; i++)
        {
            Columns->Items[i]->Title->Font->Style = Columns->Items[i]->Title->Font->Style >> fsBold;
        }
    }
    TDBGrid::LinkActive(Value);
}

/* Двойной щелчок на строке
*/
void __fastcall TDBGridAlt::DblClick()
{
    //if (DataSource == NULL || DataSource->DataSet == NULL)
    if ( !this->DataLink->Active )
    {
        return;
    }

    TPoint pt;
    pt = Mouse->CursorPos;
    pt = ScreenToClient (pt);

    int RowHeight = RowHeights[0];

    if ( pt.y > RowHeight )     // Если двойной щелчек не по заголовку
    {
        invertCheck();
        Refresh();
    }
    else
    {
        // Если двойной щелчек по заголовку
    }
}

/*
*/
void __fastcall TDBGridAlt::setCheck(bool value)
{
    if ( recordCount == 0 )
    {
        return;
    }
    _dataSet->Edit();
    _dataSet->FieldByName(_checkDataFieldName)->Value = (value ? CT_CHECKED : CT_UNCHECKED);
    _dataSet->Post();
}

/* Сортировка по щелчку на заголовке DBGrid
*/
void __fastcall TDBGridAlt::KeyDown(Word &Key, Classes::TShiftState Shift)
{
    if ( !this->DataLink->Active )
    {
        return;
    }

	switch (Key)
    {
	case VK_SPACE:
    	// Помечаем выделенную строку
        invertCheck();
    	//DblClick();
        break;
    case VK_INSERT:
    	// Помечаем выделенную строку
    	//DblClick();
        invertCheck();
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

    case VK_DOWN:
        // Чтобы предотвратить возможность перехода в режим добавления новой записи

        //ShowMessage(IntToStr(FTest) + " - " +IntToStr(FAllowManualAppend) + " or " + IntToStr(DataSource->DataSet->RecordCount) + " != " + IntToStr(DataSource->DataSet->RecNo));

        if (FAllowManualAppend || _dataSet->RecNo != _dataSet->RecordCount)
        {
            TDBGrid::KeyDown(VK_DOWN, Shift);
        }

        break;

    default:
        TDBGrid::KeyDown(Key, Shift);
        break;
    }
    Refresh();
}

/* Прокрутка колесиком мыши
*/
void __fastcall TDBGridAlt::MouseWheelHandler(TMessage &Msg)
{
    KeyDown((short) Msg.WParamHi == WHEEL_DELTA ? VK_UP : VK_DOWN, TShiftState());
}

/* Сортировка по щелчку на заголовке DBGrid
*/
void __fastcall TDBGridAlt::TitleClick(TColumn *Column)
{
    //ShowMessage("TitleClick");
    //ShowMessage( "title " + ((TOraQuery*)DataSource->DataSet)->IndexFieldNames);

    // CIS - Case InSensitive
    // Сортируем по выбранному полю + по полю Num
    // Выделение заголовка
    //if (DataSource == NULL || DataSource->DataSet == NULL || !DataSource->DataSet->Active)
    if (!this->DataLink->Active)
    {
        return;
    }

    if (_sortColumnIndex == Column->Index)
    {
        if (SortType == 2)
        {
            SortType = 0;
            _sortColumnIndex = -1;
            Column->Title->Font->Style = Column->Title->Font->Style >> fsBold;
        }
        else
        {
            SortType++;
            //Column->Title->Font->Style = Column->Title->Font->Style << fsBold;
        }
    }
    else
    {
        if (_sortColumnIndex >= 0)
        {
            Columns->Items[_sortColumnIndex]->Title->Font->Style = Columns->Items[_sortColumnIndex]->Title->Font->Style >> fsBold;
        }
        Column->Title->Font->Style = Column->Title->Font->Style << fsBold;
        _sortColumnIndex = Column->Index;
        SortType = 1;
    }

    AnsiString sSort;

    switch(SortType)
    {
    case 0:
        if (FDefaultSortFieldName != "" && _dataSet->FindField(FDefaultSortFieldName) != NULL )
        {
            _dataSet->IndexFieldNames =  FDefaultSortFieldName + " ASC CIS";//_dataSet->FieldByName(FDefaultSortFieldName)->DisplayName
        }
        else
        {
            _dataSet->IndexFieldNames = "";//Column->FieldName;
        }
        //((TOraQuery*)_dataSet)->IndexFieldNames = DataSource->DataSet->Fields->FieldByName(_checkDataFieldName)->DisplayName;//Column->FieldName;
        break;
    case 1:
        sSort = " ASC CIS";
        _dataSet->IndexFieldNames = Column->FieldName + sSort;
        break;
    case 2:
        sSort = " DESC CIS";
        _dataSet->IndexFieldNames = Column->FieldName + sSort;
        break;
    }

    _dataSet->First();
    Refresh();
}

/* Прорисовка
*/
void __fastcall TDBGridAlt::DrawColumnCell(const TRect &Rect, int DataCol, TColumn* Column, TGridDrawState State)
{
    if ( !this->DataLink->Active || this->recordCount == 0 ) // тест
    {
        return;
    }

    Canvas->Lock();    // Блокирум канвас перед рисованием

	// Расскрашиваем нечетные строки
	if ( (_dataSet->RecNo % 2 ) == 0)
    {
    	Canvas->Brush->Color = FOddRowColor;
    }
    else
    {
    	Canvas->Brush->Color = FEvenRowColor;
    }

    // Расскрашиваем столбец, по которому проведена сортировка
    if (SortType != 0 && Column->Index == _sortColumnIndex)
    {
        Canvas->Brush->Color = FSortColumnColor;
    }

    //bool k;
    //static TColumn* c;

    /*if  ( State.Contains(gdFocused) )
    {
        edit->Left = Rect.Left ;
        edit->Top  = Rect.Top;
        //k = edit->Text;

        edit->Text = this->DataSource->DataSet->FieldByName(Column->FieldName)->AsString;
        edit->Width = Rect.Right - Rect.Left + 2;
        edit->Height = Rect.Bottom - Rect.Top + 2;
        //edit->Clear();
        edit->Visible = true;
        edit->BringToFront();
        edit->SetFocus();

        c = Column;
    }
    else
    {
        this->Canvas->FillRect(Rect);
        this->Canvas->dr
        DrawText(grid.Canvas.Handle, PChar(maskValue), Length(maskValue), aRect,
          DT_SINGLELINE or DT_LEFT or DT_VCENTER);
                  //edit->Visible = false;
    }*/


    //k = c->Showing;

    // Оставляем выделение текущей позиции курсора
    // если рисуется ячейка с соответствующим статусом
    // или активная запись

    //enum  { gdSelected, gdFocused, gdFixed };
	//if  (State.Contains(gdSelected))
    //static int curSelRecNo;
	if  (State.Contains(gdSelected)
        || this->DataLink->ActiveRecord == this->Row - 1)
    {
        Canvas->Brush->Color= clHighlight;
        Canvas->Font->Color = clHighlightText;
        //curSelRecNo = DataSource->DataSet->RecNo;

	}
    else
    {
        if ( Column->ReadOnly == false) // Если редактируемый
        {
            Canvas->Font = FEditableFont;
        }

    }

    //DataSource->DataSet->RecNo
    // Выделяем цветом помеченные строки
    if ( _dataSet->FieldByName(_checkDataFieldName)->AsInteger == CT_CHECKED)
    {
        Canvas->Font = FCheckedFont;
    }
    Canvas->Unlock();    // Освобождаем канвас

	DefaultDrawColumnCell(Rect, DataCol, Column, State);
}

/* Параметр Спрятать ключевое поле
*/
void __fastcall TDBGridAlt::setAllowChecked(bool allowChecked)
{
    FAllowChecked = allowChecked;
}

/*
*/
void __fastcall TDBGridAlt::setColumnAutosize(bool ColumnAutosizeFlg)
{
    FColumnAutosize = ColumnAutosizeFlg;
    if (Columns->Count > 0 && ColumnAutosizeFlg)
    {
        setAutosize();
    }
}

/* Проверить, является ли строка отмеченой
*/
bool __fastcall TDBGridAlt::isChecked()
{
    return _dataSet->FieldByName(_checkDataFieldName)->Value == CT_CHECKED;
}

//---------------------------------------------------------------------------
// Пометить все строки
void __fastcall TDBGridAlt::setCheckAll(bool value)
{
    int N = _dataSet->RecNo;
    _dataSet->DisableControls();

    _dataSet->First();
    while( !_dataSet->Eof )
    {
        setCheck(value);
        _dataSet->Next();
    }

    _dataSet->RecNo = N;
    _dataSet->EnableControls();

    if (FOnChangeCheck != NULL)
    {
        FOnChangeCheck(this);
    }
}
//---------------------------------------------------------------------------
// Снять пометку со всех строк
/*void __fastcall TDBGridAlt::uncheckAll()
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
}*/

/* Инверсия выделения пункта
*/
void __fastcall TDBGridAlt::invertCheck()
{
    if ( !FAllowChecked )
    {
        return;
    }

    setCheck( !isChecked() );

    if (FOnChangeCheck != NULL)
    {
        FOnChangeCheck(this);
    }
}

/* Подсчет количества отмеченных строк
*/
int __fastcall TDBGridAlt::getRecordCountChecked()
{
    if ( !this->DataLink->Active )
    {
        return 0;
    }

    int count = 0;

    _dataSet->DisableControls();
    int N = _dataSet->RecNo;

    _dataSet->First();
    while( !_dataSet->Eof )
    {
        if ( isChecked() )
        {
            count++;
        }
        _dataSet->Next();
    }

    _dataSet->RecNo = N;
    _dataSet->EnableControls();

    return count;
}

/*
*/
int __fastcall TDBGridAlt::getRecordCountFiltered()
{
    return _dataSet->RecordCount;
}

/* Устанавливает выделение по индексу
*/
/*void __fastcall TDBGridAlt::setCheck(int index, bool value)
{
    //ItemsFlg[index] = value;
    DataSource->DataSet->FieldByName(_checkDataFieldName)->Value = value ? FT_CHECKED : FT_UNCHECKED;
} */

/* Пометить все отфильтрованные строки
*/
void __fastcall TDBGridAlt::setCheckFiltered(bool value)
{
    //AnsiString bookmark = DataSource->DataSet->Bookmark;
    if ( !this->DataLink->Active )
    {
        return;
    }

    int N = _dataSet->RecNo;
    _dataSet->DisableControls();

    _dataSet->First();
    while( !_dataSet->Eof )
    {
        setCheck(value);
        _dataSet->Next();
    }

    _dataSet->RecNo = N;
    //DataSource->DataSet->Bookmark = bookmark;


    //_dataSet->Refresh();
    _dataSet->EnableControls();

    if (FOnChangeCheck != NULL)
    {
        FOnChangeCheck(this);
    }
}

/* Обновляет строку фильтра в dataset
*/
/*void __fastcall TDBGridAlt::SetFilterText(const AnsiString Value)
{
    ShowMessage("aaab");

    /*if (FOnChangeFilter != NULL)
    {
        FOnChangeFilter(this);
    }*//*
}    */

/* Обновляет строку фильтра в dataset
*/
/*void __fastcall TDBGridAlt::refreshFilter()
{
    if (_filter != NULL)
    {
        this->BeginUpdate();
        _dataSet->Filter = _filter->getFilterString(" AND ");
        this->EndUpdate();
    }

    if (FOnChangeFilter != NULL)
    {
        FOnChangeFilter(this);
    }
}*/

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

/* Возвращает скролл на позицию */
void __fastcall TDBGridAlt::ScrollActiveToRow(int ARow)
{
    TRect NewRect;

    int newRow = this->Row;
    int FTitleOffset = 0;

    if (this->Options.Contains(dgTitles) )
    {
        FTitleOffset++;
    }

    if (ARow == newRow)
    {
        return;
    }

    this->BeginUpdate();
    this->Scroll(newRow - ARow);

    if (newRow - ARow < 0)
    {
        this->DataLink->ActiveRecord = 0;
    }
    else
    {
        this->DataLink->ActiveRecord = this->VisibleRowCount - 1;
    }

    int SDistance = _dataSet->MoveBy(newRow - ARow);
    newRow = newRow - SDistance;
    _dataSet->MoveBy(ARow - this->DataLink->ActiveRecord - FTitleOffset);
    int RowHeight = this->DefaultRowHeight;

    NewRect = this->BoxRect(0, FTitleOffset, ColCount - 1, 1000);
    ScrollWindowEx(this->Handle, 0, - RowHeight * SDistance, &NewRect, &NewRect, 0, NULL, 1);
    MoveColRow(Col, newRow, false, false);
    this->EndUpdate();

}

/* Количество и сумма по столбцу всего */
TSumResult __fastcall TDBGridAlt::getSum(const String& fieldName, const String& expression, bool filtered)
{
    //int topRow = this->TopRow;
    //void* bookmark = DataSource->DataSet->GetBookmark();
    //AnsiString bookmark = DataSource->DataSet->Bookmark;
    //int MyActiveRec = DataSource->DataSet->ActiveRecord;
    if ( !this->DataLink->Active || this->recordCount == 0 /*_dataSet == NULL || !_dataSet->Active*/)
    {
        return std::pair<unsigned int, double> (0, 0);
    }

    //ShowMessage(this->recordCount);

    _dataSet->DisableControls();

    int N = _dataSet->RecNo;

    int oldRow = this->Row; // Запоминаем позицию скрола

    //this->BeginUpdate();

    String oldFilter = _dataSet->Filter;
    bool oldFiltered = _dataSet->Filtered;

    if (filtered)
    {
        if (expression != "")
        {
            if (_dataSet->Filter != "")
            {
                _dataSet->Filter = "(" + _dataSet->Filter + ") AND (" + expression + " )";
            }
            else
            {
                _dataSet->Filter = expression;
            }

            _dataSet->Filtered = true;
        }
    }
    else
    {
        _dataSet->Filter = expression;
        _dataSet->Filtered = true;
    }


    _dataSet->First();

    double sum = 0;
    unsigned int count = 0;

    if (fieldName != "")
    {
        TField* field = _dataSet->FieldByName(fieldName);
        while( !_dataSet->Eof )
        {
            sum += field->AsFloat;
            _dataSet->Next();
        }
    }
    else
    {
        sum = 0;
    }
    count = _dataSet->RecordCount;

    // Возвращаем прежнее состояние
    _dataSet->Filter = oldFilter;
    _dataSet->Filtered = oldFiltered;
    _dataSet->RecNo = N;
    _dataSet->EnableControls();

    ScrollActiveToRow(oldRow);

    return std::pair<unsigned int, double> (count, sum);
}






// Сумма по столбцу с учетом фильтра
// Сумма по столбцу с учетом выделенных


/* Задает поле для хранения данных об отметке записи */
void __fastcall TDBGridAlt::SetCheckDataFieldName(const String value)
{
    _checkDataFieldName = value;
}


/* Задает поля для сортировки по умолчанию */
void __fastcall TDBGridAlt::SetDefaultSortFieldName(const String value)
{
    FDefaultSortFieldName = value;
    /*if (_dataSet != NULL && _dataSet->Active)
    {
    } */
}

//------------------------------------------------------------------------------
// Объединяет вектор подстрок в одну строку используя соединитель
String TDBGridAlt::MergeStr(const String& s1, const String& s2, const String &glue)
{

	if (s1 != "" && s2 != "")
	{
		return s1 + glue + s2;
	}
 	else
	{
		return s1 == "" ? s2 : s1;
	}
}
