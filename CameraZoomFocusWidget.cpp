#include "CameraZoomFocusWidget.h"
#include "ui_CameraZoomFocusWidget.h"

#include <QHeaderView>
#include <QJsonArray>

#include "StringLoader.h"
#include "SpeedUnitManager.h"
#include "ConfigManager.h"
#include "SerialLaserManager.h"
#include "SerialPacket.h"
#include "ViscaPacket.h"
#include "FileManager.h"
#include "camera.h"
#include "Logger.h"
#include "WidgetSize.h"

using namespace CAM_Capture;

QStringList lt_day_focus = {"2A83", "2CED", "2DF9", "2DFC", "2D01", "2CA2"};
QStringList lt_night_focus = {"2A83", "2CED", "2DF9", "2DFC", "2D01", "2CA2"};

extern SerialLaserManager* g_pSerialLaserManager;
extern SerialViscaManager* g_pSerialViscaManager;

CameraZoomFocusWidget::CameraZoomFocusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraZoomFocusWidget)
{
    ui->setupUi(this);

//    ui->zoomLabel->setText("Z: 2FCE");
//    ui->focusLabel->setText("F: 1585");
    m_pSerialLaserManager =  g_pSerialLaserManager;
    m_pSerialViscaManager = g_pSerialViscaManager;

    ui->optPushButton->setText(LoadString("IDS_OPT"));
    ui->optPushButton->setFontSize(23);
    ui->focusPlusPushButton->setText(LoadString("IDS_FOCUS_PLIS"));
    ui->focusPlusPushButton->setFontSize(23);
    ui->FocusMinusPushButton->setText(LoadString("IDS_FOCUS_MINUS"));
    ui->FocusMinusPushButton->setFontSize(23);

    ui->dayComboBox->addItem(LoadString("IDS_DAY"));
    ui->dayComboBox->setFontSize(23);
    ui->dayComboBox->addItem(LoadString("IDS_NIGHT"));
    ui->dayComboBox->setFontSize(23);

    ui->autoTriggerPushButton->setText(LoadString("IDS_READY"));
    ui->autoTriggerPushButton->setFontSize(23);
    ui->initPushButton->setText(LoadString("IDS_INIT"));
    ui->initPushButton->setFontSize(23);

    ui->zoomRangePushButton->setText(LoadString("IDS_ZOOM_60_100"));
    ui->zoomRangePushButton->setFontSize(23);
    ui->dzPlusPushButton->setText(LoadString("IDS_DZ_PLUS"));
    ui->dzPlusPushButton->setFontSize(23);
    ui->dzMinusPushButton->setText(LoadString("IDS_DZ_MINUS"));
    ui->dzMinusPushButton->setFontSize(23);

    ui->pgrsSavePushButton->setText(LoadString("IDS_PQRS_SAVE"));
    ui->pgrsSavePushButton->setFontSize(23);
    ui->jpgSavePushButton->setText(LoadString("IDS_JPG_SAVE"));
    ui->jpgSavePushButton->setFontSize(23);

    ui->autoTriggerPushButton->setCheckable(true);

    ui->focusLabel->setColor(Qt::white);
    ui->dFocusLabel->setColor(Qt::white);
    ui->zoomLabel->setColor(Qt::white);

    m_pSpeedSensitivitylabel = new CustomLabel(this);
    m_pSpeedSensitivitylabel->setStyleSheet("color: white; background: transparent;");
    m_pSpeedSensitivitylabel->setAlignment(Qt::AlignCenter);
    m_pSpeedSensitivitylabel->setGeometry(GetWidgetSizePos(QRect(QPoint(464,595-125), QSize(678, 115))));

//    ui->speedSensitivitylabel->setColor(Qt::white);

//    ui->autoTriggerPushButton->setEnabled(true);

    ConfigManager con = ConfigManager("parameter_setting1.json");
    QJsonObject object = con.GetConfig();


    if (object["speed selection"].toInt() == 1)
        m_UserModeOn = true;
    else
        m_UserModeOn = false;

    ConfigManager config = ConfigManager("zoom_level.json");
    QJsonObject object2 = config.GetConfig();
    QJsonArray m_captureSpeed;// = object2["st mode meter dist"].toArray();

    if (distance() == meter)
        m_captureSpeed = object2["lt mode meter dist"].toArray();
    else
        m_captureSpeed = object2["lt mode feet dist"].toArray();
    foreach(auto item, m_captureSpeed)
    {
        m_ltmetervector.push_back(item.toString());
    }
    if (m_ltmetervector.size() == 5)
    {
        if (distance() == meter)
        {
            m_ltmetervector.push_back("Z:260");
        }
        else
        {
            m_ltmetervector.push_back("Z:850");
        }
    }
//    m_captureSpeed = object2["lt mode feet dist"].toArray();

    m_object2 = m_coofigManager2.GetConfig();
    m_object3 = m_coofigManager3.GetConfig();


//    ui->tableWidget->setColumnWidth(0, ui->pgrsSavePushButton->width() / 2);
//    ui->tableWidget->setColumnWidth(1, ui->pgrsSavePushButton->width() * 1.5);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableWidget

    QStringList rowHeaders;
    rowHeaders.append(LoadString("IDS_DAY"));
    rowHeaders.append(LoadString("IDS_NIGHT"));
    ui->tableWidget->setHorizontalHeaderLabels(rowHeaders);

    QStringList columnHeaders;
    foreach (auto item, m_ltmetervector)
    {
        columnHeaders.append(item);
    }
//    columnHeaders.append(LoadString("IDS_Z10_36"));
//    columnHeaders.append(LoadString("IDS_Z36_60"));
//    columnHeaders.append(LoadString("IDS_Z60_100"));
//    columnHeaders.append(LoadString("IDS_Z100_160"));
//    columnHeaders.append(LoadString("IDS_Z160"));
//    columnHeaders.append(LoadString("IDS_Z260"));
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setPixelSize( 12 );
    ui->tableWidget->horizontalHeader()->setFont( font );
    ui->tableWidget->verticalHeader()->setFont( font );
    ui->tableWidget->setVerticalHeaderLabels(columnHeaders);
    ui->pgrsSavePushButton->setDisabled(true);

    m_nTableIndex = QPoint(m_nLtIndex,m_nDNIndex);
    m_object = config.GetConfig();

    ui->zoomRangePushButton->setText(m_ltmetervector[m_nLtIndex]+distanceValue());

    setTableInit();

//    m_serialViscaManager.connectVisca();

//    if (m_pSerialLaserManager == nullptr)
//        m_pSerialLaserManager = new SerialLaserManager;

    m_pSerialViscaManager->show_dzoomPosition();

    connect(m_pSerialLaserManager->getLaser_packet(), SIGNAL(sig_showDistance(float,int)), this, SLOT(on_showDistance(float,int)));

    connect(m_pSerialViscaManager->getVisca_packet(), SIGNAL(sig_show_dzoom(QString)), this, SLOT(on_show_dzoom(QString)));
    connect(m_pSerialViscaManager->getVisca_packet(), SIGNAL(sig_show_zoom(QString)), this, SLOT(on_show_zoom(QString)));
    connect(m_pSerialViscaManager->getVisca_packet(), SIGNAL(sig_show_focus(QString)), this, SLOT(on_show_focus(QString)));

    connect(&m_ClearTimer, SIGNAL(timeout), this, SLOT(ClearDisplay()));

    camInit();
    laserInit();
    m_hud.HUDZoomFocusInit();

    ConfigManager configManager = ConfigManager("setting_reticle.json");
    QJsonObject m_object = configManager.GetConfig();
    QJsonArray ar = m_object["Camera reticle pos"].toArray();
    m_LaserPoint = QPoint(ar[0].toInt() - Laser_x, ar[1].toInt() - Laser_y);

    SendViscaValue();
}

