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

    String TextOld;
    int SelStartOld;

protected:
    bool FUseDot;      // Использовать разделитель
    bool FUseSign;      // Использовать знаки + -

    TEditDataType FEditDataType;
	void __fastcall SetDataType(TEditDataType EditDataType);

public:
    __fastcall TEditAlt(TComponent* Owner);
   bool __fastcall IsNumber(String Value, bool bFloat, bool bSign);
   int __fastcall GetValue();
   __fastcall SetValue(int value);


__published:
    __property TEditDataType EditDataType = {read = FEditDataType, write = SetDataType, default = edtInteger};
    __property int AsInteger = {read=GetValue, write=SetValue};

};
//---------------------------------------------------------------------------
#endif
