#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "loadingworker.h"

enum commands{LS_cmd,CD_cmd,EXIT_cmd,REMOVE_cmd,HELP_cmd,COPY_cmd,RENAME_cmd};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void keyPressEvent(QKeyEvent* event) { key = event->key(); }
    virtual void keyReleaseEvent(QKeyEvent* event);
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_enter_clicked();

    void on_pushButton_clicked();

private:
    QThread thread;
    int key;
    LoadingWorker *worker;
    void initilizePath(const QString path);
    Ui::MainWindow *ui;
    QString path;
    QMap<QString,commands> comand;
    void helpFunction();



signals:
    void exitProgram();
    void startLsCommand(const QString &path, const QStringList &listComands);
    public slots:
    void processResult(const QString& result);

};

#endif // MAINWINDOW_H
