//---------------------------------------------------------------------------

#ifndef Un_FileMH
#define Un_FileMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TFr_Main : public TForm
{
__published:	// IDE-managed Components
        TListView *Lv1;
        TButton *Button1;
        TImageList *ImgL1;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Lv1DblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        TListColumn  *NewColumn;
        TListItem  *ListItem;
        TSearchRec sr;
        AnsiString dl, path; // �������� ������ �����;
        TStringList *LDir, *LFile,*LPath;
        int cD,cF,fl;
        __fastcall TFr_Main(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFr_Main *Fr_Main;
//---------------------------------------------------------------------------
#endif
