/**
 * @file PasswordChangingWidget.h
 
 * @brief 패스워드 변경 위젯 BaseDialog에 삽입된다.
 * @version 0.1
 * @date 2023-12-06
 * 

 * 
 */

#ifndef PASSWORDCHANGINGWIDGET_H
#define PASSWORDCHANGINGWIDGET_H

#include <QWidget>

namespace Ui {
class PasswordChangingWidget;
}

/**
 * @brief 패스워드 변경 위젯 클래스
 * 
 */
class PasswordChangingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordChangingWidget(bool, QWidget *parent = 0);
    ~PasswordChangingWidget();
private:
    QDialog* m_pParent;
private slots:
    void on_okPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::PasswordChangingWidget *ui;
    bool m_bNetwork;
signals:
    void sig_sendPW(QString);
};

#endif // PASSWORDCHANGINGWIDGET_H
