#include "Un_FileM.h"
class TFiles
{
public:
        void DiskList(TStringList* &L, int &DiskCount);
        void FileList(AnsiString &path, TStringList* LD, TStringList* LF, TStringList* LE, TStringList* LS);
};

void TFiles::DiskList(TStringList* &L, int &DiskCount)
{
  int BufferSize = GetLogicalDriveStrings(0, NULL);
  char *Buffer = new char[BufferSize];
  TStringList *DiskList = new TStringList;
  GetLogicalDriveStrings(BufferSize, Buffer);
  DiskCount = (BufferSize - 1) / 4 ;
  for(int i = 0; i < DiskCount; i++)
  {
    char DiskString[4];
    CopyMemory(DiskString, (Buffer + i * 4), 4);
    L->Add(DiskString);

    int DriveType = GetDriveType(DiskString);
    switch(DriveType)
    {
    case DRIVE_UNKNOWN:
      L->Add("Неизвестен");
      break;
    case DRIVE_NO_ROOT_DIR:
      L->Add("Отсутствует");
      break;
    case DRIVE_REMOVABLE:
      L->Add("Сменный");
      break;
    case DRIVE_FIXED:
      L->Add("Жесткий");
      break;
    case DRIVE_REMOTE:
      L->Add("Сетевой");
      break;
    case DRIVE_CDROM:
      L->Add("CD-ROM");
      break;
    case DRIVE_RAMDISK:
      L->Add("RAM");
      break;
    default:
      L->Add("Неизвестен");
      break;
    }
  }
  delete [] Buffer;
  delete DiskList;
}

void TFiles::FileList(AnsiString &path, TStringList* LD, TStringList* LF, TStringList* LE, TStringList* LS)
{
  TSearchRec sr;
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
          LD->Add(sr.Name);
        }
      }
      else
      {
        LF->Add(sr.Name);
        LE->Add(ExtractFileExt(sr.Name));
        struct stati64 statbuf;
        AnsiString p=path+sr.Name;
        _stati64(p.c_str(), &statbuf);
        if (statbuf.st_size>=1073741824)
          LS->Add(FloatToStr(statbuf.st_size/1073741824)+" ГБ");
        else if (statbuf.st_size>=1048576)
          LS->Add(FloatToStr(statbuf.st_size/1048576)+" МБ");
        else if (statbuf.st_size>=1024)
          LS->Add(FloatToStr(statbuf.st_size/1024)+" КБ");
        else if (statbuf.st_size<1024)
          LS->Add(FloatToStr(statbuf.st_size)+" Б");
      }
    } while (FindNext(sr) == 0);
    FindClose(sr);
  }
  Application->ProcessMessages();
}


