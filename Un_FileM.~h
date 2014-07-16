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
        TPopupMenu *PopupMenu1;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *N9;
        TMenuItem *N4;
        TListView *Lv2;
        TPopupMenu *PopupMenu2;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N7;
        TMenuItem *MenuItem4;
        TMenuItem *N8;
        TButton *BtHome1;
        TButton *BtHome2;
        TEdit *Ed1;
        TButton *BtS1;
        TEdit *Ed2;
        TButton *BtS2;
        TPopupMenu *PopupMenu3;
        TMenuItem *ToPath1;
        TLabel *Lb1;
        TLabel *Lb2;
        TPopupMenu *PopupMenu4;
        TMenuItem *ToPath2;
        void __fastcall ListDblClick(TObject *Sender);
        void __fastcall DiskList(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FileList(TObject *Sender);
        void __fastcall Copy(TObject *Sender);
        void __fastcall Paste(TObject *Sender);
        void __fastcall Del(TObject *Sender);
        void __fastcall Lv(TObject *Sender);
        void __fastcall Cut(TObject *Sender);
        void __fastcall Ident(int imInd, AnsiString capt);
        void __fastcall PopupMenu(TObject *Sender);
        void __fastcall PopupMenuS(TObject *Sender);
        void __fastcall DelDir(AnsiString Dir);
        void __fastcall PopupIdent(TObject *Sender, TPopupMenu *Popup, int fl);
        void __fastcall PopupIdent(TObject *Sender, TPopupMenu *Popup);
        void __fastcall PopupEnable(TObject *Sender, bool ct, bool cp, bool pt, bool dt);
        void __fastcall Home(TObject *Sender);
        void __fastcall SearchIdent(TObject *Sender);
        void __fastcall Search(TObject *Sender);
        void __fastcall Find(AnsiString p, AnsiString t);
        void __fastcall Ed(TObject *Sender);
        void __fastcall ToPathClick(TObject *Sender);
        void __fastcall LvKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        TListView *LV;
        TListColumn  *NewColumn;
        TListItem  *ListItem;
        TSearchRec sr;
        AnsiString path1, path2, cpPath, file, ctPath, tmp;
        TStringList *LDir, *LFile, *LExt, *LSize, *LPath, *LSearchD, *LSearchF;
        int fl1, fl2, flCut, flPaste, ctImInd;
        WORD K;

        __fastcall TFr_Main(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFr_Main *Fr_Main;
//---------------------------------------------------------------------------
#endif
