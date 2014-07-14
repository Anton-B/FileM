//---------------------------------------------------------------------------

#ifndef Un_FileM_SearchH
#define Un_FileM_SearchH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFr_Search : public TForm
{
__published:	// IDE-managed Components
        TEdit *Ed1;
        TLabel *Label1;
        TButton *BtSearch;
private:	// User declarations
public:		// User declarations
        __fastcall TFr_Search(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFr_Search *Fr_Search;
//---------------------------------------------------------------------------
#endif
