/**
 * @file Setting6Widget.h
 
 * @brief 새팅6 화면 위젯
 * @version 0.1
 * @date 2023-12-06
 * 

 * 
 */

#ifndef SETTING6WIDGET_H
#define SETTING6WIDGET_H

#include <QWidget>
#include <QJsonObject>

#include "ConfigManager.h"

namespace Ui {
class Setting6Widget;
}

/**
 * @brief 세팅6 화면 위젯 클래스
 * 
 */
class Setting6Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Setting6Widget(QWidget *parent = 0);
    ~Setting6Widget();
    void SaveConfig();
    void initializeStringTable();
private slots:
    void on_searchPushButton_clicked();

    void on_languageComboBox_currentIndexChanged(const QString &arg1);
    void on_languageComboBox_currentIndexChanged(int index);

    void on_bluetoothComboBox_currentIndexChanged(int index);

    void on_ftpComboBox_currentIndexChanged(int index);

    void on_ftpPortLineEdit_textChanged(const QString &arg1);

    void on_userNameLineEdit_textChanged(const QString &arg1);

    void on_userPassLineEdit_textChanged(const QString &arg1);

private:
    QJsonObject m_jsonObject;
    QJsonObject m_newJsonObject;
    ConfigManager m_config = ConfigManager("parameter_setting6.json");

private:
    Ui::Setting6Widget *ui;

protected slots:
    void on_text(QString);
};

#endif // SETTING6WIDGET_H
