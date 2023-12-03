#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QTextEdit>
#include <QDebug>
#include <QStringList>
#include <QList>
#include <QDateTime>
#include <QFile>
#include <QImageReader>
#include <QImageIOHandler>
#include <QFlags>
#include <QFlag>
#include <QListIterator>
//-------------------
#include "imageinfo.h"
#include "json_resolve.h"
/*------------------*/
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
void findFile(const QString& path, QStringList *fileName);
private slots:
    void on_pushButton_file_dir_clicked();
private:
    Ui::MainWindow *ui;
    ImageInfo *jpg_info;
    Json_resolve *json;
};
#endif // MAINWINDOW_H
