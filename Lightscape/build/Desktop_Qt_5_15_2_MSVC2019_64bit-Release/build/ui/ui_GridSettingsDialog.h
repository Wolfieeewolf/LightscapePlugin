/********************************************************************************
** Form generated from reading UI file 'GridSettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDSETTINGSDIALOG_H
#define UI_GRIDSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GridSettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *dimensionsGroup;
    QHBoxLayout *dimensionsLayout;
    QLabel *widthLabel;
    QSpinBox *widthSpin;
    QLabel *heightLabel;
    QSpinBox *heightSpin;
    QLabel *depthLabel;
    QSpinBox *depthSpin;
    QTabWidget *layerTabs;
    QHBoxLayout *buttonLayout;
    QPushButton *resetButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *applyButton;
    QPushButton *closeButton;

    void setupUi(QDialog *GridSettingsDialog)
    {
        if (GridSettingsDialog->objectName().isEmpty())
            GridSettingsDialog->setObjectName(QString::fromUtf8("GridSettingsDialog"));
        GridSettingsDialog->resize(600, 400);
        verticalLayout = new QVBoxLayout(GridSettingsDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        dimensionsGroup = new QGroupBox(GridSettingsDialog);
        dimensionsGroup->setObjectName(QString::fromUtf8("dimensionsGroup"));
        dimensionsLayout = new QHBoxLayout(dimensionsGroup);
        dimensionsLayout->setObjectName(QString::fromUtf8("dimensionsLayout"));
        widthLabel = new QLabel(dimensionsGroup);
        widthLabel->setObjectName(QString::fromUtf8("widthLabel"));

        dimensionsLayout->addWidget(widthLabel);

        widthSpin = new QSpinBox(dimensionsGroup);
        widthSpin->setObjectName(QString::fromUtf8("widthSpin"));
        widthSpin->setMinimum(1);
        widthSpin->setMaximum(10);
        widthSpin->setValue(3);

        dimensionsLayout->addWidget(widthSpin);

        heightLabel = new QLabel(dimensionsGroup);
        heightLabel->setObjectName(QString::fromUtf8("heightLabel"));

        dimensionsLayout->addWidget(heightLabel);

        heightSpin = new QSpinBox(dimensionsGroup);
        heightSpin->setObjectName(QString::fromUtf8("heightSpin"));
        heightSpin->setMinimum(1);
        heightSpin->setMaximum(10);
        heightSpin->setValue(3);

        dimensionsLayout->addWidget(heightSpin);

        depthLabel = new QLabel(dimensionsGroup);
        depthLabel->setObjectName(QString::fromUtf8("depthLabel"));

        dimensionsLayout->addWidget(depthLabel);

        depthSpin = new QSpinBox(dimensionsGroup);
        depthSpin->setObjectName(QString::fromUtf8("depthSpin"));
        depthSpin->setMinimum(1);
        depthSpin->setMaximum(5);
        depthSpin->setValue(3);

        dimensionsLayout->addWidget(depthSpin);


        verticalLayout->addWidget(dimensionsGroup);

        layerTabs = new QTabWidget(GridSettingsDialog);
        layerTabs->setObjectName(QString::fromUtf8("layerTabs"));

        verticalLayout->addWidget(layerTabs);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        resetButton = new QPushButton(GridSettingsDialog);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));

        buttonLayout->addWidget(resetButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        applyButton = new QPushButton(GridSettingsDialog);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        buttonLayout->addWidget(applyButton);

        closeButton = new QPushButton(GridSettingsDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));

        buttonLayout->addWidget(closeButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(GridSettingsDialog);

        layerTabs->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(GridSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *GridSettingsDialog)
    {
        GridSettingsDialog->setWindowTitle(QCoreApplication::translate("GridSettingsDialog", "Grid Settings", nullptr));
        dimensionsGroup->setTitle(QCoreApplication::translate("GridSettingsDialog", "Grid Dimensions", nullptr));
        widthLabel->setText(QCoreApplication::translate("GridSettingsDialog", "Width:", nullptr));
        heightLabel->setText(QCoreApplication::translate("GridSettingsDialog", "Height:", nullptr));
        depthLabel->setText(QCoreApplication::translate("GridSettingsDialog", "Layers:", nullptr));
        resetButton->setText(QCoreApplication::translate("GridSettingsDialog", "Reset to Defaults", nullptr));
        applyButton->setText(QCoreApplication::translate("GridSettingsDialog", "Apply", nullptr));
        closeButton->setText(QCoreApplication::translate("GridSettingsDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GridSettingsDialog: public Ui_GridSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDSETTINGSDIALOG_H