CameraZoomFocusWidget::~CameraZoomFocusWidget()
{
    m_hud.HUDClear();

    m_pSerialLaserManager->stop_laser();
    m_pSerialLaserManager->request_distance(false);

//    delete m_pSerialLaserManager;
//    m_serialViscaManager.close();
    delete m_pSpeedSensitivitylabel;
    delete ui;
}

void CameraZoomFocusWidget::SetCamera(Camera *camera)
{
    if (camera != nullptr)
        m_pCamera = camera;
}

void CameraZoomFocusWidget::setPSerialLaserManager(SerialLaserManager *newPSerialLaserManager)
{
    m_pSerialLaserManager = newPSerialLaserManager;

    connect(m_pSerialLaserManager->getLaser_packet(), SIGNAL(sig_showDistance(float,int)), this, SLOT(on_showDistance(float,int)));
}

void CameraZoomFocusWidget::ZoomRange()
{
    int zoom_index = 0;
    m_nLtIndex++;
    if (m_nLtIndex >= m_ltmetervector.size())
        m_nLtIndex = 0;

    m_nTableIndex.setX(m_nLtIndex);
    m_nTableIndex.setY(m_nDNIndex);

    zoom_index = m_nLtIndex;
    ui->zoomRangePushButton->setText(m_ltmetervector[m_nLtIndex]+distanceValue());

    int ndaynight;
    if(ui->dayComboBox->currentIndex() == 0)
    {
       ndaynight = 1;
    }else
    {
       ndaynight = 4;
    }

    m_pSerialViscaManager->SetZoomForZoomFocus(zoom_index);
    setZoomGoal(zoom_index);
    m_pSerialViscaManager->SetFocusForZoomFocus(zoom_index, ndaynight);
    setFocusGoal(zoom_index, ndaynight);
    m_pSerialViscaManager->SetDZoomForZoomFocus(zoom_index);

    SetLogMsg(BUTTON_CLICKED, "ZOOM_INDEX, " + ui->zoomRangePushButton->text());

    SetLaserDetectionAreaDistance(zoom_index);

    if (m_mTableStatus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())] == 1)
        ui->pgrsSavePushButton->setDisabled(false);
    else
        ui->pgrsSavePushButton->setDisabled(true);

    SendViscaValue();
}

void CameraZoomFocusWidget::on_optPushButton_clicked()
{
    m_pSerialViscaManager->set_AF_one_push_trigger();
//    m_pSerialViscaManager->m_pTimerCheckOPTdone->start(500);
    m_pSerialViscaManager->show_focusPosition();
}


void CameraZoomFocusWidget::on_focusPlusPushButton_clicked()
{
    m_pSerialViscaManager->plus_focus();
//    ui->focusLabel->setText("F:"+m_pSerialViscaManager->getFocus_pqrs().toUpper()+"("+m_pSerialViscaManager->getFocus_pqrs().toUpper()+")");
}


