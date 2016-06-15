#include "settingsdialog.h"
#include "ui/ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, DBB::DBBConfigdata* configDataIn) :
QDialog(parent), configData(configDataIn),
ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(this->ui->resetDefaultsButton, SIGNAL(clicked()), this, SLOT(resetDefaults()));
    connect(this->ui->saveButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(this->ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::resetDefaults()
{
    ui->walletServiceURL->setText(QString::fromStdString(configData->getDefaultBWSULR()));
    ui->smartVerificationURL->setText(QString::fromStdString(configData->getDefaultComServerULR()));
}

void SettingsDialog::cancel()
{
    cancleOnClose = true;
    close();
}

void SettingsDialog::showEvent(QShowEvent* event)
{
    cancleOnClose = false;
    loadSettings();
    QWidget::showEvent(event);
}

void SettingsDialog::closeEvent(QCloseEvent *event)
{
    if (!cancleOnClose)
        storeSettings();

    QWidget::closeEvent(event);
}

void SettingsDialog::loadSettings()
{
    ui->walletServiceURL->setText(QString::fromStdString(configData->getBWSBackendURL()));
    ui->smartVerificationURL->setText(QString::fromStdString(configData->getComServerURL()));
}

void SettingsDialog::storeSettings()
{
    configData->setBWSBackendURL(ui->walletServiceURL->text().toStdString());
    configData->setComServerURL(ui->smartVerificationURL->text().toStdString());
    configData->write();
    emit settingsDidChange();
}