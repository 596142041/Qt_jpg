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
    QString indexOf_str;
    QDir dst_tmp;
    QString desFileName;
    QFileInfo File_Info;
    QFileInfo Write_fileInfo;
    QSize jpg_size;
    QDir img_path;
    QString dst_path;
    if(ui->lineEdit_file_path->text().isEmpty () == false)
    {
        history_path = ui->lineEdit_file_path->text();
    }
    QString path = QFileDialog::getExistingDirectory(
                    this,
                    tr("打开文件"),
                    history_path,
                    QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks
                    );
    if(path == "")
    {
        return;
    }
    dst_tmp.setPath (path);
    json->Json_Set_KeyValue("config.json","History",path);
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
        else
        {
            File_Info.setFile (filename);
            desFileName.setNum (fileNames_list.indexOf (filename)+1).insert (0,dst_tmp.dirName ().append ("_").insert (0,path+QDateTime::currentDateTime().toString("/MMdd"))).append (".").append (File_Info.suffix());
            if(Write_fileInfo.exists())
            {
                QFile::remove(desFileName);
            }
            else
            {
                if(!Write_fileInfo.dir().exists())
                {
                    Write_fileInfo.dir().mkpath(Write_fileInfo.absolutePath());
                }
            }
            QFile::rename(filename,desFileName);//对无需移动的图片进行重命名
        }
    }
    desFileName.clear ();
    //避免无宽幅图片或者全是宽幅图片的情况
    if(fileNames_list.size () == movefile_list.size ())
    {
        ui->textEdit_file_list->append("全为宽幅图片");
        return;
    }
    if(movefile_list.size () != 0)
    {
        dst_path = path.append ("/")+dst_tmp.dirName();
    }
    else
    {
        ui->textEdit_file_list->append("宽幅图片数量:0");
        return;
    }
    //移动文件
    foreach (const QString& filename, movefile_list)
    {
        dst_tmp.setPath (dst_path);
        File_Info.setFile (filename);
        QString suffix_t = QDateTime::currentDateTime().toString("/MMdd").append (dst_tmp.dirName().append ('~')).append ( indexOf_str.setNum (movefile_list.indexOf (filename)+1)+".");
        indexOf_str.setNum (movefile_list.indexOf (filename)+1);
        desFileName = dst_path + suffix_t.append (File_Info.suffix());
        Write_fileInfo.setFile (desFileName);
        if(Write_fileInfo.exists())
        {
            QFile::remove(desFileName);
        }
        else
        {
            if(!Write_fileInfo.dir().exists())
            {
                Write_fileInfo.dir().mkpath(Write_fileInfo.absolutePath());
            }
        }
        QFile::rename(filename, desFileName);
        ui->textEdit_file_list->append(Write_fileInfo.fileName());
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