void CameraZoomFocusWidget::on_FocusMinusPushButton_clicked()
{
    m_pSerialViscaManager->minus_focus();
//    ui->focusLabel->setText("F:"+m_pSerialViscaManager->getFocus_pqrs().toUpper()+"("+m_pSerialViscaManager->getFocus_pqrs().toUpper()+")");
}

/*
void CameraZoomFocusWidget::on_dayComboBox_currentIndexChanged(int index)
{
    m_nTableIndex.setY(index);
    ConfigManager configManager = ConfigManager("exposure.json");
    QJsonObject object1 = configManager.GetConfig();
    QJsonObject object2;

    bool isAutoIris = false;
    if (index == 0)
    {
        m_pSerialViscaManager->set_infrared_mode_off();
        object2 = object1["Day"].toObject()["Dark"].toObject();
        isAutoIris = true;
    }
    else
    {
        m_pSerialViscaManager->set_infrared_mode_on();
        object2 = object1["Night"].toObject()["Dark"].toObject();
    }
//    m_pSerialViscaManager->set_AE_Mode("03");
//    m_pSerialViscaManager->set_iris(object["Iris"].toInt());
//    m_pSerialViscaManager->set_shutter_speed(object["Shutter"].toInt());
//    m_pSerialViscaManager->set_gain(object["Gain"].toInt());
//    m_pSerialViscaManager->set_AE_Mode(object["Priority"].toString());
    m_pSerialViscaManager->set_AE_Mode("03");
    m_pSerialViscaManager->set_iris_from_pq(object["Iris"].toString(), isAutoIris);
    m_pSerialViscaManager->set_shutter_speed_from_pq(object["Shutter"].toString());
    m_pSerialViscaManager->set_gain_from_pq(object["Gain"].toString());
    m_pSerialViscaManager->set_AE_Mode(object["Priority"].toString());

    m_pSerialViscaManager->set_noise_reduction_on(object["DNR"].toString());
    object["DIS"].toBool() ? m_pSerialViscaManager->set_DIS_on() : m_pSerialViscaManager->set_DIS_off();
    object["DEFOG"].toBool() ? m_pSerialViscaManager->set_defog_on() : m_pSerialViscaManager->set_defog_off();
    object["HLC"].toBool() ? m_pSerialViscaManager->set_HLC_on() : m_pSerialViscaManager->set_HLC_off();

    object = ConfigManager("focus.json").GetConfig();
    QJsonArray ar;
    if (index == 0)
    {
        ar = object["lt day focus"].toArray();
        m_pSerialViscaManager->set_focus(ar[m_nLtIndex].toString());
    }
    else
    {
        ar = object["lt night focus"].toArray();
        m_pSerialViscaManager->set_focus(ar[m_nLtIndex].toString());
    }

//    m_pSerialViscaManager->set_focus()

    if (m_mTableStatus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())] == 1)
        ui->pgrsSavePushButton->setDisabled(false);
    else
        ui->pgrsSavePushButton->setDisabled(true);

    SendViscaValue();
}*/

void CameraZoomFocusWidget::on_dayComboBox_currentIndexChanged(int index)
{
    m_pSerialViscaManager->set_IRCorrection_standard();
    m_pSerialViscaManager->set_AE_Mode("03");

    int ndaynight;
    if(ui->dayComboBox->currentIndex() == 0)
    {
       ndaynight = 1;
    }else
    {
       ndaynight = 4;
    }

    m_pSerialViscaManager->SetDayMode(ndaynight);

    m_pSerialViscaManager->set_manual_focus();
    //    m_pSerialViscaManager.set_AE_mode2e();
    m_pSerialViscaManager->separate_zoom_mode();

    //    Config
    m_pSerialViscaManager->SetZoomForZoomFocus(m_nLtIndex);
    setZoomGoal(m_nLtIndex);
    m_pSerialViscaManager->SetFocusForZoomFocus(m_nLtIndex, ndaynight);
    setFocusGoal(m_nLtIndex, ndaynight);
    m_pSerialViscaManager->SetDZoomForZoomFocus(m_nLtIndex);
}



void CameraZoomFocusWidget::on_initPushButton_clicked()
{
//    setTableInit();
    setTableDefualtInit();
    setFocusEditJsonInit();
}

