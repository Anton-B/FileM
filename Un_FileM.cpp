//---------------------------------------------------------------------------

#include <vcl.h>
#include <fstream.h>
#include <sys\stat.h>
#include <clipbrd.hpp>
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
  LPath=new TStringList;
  if (FileExists("fm.fm"))
  {
    LPath->LoadFromFile("fm.fm");
    path=LPath->Strings[0];
    FileList(Owner);
  }
  else
    DiskList(Owner);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::DiskList(TObject *Sender)
{
  MenuEdit->Enabled=false;
  Lv1->Clear();
  LDir->Clear();
  LDir->Clear();
  LExt->Clear();
  LSize->Clear();
  if (Lv1->Columns->Count>2)
    Lv1->Columns->Delete(2);
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
      ListItem->SubItems->Add("Неизвестен");
      break;
    case DRIVE_NO_ROOT_DIR:
      ListItem->SubItems->Add("Отсутствует");
      break;
    case DRIVE_REMOVABLE:
      ListItem->SubItems->Add("Сменный");
      break;
    case DRIVE_FIXED:
      ListItem->SubItems->Add("Жесткий");
      break;
    case DRIVE_REMOTE:
      ListItem->SubItems->Add("Сетевой");
      break;
    case DRIVE_CDROM:
      ListItem->SubItems->Add("CD-ROM");
      break;
    case DRIVE_RAMDISK:
      ListItem->SubItems->Add("RAM");
      break;
    default:
      ListItem->SubItems->Add("Неизвестен");
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
  MenuEdit->Enabled=true;
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
  FileList(Owner);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::FileList(TObject *Sender)
{
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
          LSize->Add(FloatToStr(statbuf.st_size/1073741824)+" ГБ");
        else if (statbuf.st_size>=1048576)
          LSize->Add(FloatToStr(statbuf.st_size/1048576)+" МБ");
        else if (statbuf.st_size>=1024)
          LSize->Add(FloatToStr(statbuf.st_size/1024)+" КБ");
        else if (statbuf.st_size<1024)
          LSize->Add(FloatToStr(statbuf.st_size)+" Б");
      }
    } while (FindNext(sr) == 0);
    FindClose(sr);
  }
  Lv1->Clear();
  if (Lv1->Columns->Count<=2)
  {
    NewColumn=Lv1->Columns->Add();
    NewColumn->Caption="Размер";
    NewColumn->Alignment=taCenter;
    NewColumn->Width=Lv1->Column[1]->Width;
  }
  for (int i=0;i<LDir->Count;i++)
  {
    if ((i==0)&&(LDir->Strings[i]!="<--"))
    {
      ListItem = Lv1->Items->Add();
      ListItem->Caption = "<--";
      ListItem->ImageIndex=0;
      i=1;
      fl=-1;
    }
    ListItem = Lv1->Items->Add();
    ListItem->Caption = LDir->Strings[i];
    ListItem->SubItems->Add("Папка");
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

void __fastcall TFr_Main::FormDestroy(TObject *Sender)
{
  DeleteFile("fm.fm");
  if (path!="")
  {
    LPath->Clear();
    LPath->Add(path);
    LPath->SaveToFile("fm.fm");
    //FileSetAttr("fm.fm",6);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Copy(TObject *Sender)
{
  if (Lv1->Selected->ImageIndex==-1)
    file= Lv1->Selected->Caption;
  cpPath=path+Lv1->Selected->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Paste(TObject *Sender)
{
  if (file=="")
  {
    SHFILEOPSTRUCT fos;
    memset(&fos,0,sizeof(SHFILEOPSTRUCT));
    fos.hwnd = Application->Handle;
    fos.wFunc = FO_COPY;
    fos.pFrom = cpPath.c_str();
    fos.pTo = path.c_str();
    fos.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMMKDIR;
    SHFileOperation(&fos);
  }
  else
    CopyFile(cpPath.c_str(),(path+file).c_str(),1);
  file="";
  cpPath="";
  FileList(Owner);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Del(TObject *Sender)
{
  AnsiString tmp=path;
  if (MessageDlg("Вы уверены, что хотите безвозвратно удалить "+Lv1->Selected->Caption+"?",mtConfirmation,TMsgDlgButtons()<<mbYes<<mbNo,0)==mrYes)
  {
    if (Lv1->Selected->ImageIndex==0)
    {
      RemoveDirectory((tmp+Lv1->Selected->Caption).c_str());
    }
    else
      DeleteFile((path+Lv1->Selected->Caption).c_str());
  }
  else
    return;
  FileList(Owner);
}
//---------------------------------------------------------------------------

