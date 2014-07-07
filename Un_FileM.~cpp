//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Un_FileM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFr_Main *Fr_Main;
//---------------------------------------------------------------------------
__fastcall TFr_Main::TFr_Main(TComponent* Owner)
        : TForm(Owner)
{
  path="D:\\";
  cD=0;
  cF=0;
  LDir=new TStringList;
  LFile=new TStringList;
  if (FindFirst(path+"*.*", faAnyFile, sr) == 0)
  {
    do
    {
      if (sr.Attr & faDirectory)
      {
        LDir->Add(sr.Name);
        cD++;
        //ShowMessage("Dir:\n"+sr.Name);
        /*if (sr.Name!=".")
        if (sr.Name!="..")
        {        }*/
      }
      else
      {
        LFile->Add(sr.Name);
        cF++;
        //ShowMessage("File:\n"+sr.Name);
        /*AnsiString Ext=ExtractFileExt(sr.Name).UpperCase();
        if (Ext==".cpp")
          List->Add(path+sr.Name);*/

      }

    } while (FindNext(sr) == 0);
    FindClose(sr);  
  }
  Lv1->SmallImages=ImgL1;
  for (int i=0;i<cD;i++)
  {
    ListItem = Lv1->Items->Add();
    ListItem->Caption = LDir->Strings[i];
    ListItem->ImageIndex=0;
  }
  for (int i=0;i<cF;i++)
  {
    ListItem = Lv1->Items->Add();
    ListItem->Caption = LFile->Strings[i];
    ListItem->ImageIndex=-1;
  }
  Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::ListFiles(AnsiString path, TStrings* List)
{
  /*TSearchRec sr;
  if (FindFirst(path+"*.*", faAnyFile, sr) == 0)
  {
    do
    {
      if (sr.Attr & faDirectory)
      {
        ShowMessage("Dir:\n"+sr.Name);
        /*if (sr.Name!=".")
        if (sr.Name!="..")
        {

        }*/
      /*}
      else
      {
        ShowMessage("File:\n"+sr.Name);
        /*AnsiString Ext=ExtractFileExt(sr.Name).UpperCase();
        if (Ext==".cpp")
          List->Add(path+sr.Name);*/

    /*  }
    } while (FindNext(sr) == 0);
    FindClose(sr);
  }  */
  
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Button1Click(TObject *Sender)
{
  //ListFiles("D:\\",Memo1->Lines);
  /*const char Names[6][2][10] =
   {{"Rubble","Barny"},
    {"Michael", "Johnson"},
    {"Bunny", "Bugs"},
    {"Silver", "HiHo"},
    {"Simpson", "Bart"},
    {"Squirrel", "Rockey"}};
 
  TListColumn  *NewColumn;
  TListItem  *ListItem;
  TListView   *ListView = new TListView(this);
 
  ListView->Parent = this;
  ListView->Align = alClient;
  ListView->ViewStyle = vsReport;
  NewColumn = ListView->Columns->Add();
  NewColumn->Caption = "Last";

  NewColumn = ListView->Columns->Add();
  NewColumn->Caption = "First";
  for (int i = 0; i < 6; i++)
  {
    ListItem = ListView->Items->Add();
    ListItem->Caption = Names[i][0];
    ListItem->SubItems->Add(Names[i][1]);
  }*/
  

  Application->ProcessMessages();
}
//---------------------------------------------------------------------------