void CameraZoomFocusWidget::SetLaserDetectionAreaDistance(int zoom_index)
{
   int distance = 0, area = 0;
    switch (zoom_index)
    {
    case 1:
    {
        if (m_UserModeOn)
        {
            distance = 20;
            area = 3;
        }
        else
        {
            distance = 18;
            area = 18;
        }
    }
        break;
    case 2:
    {
        if (m_UserModeOn)
        {
            distance = 40;
            area = 3;
        }
        else
        {
            distance = 48;
            area = 12;
        }

    }
        break;
    case 3:
    {
        if (m_UserModeOn)
        {
            distance = 60;
            area = 3;
        }
        else
        {
            distance = 80;
            area = 20;
        }

    }
        break;
    case 4:
    {
        if (m_UserModeOn)
        {
            distance = 80;
            area = 3;
        }
        else
        {
            distance = 130;
            area = 30;
        }

    }
        break;
    case 5:
    {
        if (m_UserModeOn)
        {
            distance = 100;
            area = 3;
        }
        else
        {
            distance = 1580;
            area = 1420;
        }

    }
        break;
    case 6:
    {
        if (m_UserModeOn)
        {
            distance = 120;
            area = 3;
        }

        else
        {
            distance = 1580;
            area = 1420;
        }

    }
        break;
    case 7:
    {
        if (m_UserModeOn)
        {
            distance = 140;
            area = 3;
        }
        else
        {
            distance = 1580;
            area = 1420;
        }

    }
        break;
    case 8:
    {
        if (m_UserModeOn)
        {
            distance = 160;
            area = 3;
        }
        else
        {
            distance = 1580;
            area = 1420;
        }

    }
        break;
    case 9:
    {
        if (m_UserModeOn)
        {
            distance = 180;
            area = 3;
        }
        else
        {
            distance = 1580;
            area = 1420;
        }

    }
        break;
    case 10:
    {
        if (m_UserModeOn)
        {
            distance = 200;
            area = 3;
        }
        else
        {
            distance = 1580;
            area = 1420;
        }

    }
        break;

    case 11:
    {
        if (m_UserModeOn)
        {
            distance = 220;
            area = 3;
        }
        else
        {
            distance = 1580;
            area = 1420;
        }

    }
        break;

    case 12:
    {
        if (m_UserModeOn)
        {
            distance = 240;
            area = 3;
        }
        else
        {
            distance = 1580;
            area = 1420;
        }

    }
        break;

    case 13:
    {
        if (m_UserModeOn)
        {
            distance = 260;
            area = 3;
        }
        else
        {
            distance = 1580;
            area = 1420;
        }

    }
        break;
    }

    m_pSerialLaserManager->set_detection_distance(distance);
    m_pSerialLaserManager->set_detection_area(area);
}

void CameraZoomFocusWidget::setTableInit()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(6);

    SetTableVerticalHeader();

    ConfigManager config = ConfigManager("focus_edit.json");
    QJsonObject object = config.GetConfig();

    QJsonArray ar = m_object3["lt day focus"].toArray();
    QJsonArray ar_edit = object["lt day focus edit"].toArray();
    for (int i = 0; i < ar.size(); i++ )
    {
//        int row = ui->tableWidget->rowCount();
//        ui->tableWidget->insertRow(row);
        m_MapFocus[std::make_pair(i, 0)] = ar[i].toString();
        if(ar_edit[i] == 0)
        {
            m_mTableStatus[std::make_pair(i, 0)] = 0;
        } else
        {
            m_mTableStatus[std::make_pair(i, 0)] = 2;
        }
//        QTableWidgetItem *item = new QTableWidgetItem(ar[i].toString().toUpper());
//        ui->tableWidget->setItem(i, 0, item);
    }

    ar = m_object3["lt night focus"].toArray();
    ar_edit = object["lt night focus edit"].toArray();
    for (int i = 0; i < ar.size(); i++ )
    {
        m_MapFocus[std::make_pair(i, 1)] = ar[i].toString();
        if(ar_edit[i] == 0)
        {
            m_mTableStatus[std::make_pair(i, 1)] = 0;
        } else
        {
            m_mTableStatus[std::make_pair(i, 1)] = 2;
        }
//        QTableWidgetItem *item = new QTableWidgetItem(ar[i].toString().toUpper());
//        ui->tableWidget->setItem(i, 1, item);
    }

    EditTableValue2();
}

void CameraZoomFocusWidget::setTableDefualtInit()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(6);
    SetTableVerticalHeader();
    for (int i = 0; i < lt_day_focus.size(); i++ )
    {
        m_MapFocus[std::make_pair(i, 0)] = lt_day_focus[i];
        m_mTableStatus[std::make_pair(i, 0)] = 0;
        QTableWidgetItem *item = new QTableWidgetItem(lt_day_focus[i].toUpper());
        ui->tableWidget->setItem(i, 0, item);
    }
//    m_MapFocus[std::make_pair(5, 0)] = "";
//    m_mTableStatus[std::make_pair(5, 0)] = 0;

    for (int i = 0; i < lt_night_focus.size(); i++ )
    {
        m_MapFocus[std::make_pair(i, 1)] = lt_night_focus[i];
        m_mTableStatus[std::make_pair(i, 1)] = 0;
        QTableWidgetItem *item = new QTableWidgetItem(lt_night_focus[i].toUpper());
        ui->tableWidget->setItem(i, 1, item);
    }
//    m_MapFocus[std::make_pair(5, 1)] = "";
//    m_mTableStatus[std::make_pair(5, 1)] = 0;


}

void CameraZoomFocusWidget::checkButtonEnable()
{
//    m_mTableStatus
}

void CameraZoomFocusWidget::setFocusEditJsonInit()
{
    ConfigManager config = ConfigManager("focus_edit.json");
    QJsonObject object = config.GetConfig();
    QJsonArray ar = {0, 0, 0, 0, 0, 0};
    object["lt day focus edit"] = ar;
    object["lt night focus edit"] = ar;

    config.SetConfig(object);
    config.SaveFile();

//    ui->tableWidget->clearContents();
}

void CameraZoomFocusWidget::SaveJpg()
{
    QString filename = GetSubPath("/factory", eMMC) + "/" + GetFileName(SC);

    QPixmap pixmap = m_pCamera->grab();
    pixmap.save(filename);
//    m_pCamera->SaveImage(filename);
}

void CameraZoomFocusWidget::ModifyFocusEditJson(int x, int y, int value)
{
    ConfigManager config = ConfigManager("focus_edit.json");
    QJsonObject object = config.GetConfig();
    QJsonArray ar = object["lt day focus edit"].toArray();
    QJsonArray ar2 = object["lt night focus edit"].toArray();
    if (y == 0)
        ar[x] = value;
    else
        ar2[x] = value;

    object["lt day focus edit"] = ar;
    object["lt night focus edit"] = ar2;

    config.SetConfig(object);
    config.SaveFile();

//    ui->tableWidget->clearContents();

}

