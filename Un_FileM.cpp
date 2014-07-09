//---------------------------------------------------------------------------

#include <vcl.h>
#include <fstream.h>
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
  LDir=new TStringList;
  LFile=new TStringList;
  DiskList(Owner);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::DiskList(TObject *Sender)
{
  Lv1->Clear();
  LDir->Clear();
  LDir->Clear();
  path="";
  int BufferSize = GetLogicalDriveStrings(0, NULL);
  char *Buffer = new char[BufferSize];
  TStringList *DiskList = new TStringList;
  GetLogicalDriveStrings(BufferSize, Buffer);
  int DiskCount = (BufferSize - 1) / 4 ;
  for(int i = 0; i < DiskCount; i++)
  {
    char DiskString[4];
    CopyMemory(DiskString, (Buffer + i * 4), 4);
    ListItem = Lv1->Items->Add();
    ListItem->Caption=DiskString;
    ListItem->ImageIndex=1;
  }
  delete [] Buffer;
  delete DiskList;
  fl=0;
}
//---------------------------------------------------------------------------


void __fastcall TFr_Main::Lv1DblClick(TObject *Sender)
{
  if ((fl==-1)&&(((TListView*)Sender)->Selected->Caption=="<--"))
  {
    DiskList(Owner);
    fl=0;
    return;
  }
  if (((TListView*)Sender)->Selected->Caption=="<--")
    path+="..\\";
  else
  {
    if (fl==0)
    {
      path=((TListView*)Sender)->Selected->Caption;
    }
    else
      path+=((TListView*)Sender)->Selected->Caption+"\\";
  }
  fl=1;
  cD=0;
  cF=0;
  if (FindFirst(path+"\*.*", faAnyFile, sr) == 0)
  {
    do
    {
      if (sr.Attr & faDirectory)
      {
        if (sr.Name!=".")
        {
          if (sr.Name=="..")
            sr.Name="<--";
          LDir->Add(sr.Name);
          cD++;
        }
        /*if (sr.Name!="..")
        {        }*/
      }
      else
      {
        LFile->Add(sr.Name);
        cF++;
        /*AnsiString Ext=ExtractFileExt(sr.Name).UpperCase();
        if (Ext==".cpp")
          List->Add(path+sr.Name);*/
      }
    } while (FindNext(sr) == 0);
    FindClose(sr);
  }
  Lv1->Clear();
  for (int i=0;i<cD;i++)
  {
    if ((i==0)&&(LDir->Strings[i]!="<--"))
    {
      //ShowMessage(LDir->Strings[i]);
      ListItem = Lv1->Items->Add();
      ListItem->Caption = "<--";
      ListItem->ImageIndex=0;
      i=1;
      fl=-1;
    }
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
  LDir->Clear();
  LFile->Clear();
  Application->ProcessMessages();
}
//---------------------------------------------------------------------------

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
