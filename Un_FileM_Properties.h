//---------------------------------------------------------------------------

#ifndef Un_FileM_PropertiesH
#define Un_FileM_PropertiesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrProperties : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TEdit *EdName;
        TEdit *EdAddress;
        TEdit *EdSize;
        TEdit *EdCreate;
        TEdit *EdChange;
        TEdit *EdOpen;
        TShape *Shape1;
        TShape *Shape2;
        TShape *Shape3;
        TCheckBox *CBDir;
        TCheckBox *CBReadOnly;
        TCheckBox *CBSysFile;
        TCheckBox *CBHidden;
        TCheckBox *CBVolumeID;
        TCheckBox *CBAnyFile;
private:	// User declarations
public:		// User declarations
        __fastcall TFrProperties(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrProperties *FrProperties;
//---------------------------------------------------------------------------
#endif
