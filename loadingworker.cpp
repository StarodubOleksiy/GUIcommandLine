#include "loadingworker.h"

LoadingWorker::LoadingWorker()
{
}

void LoadingWorker::start(const QString &path)
{
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList();
  for (int i = 0; i < list.size(); ++i) {
             QFileInfo fileInfo = list.at(i);
  if(fileInfo.fileName()==".."||fileInfo.fileName()==".") continue;
  listOfFiles.push_back(fileInfo);
    }
QStringList listDir = dir.entryList(QDir::Dirs);
   foreach (QString subdir, listDir) {
           if (subdir == "." || subdir == "..") {
               continue;
           }
           start(subdir);
  }
}
 void LoadingWorker::listFiles(const QString &path, const QStringList &listComands)
 {
     if (listComands.size() > 4)
     {
       emit sendResult("Invalid command!!!");
         return;
     }
     Ls_options options;
     for(int i = 0; i < listComands.size(); ++i)
      {
      if (listComands[i] == "-R")
       options.recursive = true;
      if (listComands[i] == "-l")
       options.longList = true;
      if (listComands[i] == "-sn")
       options.sorts = byName;
      if (listComands[i] == "-ss")
       options.sorts = bySize;
      if (listComands[i] == "-sd")
       options.sorts = byDate;
      }

   if (options.recursive)
    start(path);
   else
   {
       QDir dir(path);
       QFileInfoList list = dir.entryInfoList();
       for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);
     if(fileInfo.fileName()==".."||fileInfo.fileName()==".") continue;
      listOfFiles.push_back(fileInfo);
     }
   }
   if (options.sorts == byName)
       qSort(listOfFiles.begin(),                  //sort persons
               listOfFiles.end(), compareNames() );
   if (options.sorts == bySize)
       qSort(listOfFiles.begin(),                  //sort persons
               listOfFiles.end(), compareSize() );
   if (options.sorts == byDate)
       qSort(listOfFiles.begin(),                  //sort persons
               listOfFiles.end(), compareDate());
   if (options.longList)
  {
  emit sendResult("File Size(bytes) |              File Name          | File Date");
  emit sendResult("------------------------------------------------------------------------------------------------------------");
  for (int i = 0; i < listOfFiles.size(); ++i) {
  if(listOfFiles[i].fileName()==".."||listOfFiles[i].fileName()==".") continue;
  QString res;
  res.setNum(listOfFiles[i].size());
  res.append("\t");
  res.append(listOfFiles[i].fileName());
  res.append("\t");
  res.append(listOfFiles[i].created().toString());
  emit sendResult(res);
      }
  emit sendResult("------------------------------------------------------------------------------------------------------------");

  } else
  {
  emit sendResult("File Name                  ");
  emit sendResult("------------------------------------------------------------------------------------------------------------");
  for (int i = 0; i < listOfFiles.size(); ++i) {
  if(listOfFiles[i].fileName()==".."||listOfFiles[i].fileName()==".") continue;
  emit sendResult(listOfFiles[i].fileName());
  }
  emit sendResult("------------------------------------------------------------------------------------------------------------");
  }


listOfFiles.clear();
 }

