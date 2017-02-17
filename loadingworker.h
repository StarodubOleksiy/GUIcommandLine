#ifndef LOADINGWORKER_H
#define LOADINGWORKER_H

#include <QMainWindow>
#include <QWidget>
#include <QThread>
#include <QApplication>
#include <QWidget>
#include <QResizeEvent>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QString>
#include <iostream>
#include <cstring>

class LoadingWorker : public QObject
{
    Q_OBJECT
public:
    LoadingWorker();
private:
    enum sortTypes{none,byName,bySize,byDate};
    struct Ls_options
    {
     bool longList;
     bool recursive;
     sortTypes sorts;
     Ls_options():longList(false),recursive(false),sorts(none){}
    };

    class compareNames
           {
           public:
           bool operator() (const QFileInfo &file1,
                            const QFileInfo &file2) const
              { return file1.fileName().toLower() < file2.fileName().toLower(); }
           };

        class compareSize
           {
           public:
           bool operator() (const QFileInfo &file1,
                            const QFileInfo &file2) const
              { return file1.size() < file2.size(); }
           };

        class compareDate
           {
           public:
           bool operator() (const QFileInfo &file1,
                            const QFileInfo &file2) const
              { return file1.created().toString() < file2.created().toString() ; }
           };
    void start(const QString& path);
    QVector<QFileInfo> listOfFiles;
    public slots:
    void listFiles(const QString &path, const QStringList &listComands);
    signals:
    void sendResult(const QString &result);

};

#endif // LOADINGWORKER_H