void CameraZoomFocusWidget::SaveFocusJson()
{
    ConfigManager config = ConfigManager("focus.json");
    QJsonObject object = config.GetConfig();
    QJsonArray ar = object["lt day focus"].toArray();
    QJsonArray ar2 = object["lt night focus"].toArray();
    QJsonArray ar3 = object["st day focus"].toArray();
    QJsonArray ar4 = object["st night focus"].toArray();

//    if (m_nTableIndex.y() == 0)
//    {
//        ar[m_nTableIndex.x()] = m_MapFocus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())];
//        SetLtValue(m_nTableIndex.x(), ar, ar3);
//    }
//    else
//    {
//        ar2[m_nTableIndex.x()] = m_MapFocus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())];
//        SetLtValue(m_nTableIndex.x(), ar2, ar4);
//    }


    for (int i = 0 ; i < 6 ; i++)
    {
        for( int j = 0 ; j < 2 ; j++)
        {
            if (m_mTableStatus[std::make_pair(i, j)] == 1)
            {
                if(j == 0)
                {
                    ar[i] = m_MapFocus[std::make_pair(i, j)];
                    SetStValue(i, ar, ar3);
                }
                else
                {
                    ar2[i] = m_MapFocus[std::make_pair(i, j)];
                    SetStValue(i, ar2, ar4);
                }
            }
        }
    }

    object["lt day focus"] = ar;
    object["lt night focus"] = ar2;
    object["st day focus"] = ar3;
    object["st night focus"] = ar4;

    config.SetConfig(object);
    config.SaveFile();

}

void CameraZoomFocusWidget::SaveFocusEditJson()
{
    ConfigManager config = ConfigManager("focus_edit.json");
    QJsonObject object = config.GetConfig();
    QJsonArray ar = object["lt day focus edit"].toArray();
    QJsonArray ar2 = object["lt night focus edit"].toArray();

    for (int i = 0 ; i < 6 ; i++)
    {
        for( int j = 0 ; j < 2 ; j++)
        {
            if (m_mTableStatus[std::make_pair(i, j)] == 2)
            {
                if(j == 0)
                {
                    ar[i] = 1;
                }
                else
                {
                    ar2[i] = 1;
                }
            }
        }
    }

    object["lt day focus edit"] = ar;
    object["lt night focus edit"] = ar2;

    config.SetConfig(object);
    config.SaveFile();

}

void CameraZoomFocusWidget::SaveDZoomJson()
{
    ConfigManager config = ConfigManager("dzoom.json");
    QJsonObject object = config.GetConfig();
    QJsonArray ar = object["lt dzoom"].toArray();

    ar[m_nLtIndex] = m_strDZoom;

    object["lt dzoom"] = ar;

    config.SetConfig(object);
    config.SaveFile();
}

void CameraZoomFocusWidget::SetStValue(int index, QJsonArray& ar, QJsonArray& ar2)
{
    bool bStatus = false;
    int ar4 = ar[4].toString().toInt(&bStatus, 16);
    int ar5 = ar[5].toString().toInt(&bStatus, 16);

    int A = (ar5 - ar4) / 5;

    switch(index)
    {
    case 0:
    {
        ar2[0] = ar[index];
    }
        break;
    case 1:
    {
        ar2[1] = ar[index];

    }
        break;
    case 2:
    {
        ar2[2] = ar[index];
        ar2[3] = ar[index];

    }
        break;
    case 3:
    {
        ar2[4] = ar[index];
        ar2[5] = ar[index];
        ar2[6] = ar[index];

    }
        break;
    case 4:
    {
        ar2[7] = ar[index];
        int ar7 = ar[index].toString().toInt(&bStatus, 16);
        QString hex;
        ar2[8] = hex.sprintf("%04X", ar7 + A);
        ar2[9] = hex.sprintf("%04X", ar7 + A * 2);
        ar2[10] = hex.sprintf("%04X", ar7 + A * 3);
        ar2[11] = hex.sprintf("%04X", ar7 + A * 4);
    }
        break;
    case 5:
    {
        int ar7 = ar[4].toString().toInt(&bStatus, 16);
        QString hex;
        ar2[8] = hex.sprintf("%04X", ar7 + A);
        ar2[9] = hex.sprintf("%04X", ar7 + A * 2);
        ar2[10] = hex.sprintf("%04X", ar7 + A * 3);
        ar2[11] = hex.sprintf("%04X", ar7 + A * 4);
        ar2[12] = ar[index];
    }
        break;
    }
}

void CameraZoomFocusWidget::SendViscaValue()
{
    m_pSerialViscaManager->show_dzoomPosition();
    m_pSerialViscaManager->show_zoomPosition();
    m_pSerialViscaManager->show_focusPosition();
}

void CameraZoomFocusWidget::EditTableValue()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(6);
    SetTableVerticalHeader();
    for (int i = 0 ; i < 6 ; i++)
    {
        for( int j = 0 ; j < 2 ; j++)
        {
            QString focus;
            if (m_nTableIndex.x() == i && m_nTableIndex.y() == j)
            {
                focus = m_currentFocus;
                m_MapFocus[std::make_pair(i, j)] = m_currentFocus;
            }
            else
            {
                focus = m_MapFocus[std::make_pair(i, j)];
            }
            QTableWidgetItem *item = new QTableWidgetItem(focus.toUpper());
            if (m_mTableStatus[std::make_pair(i, j)] == 1)
                item->setTextColor(Qt::red);
            if (m_mTableStatus[std::make_pair(i, j)] == 2)
                item->setTextColor(Qt::green);

            ui->tableWidget->setItem(i, j, item);
        }
    }
}

