//---------------------------------------------------------------------------

#include <vcl.h>
#include <fstream.h>
#include <sys\stat.h>
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
  LExt=new TStringList;
  LSize=new TStringList;
  DiskList(Owner);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::DiskList(TObject *Sender)
{
  Lv1->Clear();
  LDir->Clear();
  LDir->Clear();
  LExt->Clear();
  LSize->Clear();
  if (Lv1->Columns->Count>2)
  {
    Lv1->Columns->Delete(2);
  }
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
    
    int DriveType = GetDriveType(DiskString);
    switch(DriveType)
    {
    case DRIVE_UNKNOWN:
      ListItem->SubItems->Add("����������");
      break;
    case DRIVE_NO_ROOT_DIR:
      ListItem->SubItems->Add("�����������");
      break;
    case DRIVE_REMOVABLE:
      ListItem->SubItems->Add("�������");
      break;
    case DRIVE_FIXED:
      ListItem->SubItems->Add("�������");
      break;
    case DRIVE_REMOTE:
      ListItem->SubItems->Add("�������");
      break;
    case DRIVE_CDROM:
      ListItem->SubItems->Add("CD-ROM");
      break;
    case DRIVE_RAMDISK:
      ListItem->SubItems->Add("RAM");
      break;
    default:
      ListItem->SubItems->Add("����������");
      break;
    }
  }
  delete [] Buffer;
  delete DiskList;
  fl=0;
}
//---------------------------------------------------------------------------


void __fastcall TFr_Main::Lv1DblClick(TObject *Sender)
{
  if (((TListView*)Sender)->Selected->ImageIndex==-1)
  {
    ShellExecute(Handle, "open",((TListView*)Sender)->Selected->Caption.c_str(),NULL,path.c_str(),SW_SHOWNORMAL);
    return;
  }
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
        }
      }
      else
      {
        LFile->Add(sr.Name);
        LExt->Add(ExtractFileExt(sr.Name).UpperCase());
        struct stati64 statbuf;
        AnsiString p=path+sr.Name;
        _stati64(p.c_str(), &statbuf);
        if (statbuf.st_size>=1073741824)
          LSize->Add(FloatToStr(statbuf.st_size/1073741824)+" ��");
        else if (statbuf.st_size>=1048576)
          LSize->Add(FloatToStr(statbuf.st_size/1048576)+" ��");
        else if (statbuf.st_size>=1024)
          LSize->Add(FloatToStr(statbuf.st_size/1024)+" ��");
        else if (statbuf.st_size<1024)
          LSize->Add(FloatToStr(statbuf.st_size)+" �");
      }
    } while (FindNext(sr) == 0);
    FindClose(sr);
  }
  Lv1->Clear();
  if (Lv1->Columns->Count<=2)
  {
    NewColumn=Lv1->Columns->Add();
    NewColumn->Caption="������";
    NewColumn->Alignment=taCenter;
    NewColumn->Width=Lv1->Column[1]->Width;
  }
  for (int i=0;i<LDir->Count;i++)
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
    ListItem->SubItems->Add("�����");
    ListItem->ImageIndex=0;
  }
  for (int i=0;i<LFile->Count;i++)
  {
    ListItem = Lv1->Items->Add();
    ListItem->Caption = LFile->Strings[i];
    ListItem->SubItems->Add(LExt->Strings[i]);
    ListItem->SubItems->Add(LSize->Strings[i]);
    ListItem->ImageIndex=-1;
  }
  LDir->Clear();
  LFile->Clear();
  LExt->Clear();
  LSize->Clear();
  Application->ProcessMessages();
}
//---------------------------------------------------------------------------

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





