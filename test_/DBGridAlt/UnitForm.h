//---------------------------------------------------------------------------

#ifndef UnitFormH
#define UnitFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Ora.hpp"
#include <DB.hpp>
#include "DBGridAlt.h"
#include <DBGrids.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TOraSession *TestSession;
    TOraQuery *TestQuery;
    TOraDataSource *TestDataSource;
    TDBGridAlt *DBGridAlt1;
    TButton *Button1;
    TButton *GetSum;
    TStringField *TestQueryACCT_ID;
    TStringField *TestQueryPOSTAL;
    TStringField *TestQueryADDRESS;
    TStringField *TestQueryCITY;
    TStringField *TestQueryULITSA;
    TStringField *TestQueryDOM;
    TStringField *TestQueryKORP;
    TStringField *TestQueryKVARTIRA;
    TStringField *TestQueryFIO;
    TDateTimeField *TestQueryACCT_SETUP_DT;
    TDateTimeField *TestQuerySA_END_DT;
    TStringField *TestQueryPREM_TYPE_CD;
    TIntegerField *TestQueryKOL_PROP;
    TFloatField *TestQuerySQUARE;
    TStringField *TestQueryPHONES;
    TStringField *TestQueryACCT_OTDELEN;
    TStringField *TestQueryALERT_INFO;
    TStringField *TestQueryPRNT_PREM_TYPE_CD;
    TFloatField *TestQueryCHECK_DATA;
    TFloatField *TestQueryROWNUM;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall GetSumClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