void CameraZoomFocusWidget::EditTableValue2()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(6);
    SetTableVerticalHeader();
    for (int i = 0 ; i < 6 ; i++)
    {
        for( int j = 0 ; j < 2 ; j++)
        {
            QString focus;
            focus = m_MapFocus[std::make_pair(i, j)];

            QTableWidgetItem *item = new QTableWidgetItem(focus.toUpper());
            if (m_mTableStatus[std::make_pair(i, j)] == 1)
                item->setTextColor(Qt::red);
            if (m_mTableStatus[std::make_pair(i, j)] == 2)
                item->setTextColor(Qt::green);

            ui->tableWidget->setItem(i, j, item);
        }
    }
}

void CameraZoomFocusWidget::SetTableVerticalHeader()
{
    QStringList columnHeaders;
    foreach (auto item, m_ltmetervector)
    {
        columnHeaders.append(item);
    }
    ui->tableWidget->setVerticalHeaderLabels(columnHeaders);

}

void CameraZoomFocusWidget::camInit()
{

    m_pSerialViscaManager->set_IRCorrection_standard();
    m_pSerialViscaManager->set_AE_Mode("03");

    int ndaynight;
    if(ui->dayComboBox->currentIndex() == 0)
    {
       ndaynight = 1;
    }else
    {
       ndaynight = 4;
    }

    m_pSerialViscaManager->SetDayMode(ndaynight);

    m_pSerialViscaManager->set_manual_focus();
    //    m_pSerialViscaManager.set_AE_mode2e();
    m_pSerialViscaManager->separate_zoom_mode();

    //    Config
    m_pSerialViscaManager->SetZoomForZoomFocus(m_nLtIndex);
    setZoomGoal(m_nLtIndex);
    m_pSerialViscaManager->SetFocusForZoomFocus(m_nLtIndex, ndaynight);
    setFocusGoal(m_nLtIndex, ndaynight);
    m_pSerialViscaManager->SetDZoomForZoomFocus(m_nLtIndex);

    //    ConfigManager config = ConfigManager("parameter_enforcement.json");
    //    QJsonObject object = config.GetConfig();

}

void CameraZoomFocusWidget::laserInit()
{
    ConfigManager config = ConfigManager("parameter_setting1.json");
    QJsonObject object = config.GetConfig();
    ConfigManager config2 = ConfigManager("parameter_setting2.json");
    QJsonObject object2 = config2.GetConfig();

    if (object2["weather selection"].toInt() == 1)
        m_pSerialLaserManager->set_weather_mode(0);
    else
        m_pSerialLaserManager->set_weather_mode(1);

    if (object2["anti-jamming selection"].toInt() == 1)
        m_pSerialLaserManager->set_AJamming_mode(1);
    else
        m_pSerialLaserManager->set_AJamming_mode(0);

    if (object2["buzzer selection"].toInt() == 1)
        m_pSerialLaserManager->set_buzzer_mode(1);
    else
        m_pSerialLaserManager->set_buzzer_mode(0);



    ConfigManager config3 = ConfigManager("parameter_enforcement.json");
    QJsonObject object3 = config3.GetConfig();

    int zoom_index = object3["zoom index"].toInt();

    SetLaserDetectionAreaDistance(zoom_index);

    int dn = object["day&night selection"].toInt();
    if (dn >= 0 && dn <=3)
        m_pSerialLaserManager->set_night_mode(0);
    else
        m_pSerialLaserManager->set_night_mode(1);
    m_pSerialLaserManager->set_speed_measure_mode(1);

}

void CameraZoomFocusWidget::on_zoomRangePushButton_clicked()
{
    ZoomRange();
//    SendViscaValue();
}


void CameraZoomFocusWidget::on_dzPlusPushButton_clicked()
{
    m_pSerialViscaManager->plus_dzoom();
    m_pSerialViscaManager->show_dzoomPosition();
}


void CameraZoomFocusWidget::on_dzMinusPushButton_clicked()
{
    m_pSerialViscaManager->minus_dzoom();
    m_pSerialViscaManager->show_dzoomPosition();
}

void CameraZoomFocusWidget::on_showDistance(float fDistance, int nSensitivity)
{
    if(fDistance == 9999.0)
        m_pSpeedSensitivitylabel->setText("----.-" + distanceValue() + "(" + QString::number(nSensitivity)+ ")");
    else
        m_pSpeedSensitivitylabel->setText(QString::number(getDistanceValue(fDistance), 'f', 1) + distanceValue() + "(" + QString::number(nSensitivity)+ ")");

    m_hud.HUDZoomFocus(fDistance);

    m_ClearTimer.start(200);
}


void CameraZoomFocusWidget::on_jpgSavePushButton_clicked()
{
    m_mTableStatus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())] = 1;

    EditTableValue();
//    if (m_mTableStatus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())] != 0)
//        return;
//    ui->tableWidget->setRowCount(0);
//    ui->tableWidget->setRowCount(6);
//    QJsonArray ar = m_object3["lt day focus"].toArray();
//    for (int i = 0; i < ar.size(); i++ )
//    {
//        QTableWidgetItem *item = new QTableWidgetItem(ar[i].toString());

