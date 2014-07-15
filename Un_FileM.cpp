//---------------------------------------------------------------------------

#include <vcl.h>
#include <dir.h>
#include <fstream.h>
#include <sys\stat.h>
#include <Masks.hpp>
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
  Fr_Main->Enabled=true;
  flPaste=0;
  LDir=new TStringList;
  LFile=new TStringList;
  LExt=new TStringList;
  LSize=new TStringList;
  LPath=new TStringList;
  LSearchD=new TStringList;
  LSearchF=new TStringList;
  if (FileExists("fml.fml"))
  {
    LPath->LoadFromFile("fml.fml");
    path1=LPath->Strings[0];
    FileList(Lv1);
  }
  else
    DiskList(Lv1);
  LPath->Clear();
  if (FileExists("fmr.fmr"))
  {
    LPath->LoadFromFile("fmr.fmr");
    path2=LPath->Strings[0];
    FileList(Lv2);
  }
  else
    DiskList(Lv2);
  LPath->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::DiskList(TObject *Sender)
{
  ((TListView*)Sender)->Clear();
  if (((TListView*)Sender)==Lv1)
    Lv1->PopupMenu=PopupMenu1;
  else
    Lv2->PopupMenu=PopupMenu2;
  LDir->Clear();
  LDir->Clear();
  LExt->Clear();
  LSize->Clear();
  if (((TListView*)Sender)->Columns->Count>2)
    ((TListView*)Sender)->Columns->Delete(2);
  ((TListView*)Sender)->Column[1]->Caption="���";
  ((TListView*)Sender)->Column[1]->Alignment=taCenter;
  if (((TListView*)Sender)==Lv1)
  {
    path1="";
    fl1=0;
  }
  else
  {
    path2="";
    fl2=0;
  }
  int BufferSize = GetLogicalDriveStrings(0, NULL);
  char *Buffer = new char[BufferSize];
  TStringList *DiskList = new TStringList;
  GetLogicalDriveStrings(BufferSize, Buffer);
  int DiskCount = (BufferSize - 1) / 4 ;
  for(int i = 0; i < DiskCount; i++)
  {
    char DiskString[4];
    CopyMemory(DiskString, (Buffer + i * 4), 4);
    ListItem = ((TListView*)Sender)->Items->Add();
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
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::ListDblClick(TObject *Sender)
{
  AnsiString path;
  int fl;
  if (((TListView*)Sender)==Lv1)
  {
    path=path1;
    fl=fl1;
  }
  else
  {
    path=path2;
    fl=fl2;
  }
  if (((TListView*)Sender)->Selected)
  {
    if (((TListView*)Sender)->Selected->ImageIndex==-1)
    {
      ShellExecute(Handle, "open",((TListView*)Sender)->Selected->Caption.c_str(),NULL,path.c_str(),SW_SHOWNORMAL);
      return;
    }
    if ((fl==-1)&&(((TListView*)Sender)->Selected->Caption=="<--"))
    {
      DiskList(((TListView*)Sender));
      return;
    }
    if (((TListView*)Sender)->Selected->Caption=="<--")
    {
      char *p=path.c_str();
      p[path.Length()-1]='\0';
      for (int i=path.Length();p[i-2]!='\\';i--)
        p[i-2]='\0';
      path=p;
    }
    else
    {
      if (fl==0)
      {
        path=((TListView*)Sender)->Selected->Caption;
      }
      else
        path+=((TListView*)Sender)->Selected->Caption+"\\";
    }
    if (((TListView*)Sender)==Lv1)
    {
      path1=path;
      fl1=fl;
    }
    else
    {
      path2=path;
      fl2=fl;
    }
    FileList(((TListView*)Sender));
  }
  else
    return;
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::FileList(TObject *Sender)
{
  AnsiString path;
  int fl;
  if (((TListView*)Sender)==Lv1)
  {
    Lv1->PopupMenu=PopupMenu1;
    path=path1;
    fl=fl1;
  }
  else
  {
    Lv2->PopupMenu=PopupMenu2;
    path=path2;
    fl=fl2;
  }
  ((TListView*)Sender)->Column[1]->Caption="���";
  ((TListView*)Sender)->Column[1]->Alignment=taCenter;
  fl=1;
  if (FindFirst(path+"\\*.*", faAnyFile, sr) == 0)
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
        LExt->Add(ExtractFileExt(sr.Name));
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
  ((TListView*)Sender)->Clear();
  if (((TListView*)Sender)->Columns->Count<=2)
  {
    NewColumn=((TListView*)Sender)->Columns->Add();
    NewColumn->Caption="������";
    NewColumn->Alignment=taCenter;
    NewColumn->Width=((TListView*)Sender)->Column[1]->Width;
  }
  for (int i=0;i<LDir->Count;i++)
  {
    if ((i==0)&&(LDir->Strings[i]!="<--"))
    {
      ListItem = ((TListView*)Sender)->Items->Add();
      ListItem->Caption = "<--";
      ListItem->ImageIndex=0;
      i=1;
      fl=-1;
    }
    ListItem = ((TListView*)Sender)->Items->Add();
    ListItem->Caption = LDir->Strings[i];
    ListItem->SubItems->Add("�����");
    ListItem->ImageIndex=0;
  }
  for (int i=0;i<LFile->Count;i++)
  {
    ListItem = ((TListView*)Sender)->Items->Add();
    ListItem->Caption = LFile->Strings[i];
    ListItem->SubItems->Add(LExt->Strings[i]);
    ListItem->SubItems->Add(LSize->Strings[i]);
    ListItem->ImageIndex=-1;
  }
  LDir->Clear();
  LFile->Clear();
  LExt->Clear();              
  LSize->Clear();
  if (((TListView*)Sender)==Lv1)
  {
    path1=path;
    fl1=fl;
  }
  else
  {
    path2=path;
    fl2=fl;
  }
  Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::FormDestroy(TObject *Sender)
{
  DeleteFile("fml.fml");
  DeleteFile("fmr.fmr");
  if (path1!="")
  {
    LPath->Add(path1);
    LPath->SaveToFile("fml.fml");
    //FileSetAttr("fml.fml",6);
  }
  LPath->Clear();
  if (path2!="")
  {
    LPath->Add(path2);
    LPath->SaveToFile("fmr.fmr");
    //FileSetAttr("fmr.fmr",6);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Copy(TObject *Sender)
{
  flPaste=1;
  if (((TListView*)Sender)==Lv1)
    tmp=path1;
  else
    tmp=path2;
  if (((TListView*)Sender)->Selected->ImageIndex==-1)
    file=((TListView*)Sender)->Selected->Caption;
  cpPath=tmp+((TListView*)Sender)->Selected->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Paste(TObject *Sender)
{
  AnsiString path;
  if (((TListView*)Sender)==Lv1)
    path=path1;
  else
    path=path2;
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
  if (flCut==1)
    Del(((TListView*)Sender));
  flCut=0;
  file="";
  cpPath="";
  FileList(Lv1);
  FileList(Lv2);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Del(TObject *Sender)
{
  AnsiString path;
  if (((TListView*)Sender)==Lv1)
    path=path1;
  else
    path=path2;
  if (flCut==0)
  {
    if (MessageDlg("�� �������, ��� ������ ������������ ������� "+((TListView*)Sender)->Selected->Caption+"?",mtConfirmation,TMsgDlgButtons()<<mbYes<<mbNo,0)==mrYes)
    {
      Ident(((TListView*)Sender)->Selected->ImageIndex,path+((TListView*)Sender)->Selected->Caption);
    }
    else
      return;
  }
  else
    Ident(ctImInd,ctPath);
  flCut=0;
  FileList(((TListView*)Sender));
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Cut(TObject *Sender)
{
  flPaste=1;
  flCut=1;
  Copy(((TListView*)Sender));
  ctPath=tmp+((TListView*)Sender)->Selected->Caption;
  ctImInd=((TListView*)Sender)->Selected->ImageIndex;
  ((TListView*)Sender)->Selected->Caption="(��������) "+((TListView*)Sender)->Selected->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Ident(int imInd, AnsiString p)
{
  if (imInd==0)
  {
    DelDir(p);
  }
  else
    DeleteFile((p).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::PopupMenu(TObject *Sender)
{
  if (((TPopupMenu*)Sender)->Name=="PopupMenu1")
    PopupIdent(Lv1,((TPopupMenu*)Sender),fl1);
  else
    PopupIdent(Lv2,((TPopupMenu*)Sender),fl2);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::PopupMenuS(TObject *Sender)
{
  if (((TPopupMenu*)Sender)->Name=="PopupMenu3")
    PopupIdent(Lv1,((TPopupMenu*)Sender));
  else
    PopupIdent(Lv2,((TPopupMenu*)Sender));
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::PopupIdent(TObject *Sender, TPopupMenu *Popup, int fl)
{
  if (fl==0)
  {
    PopupEnable(((TPopupMenu*)Popup),false,false,false,false);
    return;
  }
  if (((TListView*)Sender)->Selected)
  {
    PopupEnable(((TPopupMenu*)Popup),true,true,false,true);
    return;
  }
  if (!((TListView*)Sender)->Selected)
  {
    if (flPaste==0)
      PopupEnable(((TPopupMenu*)Popup),false,false,false,false);
    else
      PopupEnable(((TPopupMenu*)Popup),false,false,true,false);
    return;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::PopupIdent(TObject *Sender, TPopupMenu *Popup)
{
  if (((TListView*)Sender)->Selected)
  {
    ((TPopupMenu*)Popup)->Items->Find("������� � �����������������")->Enabled=true;
    return;
  }
  if (!((TListView*)Sender)->Selected)
  {
    ((TPopupMenu*)Popup)->Items->Find("������� � �����������������")->Enabled=false;
    return;
  }
}
void __fastcall TFr_Main::PopupEnable(TObject *Sender, bool ct, bool cp, bool pt, bool dt)
{
  if (((TPopupMenu*)Sender)->Items->Find("��������"))
    ((TPopupMenu*)Sender)->Items->Find("��������")->Enabled=ct;
  if (((TPopupMenu*)Sender)->Items->Find("����������"))
    ((TPopupMenu*)Sender)->Items->Find("����������")->Enabled=cp;
  if (((TPopupMenu*)Sender)->Items->Find("��������"))
    ((TPopupMenu*)Sender)->Items->Find("��������")->Enabled=pt;
  if (((TPopupMenu*)Sender)->Items->Find("�������"))
    ((TPopupMenu*)Sender)->Items->Find("�������")->Enabled=dt;
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::DelDir(AnsiString Dir)
{
  SHFILEOPSTRUCT sh;
  sh.hwnd=Fr_Main->Handle;
  sh.wFunc = FO_DELETE;
  sh.pFrom = (Dir+"\0").c_str();
  sh.pTo = NULL;
  sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
  sh.hNameMappings = 0;
  sh.lpszProgressTitle = NULL;
  SHFileOperation(&sh);
}
//---------------------------------------------------------------------------
void __fastcall TFr_Main::Home(TObject *Sender)
{
  if (((TButton*)Sender)->Name=="BtHome1")
    DiskList(Lv1);
  else
    DiskList(Lv2);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Lv(TObject *Sender)
{
  switch (((TMenuItem*)Sender)->Tag)
  {
    case 1:
      Cut(Lv1);
      break;
    case 2:
      Copy(Lv1);
      break;
    case 3:
      Paste(Lv1);
      break;
    case 4:
      Del(Lv1);
      break;
    case 5:
      Cut(Lv2);
      break;
    case 6:
      Copy(Lv2);
      break;
    case 7:
      Paste(Lv2);
      break;
    case 8:
      Del(Lv2);
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::SearchIdent(TObject *Sender)
{
  Fr_Main->Enabled=false;
  if (((TButton*)Sender)->Name=="BtS1")
  {
    Lb1->Caption="����������� �����...";
    Search(Lv1);
    Lb1->Caption="";
  }
  else
  {
    Lb2->Caption="����������� �����...";
    Search(Lv2);
    Lb2->Caption="";
  }
  Fr_Main->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Search(TObject *Sender)
{
  AnsiString text;
  AnsiString path;
  int fl;
  if (((TListView*)Sender)==Lv1)
  {
    path=path1;
    if (path=="")
      DiskList(((TListView*)Sender));
    text=Ed1->Text;
    Lv1->PopupMenu=PopupMenu3;
  }
  else
  {
    path=path2;
    if (path=="")
      DiskList(((TListView*)Sender));
    text=Ed2->Text;
    Lv2->PopupMenu=PopupMenu4;
  }
  if (path=="")
  {
    for (int i=0;i<((TListView*)Sender)->Items->Count;i++)
      Find(((TListView*)Sender)->Items->Item[i]->Caption,text);
    path="���������";
  }
  else
    Find(path,text);
  ((TListView*)Sender)->Clear();
  if (((TListView*)Sender)->Columns->Count>2)
    ((TListView*)Sender)->Columns->Delete(2);
  ((TListView*)Sender)->Column[1]->Caption="�����";
  ((TListView*)Sender)->Column[1]->Alignment=taLeftJustify;

  for (int i=0;i<LDir->Count;i++)
  {
    ListItem = ((TListView*)Sender)->Items->Add();
    ListItem->Caption = LDir->Strings[i];
    ListItem->SubItems->Add(LSearchD->Strings[i]);
    ListItem->ImageIndex=0;
  }
  for (int i=0;i<LFile->Count;i++)
  {
    ListItem = ((TListView*)Sender)->Items->Add();
    ListItem->Caption = LFile->Strings[i];
    ListItem->SubItems->Add(LSearchF->Strings[i]);
    ListItem->ImageIndex=-1;
  }
  MessageDlg("� �������� "+path+" ������� "+((TListView*)Sender)->Items->Count+" ������ �/��� �����.",mtInformation,TMsgDlgButtons()<<mbOK,0);
  LDir->Clear();
  LFile->Clear();
  LSearchD->Clear();
  LSearchF->Clear();
  path="";
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Find(AnsiString p, AnsiString t)
{
  AnsiString name,ext;
  char n[MAXFILE], e[MAXEXT];
  TSearchRec sr;
  if(FindFirst(p + "*.*", faAnyFile, sr) == 0)
  {
    do
    {
      if((sr.Attr & faDirectory) != faDirectory)
      {
        fnsplit((p+sr.Name).c_str(),NULL,NULL,n,e);
        name=n;
        ext=e;
        if(((sr.Attr & faAnyFile) == sr.Attr) &&(MatchesMask(sr.Name, t)))
        {
          LFile->Add(sr.Name);
          LSearchF->Add(p);
        }
        else if (((sr.Attr & faAnyFile) == sr.Attr) &&(name==t))
        {
          LFile->Add(sr.Name);
          LSearchF->Add(p);
        }
        else if (((sr.Attr & faAnyFile) == sr.Attr) &&(ext==t))
        {
          LFile->Add(sr.Name);
          LSearchF->Add(p);
        }
      }
      else
      {
        if(sr.Name != "." && sr.Name!= "..")
        {
          if(((sr.Attr & faAnyFile) == sr.Attr) &&(sr.Name==t))
          {
            LDir->Add(sr.Name);
            LSearchD->Add(p);
          }
          Find(p + sr.Name + "\\", t);
        }
      }
      Application->ProcessMessages();
    } while (FindNext(sr)==0);
    FindClose(sr);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFr_Main::Ed(TObject *Sender)
{
  if (((TEdit*)Sender)->Font->Color==clInactiveCaption)
  {
    ((TEdit*)Sender)->Font->Color=clWindowText;
    ((TEdit*)Sender)->Text="";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::ToPathClick(TObject *Sender)
{
  if (Lv1->Selected)
  {
    path1=Lv1->Selected->SubItems->Text;
    char *p=path1.c_str();
    p[path1.Length()-1]='\0';
    for (int i=path1.Length();p[i-2]!='\\';i--)
      p[i-2]='\0';
    path1=p;
    FileList(Lv1);
  }
  else
  {
    path2=Lv2->Selected->SubItems->Text;
    char *p=path2.c_str();
    p[path2.Length()-1]='\0';
    for (int i=path2.Length();p[i-2]!='\\';i--)
      p[i-2]='\0';
    path2=p;
    FileList(Lv2);
  }
}
//---------------------------------------------------------------------------

