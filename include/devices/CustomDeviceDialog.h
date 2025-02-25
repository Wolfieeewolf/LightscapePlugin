#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QString>

class CustomDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomDeviceDialog(QWidget *parent = nullptr);
    QString getDeviceName() const;
    QString getDeviceType() const;

private slots:
    void onAddTypeClicked();
    void validateInput();

private:
    QLineEdit* nameEdit;
    QComboBox* typeCombo;
    QPushButton* okButton;
    QPushButton* addTypeButton;
    
    void setupUi();
    void setupConnections();
};