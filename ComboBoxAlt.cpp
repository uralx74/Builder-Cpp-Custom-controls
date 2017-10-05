//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "ComboBoxAlt.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TComboBoxAlt *)
{
    new TComboBoxAlt(NULL);
}
//---------------------------------------------------------------------------
__fastcall TComboBoxAlt::TComboBoxAlt(TComponent* Owner)
    : TComboBox(Owner)
{
}

 void __fastcall TComboBoxAlt::DestroyWnd(void)
{
    for(TSourceList::iterator it=_sourceList.begin(); it != _sourceList.end(); it++)
    {
        ClearItems(it->ds);
    }
}


__fastcall TComboBoxAlt::~TComboBoxAlt()
{
}

//---------------------------------------------------------------------------
namespace Comboboxalt
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TComboBoxAlt)};
         RegisterComponents("AltCtrl", classes, 0);
    }
}
//---------------------------------------------------------------------------
/* ��������� ������ �� dataset � ������
   ������� ��� ���������� �������������
*/
//void __fastcall TComboBoxAlt::AddItems(TDataSet* ds, String DisplayField, String ValueField, bool bShowValue)
void __fastcall TComboBoxAlt::AddItems(TSourceItem* SourceItem)
{
    SourceItem->ds->DisableControls();
    SourceItem->ds->First();
    while( !SourceItem->ds->Eof )
    {
        TListValue* lv = new TListValue(SourceItem->ds,
            SourceItem->ds->FieldByName(SourceItem->ValueField)->AsString,
            SourceItem->ds->FieldByName(SourceItem->DisplayField)->AsString );

        if ( SourceItem->bShowValue )
        {
            this->Items->AddObject(

            SourceItem->ds->FieldByName(SourceItem->DisplayField)->AsString + " (" +
                SourceItem->ds->FieldByName(SourceItem->ValueField)->AsString + ")", (TObject*)lv);
        }
        else
        {
            this->Items->AddObject(SourceItem->ds->FieldByName(SourceItem->DisplayField)->AsString, (TObject*)lv);
        }

        SourceItem->ds->Next();
    }
    SourceItem->ds->EnableControls();

}

/*
  ������� ������
  ������� ��� ���������� �������������
*/
void __fastcall TComboBoxAlt::ClearItems(TDataSet* ds)
{
    for(int i = this->Items->Count-1; i >= 0; i--)
    {
        if ( this->Items->Objects[i] != NULL && ((TListValue*)this->Items->Objects[i])->ds == ds)
        {
            delete ((TListValue*)this->Items->Objects[i]);
            this->Items->Delete(i);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TComboBoxAlt::AddSource(TDataSet* ds, String DisplayField, String ValueField, bool bShowValue)
{

    // ���������, �� ��������� �� �������� �����
    for(TSourceList::iterator it=_sourceList.begin(); it != _sourceList.end(); it++)
    {
        if ( it->ds == ds)
        {
            return;
        }
    }

    _sourceList.push_back( TSourceItem(ds, DisplayField, ValueField, bShowValue) );

    // ��������� � ComboBox
    //AddItems(ds, DisplayField, ValueField);
    AddItems(&_sourceList.back());
}


/*
  ��������� ������ � ������������ � �����������
*/
void __fastcall TComboBoxAlt::RefreshItems()
{
    for(TSourceList::iterator it=_sourceList.begin(); it != _sourceList.end(); it++)
    {
        ClearItems(it->ds);
        //AddItems(it->ds, it->DisplayField, it->ValueField, it->bShowValue);
        AddItems(it);
    }
}

/*
*/
void __fastcall TComboBoxAlt::DeleteSource(TDataSet* ds)
{
    ClearItems(ds); // ������� ������

    for(TSourceList::iterator it=_sourceList.begin(); it != _sourceList.end(); it++)
    {
        if ( it->ds == ds)  // ������� �� ����������
        {
            _sourceList.erase(it);
            break;
        }
    }
}

/* ���������� ������� ������� */
TListValue* __fastcall TComboBoxAlt::FGetCurrentItem()
{
    return this->ItemIndex>=0 ? (TListValue*)this->Items->Objects[this->ItemIndex] : NULL;
}

/* ���������� ������� �������� value �������� */
String __fastcall TComboBoxAlt::FGetValue()
{
    if (this->ItemIndex >= 0 && this->Items->Objects[this->ItemIndex] != NULL)
    {
        return ((String)((TListValue*)this->Items->Objects[this->ItemIndex])->value);
    }
    else
    {
        return "";
    }
}

/* ���������� �������� ����� �������� */
String __fastcall TComboBoxAlt::FGetText()
{
    if (this->ItemIndex >= 0 && this->Items->Objects[this->ItemIndex] != NULL)
    {
        return ((String)((TListValue*)this->Items->Objects[this->ItemIndex])->text);
    }
    else
    {
        return "";
    }
}

