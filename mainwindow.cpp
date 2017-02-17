#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(thread.isRunning() )
       {
       thread.quit();
       }
       thread.wait();
       event->accept();
}

void MainWindow::initilizePath(const QString path)
{
    QString textToCurrentDir("Current dir is: ");
    textToCurrentDir.append(&path);
    ui->show_Current_Dir->setText(textToCurrentDir);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),path(QDir::current().absolutePath())
{
    ui->setupUi(this);
    initilizePath(path);
    comand["ls"] = LS_cmd;
    comand["cd"] = CD_cmd;
    comand["exit"] = EXIT_cmd;
    comand["rm"] = REMOVE_cmd;
    comand["help"] = HELP_cmd;
    comand["cp"] = COPY_cmd;
    comand["rn"] = RENAME_cmd;
    worker = new LoadingWorker();
    worker->moveToThread(&thread);
    QObject::connect(&thread, &QThread::finished, worker, &QObject::deleteLater );
    QObject::connect(this, SIGNAL(exitProgram()),qApp, SLOT(quit()));
    QObject::connect(this, SIGNAL(startLsCommand(const QString&, const QStringList&)),
                                       worker, SLOT(listFiles(const QString& , const QStringList&)));
    QObject::connect(worker, SIGNAL(sendResult(const QString &)),
                         this, SLOT(processResult(const QString&) ) );

    thread.start();
}


MainWindow::~MainWindow()
{
    thread.quit();
     thread.wait();
    delete ui;
}



void MainWindow::on_enter_clicked()
{
QString str = ui->enterCommand->text();
QStringList list = str.split(" ");
QMap<QString,commands>::iterator found = comand.find(list[0]);
if( found == comand.end() )
 {
     ui->showResult->append("Invalid command!");
  }
switch (comand[list[0]])
     {
    case LS_cmd:
    {
    emit startLsCommand(path, list);
    break;
     }
    case COPY_cmd:
    {
    if(list.size()==3)
     {
     if(QFile::copy(list[1],list[2]))
     ui->showResult->append("The file was copied successfully!");
     else
     ui->showResult->append("The file was not copied!!!");
      break;
     } else
     {
     ui->showResult->append("Invalid command!");
     break;
     }
     }
    case CD_cmd:
    {
    if(list.size() == 2)
     {
    if (path==".")
        {
        initilizePath(path);
        break;
        }
     path.clear();
     path.append(list[1]);
     if (path=="..")
     {
     QDir::cleanPath(path);
     }

     QDir::current().setCurrent(path);
     path.clear();
     path.append(QDir::current().absolutePath());
     initilizePath(path);
     break;
     } else
     {
      ui->showResult->append("Invalid command!");
     break;
     }
     }
    case EXIT_cmd:
    {
    emit exitProgram();
    break;
    }
    case REMOVE_cmd:
   {
    if(list.size() == 2)
     {
     QDir dir(path);
     if (dir.remove(list[1]))
     ui->showResult->append("The file was removed successfully!");
     else
     ui->showResult->append("The file was not removed!!!");
     break;
     } else
     {
      ui->showResult->append("Invalid command!");
     break;
     }
   }
case HELP_cmd:
{
helpFunction();
break;
}
case RENAME_cmd:
{
if(list.size() == 3)
{
 QDir dir(path);
 if (dir.rename(list[1],list[2]))
 ui->showResult->append("The file was renamed successfully!");
 else
 ui->showResult->append("The file was not renamed!!!");
 break;

} else
 {
 ui->showResult->append("Invalid command!");
 break;
}
}
default :
{
 ui->showResult->append("Invalid command!");
     break;

}
}
}

void MainWindow::helpFunction()
{
  ui->showResult->append("cd name_of_folder - Changes the current folder.");
  ui->showResult->append("ls - Displays a list of a directory's and files in current directory.");
  ui->showResult->append("ls -l - Displays a list of names of files, sizes of files, dates of files and types of files in current directory.");
  ui->showResult->append("ls ss - Displays a list of files in current directory sorted by size.");
  ui->showResult->append("ls sn - Displays a list of files in current directory sorted by name.");
  ui->showResult->append("ls sd - Displays a list of files in current directory sorted by date.");
  ui->showResult->append("ls -R - Displays a list of a directory's and files and subdirectories in current directory and files in subdirectories.");
  ui->showResult->append("exit - Exit the program.");
  ui->showResult->append("rm file_name - Remove the current file.");

}


void MainWindow::on_pushButton_clicked()
{
  ui->showResult->clear();
}

void MainWindow::processResult(const QString &result)
{
   ui->showResult->append(result);
}

 void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() != key)
    return;
    key = 0;
    switch(event->key()) {
    case Qt::Key_Enter:
    {
    emit ui->enter->clicked();
    break;
    }
    case Qt::Key_Return:
    {
    emit ui->enter->clicked();
    break;
    }
    default : break;
    }
    }

