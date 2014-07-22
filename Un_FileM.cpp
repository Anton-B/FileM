//---------------------------------------------------------------------------

#define NO_WIN32_LEAN_AND_MEAN
#include <vcl.h>
#include <dir.h>
#include <fstream.h>
#include <sys\stat.h>
#include <Masks.hpp>
#pragma hdrstop

/*#include <windows.h>
#include <objidl.h>
#include <intshcut.h>
#include <shlobj.h>
#include <iostream>
#include <string>   */

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
  if (FileExists("fm.fm"))
  {
    LPath->LoadFromFile("fm.fm");
    LoadPath(Lv1,path1,0);
    LoadPath(Lv2,path2,1);
  }
  else
  {
    CreateDList(Lv1);
    CreateDList(Lv2);
  }
  LPath->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::LoadPath(TListView *LV, AnsiString &path, int i)
{
  if (LPath->Strings[i]=="")
    CreateDList(LV);
  else
  {
    path=LPath->Strings[i];
    CreateFList(LV);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::FormDestroy(TObject *Sender)
{
  DeleteFile("fm.fm");
  LPath->Clear();
  LPath->Add(path1);
  LPath->Add(path2);
  LPath->SaveToFile("fm.fm");
  //FileSetAttr("fm.fm",6);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::IdentLV(TObject *Sender, int i)
{
  if (i==0)
  {
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
  }
  else
  {
    if (((TListView*)Sender)==Lv1)
    {
      path1=path;
      fl1=fl;
      EdPath1->Text=path1;
    }
    else
    {
      path2=path;
      fl2=fl;
      EdPath2->Text=path2;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::CreateDList(TObject *Sender)
{
  IdentLV(((TListView*)Sender),0);
  path="";
  fl=0;
  TStringList *LDisk= new TStringList;
  int count;
  ((TListView*)Sender)->Clear();
  /*LDir->Clear();
  LFile->Clear();
  LExt->Clear();
  LSize->Clear();*/
  if (((TListView*)Sender)->Columns->Count>2)
    ((TListView*)Sender)->Columns->Delete(2);
  ((TListView*)Sender)->Column[1]->Caption="Тип";
  ((TListView*)Sender)->Column[1]->Alignment=taCenter;
  TFiles D;
  D.DiskList(LDisk,count);
  for (int i=0;i<count*2;i+=2)
  {
    ListItem = ((TListView*)Sender)->Items->Add();
    ListItem->Caption=LDisk->Strings[i];
    ListItem->ImageIndex=1;
    ListItem->SubItems->Add(LDisk->Strings[i+1]);
  }
  IdentLV(((TListView*)Sender),1);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::ListDblClick(TObject *Sender)
{
  IdentLV(((TListView*)Sender),0);

  if  (((TListView*)Sender)->Selected)
    if (((TListView*)Sender)->Selected->Caption=="<--")
      K=8;
  if (K==8)
  {
    K=0;
    if ((fl==-1)||(fl==0))
    {
      IdentLV(((TListView*)Sender),1);
      CreateDList(((TListView*)Sender));
      return;
    }
    else
    {
      char *p=path.c_str();
      p[path.Length()-1]='\0';
      for (int i=path.Length();p[i-2]!='\\';i--)
        p[i-2]='\0';
      path=p;
      IdentLV(((TListView*)Sender),1);
      CreateFList(((TListView*)Sender));
    }
  }
  else if  (((TListView*)Sender)->Selected)
  {
    K=0;
    /*if (ExtractFileExt(((TListView*)Sender)->Selected->Caption)==".lnk")
    {
    wstring lnk=path.c_str();

    wchar_t link;
    IPersistFile* pPersistFile=NULL;
    IShellLink* pLink=NULL;
    TCHAR szPath[MAX_PATH]={0};
        if(FAILED(CoInitialize(NULL)))
            throw "CoInitialize";
        if(FAILED(CoCreateInstance(CLSID_ShellLink, NULL,CLSCTX_INPROC_SERVER,__uuidof(IShellLink), (void **) &pLink)))
            throw "CoCreateInstance";
        if(FAILED(pLink->QueryInterface(IID_IPersistFile,(void **)&pPersistFile)))
            throw "QueryInterface";
        if(FAILED(pPersistFile->Load(lnk.c_str(),0x0002)))
            throw "Load";
        if(FAILED(pLink->GetPath(szPath,MAX_PATH,NULL,SLGP_RAWPATH)))
            throw "GetPath";
        cout<<szPath<<endl;
    if(pPersistFile)
        pPersistFile->Release();
    if(pLink)
        pLink->Release();
    CoUninitialize();
    ShowMessage(szPath);
   } */
    if (((TListView*)Sender)->Selected->ImageIndex==-1)
    {
      ShellExecute(Handle, "open",((TListView*)Sender)->Selected->Caption.c_str(),NULL,path.c_str(),SW_SHOWNORMAL);
      return;
    }
    if (fl==0)
      path=((TListView*)Sender)->Selected->Caption;
    else
      path+=((TListView*)Sender)->Selected->Caption+"\\";
    IdentLV(((TListView*)Sender),1);
    CreateFList(((TListView*)Sender));
  }
  else
    return;
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::CreateFList(TObject *Sender)
{
  //TStringList *LDir=new TStringList, *LFile=new TStringList, *LExt=new TStringList, *LSize=new TStringList;
  IdentLV(((TListView*)Sender),0);
  fl=1;
  TFiles F;
  F.FileList(path,LDir,LFile,LExt,LSize);
  ((TListView*)Sender)->Column[1]->Caption="Тип";
  ((TListView*)Sender)->Column[1]->Alignment=taCenter;
  ((TListView*)Sender)->Clear();
  if (((TListView*)Sender)->Columns->Count<=2)
  {
    NewColumn=((TListView*)Sender)->Columns->Add();
    NewColumn->Caption="Размер";
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
    ListItem->SubItems->Add("Папка");
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
  IdentLV(((TListView*)Sender),1);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Lv(TObject *Sender)
{
  TFiles O;
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

void __fastcall TFr_Main::Copy(TObject *Sender)
{
  flPaste=1;
  IdentLV(((TListView*)Sender),0);
  tmp=path;
  if (((TListView*)Sender)->Selected->ImageIndex==-1)
    file=((TListView*)Sender)->Selected->Caption;
  cpPath=tmp+((TListView*)Sender)->Selected->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Paste(TObject *Sender)
{
  IdentLV(((TListView*)Sender),0);
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
  flPaste=0;
  file="";
  cpPath="";
  if (path1=="")
    CreateDList(Lv1);
  else
    CreateFList(Lv1);
  if (path2=="")
    CreateDList(Lv2);
  else
    CreateFList(Lv2);
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Del(TObject *Sender)
{
  IdentLV(((TListView*)Sender),0);
  if (flCut==0)
  {
    if (MessageDlg("Вы уверены, что хотите безвозвратно удалить "+((TListView*)Sender)->Selected->Caption+"?",mtConfirmation,TMsgDlgButtons()<<mbYes<<mbNo,0)==mrYes)
    {
      Ident(((TListView*)Sender)->Selected->ImageIndex,path+((TListView*)Sender)->Selected->Caption);
    }
    else
      return;
  }
  else
    Ident(ctImInd,ctPath);
  flCut=0;
  CreateFList(((TListView*)Sender));
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Cut(TObject *Sender)
{
  flPaste=1;
  flCut=1;
  Copy(((TListView*)Sender));
  ctPath=tmp+((TListView*)Sender)->Selected->Caption;
  ctImInd=((TListView*)Sender)->Selected->ImageIndex;
  ((TListView*)Sender)->Selected->Caption="(Вырезано) "+((TListView*)Sender)->Selected->Caption;
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
    ((TPopupMenu*)Popup)->Items->Find("Перейти к месторасположению")->Enabled=true;
    return;
  }
  if (!((TListView*)Sender)->Selected)
  {
    ((TPopupMenu*)Popup)->Items->Find("Перейти к месторасположению")->Enabled=false;
    return;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::PopupEnable(TObject *Sender, bool ct, bool cp, bool pt, bool dt)
{
  if (((TPopupMenu*)Sender)->Items->Find("Вырезать"))
    ((TPopupMenu*)Sender)->Items->Find("Вырезать")->Enabled=ct;
  if (((TPopupMenu*)Sender)->Items->Find("Копировать"))
    ((TPopupMenu*)Sender)->Items->Find("Копировать")->Enabled=cp;
  if (((TPopupMenu*)Sender)->Items->Find("Вставить"))
    ((TPopupMenu*)Sender)->Items->Find("Вставить")->Enabled=pt;
  if (((TPopupMenu*)Sender)->Items->Find("Удалить"))
    ((TPopupMenu*)Sender)->Items->Find("Удалить")->Enabled=dt;
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
  {
    CreateDList(Lv1);
    Lv1->SetFocus();
  }
  else
  {
    CreateDList(Lv2);
    Lv2->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::SearchIdent(TObject *Sender)
{
  Fr_Main->Enabled=false;
  NumOfFiles=0;
  if (((TButton*)Sender)->Name=="BtS1")
  {
    PrBar1->Visible=true;
    Search(Lv1,PrBar1);
    PrBar1->Visible=false;
    Lv1->SetFocus();
  }
  else
  {
    PrBar2->Visible=true;
    Search(Lv2,PrBar2);
    PrBar2->Visible=false;
    Lv2->SetFocus();
  }
  Fr_Main->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Search(TObject *Sender, TProgressBar* PB)
{
  AnsiString text;
  IdentLV(((TListView*)Sender),0);
  if (path=="")
      CreateDList(((TListView*)Sender));
  if (((TListView*)Sender)==Lv1)
  {
    text=Ed1->Text;
    Lv1->PopupMenu=PopupMenu3;
  }
  else
  {
    text=Ed2->Text;
    Lv2->PopupMenu=PopupMenu4;
  }
  if (path=="")
  {
    for (int i=0;i<((TListView*)Sender)->Items->Count;i++)
    {
      FilesNum(((TListView*)Sender)->Items->Item[i]->Caption);
      PB->Max=NumOfFiles;
      Find(((TListView*)Sender)->Items->Item[i]->Caption,text,PB);
    }
    path="Компьютер";
  }
  else
  {
    FilesNum(path);
    PB->Max=NumOfFiles;
    Find(path,text,PB);
  }
  ((TListView*)Sender)->Clear();
  if (((TListView*)Sender)->Columns->Count>2)
    ((TListView*)Sender)->Columns->Delete(2);
  ((TListView*)Sender)->Column[1]->Caption="Адрес";
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
  MessageDlg("В каталоге "+path+" найдено "+((TListView*)Sender)->Items->Count+" файлов и/или папок.",mtInformation,TMsgDlgButtons()<<mbOK,0);
  LDir->Clear();
  LFile->Clear();
  LSearchD->Clear();
  LSearchF->Clear();
  path="";
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::FilesNum(AnsiString p)
{
  TSearchRec sr;
  if(FindFirst(p + "*.*", faAnyFile, sr) == 0)
  {
    do
    {
      if((sr.Attr & faDirectory) != faDirectory)
        NumOfFiles++;
      else
      {
        if(sr.Name != "." && sr.Name!= "..")
        {
          NumOfFiles++;
          FilesNum(p + sr.Name + "\\");
        }
      }
    } while (FindNext(sr)==0);
    FindClose(sr);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::Find(AnsiString p, AnsiString t, TProgressBar* PB)
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
        PB->Position+=1;
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
          PB->Position+=1;
          if(((sr.Attr & faAnyFile) == sr.Attr) &&(sr.Name==t))
          {
            LDir->Add(sr.Name);
            LSearchD->Add(p);
          }
          Find(p + sr.Name + "\\", t, PB);
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
  if (((TEdit*)Sender)->Text=="Введите слово для поиска...")
    ((TEdit*)Sender)->Text="";
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
    CreateFList(Lv1);
  }
  else
  {
    path2=Lv2->Selected->SubItems->Text;
    char *p=path2.c_str();
    p[path2.Length()-1]='\0';
    for (int i=path2.Length();p[i-2]!='\\';i--)
      p[i-2]='\0';
    path2=p;
    CreateFList(Lv2);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::LvKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==VK_RETURN||Key==VK_BACK)
  {
    if (Key==VK_BACK)
      K=8;
    ListDblClick(((TListView*)Sender));
    K=0;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr_Main::BtPathClick(TObject *Sender)
{
  if (((TButton*)Sender)->Name=="BtPath1")
  {
    path=path1=EdPath1->Text;
    if (path1=="")
      CreateDList(Lv1);
    else
    {
      char *p=path.c_str();
      if (p[path.Length()-1]!='\\')
        path+="\\";
      path1=path;
      CreateFList(Lv1);
    }
    Lv1->SetFocus();
  }
  else
  {
    path=path2=EdPath2->Text;
    if (path2=="")
      CreateDList(Lv2);
    else
      {
        char *p=path.c_str();
        if (p[path.Length()-1]!='\\')
          path+="\\";
        path2=path;
        CreateFList(Lv2);
    }
    Lv2->SetFocus();
  }
}
//---------------------------------------------------------------------------


