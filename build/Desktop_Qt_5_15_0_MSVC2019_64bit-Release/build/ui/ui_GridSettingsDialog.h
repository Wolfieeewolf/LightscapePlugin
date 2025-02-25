/********************************************************************************
** Form generated from reading UI file 'GridSettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDSETTINGSDIALOG_H
#define UI_GRIDSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
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
    QVBoxLayout *mainLayout;
    QHBoxLayout *dimensionsLayout;
    QFormLayout *formLayout;
    QLabel *widthLabel;
    QSpinBox *widthSpin;
    QLabel *heightLabel;
    QSpinBox *heightSpin;
    QLabel *depthLabel;
    QSpinBox *depthSpin;
    QTabWidget *layerTabs;
    QHBoxLayout *userPositionLayout;
    QPushButton *setUserPositionButton;
    QPushButton *clearUserPositionButton;
    QLabel *warningLabel;
    QHBoxLayout *buttonLayout;
    QPushButton *resetButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *applyButton;
    QPushButton *closeButton;

    void setupUi(QDialog *GridSettingsDialog)
    {
        if (GridSettingsDialog->objectName().isEmpty())
            GridSettingsDialog->setObjectName(QString::fromUtf8("GridSettingsDialog"));
        GridSettingsDialog->resize(600, 500);
        mainLayout = new QVBoxLayout(GridSettingsDialog);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        dimensionsLayout = new QHBoxLayout();
        dimensionsLayout->setObjectName(QString::fromUtf8("dimensionsLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        widthLabel = new QLabel(GridSettingsDialog);
        widthLabel->setObjectName(QString::fromUtf8("widthLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, widthLabel);

        widthSpin = new QSpinBox(GridSettingsDialog);
        widthSpin->setObjectName(QString::fromUtf8("widthSpin"));
        widthSpin->setMinimum(1);
        widthSpin->setMaximum(100);

        formLayout->setWidget(0, QFormLayout::FieldRole, widthSpin);

        heightLabel = new QLabel(GridSettingsDialog);
        heightLabel->setObjectName(QString::fromUtf8("heightLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, heightLabel);

        heightSpin = new QSpinBox(GridSettingsDialog);
        heightSpin->setObjectName(QString::fromUtf8("heightSpin"));
        heightSpin->setMinimum(1);
        heightSpin->setMaximum(100);

        formLayout->setWidget(1, QFormLayout::FieldRole, heightSpin);

        depthLabel = new QLabel(GridSettingsDialog);
        depthLabel->setObjectName(QString::fromUtf8("depthLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, depthLabel);

        depthSpin = new QSpinBox(GridSettingsDialog);
        depthSpin->setObjectName(QString::fromUtf8("depthSpin"));
        depthSpin->setMinimum(1);
        depthSpin->setMaximum(10);

        formLayout->setWidget(2, QFormLayout::FieldRole, depthSpin);


        dimensionsLayout->addLayout(formLayout);


        mainLayout->addLayout(dimensionsLayout);

        layerTabs = new QTabWidget(GridSettingsDialog);
        layerTabs->setObjectName(QString::fromUtf8("layerTabs"));

        mainLayout->addWidget(layerTabs);

        userPositionLayout = new QHBoxLayout();
        userPositionLayout->setObjectName(QString::fromUtf8("userPositionLayout"));
        setUserPositionButton = new QPushButton(GridSettingsDialog);
        setUserPositionButton->setObjectName(QString::fromUtf8("setUserPositionButton"));

        userPositionLayout->addWidget(setUserPositionButton);

        clearUserPositionButton = new QPushButton(GridSettingsDialog);
        clearUserPositionButton->setObjectName(QString::fromUtf8("clearUserPositionButton"));

        userPositionLayout->addWidget(clearUserPositionButton);


        mainLayout->addLayout(userPositionLayout);

        warningLabel = new QLabel(GridSettingsDialog);
        warningLabel->setObjectName(QString::fromUtf8("warningLabel"));

        mainLayout->addWidget(warningLabel);

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


        mainLayout->addLayout(buttonLayout);


        retranslateUi(GridSettingsDialog);

        QMetaObject::connectSlotsByName(GridSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *GridSettingsDialog)
    {
        GridSettingsDialog->setWindowTitle(QCoreApplication::translate("GridSettingsDialog", "Grid Settings", nullptr));
        widthLabel->setText(QCoreApplication::translate("GridSettingsDialog", "Width:", nullptr));
        heightLabel->setText(QCoreApplication::translate("GridSettingsDialog", "Height:", nullptr));
        depthLabel->setText(QCoreApplication::translate("GridSettingsDialog", "Depth:", nullptr));
        setUserPositionButton->setText(QCoreApplication::translate("GridSettingsDialog", "Set User Position", nullptr));
        clearUserPositionButton->setText(QCoreApplication::translate("GridSettingsDialog", "Clear User Position", nullptr));
        warningLabel->setStyleSheet(QCoreApplication::translate("GridSettingsDialog", "color: red;", nullptr));
        warningLabel->setText(QString());
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
