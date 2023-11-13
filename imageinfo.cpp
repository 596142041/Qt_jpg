#include "imageinfo.h"
ImageInfo::ImageInfo(QObject *parent) :
    QObject(parent)
{
    qDebug() << "---------------------------- image info constructed ";
}
ImageInfo::~ImageInfo()
{
}
QDate ImageInfo::getImageDate(QString imageUrl)
{
    QDate date;
    if(!imageUrl.isEmpty()) {
        QUrl fileUrl(imageUrl);
        QString filePath = fileUrl.toLocalFile();
        if(QFile::exists(filePath)) {
            QFileInfo fileinfo(filePath);
            date = fileinfo.lastModified().date();
        }
    }
    return date;
}
//从文件头中读取相应字段以判断图片格式
//详情参看: http://www.garykessler.net/library/file_sigs.html
int ImageInfo::getImageFormat(std::string path)
{
    //BMP格式特征码
    unsigned char BMPHeader[] = {0x42, 0x4d};
    //JPG,JPEG格式特征码
    unsigned char JPGHeader1[] = {0xff, 0xd8, 0xff, 0xdb};
    unsigned char JPGHeader2[] = {0xff, 0xd8, 0xff, 0xe0};
    unsigned char JPGHeader3[] = {0xff, 0xd8, 0xff, 0xe1};
    unsigned char JPGHeader4[] = {0xff, 0xd8, 0xff, 0xe2};
    unsigned char JPGHeader5[] = {0xff, 0xd8, 0xff, 0xe3};
    unsigned char JPGHeader6[] = {0xff, 0xd8, 0xff, 0xe8};
    //GIF格式特征码
    unsigned char GIFHeader1[] = {0x47, 0x49, 0x46, 0x38, 0x37, 0x61};
    unsigned char GIFHeader2[] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};
    //PNG格式特征码
    unsigned char PNGHeader[] = {0x89, 0x50, 0x4E, 0x47};
    int count = 0;
    unsigned char header[16];
    return 0;
}
QString ImageInfo::getImageFormat(QString imageUrl)
{
    QString strFormat = "NA";
    if(!imageUrl.isEmpty()) {
        QUrl fileUrl(imageUrl);
        QString filePath = fileUrl.toLocalFile();
        if(QFile::exists(filePath)) {
            std::string path = filePath.toStdString();
            int iFormat = getImageFormat(path);
            switch(iFormat) {
            case BMP_FORMAT:
                strFormat = "BMP";
                break;
            case JPG_FORMAT:
                strFormat = "JPG";
                break;
            case GIF_FORMAT:
                strFormat = "GIF";
                break;
            case PNG_FORMAT:
                strFormat = "PNG";
                break;
            default:
                break;
            }
        }
    }
    return strFormat;
}
QString ImageInfo::getImageSize(QString imageUrl)
{
    QString strSize;
    long size = 0;
    if(!imageUrl.isEmpty()) {
        QUrl fileUrl(imageUrl);
        QString filePath = fileUrl.toLocalFile();
        if(QFile::exists(filePath)) {
            QFile file(filePath);
            bool ret = file.open(QIODevice::ReadOnly);
            if (!ret) {
                qDebug()<<"打开文件失败";
                size = 0;
            } else {
                size = file.size();
            }
            file.close();
        }
    }
    qDebug()<<"!!!!!"<<size;
    strSize = QString::number(size, 10);
    qDebug()<<strSize;
    return strSize;
}
//BMP文件头的第2、3字为文件大小信息
long ImageInfo::getBMPSize(std::string path)
{
    FILE *fid;
    long int size;
    if((fid=fopen(path.c_str(),"rb+"))==NULL) {
        qDebug()<<"打开文件失败";
        return 0;
    }
    //跳过图片特征码
    fseek(fid, 2, SEEK_SET);
    fread(&size, sizeof(long), 1, fid);
    fclose(fid);
    qDebug()<<"size="<<size;
    return size;
}
long ImageInfo::getGIFSize(std::string path)
{
    Q_UNUSED(path);
    return 0;
}
long ImageInfo::getPNGSize(std::string path)
{
    Q_UNUSED(path);
    return 0;
}
long ImageInfo::getJPGSize(std::string path)
{
    FILE *fid;
    long int size;
    if((fid = fopen(path.c_str(),"rb+")) == NULL) {
        qDebug()<<"打开文件失败";
        return 0;
    }
    fseek(fid, 0, SEEK_END);
    size = ftell(fid);
    fclose(fid);
    qDebug()<<"size="<<size;
    return size;
}
//BMP文件头的第10/11字为文件宽度信息
//BMP文件头的第12/13字为文件高度信息
QSize ImageInfo::getBMPDimension(std::string path)
{
    FILE *fid;
    if((fid=fopen(path.c_str(),"rb+"))==NULL) {
        qDebug()<<"打开文件失败";
        return QSize(0, 0);
    }
    long int width;
    long int height;
    //读取宽度和高度
    fseek(fid, 18, SEEK_SET); //偏移18个字节
    fread(&width, sizeof(long), 1, fid);
    fread(&height, sizeof(long), 1, fid);
    qDebug()<<"width="<<width;
    qDebug()<<"height="<<height;
    fclose(fid);
    return QSize(width, height);
}
//参考: http://mcljc.blog.163.com/blog/static/83949820102239610974/
/*
基于Qt做了修改

*/
QSize ImageInfo::getJPGDimension(QString path)
{
    long int width = 0,height = 0;
    QFile File(path);
    QByteArray jpg_data;
    if(!File.open (QFile::ReadOnly))
    {
         return QSize(0, 0);
    }
    jpg_data = File.readAll ();
    File.close ();
    QFileInfo fileInfo(path);
    QByteArray list;
    QByteArray list_C1;
    list.insert (0,0xFF);
    list.insert (1,0xC0);
    list_C1.insert (0,0xFF);
    list_C1.insert (1,0xC1);
    int C0_Pos_L = jpg_data.lastIndexOf (list);
    int C1_Pos_L = jpg_data.lastIndexOf (list_C1);
    /*
    FF C0 00 11 08 01 68 02 80 03 01 22 00 02 11 01 03 11 01

        0x00 11，表示SOF包含了17个字节信息；

        0x08，表示采样精度，一个像素点可以有256级过渡，即28=2562^8=25628=256

        0x01 68:表示图像的高度,360

        0x02 80:表示图像的宽度,640
*/
    if(C1_Pos_L != -1)//说明存在FF C1
    {
        //获取图像高度
        width = (jpg_data.at(C0_Pos_L+7)<<8)+jpg_data.at(C0_Pos_L+8);
        height  = (jpg_data.at(C0_Pos_L+5)<<8)+jpg_data.at(C0_Pos_L+6);
    }
    else//说明存在FF C0
    {
        //获取图像高度
        width = (jpg_data.at(C0_Pos_L+7)<<8)+jpg_data.at(C0_Pos_L+8);
        height  = (jpg_data.at(C0_Pos_L+5)<<8)+jpg_data.at(C0_Pos_L+6);
    }

    return QSize(width, height);
}
//PNG文件头的第9字为文件宽度信息
//PNG文件头的第10字为文件高度信息
//参考：http://blog.chinaunix.net/uid-25799257-id-3358174.html
QSize ImageInfo::getPNGDimension(std::string path)
{
    FILE *fid = NULL;
    if((fid=fopen(path.c_str(),"rb+"))==NULL) {
        qDebug()<<"打开文件失败";
        return QSize(0, 0);
    }
    long int width;
    long int height;
    unsigned char wtmp[4]={'0'};   //宽度
    unsigned char htmp[4]={'0'};   //高度
    fseek(fid, 16, SEEK_SET);
    fread(wtmp, 4, 1, fid);         // example 00000080
    fread(htmp, 4, 1, fid);         // example 00000080
    fclose(fid);
    width = ((int)(unsigned char)wtmp[2]) * 256 + (int)(unsigned char)wtmp[3];
    height = ((int)(unsigned char)htmp[2]) * 256 + (int)(unsigned char)htmp[3];
    qDebug()<<"width="<<width;
    qDebug()<<"height="<<height;
    return QSize(width, height);
}
//GIF文件头的第4字为文件宽度信息
//GIF文件头的第5字为文件高度信息
//参考：http://blog.csdn.net/zhaoweikid/article/details/156422
//参考：http://blog.csdn.net/asaasa66/article/details/5875340
QSize ImageInfo::getGIFDimension(std::string path)
{
    std::ifstream ffin(path.c_str(), std::ios::binary);
    if (!ffin){
        std::cout<<"Can not open this file."<<std::endl;
        return QSize(0, 0);
    }
    long int width;
    long int height;
    char s1[2] = {0}, s2[2] = {0};
    ffin.seekg(6);
    ffin.read(s1, 2);
    ffin.read(s2, 2);
    width = (unsigned int)(s1[1])<<8|(unsigned int)(s1[0]);
    height = (unsigned int)(s2[1])<<8|(unsigned int)(s2[0]);
    ffin.close();
    qDebug()<<"width="<<width;
    qDebug()<<"height="<<height;
    return QSize(width, height);
}
QSize ImageInfo::getImageDimension(QString imageUrl)
{
    QSize dimension;
    if(!imageUrl.isEmpty()) {
        QUrl fileUrl(imageUrl);
        QString filePath = fileUrl.toLocalFile();
        if(QFile::exists(filePath)) {
            std::string path = filePath.toStdString();
            int iFormat = getImageFormat(path);
            switch(iFormat) {
            case BMP_FORMAT:
                dimension = getBMPDimension(path);
                break;
            /*
            case JPG_FORMAT:
                dimension = getJPGDimension(path);
                break;
            */
            case GIF_FORMAT:
                dimension = getGIFDimension(path);
                break;
            case PNG_FORMAT:
                dimension = getPNGDimension(path);
                break;
            default:
                break;
            }
        }
    }
    qDebug()<<"图片尺寸:"<<dimension;
    return dimension;
}
QString ImageInfo::getImageTitle(QString imageUrl)
{
    QString title;
    if(!imageUrl.isEmpty()) {
        QUrl fileUrl(imageUrl);
        QString filePath = fileUrl.toLocalFile();
        if(QFile::exists(filePath)) {
            QFileInfo fileinfo(filePath);
            title = fileinfo.baseName();
        }
    }
    return title;
}    
