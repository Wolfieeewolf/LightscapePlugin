/********************************************************************************
** Form generated from reading UI file 'EffectWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EFFECTWIDGET_H
#define UI_EFFECTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EffectWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *previewBox;
    QVBoxLayout *previewLayout;
    QHBoxLayout *previewHeaderLayout;
    QCheckBox *previewCheckBox;
    QCheckBox *reduceFPSCheckBox;
    QWidget *previewWidget;
    QGroupBox *effectsBox;
    QVBoxLayout *effectsLayout;
    QListWidget *effectsList;
    QGroupBox *devicesBox;
    QVBoxLayout *devicesLayout;
    QHBoxLayout *deviceHeaderLayout;
    QPushButton *selectAllButton;
    QScrollArea *deviceScrollArea;
    QWidget *deviceScrollContent;
    QVBoxLayout *deviceListLayout;
    QGroupBox *controlsBox;
    QVBoxLayout *controlsLayout;

    void setupUi(QWidget *EffectWidget)
    {
        if (EffectWidget->objectName().isEmpty())
            EffectWidget->setObjectName(QString::fromUtf8("EffectWidget"));
        EffectWidget->resize(350, 600);
        verticalLayout = new QVBoxLayout(EffectWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        previewBox = new QGroupBox(EffectWidget);
        previewBox->setObjectName(QString::fromUtf8("previewBox"));
        previewLayout = new QVBoxLayout(previewBox);
        previewLayout->setObjectName(QString::fromUtf8("previewLayout"));
        previewHeaderLayout = new QHBoxLayout();
        previewHeaderLayout->setObjectName(QString::fromUtf8("previewHeaderLayout"));
        previewCheckBox = new QCheckBox(previewBox);
        previewCheckBox->setObjectName(QString::fromUtf8("previewCheckBox"));
        previewCheckBox->setChecked(true);

        previewHeaderLayout->addWidget(previewCheckBox);

        reduceFPSCheckBox = new QCheckBox(previewBox);
        reduceFPSCheckBox->setObjectName(QString::fromUtf8("reduceFPSCheckBox"));

        previewHeaderLayout->addWidget(reduceFPSCheckBox);


        previewLayout->addLayout(previewHeaderLayout);

        previewWidget = new QWidget(previewBox);
        previewWidget->setObjectName(QString::fromUtf8("previewWidget"));
        previewWidget->setMinimumSize(QSize(0, 150));

        previewLayout->addWidget(previewWidget);


        verticalLayout->addWidget(previewBox);

        effectsBox = new QGroupBox(EffectWidget);
        effectsBox->setObjectName(QString::fromUtf8("effectsBox"));
        effectsLayout = new QVBoxLayout(effectsBox);
        effectsLayout->setObjectName(QString::fromUtf8("effectsLayout"));
        effectsList = new QListWidget(effectsBox);
        effectsList->setObjectName(QString::fromUtf8("effectsList"));

        effectsLayout->addWidget(effectsList);


        verticalLayout->addWidget(effectsBox);

        devicesBox = new QGroupBox(EffectWidget);
        devicesBox->setObjectName(QString::fromUtf8("devicesBox"));
        devicesLayout = new QVBoxLayout(devicesBox);
        devicesLayout->setObjectName(QString::fromUtf8("devicesLayout"));
        deviceHeaderLayout = new QHBoxLayout();
        deviceHeaderLayout->setObjectName(QString::fromUtf8("deviceHeaderLayout"));
        selectAllButton = new QPushButton(devicesBox);
        selectAllButton->setObjectName(QString::fromUtf8("selectAllButton"));

        deviceHeaderLayout->addWidget(selectAllButton);


        devicesLayout->addLayout(deviceHeaderLayout);

        deviceScrollArea = new QScrollArea(devicesBox);
        deviceScrollArea->setObjectName(QString::fromUtf8("deviceScrollArea"));
        deviceScrollArea->setWidgetResizable(true);
        deviceScrollContent = new QWidget();
        deviceScrollContent->setObjectName(QString::fromUtf8("deviceScrollContent"));
        deviceListLayout = new QVBoxLayout(deviceScrollContent);
        deviceListLayout->setObjectName(QString::fromUtf8("deviceListLayout"));
        deviceScrollArea->setWidget(deviceScrollContent);

        devicesLayout->addWidget(deviceScrollArea);


        verticalLayout->addWidget(devicesBox);

        controlsBox = new QGroupBox(EffectWidget);
        controlsBox->setObjectName(QString::fromUtf8("controlsBox"));
        controlsLayout = new QVBoxLayout(controlsBox);
        controlsLayout->setObjectName(QString::fromUtf8("controlsLayout"));

        verticalLayout->addWidget(controlsBox);


        retranslateUi(EffectWidget);

        QMetaObject::connectSlotsByName(EffectWidget);
    } // setupUi

    void retranslateUi(QWidget *EffectWidget)
    {
        previewBox->setTitle(QCoreApplication::translate("EffectWidget", "Preview", nullptr));
        previewCheckBox->setText(QCoreApplication::translate("EffectWidget", "Enable Preview", nullptr));
        reduceFPSCheckBox->setText(QCoreApplication::translate("EffectWidget", "Reduce FPS", nullptr));
        effectsBox->setTitle(QCoreApplication::translate("EffectWidget", "Effects", nullptr));
        devicesBox->setTitle(QCoreApplication::translate("EffectWidget", "Effect Devices", nullptr));
        selectAllButton->setText(QCoreApplication::translate("EffectWidget", "Select All", nullptr));
        controlsBox->setTitle(QCoreApplication::translate("EffectWidget", "Effect Controls", nullptr));
        (void)EffectWidget;
    } // retranslateUi

};

namespace Ui {
    class EffectWidget: public Ui_EffectWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EFFECTWIDGET_H
