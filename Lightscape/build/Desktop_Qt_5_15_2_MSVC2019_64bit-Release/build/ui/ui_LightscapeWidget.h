/********************************************************************************
** Form generated from reading UI file 'LightscapeWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIGHTSCAPEWIDGET_H
#define UI_LIGHTSCAPEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LightscapeWidget
{
public:
    QVBoxLayout *mainLayout;
    QHBoxLayout *toolbarLayout;
    QPushButton *configureButton;
    QSpacerItem *toolbarSpacer;
    QSplitter *mainSplitter;
    QWidget *gridWidget;
    QVBoxLayout *gridLayout;
    QTabWidget *layerTabs;
    QWidget *topLayer;
    QGridLayout *topGrid;
    QWidget *middleLayer;
    QGridLayout *middleGrid;
    QWidget *bottomLayer;
    QGridLayout *bottomGrid;
    QWidget *controlWidget;
    QVBoxLayout *controlLayout;
    QGroupBox *deviceGroupBox;
    QVBoxLayout *deviceLayout;
    QComboBox *deviceCombo;
    QRadioButton *wholeDeviceRadio;
    QRadioButton *zoneRadio;
    QRadioButton *ledRadio;
    QComboBox *selectionCombo;
    QGroupBox *assignmentGroupBox;
    QVBoxLayout *assignmentLayout;
    QListWidget *assignmentList;
    QHBoxLayout *buttonLayout;
    QPushButton *assignButton;
    QPushButton *removeButton;
    QPushButton *clearButton;
    QPushButton *colorButton;
    QGroupBox *effectsGroupBox;
    QVBoxLayout *effectsLayout;
    QComboBox *effectCombo;
    QFormLayout *effectParamsLayout;
    QLabel *speedLabel;
    QSlider *speedSlider;
    QLabel *intensityLabel;
    QSlider *intensitySlider;

    void setupUi(QWidget *LightscapeWidget)
    {
        if (LightscapeWidget->objectName().isEmpty())
            LightscapeWidget->setObjectName(QString::fromUtf8("LightscapeWidget"));
        LightscapeWidget->resize(800, 600);
        mainLayout = new QVBoxLayout(LightscapeWidget);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        toolbarLayout = new QHBoxLayout();
        toolbarLayout->setObjectName(QString::fromUtf8("toolbarLayout"));
        configureButton = new QPushButton(LightscapeWidget);
        configureButton->setObjectName(QString::fromUtf8("configureButton"));

        toolbarLayout->addWidget(configureButton);

        toolbarSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolbarLayout->addItem(toolbarSpacer);


        mainLayout->addLayout(toolbarLayout);

        mainSplitter = new QSplitter(LightscapeWidget);
        mainSplitter->setObjectName(QString::fromUtf8("mainSplitter"));
        mainSplitter->setOrientation(Qt::Horizontal);
        gridWidget = new QWidget(mainSplitter);
        gridWidget->setObjectName(QString::fromUtf8("gridWidget"));
        gridLayout = new QVBoxLayout(gridWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        layerTabs = new QTabWidget(gridWidget);
        layerTabs->setObjectName(QString::fromUtf8("layerTabs"));
        topLayer = new QWidget();
        topLayer->setObjectName(QString::fromUtf8("topLayer"));
        topGrid = new QGridLayout(topLayer);
        topGrid->setObjectName(QString::fromUtf8("topGrid"));
        layerTabs->addTab(topLayer, QString());
        middleLayer = new QWidget();
        middleLayer->setObjectName(QString::fromUtf8("middleLayer"));
        middleGrid = new QGridLayout(middleLayer);
        middleGrid->setObjectName(QString::fromUtf8("middleGrid"));
        layerTabs->addTab(middleLayer, QString());
        bottomLayer = new QWidget();
        bottomLayer->setObjectName(QString::fromUtf8("bottomLayer"));
        bottomGrid = new QGridLayout(bottomLayer);
        bottomGrid->setObjectName(QString::fromUtf8("bottomGrid"));
        layerTabs->addTab(bottomLayer, QString());

        gridLayout->addWidget(layerTabs);

        mainSplitter->addWidget(gridWidget);
        controlWidget = new QWidget(mainSplitter);
        controlWidget->setObjectName(QString::fromUtf8("controlWidget"));
        controlLayout = new QVBoxLayout(controlWidget);
        controlLayout->setObjectName(QString::fromUtf8("controlLayout"));
        controlLayout->setContentsMargins(0, 0, 0, 0);
        deviceGroupBox = new QGroupBox(controlWidget);
        deviceGroupBox->setObjectName(QString::fromUtf8("deviceGroupBox"));
        deviceLayout = new QVBoxLayout(deviceGroupBox);
        deviceLayout->setObjectName(QString::fromUtf8("deviceLayout"));
        deviceCombo = new QComboBox(deviceGroupBox);
        deviceCombo->setObjectName(QString::fromUtf8("deviceCombo"));

        deviceLayout->addWidget(deviceCombo);

        wholeDeviceRadio = new QRadioButton(deviceGroupBox);
        wholeDeviceRadio->setObjectName(QString::fromUtf8("wholeDeviceRadio"));
        wholeDeviceRadio->setChecked(true);

        deviceLayout->addWidget(wholeDeviceRadio);

        zoneRadio = new QRadioButton(deviceGroupBox);
        zoneRadio->setObjectName(QString::fromUtf8("zoneRadio"));

        deviceLayout->addWidget(zoneRadio);

        ledRadio = new QRadioButton(deviceGroupBox);
        ledRadio->setObjectName(QString::fromUtf8("ledRadio"));

        deviceLayout->addWidget(ledRadio);

        selectionCombo = new QComboBox(deviceGroupBox);
        selectionCombo->setObjectName(QString::fromUtf8("selectionCombo"));

        deviceLayout->addWidget(selectionCombo);


        controlLayout->addWidget(deviceGroupBox);

        assignmentGroupBox = new QGroupBox(controlWidget);
        assignmentGroupBox->setObjectName(QString::fromUtf8("assignmentGroupBox"));
        assignmentLayout = new QVBoxLayout(assignmentGroupBox);
        assignmentLayout->setObjectName(QString::fromUtf8("assignmentLayout"));
        assignmentList = new QListWidget(assignmentGroupBox);
        assignmentList->setObjectName(QString::fromUtf8("assignmentList"));

        assignmentLayout->addWidget(assignmentList);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        assignButton = new QPushButton(assignmentGroupBox);
        assignButton->setObjectName(QString::fromUtf8("assignButton"));

        buttonLayout->addWidget(assignButton);

        removeButton = new QPushButton(assignmentGroupBox);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));

        buttonLayout->addWidget(removeButton);

        clearButton = new QPushButton(assignmentGroupBox);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));

        buttonLayout->addWidget(clearButton);


        assignmentLayout->addLayout(buttonLayout);

        colorButton = new QPushButton(assignmentGroupBox);
        colorButton->setObjectName(QString::fromUtf8("colorButton"));

        assignmentLayout->addWidget(colorButton);


        controlLayout->addWidget(assignmentGroupBox);

        effectsGroupBox = new QGroupBox(controlWidget);
        effectsGroupBox->setObjectName(QString::fromUtf8("effectsGroupBox"));
        effectsLayout = new QVBoxLayout(effectsGroupBox);
        effectsLayout->setObjectName(QString::fromUtf8("effectsLayout"));
        effectCombo = new QComboBox(effectsGroupBox);
        effectCombo->addItem(QString());
        effectCombo->addItem(QString());
        effectCombo->addItem(QString());
        effectCombo->addItem(QString());
        effectCombo->addItem(QString());
        effectCombo->setObjectName(QString::fromUtf8("effectCombo"));

        effectsLayout->addWidget(effectCombo);

        effectParamsLayout = new QFormLayout();
        effectParamsLayout->setObjectName(QString::fromUtf8("effectParamsLayout"));
        speedLabel = new QLabel(effectsGroupBox);
        speedLabel->setObjectName(QString::fromUtf8("speedLabel"));

        effectParamsLayout->setWidget(0, QFormLayout::LabelRole, speedLabel);

        speedSlider = new QSlider(effectsGroupBox);
        speedSlider->setObjectName(QString::fromUtf8("speedSlider"));
        speedSlider->setOrientation(Qt::Horizontal);
        speedSlider->setMinimum(1);
        speedSlider->setMaximum(100);
        speedSlider->setValue(50);

        effectParamsLayout->setWidget(0, QFormLayout::FieldRole, speedSlider);

        intensityLabel = new QLabel(effectsGroupBox);
        intensityLabel->setObjectName(QString::fromUtf8("intensityLabel"));

        effectParamsLayout->setWidget(1, QFormLayout::LabelRole, intensityLabel);

        intensitySlider = new QSlider(effectsGroupBox);
        intensitySlider->setObjectName(QString::fromUtf8("intensitySlider"));
        intensitySlider->setOrientation(Qt::Horizontal);
        intensitySlider->setMinimum(1);
        intensitySlider->setMaximum(100);
        intensitySlider->setValue(100);

        effectParamsLayout->setWidget(1, QFormLayout::FieldRole, intensitySlider);


        effectsLayout->addLayout(effectParamsLayout);


        controlLayout->addWidget(effectsGroupBox);

        mainSplitter->addWidget(controlWidget);

        mainLayout->addWidget(mainSplitter);


        retranslateUi(LightscapeWidget);

        QMetaObject::connectSlotsByName(LightscapeWidget);
    } // setupUi

    void retranslateUi(QWidget *LightscapeWidget)
    {
        configureButton->setText(QCoreApplication::translate("LightscapeWidget", "Configure Grid", nullptr));
        layerTabs->setTabText(layerTabs->indexOf(topLayer), QCoreApplication::translate("LightscapeWidget", "Top Layer", nullptr));
        layerTabs->setTabText(layerTabs->indexOf(middleLayer), QCoreApplication::translate("LightscapeWidget", "Middle Layer", nullptr));
        layerTabs->setTabText(layerTabs->indexOf(bottomLayer), QCoreApplication::translate("LightscapeWidget", "Bottom Layer", nullptr));
        deviceGroupBox->setTitle(QCoreApplication::translate("LightscapeWidget", "Device Control", nullptr));
        wholeDeviceRadio->setText(QCoreApplication::translate("LightscapeWidget", "Whole Device", nullptr));
        zoneRadio->setText(QCoreApplication::translate("LightscapeWidget", "Zone", nullptr));
        ledRadio->setText(QCoreApplication::translate("LightscapeWidget", "LED", nullptr));
        assignmentGroupBox->setTitle(QCoreApplication::translate("LightscapeWidget", "Assignments", nullptr));
        assignButton->setText(QCoreApplication::translate("LightscapeWidget", "Assign", nullptr));
        removeButton->setText(QCoreApplication::translate("LightscapeWidget", "Remove", nullptr));
        clearButton->setText(QCoreApplication::translate("LightscapeWidget", "Clear", nullptr));
        colorButton->setText(QCoreApplication::translate("LightscapeWidget", "Set Color", nullptr));
        effectsGroupBox->setTitle(QCoreApplication::translate("LightscapeWidget", "Effects", nullptr));
        effectCombo->setItemText(0, QCoreApplication::translate("LightscapeWidget", "None", nullptr));
        effectCombo->setItemText(1, QCoreApplication::translate("LightscapeWidget", "Radial Fade", nullptr));
        effectCombo->setItemText(2, QCoreApplication::translate("LightscapeWidget", "Wave", nullptr));
        effectCombo->setItemText(3, QCoreApplication::translate("LightscapeWidget", "Ripple", nullptr));
        effectCombo->setItemText(4, QCoreApplication::translate("LightscapeWidget", "Layer Cascade", nullptr));

        speedLabel->setText(QCoreApplication::translate("LightscapeWidget", "Speed:", nullptr));
        intensityLabel->setText(QCoreApplication::translate("LightscapeWidget", "Intensity:", nullptr));
        (void)LightscapeWidget;
    } // retranslateUi

};

namespace Ui {
    class LightscapeWidget: public Ui_LightscapeWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHTSCAPEWIDGET_H
