
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsdialog.h"
#include <QDateTime>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <string>
#include <string_view>

#include "binance.h"
#include "pexpay.h"
#include "bybit.h"

using json = nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fixDateTimeWidgets();

    void exportHistory();
    void openSettings();

private slots:
    void on_exportButton_pushButton_clicked();

    void on_settingsButton_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    SettingsDialog *settingsDialog;

    struct settings_data_s {
        struct default_s {
            QString savePath;
        };
        struct binance_s {
            QString apiKey;
            QString secretKey;
        };
        struct pexpay_s {
            QString apiKey;
            QString secretKey;
        };
        struct huobi_s {
            QString apiKey;
            QString secretKey;
        };
        struct bybit_s {
            QString apiKey;
            QString secretKey;
        };

        default_s Default;
        binance_s Binance;
        pexpay_s Pexpay;
        huobi_s Huobi;
        bybit_s Bybit;
    };

    QString settingsFilename = "settings.ini";
    QSettings *settings;
    settings_data_s settingsData;
};

#endif // MAINWINDOW_H
