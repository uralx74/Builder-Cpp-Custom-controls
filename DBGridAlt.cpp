
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
         RegisterComponents("Alt", classes, 0);
    }
}

//---------------------------------------------------------------------------
//
__fastcall TDBGridAlt::TDBGridAlt(TComponent* Owner)
    : TDBGrid(Owner)
{
    FOddRowColor = RGB(250,250,250);
    FEvenRowColor = clWhite;
    FSortColumnColor=RGB(255,240,210);
    Options = TDBGridOptions() << dgRowSelect << dgTitles << dgRowLines << dgColLines << dgColumnResize << dgEditing << dgConfirmDelete << dgCancelOnExit << dgIndicator << dgTabs;
    FCheckedFont = new TFont();
    FCheckedFont->Color = clRed;
    FCheckedFont->Style = TFontStyles();
    FHideKeyField = true;
    //FKeyFieldIndex = 1;
    FColumnAutosize = true;

    Filter = new TDBGridAltFilter();
}

//---------------------------------------------------------------------------
//
__fastcall TDBGridAlt::~TDBGridAlt()
{
    delete FCheckedFont;
    ItemsFlg.clear();
}


//---------------------------------------------------------------------------
// Событие при открытии DataSet
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
        for(int i=0; i <= DataSource->DataSet->RecordCount; i++) {
            ItemsFlg.push_back(false);
            //DataSource->DataSet->Fields->FieldByNumber(1)->Tag = i;
            //DataSource->DataSet->Next();
        }
        DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->Visible = !FHideKeyField;
        if (FColumnAutosize)
            SetAutosize();

        //Refresh();
        //ShowMessage( IntToStr(DataSource->DataSet->RecordCount) + " + " + IntToStr(ItemsFlg.size()) + " + " + IntToStr(ItemsFlg[0]));
    } else {
        for (int i = 0; i < Columns->Count; i++)
            Columns->Items[i]->Title->Font->Style = Columns->Items[i]->Title->Font->Style >> fsBold;
    }

    TDBGrid::LinkActive(Value);
}
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
    TPoint pt;
    pt = Mouse->CursorPos;
    pt = ScreenToClient (pt);

    int RowHeight = RowHeights[0];

    if (pt.y > RowHeight ) {    // Если двойной щелчек не по заголовку
        InvertCheck(DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger);
        Refresh();
    } else {     // Если двойной щелчек по заголовку
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
        InvertCheck(DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger);
    	//DblClick();
        break;
    case VK_INSERT:
    	// Помечаем выделенную строку
    	//DblClick();
        InvertCheck(DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger);
        // Перемещаем курсор на следующую строку в Гриде
        TDBGrid::KeyDown(VK_DOWN, Shift);
        //DataSource->DataSet->Next();
        break;
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

    if (SortColumnIndex == Column->Index) {
        if (SortType == 2) {
            SortType = 0;
            SortColumnIndex = -1;
            Column->Title->Font->Style = Column->Title->Font->Style >> fsBold;
        }
        else {
            SortType++;
            //Column->Title->Font->Style = Column->Title->Font->Style << fsBold;
        }
    } else {
        if (SortColumnIndex >= 0)
            Columns->Items[SortColumnIndex]->Title->Font->Style = Columns->Items[SortColumnIndex]->Title->Font->Style >> fsBold;
        Column->Title->Font->Style = Column->Title->Font->Style << fsBold;
        SortColumnIndex = Column->Index;
        SortType = 1;
    }

    AnsiString sSort;

    switch(SortType) {
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
	if ( (DataSource->DataSet->RecNo % 2 ) == 0) {
    	Canvas->Brush->Color = FOddRowColor;
    } else {
    	Canvas->Brush->Color = FEvenRowColor;
    }

    // Расскрашиваем столбец, по которому проведена сортировка
    if (SortType != 0 && Column->Index == SortColumnIndex) {
        Canvas->Brush->Color = FSortColumnColor;
    }

    // Оставляем выделение текущей позиции курсора
	if  (State.Contains(gdSelected)) {
        Canvas->Brush->Color= clHighlight;
        Canvas->Font->Color = clHighlightText;
	}

    // Выделяем цветом помеченные строки
    if (ItemsFlg[DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger]) {
        Canvas->Font = FCheckedFont;
    }
    Canvas->Unlock();    // Освобождаем канвас

	DefaultDrawColumnCell(Rect,DataCol,Column,State);
}

//---------------------------------------------------------------------------
// Параметр Спрятать ключевое поле
void __fastcall TDBGridAlt::SetVisibleKeyField(bool VisibleFlg)
{
    FHideKeyField = VisibleFlg;
}

//---------------------------------------------------------------------------
//
void __fastcall TDBGridAlt::SetColumnAutosize(bool ColumnAutosizeFlg)
{
    FColumnAutosize = ColumnAutosizeFlg;
    if (Columns->Count > 0 && ColumnAutosizeFlg)
        SetAutosize();
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
bool __fastcall TDBGridAlt::IsChecked(int RowNum)
{
    return ItemsFlg[RowNum];
}

//---------------------------------------------------------------------------
// Пометить все строки
void __fastcall TDBGridAlt::CheckAll()
{
    for(int i=1; i < ItemsFlg.size(); i++) {
        ItemsFlg[i] = true;
    }
    Refresh();
    if (FOnChangeCheck != NULL)
        FOnChangeCheck(this);
}

//---------------------------------------------------------------------------
// Пометить все строки
void __fastcall TDBGridAlt::CheckFiltered()
{
    int N = DataSource->DataSet->RecNo;
    DataSource->DataSet->DisableControls();

    DataSource->DataSet->First();
    while(!DataSource->DataSet->Eof) {
        ItemsFlg[DataSource->DataSet->Fields->FieldByName(FKeyFieldName)->AsInteger] = true;
        DataSource->DataSet->Next();
    }


    DataSource->DataSet->RecNo = N;
    DataSource->DataSet->EnableControls();
    Refresh();

    if (FOnChangeCheck != NULL)
        FOnChangeCheck(this);
}

//---------------------------------------------------------------------------
// Снять пометку со всех строк
void __fastcall TDBGridAlt::UncheckAll()
{
    for(int i=1; i < ItemsFlg.size(); i++) {
        ItemsFlg[i] = false;
    }
    Refresh();
    if (FOnChangeCheck != NULL)
        FOnChangeCheck(this);
}

//---------------------------------------------------------------------------
// Подсчет количества отмеченных строк
int __fastcall TDBGridAlt::GetCheckedCount()
{
    int Count = 0;
    for(int i=1; i < ItemsFlg.size(); i++) {
        if (ItemsFlg[i])
            Count++;
    }
    return Count;
}

//---------------------------------------------------------------------------
// Инверсия выделения пункта
void __fastcall TDBGridAlt::InvertCheck(int Index)
{
    ItemsFlg[Index] = !ItemsFlg[Index];
    if (FOnChangeCheck != NULL)
        FOnChangeCheck(this);
}

//---------------------------------------------------------------------------
// Установка ширины столбцов
void __fastcall TDBGridAlt::SetAutosize()
{
    int ScrollWidth = GetSystemMetrics(SM_CXVSCROLL);       // Ширина вертикального скроллбара

    // Подсчитываем ширину видимых столбцов
    int AllWidth = 0;
    int VisibleColumnsCount = 0;        // Это значение будем использовать для подсчета ширины границы между столбцами
    for (int i = 0; i < Columns->Count; i++) {
        if (Columns->Items[i]->Visible) {
            AllWidth += Columns->Items[i]->Width;
            VisibleColumnsCount++;
        }
    }

    float k = float(ClientWidth - ScrollWidth - VisibleColumnsCount)/AllWidth;  // Коэффициент

    for (int i = 0; i < Columns->Count; i++) {
        if (Columns->Items[i]->Visible) {
            int r1 = Columns->Items[i]->Width;
            Columns->Items[i]->Width = ceil(Columns->Items[i]->Width * k);
        }
    }
}

//---------------------------------------------------------------------------
//
void TDBGridAltFilter::Add(String FilterName, String FilterStr)
{
    (*Items)[FilterName] = FilterStr;
}

//---------------------------------------------------------------------------
//
void TDBGridAltFilter::Delete(String FilterName)
{
    Items->erase(FilterName);
}

//---------------------------------------------------------------------------
//
void TDBGridAltFilter::Clear()
{
    Items->clear();
}

//---------------------------------------------------------------------------
//
__fastcall TDBGridAltFilter::TDBGridAltFilter()
{
    Items = new std::map<String, String>;
}

//---------------------------------------------------------------------------
//
__fastcall TDBGridAltFilter::~TDBGridAltFilter()
{
    Items->clear();
    delete Items;
}

//------------------------------------------------------------------------------
// Объединяет вектор подстрок в одну строку используя соединитель
//String GetFilterString(const vector<String> &pieces, const String &glue = "")
String TDBGridAltFilter::GetFilterString(const String &glue)
{
	String a = "";
	int leng=Items->size();
    if (Items->size() > 0) {
        std::map<String, String>::iterator it = Items->begin();
        a +=(it->second);
        it++;
        for (; it !=  Items->end(); it++) {
            a+= glue + it->second;
        }
    }

 	return a;
}

//---------------------------------------------------------------------------
//
void __fastcall TDBGridAlt::SetFiltered(bool Value)
{
    if (FFiltered != Value) {
        FFiltered = Value;

        if ((DataSource == NULL) || (DataSource->DataSet == NULL))
            return;

        if (Value && Filter->Items->size() > 0) {
            DataSource->DataSet->Filter = Filter->GetFilterString(" AND ");

        } else {
            DataSource->DataSet->Filter = "";
            //Value = false;
            //FFiltered = false;
        }
        DataSource->DataSet->Filtered = Value;
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
