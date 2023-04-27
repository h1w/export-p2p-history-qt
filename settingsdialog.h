#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QString>
#include <QtUiTools/QtUiTools>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void readSettings();
    void writeSettings();

    void fillSettingsDialogFields();

private slots:
    void on_saveSettings_pushButton_clicked();

private:
    Ui::SettingsDialog *ui;

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

#endif // SETTINGSDIALOG_H
