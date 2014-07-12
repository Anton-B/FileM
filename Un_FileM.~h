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
        TImageList *ImgL1;
        TMainMenu *MainMenu1;
        TMenuItem *MenuFileHome;
        TPopupMenu *PopupMenu1;
        TMenuItem *NCut;
        TMenuItem *NCopy;
        TMenuItem *NPaste;
        TMenuItem *N9;
        TMenuItem *NDel;
        void __fastcall Lv1DblClick(TObject *Sender);
        void __fastcall DiskList(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FileList(TObject *Sender);
        void __fastcall Copy(TObject *Sender);
        void __fastcall Paste(TObject *Sender);
        void __fastcall Del(TObject *Sender);
        void __fastcall Cut(TObject *Sender);
        void __fastcall Ident(int imInd, AnsiString capt);
        void __fastcall PopupMenu1Popup(TObject *Sender);
private:	// User declarations
public:		// User declarations
        TListColumn  *NewColumn;
        TListItem  *ListItem;
        TSearchRec sr;
        AnsiString dl, path, cpPath, file, ctPath, tmp;
        TStringList *LDir, *LFile, *LExt, *LSize, *LPath;
        int fl, flCut, ctImInd;

        __fastcall TFr_Main(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFr_Main *Fr_Main;
//---------------------------------------------------------------------------
#endif
