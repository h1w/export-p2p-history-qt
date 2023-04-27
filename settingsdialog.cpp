#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    readSettings();
    fillSettingsDialogFields();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::readSettings()
{
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
}

void SettingsDialog::writeSettings()
{
    QLineEdit* savePath = findChild<QLineEdit*>("savePath_lineEdit");
    QLineEdit* binanceApiKey = findChild<QLineEdit*>("binanceApiKey_lineEdit");
    QLineEdit* binanceSecretKey = findChild<QLineEdit*>("binanceSecretKey_lineEdit");
    QLineEdit* pexpayApiKey = findChild<QLineEdit*>("pexpayApiKey_lineEdit");
    QLineEdit* pexpaySecretKey = findChild<QLineEdit*>("pexpaySecretKey_lineEdit");
    QLineEdit* huobiApiKey = findChild<QLineEdit*>("huobiApiKey_lineEdit");
    QLineEdit* huobiSecretKey = findChild<QLineEdit*>("huobiSecretKey_lineEdit");
    QLineEdit* bybitApiKey = findChild<QLineEdit*>("bybitApiKey_lineEdit");
    QLineEdit* bybitSecretKey = findChild<QLineEdit*>("bybitSecretKey_lineEdit");

    settings->beginGroup("Default");
    settings->setValue("SavePath", savePath->text());
    settings->endGroup();

    settings->beginGroup("Binance");
    settings->setValue("ApiKey", binanceApiKey->text());
    settings->setValue("SecretKey", binanceSecretKey->text());
    settings->endGroup();

    settings->beginGroup("Pexpay");
    settings->setValue("ApiKey", pexpayApiKey->text());
    settings->setValue("SecretKey", pexpaySecretKey->text());
    settings->endGroup();

    settings->beginGroup("Huobi");
    settings->setValue("ApiKey", huobiApiKey->text());
    settings->setValue("SecretKey", huobiSecretKey->text());
    settings->endGroup();

    settings->beginGroup("Bybit");
    settings->setValue("ApiKey", bybitApiKey->text());
    settings->setValue("SecretKey", bybitSecretKey->text());
    settings->endGroup();
}

void SettingsDialog::fillSettingsDialogFields()
{
    QLineEdit* savePath = findChild<QLineEdit*>("savePath_lineEdit");
    QLineEdit* binanceApiKey = findChild<QLineEdit*>("binanceApiKey_lineEdit");
    QLineEdit* binanceSecretKey = findChild<QLineEdit*>("binanceSecretKey_lineEdit");
    QLineEdit* pexpayApiKey = findChild<QLineEdit*>("pexpayApiKey_lineEdit");
    QLineEdit* pexpaySecretKey = findChild<QLineEdit*>("pexpaySecretKey_lineEdit");
    QLineEdit* huobiApiKey = findChild<QLineEdit*>("huobiApiKey_lineEdit");
    QLineEdit* huobiSecretKey = findChild<QLineEdit*>("huobiSecretKey_lineEdit");
    QLineEdit* bybitApiKey = findChild<QLineEdit*>("bybitApiKey_lineEdit");
    QLineEdit* bybitSecretKey = findChild<QLineEdit*>("bybitSecretKey_lineEdit");

    savePath->insert(settingsData.Default.savePath);

    binanceApiKey->insert(settingsData.Binance.apiKey);
    binanceSecretKey->insert(settingsData.Binance.secretKey);

    pexpayApiKey->insert(settingsData.Pexpay.apiKey);
    pexpaySecretKey->insert(settingsData.Pexpay.secretKey);

    huobiApiKey->insert(settingsData.Huobi.apiKey);
    huobiSecretKey->insert(settingsData.Huobi.secretKey);

    bybitApiKey->insert(settingsData.Bybit.apiKey);
    bybitSecretKey->insert(settingsData.Bybit.secretKey);
}

void SettingsDialog::on_saveSettings_pushButton_clicked()
{
    writeSettings();

    this->close();
}