//        if (m_nTableIndex.x() == 0 && m_nTableIndex.y() == i)
//            item->setTextColor(Qt::red);
//        ui->tableWidget->setItem(i, 0, item);
//    }

//    ar = m_object3["lt night focus"].toArray();
//    for (int i = 0; i < ar.size(); i++ )
//    {
//        QTableWidgetItem *item = new QTableWidgetItem(ar[i].toString());
//        if (m_nTableIndex.x() == 1 && m_nTableIndex.y() == i)
//            item->setTextColor(Qt::red);
//        ui->tableWidget->setItem(i, 1, item);
//    }

//    QTableWidgetItem* item = ui->tableWidget->item(, m_nTableIndex.y());
//    item->setTextColor(Qt::red);
//    ui->tableWidget->setItem(m_nTableIndex.x(), m_nTableIndex.y(), item);
    SaveJpg();

    ui->pgrsSavePushButton->setEnabled(true);
}


void CameraZoomFocusWidget::on_pgrsSavePushButton_clicked()
{
    if (m_mTableStatus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())] != 1)
        return;


    SaveFocusJson();



//    m_mTableStatus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())] = 2;

    for (int i = 0 ; i < 6 ; i++)
    {
        for( int j = 0 ; j < 2 ; j++)
        {
            if (m_mTableStatus[std::make_pair(i, j)] == 1)
            {
                m_mTableStatus[std::make_pair(i, j)] = 2;
            }
        }
    }

    EditTableValue2();

    SaveFocusEditJson();
//    QJsonArray ar = m_object3["lt day focus"].toArray();
//    for (int i = 0; i < ar.size(); i++ )
//    {
//        QTableWidgetItem *item = new QTableWidgetItem(ar[i].toString());

//        if (m_nTableIndex.x() == 0 && m_nTableIndex.y() == i)
//            item->setTextColor(Qt::green);
//        ui->tableWidget->setItem(i, 0, item);
//    }

//    ar = m_object3["lt night focus"].toArray();
//    for (int i = 0; i < ar.size(); i++ )
//    {
//        QTableWidgetItem *item = new QTableWidgetItem(ar[i].toString());
//        if (m_nTableIndex.x() == 1 && m_nTableIndex.y() == i)
//            item->setTextColor(Qt::green);
//        ui->tableWidget->setItem(i, 1, item);
//    }

//    QTableWidgetItem* item = ui->tableWidget->item(m_nTableIndex.x(), m_nTableIndex.y());
//    item->setTextColor(Qt::green);
//    ui->tableWidget->setItem(m_nTableIndex.x(), m_nTableIndex.y(), item);

//    ModifyFocusEditJson(m_nTableIndex.x(), m_nTableIndex.y());

    ui->pgrsSavePushButton->setEnabled(false);
}


//void CameraZoomFocusWidget::on_autoTriggerPushButton_toggled(bool checked)
//{
//    m_bATChecked = checked;
//    if (!checked)
//    {
//        ui->autoTriggerPushButton->setStyleSheet("border-color: blue;");
//        m_pSerialLaserManager->start_laser();
//        m_pSerialLaserManager->request_distance(true);

//        SerialPacket* laser_packet = m_pSerialLaserManager->getLaser_packet();
//        connect(laser_packet, SIGNAL(sig_showDistance(float,int)), this, SLOT(on_showDistance(float,int)));
//    }
//    else
//    {
//        ui->autoTriggerPushButton->setStyleSheet("border-color: red;");
//        SerialPacket* laser_packet = m_pSerialLaserManager->getLaser_packet();
//        disconnect(laser_packet, SIGNAL(sig_showDistance(float,int)), this, SLOT(on_showDistance(float,int)));

//        m_pSerialLaserManager->stop_laser();
//        m_pSerialLaserManager->request_distance(false);

//    }
//}

void CameraZoomFocusWidget::on_show_zoom(QString zoom)
{
//    ui->zoomLabel->
    ui->zoomLabel->setText("Z:"+zoom.toUpper()+"("+m_zoomGoal+")");
}

void CameraZoomFocusWidget::on_show_focus(QString focus)
{
    ui->focusLabel->setText("F:"+focus.toUpper()+"("+m_focusGoal+")");
    m_currentFocus = focus;
//    if (m_mTableStatus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())] == 0)
//    {
//        m_MapFocus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())] = focus;
//    }

//    EditTableValue();
//    QTableWidgetItem item(focus);
    //    ui->tableWidget->setItem(m_nTableIndex.x(), m_nTableIndex.y(), &item);
}

void CameraZoomFocusWidget::ClearDisplay()
{
    m_hud.HUDZoomFocusClear();

    m_pSpeedSensitivitylabel->setText("----.-" + distanceValue() + "(0)");
}

void CameraZoomFocusWidget::on_show_dzoom(QString dzoom)
{
    ui->dFocusLabel->setText("DZ:"+dzoom.toUpper());
    m_strDZoom = dzoom;

    //save dzoom pq value to dzoom.json
    SaveDZoomJson();
}


