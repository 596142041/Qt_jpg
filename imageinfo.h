/*
* Authors:
* Simba <lansheng228@163.com>
*
*/
#ifndef IMAGEINFO_H
#define IMAGEINFO_H
#include <stdio.h>
#include <stdlib.h>
#include <QObject>
#include <QDebug>
#include <QUrl>
#include <string>
#include <QSize>
#include <QDate>
#include <iostream>
#include <fstream>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QFile>
#include <QByteArray>
class ImageInfo : public QObject
{
    Q_OBJECT
    enum IMAGE_FORMAT{
        BMP_FORMAT,
        JPG_FORMAT,
        GIF_FORMAT,
        PNG_FORMAT,
        NVL_FORMAT
    };
public:
    explicit ImageInfo(QObject *parent = 0);
    ~ImageInfo();
public:
    Q_INVOKABLE QString getImageFormat(QString imageUrl);
    Q_INVOKABLE QString getImageSize(QString imageUrl);
    Q_INVOKABLE QSize getImageDimension(QString imageUrl);
    Q_INVOKABLE QDate getImageDate(QString imageUrl);
    Q_INVOKABLE QString getImageTitle(QString imageUrl);
    QSize getJPGDimension(QString path);
Q_SIGNALS:
public Q_SLOTS :
private:
    int getImageFormat(std::string path);
    long getBMPSize(std::string path);
    long getGIFSize(std::string path);
    long getPNGSize(std::string path);
    long getJPGSize(std::string path);
    QSize getBMPDimension(std::string path);
    QSize getPNGDimension(std::string path);
    unsigned char state;
    unsigned char *buffer;
    QSize getGIFDimension(std::string path);
};
#endif // IMAGEINFO_H

