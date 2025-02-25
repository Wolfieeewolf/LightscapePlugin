/********************************************************************************
** Form generated from reading UI file 'AssignmentsWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASSIGNMENTSWIDGET_H
#define UI_ASSIGNMENTSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AssignmentsWidget
{
public:
    QVBoxLayout *assignmentRootLayout;
    QGroupBox *assignmentGroupBox;
    QVBoxLayout *assignmentLayout;
    QListWidget *assignmentList;
    QHBoxLayout *buttonLayout;
    QPushButton *assignButton;
    QPushButton *removeButton;
    QPushButton *clearButton;
    QPushButton *colorButton;

    void setupUi(QWidget *AssignmentsWidget)
    {
        if (AssignmentsWidget->objectName().isEmpty())
            AssignmentsWidget->setObjectName(QString::fromUtf8("AssignmentsWidget"));
        AssignmentsWidget->resize(400, 300);
        assignmentRootLayout = new QVBoxLayout(AssignmentsWidget);
        assignmentRootLayout->setObjectName(QString::fromUtf8("assignmentRootLayout"));
        assignmentGroupBox = new QGroupBox(AssignmentsWidget);
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


        assignmentRootLayout->addWidget(assignmentGroupBox);


        retranslateUi(AssignmentsWidget);

        QMetaObject::connectSlotsByName(AssignmentsWidget);
    } // setupUi

    void retranslateUi(QWidget *AssignmentsWidget)
    {
        assignmentGroupBox->setTitle(QCoreApplication::translate("AssignmentsWidget", "Assignments", nullptr));
        assignButton->setText(QCoreApplication::translate("AssignmentsWidget", "Assign", nullptr));
        removeButton->setText(QCoreApplication::translate("AssignmentsWidget", "Remove", nullptr));
        clearButton->setText(QCoreApplication::translate("AssignmentsWidget", "Clear", nullptr));
        colorButton->setText(QCoreApplication::translate("AssignmentsWidget", "Set Color", nullptr));
        (void)AssignmentsWidget;
    } // retranslateUi

};

namespace Ui {
    class AssignmentsWidget: public Ui_AssignmentsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASSIGNMENTSWIDGET_H
