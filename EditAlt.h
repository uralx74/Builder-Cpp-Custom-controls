/*
  TEditAlt
  јвтор: vsovchinnikov
  —оздан: 2017-02-28

  Ёлемент управлени€ дл€ редактировани€ числовых значений
  
*/

#ifndef EditAltH
#define EditAltH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
typedef enum {
    edtInteger,
    edtUnsignedInteger,
    edtFloat,
    edtUnsignedFloat
} TEditDataType;

//---------------------------------------------------------------------------
class PACKAGE TEditAlt : public TEdit
{
private:
    String _textOld;
    int _selStartOld;
    bool FAllowNull;
    bool _allowChange;
    bool checkValue(const String& value) const;


protected:
    bool FUseDot;      // »спользовать разделитель
    bool FUseSign;      // »спользовать знаки + -

    TEditDataType FEditDataType;
	void __fastcall SetDataType(TEditDataType EditDataType);
    void __fastcall MyWndProc(Messages::TMessage &Message);

public:
    __fastcall TEditAlt(TComponent* Owner);
   bool __fastcall IsNumber(const String& Value, bool bFloat, bool bSign) const;
   int __fastcall GetValue();
   void __fastcall SetValue(int value);
   bool __fastcall IsNull();
   void __fastcall ClearValue();

__published:
    __property TEditDataType EditDataType = {read = FEditDataType, write = SetDataType, default = edtInteger};
    __property int Value = {read=GetValue, write=SetValue};
    __property bool AllowNull = {read=FAllowNull, write=FAllowNull};
    __property bool AllowDot = {read=FUseDot, write=FUseDot};
    __property bool AllowSign = {read=FUseSign, write=FUseSign};

};
//---------------------------------------------------------------------------
#endif
