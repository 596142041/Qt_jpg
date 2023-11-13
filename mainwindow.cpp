#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint); // 设置禁止最大化
    jpg_info = new ImageInfo;
    json  = new Json_resolve;//开始解析
    ui->setupUi(this);
    ui->textEdit_file_list->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_file_dir_clicked()
{
    QString history_path = json->Json_Get_KeyValue("config.json","History");
    if(history_path.isEmpty ())
    {
        history_path = QCoreApplication::applicationDirPath();
    }
    QStringList fileNames_list;
    QStringList movefile_list;
    QSize jpg_size;
    QDir img_path;
    QString dst_path,dst_path_tmp;
    if(ui->lineEdit_file_path->text().isEmpty () == false)
    {
        history_path = ui->lineEdit_file_path->text();
    }
    QString path = QFileDialog::getExistingDirectory(this,
                    tr("Open Directory"),
                    history_path,
                    QFileDialog::ShowDirsOnly|
                    QFileDialog::DontResolveSymlinks);
    if(path == "")
    {
        return;
    }
    json->Json_Set_KeyValue("config.json","History",path);
    QStringList path_list = path.split(QLatin1Char('/'), Qt::SkipEmptyParts);
    dst_path_tmp = path_list.at (path_list.count ()-1);
    ui->textEdit_file_list->clear();
    ui->lineEdit_file_path->setText(path);
    findFile(ui->lineEdit_file_path->text(), &fileNames_list);
    foreach (const QString& filename, fileNames_list)//遍历,重要,不在使用for循环
    {
       jpg_size = jpg_info->getJPGDimension (filename);
        if(jpg_size.width() > jpg_size.height ())
        {
            movefile_list.append(filename);//添加到待移动的文件里面
        }
    }
    //避免无宽幅图片或者全是宽幅图片的情况
    if(fileNames_list.size () == movefile_list.size ())
    {
        ui->textEdit_file_list->append("全为宽幅图片");
        return;
    }
    if(movefile_list.size () != 0)
    {
        img_path.mkdir (dst_path_tmp);
        dst_path = path+"/"+dst_path_tmp;
    }
    //移动文件
    foreach (const QString& filename, movefile_list)
    {
        QString temp = filename;
        QString suffix = temp.remove(0,path.length());
        QString desFileName = dst_path + suffix;
        QFileInfo fileInfo(desFileName);
        if(fileInfo.exists())
        {
            QFile::remove(desFileName);
        }
        else
        {
            if(!fileInfo.dir().exists())
            {
                fileInfo.dir().mkpath(fileInfo.absolutePath());
            }
        }
        QFile::rename(filename, desFileName);
        ui->textEdit_file_list->append(fileInfo.fileName());
    }
}
/*
遍历文件夹
*/
void MainWindow::findFile(const QString &path, QStringList *fileNames)
{
    QDir dir(path);
    QStringList filters;
    if(!dir.exists())
    {
        return;
    }
    //获取filePath下所有文件夹和文件
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot|QDir::NoSymLinks);//文件夹|文件|不包含./和../
    filters<<"*.jpg";
    dir.setNameFilters(filters);
    //排序文件夹优先
    dir.setSorting(QDir::Size);
    for(uint i = 0; i < dir.count (); i++)
    {
        QString file_name = path+'/'+dir[i];  //文件名称
        fileNames->append(file_name);
    }
}
