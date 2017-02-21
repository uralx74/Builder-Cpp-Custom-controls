//---------------------------------------------------------------------------

#ifndef EditAltH
#define EditAltH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
typedef enum {edtInteger, edtUnsignedInteger, edtFloat, edtUnsignedFloat} TEditDataType;
//---------------------------------------------------------------------------
class PACKAGE TEditAlt : public TEdit
{
private:
    DYNAMIC void __fastcall KeyPress(char &Key);
    DYNAMIC void __fastcall Change();

    String _textOld;
    int _selStartOld;
    bool _changeFromCode;
    bool FAllowNull;

protected:
    bool FUseDot;      // Использовать разделитель
    bool FUseSign;      // Использовать знаки + -

    TEditDataType FEditDataType;
	void __fastcall SetDataType(TEditDataType EditDataType);

public:
    __fastcall TEditAlt(TComponent* Owner);
   bool __fastcall IsNumber(String Value, bool bFloat, bool bSign);
   int __fastcall GetValue();
   void __fastcall SetValue(int value);
   bool __fastcall IsNull();
   void __fastcall ClearValue();


__published:
    __property TEditDataType EditDataType = {read = FEditDataType, write = SetDataType, default = edtInteger};
    __property int Value = {read=GetValue, write=SetValue};
    __property bool AllowNull = {read=FAllowNull, write=FAllowNull};

};
//---------------------------------------------------------------------------
#endif