void CameraZoomFocusWidget::on_autoTriggerPushButton_clicked(bool checked)
{
    if (checked)
    {
        ui->autoTriggerPushButton->setStyleSheet("border-color: red;");
        ui->autoTriggerPushButton->setText(LoadString("IDS_AT"));
        m_pSpeedSensitivitylabel->show();
        m_pSerialLaserManager->start_laser();
        m_pSerialLaserManager->request_distance(true);
//        m_pSerialLaserManager->start_virtualSpeed();
    }
    else
    {
        ui->autoTriggerPushButton->setStyleSheet("border-color: blue;");
        ui->autoTriggerPushButton->setText(LoadString("IDS_READY"));
        m_pSpeedSensitivitylabel->hide();
        m_pSerialLaserManager->stop_laser();
        m_pSerialLaserManager->request_distance(false);
    }

//    SerialPacket* laser_packet = m_pSerialLaserManager->getLaser_packet();

}

void CameraZoomFocusWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen crossPen;
    crossPen.setColor(Qt::white);
    crossPen.setWidthF(0);

    crossPen.setStyle(Qt::SolidLine);
    //        crossPen.setWidth(10);
    int height2 = height() - m_pMainMenuWidgetSize.height();
    int width1 = width();
    int height1 = height2;
    int gap = 1;
    int reticle_width = 5;

    int x = m_LaserPoint.x() * 800 / 1920;
    int y = m_LaserPoint.y() * 480 / 1080;

    QRect rect = QRect(QPoint(((width1 / 2 ) + x) - reticle_width * gap, ((height1 / 2) + y) - gap), QPoint(((width1 / 2) + x) + reticle_width*gap, ((height1 / 2) + y) + gap));
    QRect rect2 = QRect(QPoint(((width1 / 2) + x) - gap, ((height1 / 2) + y) - reticle_width * gap), QPoint(((width1 / 2) + x) + gap, ((height1 / 2) + y) + reticle_width * gap));

    painter.fillRect(rect, Qt::white);
    painter.fillRect(rect2, Qt::white);
}

void CameraZoomFocusWidget::setPSerialViscaManager(SerialViscaManager *newPSerialViscaManager)
{
    m_pSerialViscaManager = newPSerialViscaManager;
}

void CameraZoomFocusWidget::setMainMenuSize(QSize size)
{
    m_pMainMenuWidgetSize = size;
}


void CameraZoomFocusWidget::setZoomGoal(int zoom_index)
{
    QJsonObject object = ConfigManager("parameter_setting1.json").GetConfig();
    int userMode = object["speed selection"].toInt();
    object = ConfigManager("zoom.json").GetConfig();
    QString magnification;

    if(zoom_index==5)
    {
        magnification = object["lt zoom"].toArray()[zoom_index-1].toString();
    }
    else
    {
        magnification = object["lt zoom"].toArray()[zoom_index].toString();
    }

    object = ConfigManager("camera_zoom_mag.json").GetConfig();
    m_zoomGoal = object.value(magnification).toString().toUpper();
}

void CameraZoomFocusWidget::setFocusGoal(int zoom_index, int ndaynight)
{
    QJsonObject object = ConfigManager("parameter_setting1.json").GetConfig();
    int userMode = object["speed selection"].toInt();

//    object = ConfigManager("parameter_setting2.json").GetConfig();
//    int daynight = object["day&night selection"].toInt();
    int daynight = ndaynight;
    object = ConfigManager("focus.json").GetConfig();
    QJsonArray ar;
    if (daynight > 0 && daynight < 4)
    {
//        if (userMode == 1)
//        {
//            ar = object["st day focus"].toArray();
//            this->set_focus(ar[index].toString());
//        }
//        else
//        {
        ar = object["lt day focus"].toArray();
        m_focusGoal = ar[zoom_index].toString().toUpper();
//        }
    }
    else
    {
//        if (userMode == 1)
//        {
//            ar = object["st night focus"].toArray();
//            this->set_focus(ar[index].toString());
//        }
//        else
//        {
        ar = object["lt night focus"].toArray();
        m_focusGoal = ar[zoom_index].toString().toUpper();
//        }
    }
}

void CameraZoomFocusWidget::on_tableWidget_cellClicked(int row, int column)
{
    int zoom_index = 0;

    m_nLtIndex = row;
    m_nDNIndex = column;

    m_nTableIndex.setX(m_nLtIndex);
    m_nTableIndex.setY(m_nDNIndex);

    zoom_index = m_nLtIndex;
    ui->zoomRangePushButton->setText(m_ltmetervector[m_nLtIndex]+distanceValue());

    if(column == 0)
        ui->dayComboBox->setCurrentIndex(0);
    else
        ui->dayComboBox->setCurrentIndex(1);

    int ndaynight;
    if(ui->dayComboBox->currentIndex() == 0)
    {
       ndaynight = 1;
    }else
    {
       ndaynight = 4;
    }

    m_pSerialViscaManager->SetZoomForZoomFocus(zoom_index);
    setZoomGoal(zoom_index);
    m_pSerialViscaManager->SetFocusForZoomFocus(zoom_index, ndaynight);
    setFocusGoal(zoom_index, ndaynight);
    m_pSerialViscaManager->SetDZoomForZoomFocus(zoom_index);

    SetLogMsg(BUTTON_CLICKED, "ZOOM_INDEX, " + ui->zoomRangePushButton->text());

    SetLaserDetectionAreaDistance(zoom_index);

    if (m_mTableStatus[std::make_pair(m_nTableIndex.x(), m_nTableIndex.y())] == 1)
        ui->pgrsSavePushButton->setDisabled(false);
    else
        ui->pgrsSavePushButton->setDisabled(true);

    SendViscaValue();
}
