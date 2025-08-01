/**
 * @file IndicatorDialog.h
 
 * @brief 메인메뉴의 인디케이터를 클릭할 때 나오는 다이얼로그
 * @version 0.1
 * @date 2023-12-06
 * 

 * 
 */

#ifndef INDICATORCAMERADIALOG_H
#define INDICATORCAMERADIALOG_H

#include <QDialog>
#include <QtDebug>

#include "ConfigManager.h"
#include "SerialLaserManager.h"
#include "IndicatorCameraFocusWidget.h"

static void thread_CommandExcute2(QString strCommand)
{
    int result = std::system(strCommand.toStdString().c_str());

    qDebug() << strCommand << " : " << result;
}

enum NetworkState {
    Active,
    InActive,
    NotConnected,
};

class CustomPushButton;
class IndicatorCameraExposeWidget;
class IndicatorCameraFocusWidget;
class MainMenuWidget;
namespace Ui {
class IndicatorDialog;
}

/**
 * @brief 메인메뉴의 인디케이터를 클릭할 때 나오는 다이얼로그 클래스
 * 
 */
class IndicatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IndicatorDialog(QWidget *parent = 0);
    ~IndicatorDialog();
    void setFocusExposeDisabled(bool = false);
    bool GetGPSStatus();
    bool GetWifiStatus();
    void SetMainMenu(MainMenuWidget*);
    void setIndicatorImage(CustomPushButton* pCustomButton, QString path_name, QString file_name);
    void enforcementIMode();
    void enforcementAMode();
    void enforcementVMode();
public slots:
    void dnnComboboxIndexChangedFromExpose(int);
private:
    void clearSecondRow();
    void initlize();
    bool isExistSecondWidget();

private slots:
    void on_cameraPushButton_clicked();
    void on_cameraExposeClicked();
    void on_cameraFocusClicked();

    void on_daynNightPushButton_clicked();
    void on_day1WidgetClicked();
    void on_day2WidgetClicked();
    void on_day3WidgetClicked();
    void on_night1WidgetClicked();
    void on_night2WidgetClicked();
    void on_night3WidgetClicked();
    void on_gpsPushButton_clicked();

    void on_speedPushButton_clicked();
    void on_speedSTPushButton_clicked();
    void on_speedLTPushButton_clicked();


    void on_enforcementPushButton_clicked();
    void on_enforcementIPushButton_clicked();
    void on_enforcementAPushButton_clicked();
    void on_enforcementVPushButton_clicked();

    void on_weatherPushButton_clicked();
    void on_clicked_sunny();
    void on_clicked_rainy();

    void on_comPushButton_clicked();
    void on_pushButton_clicked();
    void on_screenRecordingPushButton_clicked();
    void on_screenCapturePushButton_clicked();

    void doCheckNetwork();
    void on_wifiPushButton_clicked();
    void on_BTPushButton_clicked();
    void on_EthernetPushButton_clicked();

    void EnforcementClose();
    void DisableSpeedMode();
public:
    CustomPushButton* m_pCameraFocusPushButton = nullptr;
    CustomPushButton* m_pCameraExposePushButton = nullptr;
    CustomPushButton* m_pDaynNightPushButton = nullptr;
    CustomPushButton* m_pEnforcementPushButton = nullptr;
    CustomPushButton* m_pWeatherPushButton = nullptr;
    CustomPushButton* m_pSpeedModePushButton = nullptr;
    CustomPushButton* m_pSTPushButton = nullptr;
    CustomPushButton* m_pLTPushButton = nullptr;
    CustomPushButton* m_pScreenRecordPushButton = nullptr;


    bool m_bFocusExposeDisabled = true;
    bool m_bEnforcement = false;
    void setPSerialLaserManager(SerialLaserManager *newPSerialLaserManager);

    void setPSerialViscaManager(SerialViscaManager *newPSerialViscaManager);

private:
    Ui::IndicatorDialog *ui;
    CustomPushButton* m_pDay1PushButton;
    CustomPushButton* m_pDay2PushButton;
    CustomPushButton* m_pDay3PushButton;
    CustomPushButton* m_pNight1PushButton;
    CustomPushButton* m_pNight2PushButton;
    CustomPushButton* m_pNight3PushButton;

    CustomPushButton* m_pSunnyPushButton;
    CustomPushButton* m_pRainyPushButton;
    CustomPushButton* m_pImagePushButton;
    CustomPushButton* m_pImageVideoPushButton;
    CustomPushButton* m_pVideoPushButton;
    CustomPushButton* m_pWifiPushButton = nullptr;
    CustomPushButton* m_pBTPushButton = nullptr;
    CustomPushButton* m_pEthernetPushButton = nullptr;
    ConfigManager m_configManager1 = ConfigManager("parameter_setting1.json");
    ConfigManager m_configManager2 = ConfigManager("parameter_setting2.json");
    ConfigManager m_configManager3 = ConfigManager("exposure.json");
    ConfigManager m_configManager4 = ConfigManager("focus.json");
    QJsonObject m_jsonObject1;
    QJsonObject m_jsonObject2;
    QJsonObject m_jsonObject3;
    QJsonObject m_jsonObject4;
    MainMenuWidget* m_pMainMenuWidget = nullptr;
    SerialLaserManager* m_pSerialLaserManager = nullptr;
    SerialViscaManager* m_pSerialViscaManager = nullptr;
    bool m_nUserMode = true;
    QTimer m_pTimer;
    NetworkState m_nEthernetState;
    NetworkState m_nWifiState;
    QString m_srFileFullName="";

//    SerialLaserManager* m_pSerialLaserManager;
    // QWidget interface
signals:
    void sig_Night();
    void sig_STMode();
    void sig_LTMode();
    void sig_EnforcementModeI();
    void sig_EnforcementModeV();
    void sig_EnforcementModeA();
    void sig_Hide();
    void sig_Show();
    void sig_screenShot();
    void sig_dayNightChanged(int);
protected:
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void hideEvent(QHideEvent *event);
private:
    bool m_isRecording = false;

    // QWidget interface
protected:
    void showEvent(QShowEvent *event);
};

#endif // INDICATORCAMERADIALOG_H
