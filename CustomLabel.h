/**
 * @file CustomLabel.h
 
 * @brief QLabel 상속하여 공통으로 사용하는 Label 위젯
 * @version 0.1
 * @date 2023-12-06
 * 

 * 
 */

#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>

/**
 * @brief QLabel 상속하여 공통으로 사용하는 Label 위젯 클래스
 * 
 */
class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CustomLabel(QWidget *parent = 0);
//    void setIconImage(QString path_name, QString file_name);
    void setImageFromAvi(QString file_path, QSize size);
    void setImage(QImage image);
    void setImage(QString file_path, QSize size);
    void setImage(QString path_name, QString file_name);
    void setImage(QString path_name, QString file_name, QSize size);
    void setZoom(double factor, QRect rect);
    void setColor(QColor);
    void setFontSize(int);
private:
    QPixmap scaleImage(double factor);
private:
    QPixmap m_pixmap;
    QPixmap m_fitpixmap;
    QString m_styleSheet;
signals:

public slots:


    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // CUSTOMLABEL_H
