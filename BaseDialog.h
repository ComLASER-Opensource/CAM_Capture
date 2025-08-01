/**
 * @file BaseDialog.h
 
 * @brief 다양한 위젯을 넣어서 사용할 수 있는 기본 다이얼로그
 * @version 0.1
 * @date 2023-12-06
 * 

 * 
 */

#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>
#include "SelfTestWidget.h"
#include "FileFormat.h"

enum Dialog
{
    SelfTestWarningMessageWidgetType,
    LoginExpiredDateWidgetType,
    IndicatorGPSWidgetType,
    Setting1LocationWidgetType,
    Setting3SystemInfoWidgetType,
    Setting3FactoryDefaultWidgetType,
    AdminPWWidgetType,
    PasswordChangingWidgetType,
    NetworkPasswordChangingWidgetType,
    NetworkPWWidgetType,
    WifiSearchWidgetType,
    BluetoothSearchFilterWidgetType,
    EnforcementWarningMessageWidgetType,
    FileManagerErrorMessageWidgetType,
    FileManagerQuestionMessageWidgetType,
    FileManagerFileTransferWidgetType,
    SearchBoxType,
    SdCardMemoryLackType,
};

class CustomPushButton;
namespace Ui {
class BaseDialog;
}

/**
 * @brief 다양한 위젯을 넣어서 사용할 수 있는 기본 다이얼로그 클래스
 * 
 */
class BaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseDialog(Dialog, Qt::Alignment = Qt::AlignmentFlag::AlignLeft, QString = "", bool = false, QString title_text = "", QWidget *parent = 0);
    explicit BaseDialog(Dialog, Status, Status, Status, Status, Qt::Alignment = Qt::AlignmentFlag::AlignLeft, QWidget *parent = 0);
//    explicit BaseDialog(Dialog, Qt::Alignment = Qt::AlignmentFlag::AlignLeft, QWidget* pWidget = nullptr, QWidget *parent = 0);
    virtual ~BaseDialog();
    void setAvFileFormatList(QList<AVFileFormat> avFileFormatList);
    void SetSelfTestResult(bool camera, bool laser, bool battery, bool storage);
    QWidget *pWidget() const;

private:
    void setSize(QSize size);
    void setSize(int w, int h);
    void drawBorder(QRect);
private:
    Ui::BaseDialog *ui;
    bool m_bIsCloseButton = false;
    bool m_isCamera;
    bool m_isLaser;
    bool m_isBattery;
    bool m_isStorage;
    QString m_strRet;
    CustomPushButton* m_pClosePushButton = nullptr;
//    QWidget* m_pWidget = nullptr;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
private slots:
    void on_closePushButton_clicked();
    void changeConnectingState(bool);

signals:
    void sig_searchingWIFIDone();
};

#endif // BASEDIALOG_H
