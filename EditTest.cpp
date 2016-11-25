//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "EditTest.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TEditTest *)
{
        new TEditTest(NULL);
}
//---------------------------------------------------------------------------
__fastcall TEditTest::TEditTest(TComponent* Owner)
        : TEdit(Owner)
{
}
//---------------------------------------------------------------------------
namespace Edittest
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TEditTest)};
                 RegisterComponents("AltCtrl", classes, 0);
        }
}
//---------------------------------------------------------------------------
 