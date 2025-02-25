#include "devices/CustomDeviceDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>

CustomDeviceDialog::CustomDeviceDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
    setupConnections();
}

void CustomDeviceDialog::setupUi()
{
    setWindowTitle("Add Custom Device");
    setModal(true);
    
    // Create main layout
    auto mainLayout = new QVBoxLayout(this);
    auto formLayout = new QFormLayout;

    // Name input
    nameEdit = new QLineEdit(this);
    formLayout->addRow("Name:", nameEdit);

    // Type selection with Add button
    auto typeLayout = new QHBoxLayout;
    typeCombo = new QComboBox(this);
    typeCombo->addItems({"Monitor", "Speaker", "Desk", "PC Case", "Custom"});
    typeCombo->setEditable(false);
    
    addTypeButton = new QPushButton("+", this);
    addTypeButton->setMaximumWidth(30);
    addTypeButton->setToolTip("Add New Type");
    
    typeLayout->addWidget(typeCombo);
    typeLayout->addWidget(addTypeButton);
    formLayout->addRow("Type:", typeLayout);

    mainLayout->addLayout(formLayout);

    // Dialog buttons
    auto buttonLayout = new QHBoxLayout;
    okButton = new QPushButton("Create", this);
    auto* cancelButton = new QPushButton("Cancel", this);
    okButton->setEnabled(false);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Set size constraints
    setMinimumWidth(300);
    
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void CustomDeviceDialog::setupConnections()
{
    connect(addTypeButton, &QPushButton::clicked, this, &CustomDeviceDialog::onAddTypeClicked);
    connect(nameEdit, &QLineEdit::textChanged, this, &CustomDeviceDialog::validateInput);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
}

void CustomDeviceDialog::onAddTypeClicked()
{
    bool ok;
    QString newType = QInputDialog::getText(this, 
                                          "Add New Type",
                                          "Enter new device type:",
                                          QLineEdit::Normal,
                                          "",
                                          &ok);

    if (ok && !newType.isEmpty())
    {
        // Insert the new type before "Custom"
        int customIndex = typeCombo->findText("Custom");
        if (customIndex >= 0) {
            typeCombo->insertItem(customIndex, newType);
            typeCombo->setCurrentText(newType);
        }
    }
}

void CustomDeviceDialog::validateInput()
{
    QString name = nameEdit->text().trimmed();
    okButton->setEnabled(!name.isEmpty());
}

QString CustomDeviceDialog::getDeviceName() const
{
    return nameEdit->text().trimmed();
}

QString CustomDeviceDialog::getDeviceType() const
{
    return typeCombo->currentText();
}