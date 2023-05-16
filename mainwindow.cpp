
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fixDateTimeWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fixDateTimeWidgets()
{
    QDateTimeEdit* startDateTime = findChild<QDateTimeEdit*>("startDateTime_dateTimeEdit");
    QDateTimeEdit* endDateTime = findChild<QDateTimeEdit*>("endDateTime_dateTimeEdit");

    startDateTime->setDateTime(QDateTime::currentDateTime());
    startDateTime->setTime(QTime(15, 0, 0));
    endDateTime->setDateTime(QDateTime::currentDateTime());
    endDateTime->setTime(QTime(0, 0, 0));
}

void MainWindow::exportHistory()
{
    // read keys from config
    settings = new QSettings(settingsFilename, QSettings::IniFormat);

    settings->beginGroup("Default");
    settingsData.Default.savePath = settings->value("SavePath", QString()).toString();
    settings->endGroup();

    settings->beginGroup("Binance");
    settingsData.Binance.apiKey = settings->value("ApiKey", QString()).toString();
    settingsData.Binance.secretKey = settings->value("SecretKey", QString()).toString();
    settings->endGroup();

    settings->beginGroup("Pexpay");
    settingsData.Pexpay.apiKey = settings->value("ApiKey", QString()).toString();
    settingsData.Pexpay.secretKey = settings->value("SecretKey", QString()).toString();
    settings->endGroup();

    settings->beginGroup("Huobi");
    settingsData.Huobi.apiKey = settings->value("ApiKey", QString()).toString();
    settingsData.Huobi.secretKey = settings->value("SecretKey", QString()).toString();
    settings->endGroup();

    settings->beginGroup("Bybit");
    settingsData.Bybit.apiKey = settings->value("ApiKey", QString()).toString();
    settingsData.Bybit.secretKey = settings->value("SecretKey", QString()).toString();
    settings->endGroup();

    // get datetimeedit fields
    QDateTimeEdit* startDateTime_dte = findChild<QDateTimeEdit*>("startDateTime_dateTimeEdit");
    QDateTimeEdit* endDateTime_dte = findChild<QDateTimeEdit*>("endDateTime_dateTimeEdit");

    QDateTime startDateTime(startDateTime_dte->date(), startDateTime_dte->time());
    QDateTime endDateTime(endDateTime_dte->date(), endDateTime_dte->time());

    qint64 startTimestamp = startDateTime.toMSecsSinceEpoch();
    qint64 endTimestamp = endDateTime.toMSecsSinceEpoch();

    // Get History from config exchanges
    // Binance
    // only if checkbox is enabled
    if (findChild<QCheckBox*>("needExportBinance_checkBox")->isChecked())
    {
        Binance binance(settingsData.Binance.apiKey, settingsData.Binance.secretKey);
        json binanceOrderHistory = binance.getOrdersHistory(startTimestamp, endTimestamp);
    }

    // Pexpay
    if (findChild<QCheckBox*>("needExportPexpay_checkBox")->isChecked())
    {
        Binance pexpay(settingsData.Pexpay.apiKey, settingsData.Pexpay.secretKey);
        json pexpayOrderHistory = pexpay.getOrdersHistory(startTimestamp, endTimestamp);
    }

    // Huobi
    if (findChild<QCheckBox*>("needExportHuobi_checkBox")->isChecked())
    {
    }

    // Bybit
    if (findChild<QCheckBox*>("needExportBybit_checkBox")->isChecked())
    {
//        Bybit bybit(settingsData.Bybit.apiKey, settingsData.Bybit.secretKey);
//        json bybitOrderHistory = bybit.getOrdersHistory(startTimestamp, endTimestamp);
    }
}

void MainWindow::openSettings()
{
    settingsDialog = new SettingsDialog(this);
    settingsDialog->show();
}

void MainWindow::on_exportButton_pushButton_clicked()
{
    exportHistory();
}


void MainWindow::on_settingsButton_pushButton_clicked()
{
    openSettings();
}

