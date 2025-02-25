/********************************************************************************
** Form generated from reading UI file 'DeviceControlWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICECONTROLWIDGET_H
#define UI_DEVICECONTROLWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeviceControlWidget
{
public:
    QVBoxLayout *controlLayout;
    QGroupBox *deviceGroupBox;
    QVBoxLayout *deviceLayout;
    QHBoxLayout *deviceTypeLayout;
    QLabel *deviceTypeLabel;
    QComboBox *deviceTypeCombo;
    QComboBox *deviceCombo;
    QWidget *rgbControls;
    QVBoxLayout *rgbLayout;
    QRadioButton *wholeDeviceRadio;
    QRadioButton *zoneRadio;
    QRadioButton *ledRadio;
    QComboBox *selectionCombo;

    void setupUi(QWidget *DeviceControlWidget)
    {
        if (DeviceControlWidget->objectName().isEmpty())
            DeviceControlWidget->setObjectName(QString::fromUtf8("DeviceControlWidget"));
        DeviceControlWidget->resize(400, 300);
        controlLayout = new QVBoxLayout(DeviceControlWidget);
        controlLayout->setObjectName(QString::fromUtf8("controlLayout"));
        deviceGroupBox = new QGroupBox(DeviceControlWidget);
        deviceGroupBox->setObjectName(QString::fromUtf8("deviceGroupBox"));
        deviceLayout = new QVBoxLayout(deviceGroupBox);
        deviceLayout->setObjectName(QString::fromUtf8("deviceLayout"));
        deviceTypeLayout = new QHBoxLayout();
        deviceTypeLayout->setObjectName(QString::fromUtf8("deviceTypeLayout"));
        deviceTypeLabel = new QLabel(deviceGroupBox);
        deviceTypeLabel->setObjectName(QString::fromUtf8("deviceTypeLabel"));

        deviceTypeLayout->addWidget(deviceTypeLabel);

        deviceTypeCombo = new QComboBox(deviceGroupBox);
        deviceTypeCombo->addItem(QString());
        deviceTypeCombo->addItem(QString());
        deviceTypeCombo->setObjectName(QString::fromUtf8("deviceTypeCombo"));

        deviceTypeLayout->addWidget(deviceTypeCombo);


        deviceLayout->addLayout(deviceTypeLayout);

        deviceCombo = new QComboBox(deviceGroupBox);
        deviceCombo->setObjectName(QString::fromUtf8("deviceCombo"));

        deviceLayout->addWidget(deviceCombo);

        rgbControls = new QWidget(deviceGroupBox);
        rgbControls->setObjectName(QString::fromUtf8("rgbControls"));
        rgbLayout = new QVBoxLayout(rgbControls);
        rgbLayout->setObjectName(QString::fromUtf8("rgbLayout"));
        rgbLayout->setContentsMargins(0, 0, 0, 0);
        wholeDeviceRadio = new QRadioButton(rgbControls);
        wholeDeviceRadio->setObjectName(QString::fromUtf8("wholeDeviceRadio"));
        wholeDeviceRadio->setChecked(true);

        rgbLayout->addWidget(wholeDeviceRadio);

        zoneRadio = new QRadioButton(rgbControls);
        zoneRadio->setObjectName(QString::fromUtf8("zoneRadio"));

        rgbLayout->addWidget(zoneRadio);

        ledRadio = new QRadioButton(rgbControls);
        ledRadio->setObjectName(QString::fromUtf8("ledRadio"));

        rgbLayout->addWidget(ledRadio);

        selectionCombo = new QComboBox(rgbControls);
        selectionCombo->setObjectName(QString::fromUtf8("selectionCombo"));

        rgbLayout->addWidget(selectionCombo);


        deviceLayout->addWidget(rgbControls);


        controlLayout->addWidget(deviceGroupBox);


        retranslateUi(DeviceControlWidget);

        QMetaObject::connectSlotsByName(DeviceControlWidget);
    } // setupUi

    void retranslateUi(QWidget *DeviceControlWidget)
    {
        deviceGroupBox->setTitle(QCoreApplication::translate("DeviceControlWidget", "Device Control", nullptr));
        deviceTypeLabel->setText(QCoreApplication::translate("DeviceControlWidget", "Type:", nullptr));
        deviceTypeCombo->setItemText(0, QCoreApplication::translate("DeviceControlWidget", "RGB Devices", nullptr));
        deviceTypeCombo->setItemText(1, QCoreApplication::translate("DeviceControlWidget", "Non-RGB Devices", nullptr));

        wholeDeviceRadio->setText(QCoreApplication::translate("DeviceControlWidget", "Whole Device", nullptr));
        zoneRadio->setText(QCoreApplication::translate("DeviceControlWidget", "Zone", nullptr));
        ledRadio->setText(QCoreApplication::translate("DeviceControlWidget", "LED", nullptr));
        (void)DeviceControlWidget;
    } // retranslateUi

};

namespace Ui {
    class DeviceControlWidget: public Ui_DeviceControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICECONTROLWIDGET_H
