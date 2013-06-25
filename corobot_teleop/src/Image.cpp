 #include <QGraphicsScene>
 #include <QPainter>
 #include <QStyleOption>
 #include <ctime>
 #include "Image.h"


/**
 * transform a Qimage of any size into a QGraphicsItem  
 */

 Image::Image()
 {
 }

 QRectF Image::boundingRect() const
 {
     qreal adjust = 2;
     int height = image.height();
     int width = image.width();
     return QRectF(0, 0,width + adjust, width + adjust);
 }

 void Image::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
 {
     painter->drawImage(0,0,image);
 }

 void Image::setImage(QImage& image)
 //set the QImage
 {
    this->image = image;

 }


void Image::saveImage(bool save) 
// save a picture of the current image stream on the hardrive. The date and time is given in the file name
{
    QString path;
    time_t t = time(0);   // get time now
    struct tm * now = localtime(&t);
    path = QString("../picture-") + QString::number(now->tm_year + 1900) + QString("-") + QString::number(now->tm_mon + 1) + QString("-") + QString::number(now->tm_mday) + QString(".jpg");

    this->image.save(path,"JPG");
}