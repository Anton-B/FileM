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
  int n;
  bool Flag;
  DWORD dr = GetLogicalDrives(); // ������� ���������� ������� �����

  
  for(int x=0;x<26;x++) // ���������� ������ �� �����
  {
    n = ((dr>>x)&1); // ����� �������� �������� ����
    if(n) // ���� ������� - ���� � ������� x ����
    {
      dl = AnsiString((char)(65+x)); // �������� ������ �����
      AnsiString path = AnsiString((char)(65+x))+":\\";
      AnsiString out = "���� " + dl + ":\\\n";
      ListItem = Lv1->Items->Add();
      ListItem->Caption=dl + ":\\";
      ListItem->ImageIndex=1;

      // ����� ����� ������ �� ����������
      WORD OldErrorMode;
      OldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS); // ������� ����� ������
      bool ready = DirectoryExists(path); // ������c� ������� �������� ����������
      SetErrorMode(OldErrorMode); // ��������������� ������ ����� ������ ������

      if (ready)
      {
        UINT drive_type = GetDriveType((dl+":\\").c_str()); // ����� ��� �����
        out += "\n��� �����: ";
        if (drive_type == DRIVE_REMOVABLE)
          out += "REMOVABLE";
        else if (drive_type == DRIVE_FIXED)
          out += "�������";
        else if (drive_type == DRIVE_REMOTE)
          out += "REMOTE";
        else if (drive_type == DRIVE_CDROM)
          out += "CD-ROM";
        else if (drive_type == DRIVE_RAMDISK)
          out += "RAMDISK";
        else
          out += "����������\n";

        // ���� ��� HDD - ������������ ���������� � ���
        if (drive_type == DRIVE_FIXED)
        {
          unsigned __int64 FreeBytesAvailable;
          unsigned __int64 TotalNumberOfBytes;
          unsigned __int64 TotalNumberOfFreeBytes;
          char drive_label[30];
          char drive_fat[30];
          DWORD drive_sn;
          DWORD drive_name_size = sizeof(drive_label);
          // �������� ������ � ��������
          Flag = ::GetDiskFreeSpaceEx(path.c_str(),(PULARGE_INTEGER)&FreeBytesAvailable,
          (PULARGE_INTEGER)&TotalNumberOfBytes, (PULARGE_INTEGER)&TotalNumberOfFreeBytes);
          if (Flag)
          {
            out += "\n�������� �� �����: " + AnsiString(TotalNumberOfFreeBytes/1073741824) + " ��" + "\n";
            out += "����� �� �����: " + AnsiString(TotalNumberOfBytes/1073741824) + " ��" + "\n";
          }
          else
            out += "������ � GetDiskFreeSpaceEx\n";
          // �������� �����, ���������� ����� � ��.
          /*Flag = GetVolumeInformation(path.c_str(), drive_label, sizeof(drive_label),
                  &  drive_sn, &drive_name_size, NULL, drive_fat, sizeof(drive_fat));
          if (Flag)
            out += "�������� �������: " + AnsiString(drive_fat) + "\n";
          else
            out += "������ � GetVolumeInformation\n"; */
        }
      }
      else
      {
        out += "\n�� �����";
      }
      //ShowMessage(out);
    }
  }

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


}
//---------------------------------------------------------------------------



void __fastcall TFr_Main::Lv1DblClick(TObject *Sender)
{
  disk=((TListView*)Sender)->Selected->Caption;
  cD=0;
  cF=0;
  LDir=new TStringList;
  LFile=new TStringList;
  if (FindFirst(disk+"*.*", faAnyFile, sr) == 0)
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
  Lv1->Clear();
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

