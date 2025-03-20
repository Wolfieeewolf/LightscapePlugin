/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectTabHeader.cpp                                       |
|                                                           |
| Header that appears in each effect tab with controls      |
\*---------------------------------------------------------*/

#include "effects/EffectTabHeader.h"
#include "ui_EffectTabHeader.h"
#include "effects/BaseEffect.h"
#include <QInputDialog>

EffectTabHeader::EffectTabHeader(QWidget *parent, Lightscape::BaseEffect* effect)
    : QWidget(parent)
    , ui(new Ui::EffectTabHeader)
    , _effect(effect)
    , _isRunning(false)
{
    ui->setupUi(this);
    
    // Make sure the header is visible
    setVisible(true);
    
    // Add a distinct background to make it stand out
    setStyleSheet("background-color: #444; color: white; border: 1px solid #555;");
    
    // Set a minimum size to ensure visibility
    setMinimumSize(180, 35);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    
    // Set default state
    ui->startStopButton->setChecked(effect && effect->getEnabled());
    toggleRunningIndicator(effect && effect->getEnabled()); // Initialize button appearance
    
    // Connect signals
    connect(ui->closeButton, &QToolButton::clicked, this, &EffectTabHeader::onCloseClicked);
    connect(ui->startStopButton, &QToolButton::toggled, this, &EffectTabHeader::onStartStopClicked);
    
    // Set effect name
    if (effect) {
        // Use effect info name
        Lightscape::EffectInfo info = effect->GetStaticInfo();
        _effectName = info.name;
        ui->effectNameLabel->setText(_effectName);
        printf("[Lightscape][EffectTabHeader] Created header for effect: %s (ID: %s)\n", 
               _effectName.toStdString().c_str(),
               info.id.toStdString().c_str());
    } else {
        // Set a default name if no effect provided
        _effectName = "Unknown Effect";
        ui->effectNameLabel->setText(_effectName);
        printf("[Lightscape][EffectTabHeader] WARNING: Created header with null effect\n");
    }
}

EffectTabHeader::~EffectTabHeader()
{
    delete ui;
}

void EffectTabHeader::setEffectName(const QString& name)
{
    // Ensure we don't set an empty name
    if (name.isEmpty()) {
        _effectName = _effect ? _effect->GetStaticInfo().name : "Unknown Effect";
        
        // If still empty, use a default name
        if (_effectName.isEmpty()) {
            _effectName = "Effect";
        }
    } else {
        _effectName = name;
    }
    
    // Set the label text
    ui->effectNameLabel->setText(_effectName);
    printf("[Lightscape][EffectTabHeader] Set effect name to: %s\n", _effectName.toStdString().c_str());
    
    // Force the label to update
    ui->effectNameLabel->adjustSize();
    ui->effectNameLabel->update();
}

QString EffectTabHeader::getEffectName() const
{
    return _effectName;
}

void EffectTabHeader::toggleRunningIndicator(bool running)
{
    printf("[Lightscape][EffectTabHeader] Toggling running indicator to: %s\n", running ? "RUNNING" : "STOPPED");
    
    // Store the state
    _isRunning = running;
    
    // Set the button state - block signals to avoid recursive calls
    ui->startStopButton->blockSignals(true);
    ui->startStopButton->setChecked(running);
    ui->startStopButton->blockSignals(false);
    
    // Change button text to show state
    if (running) {
        ui->startStopButton->setText("■"); // Square for Stop
        ui->startStopButton->setToolTip("Stop Effect");
        ui->startStopButton->setStyleSheet("background-color: #c23616; color: white;"); // Red for running
    } else {
        ui->startStopButton->setText("▶"); // Triangle for Play
        ui->startStopButton->setToolTip("Start Effect");
        ui->startStopButton->setStyleSheet("background-color: #44bd32; color: white;"); // Green for stopped/ready
    }
}

void EffectTabHeader::setEffect(Lightscape::BaseEffect* effect)
{
    _effect = effect;
    
    if (effect) {
        // Update name from effect
        Lightscape::EffectInfo info = effect->GetStaticInfo();
        _effectName = info.name;
        ui->effectNameLabel->setText(_effectName);
        
        // Update running state
        toggleRunningIndicator(effect->getEnabled());
    }
}

Lightscape::BaseEffect* EffectTabHeader::getEffect() const
{
    return _effect;
}

void EffectTabHeader::onCloseClicked()
{
    printf("[Lightscape][EffectTabHeader] Close button clicked for effect: %s\n", 
          _effectName.toStdString().c_str());
          
    // Make sure the UI updates immediately
    setVisible(false);
    
    // Emit signal to notify the parent
    emit closeRequested();
}

void EffectTabHeader::onStartStopClicked(bool checked)
{
    printf("[Lightscape][EffectTabHeader] Start/Stop button clicked, checked: %s\n", checked ? "true" : "false");
    
    // The button state has already changed, update our visual state
    toggleRunningIndicator(checked);
    
    // Emit signal to inform parent
    emit startStopRequested(checked);
}

void EffectTabHeader::onRenameClicked()
{
    bool ok;
    QString newName = QInputDialog::getText(
        this, 
        "Rename Effect", 
        "Enter new name:", 
        QLineEdit::Normal,
        _effectName, 
        &ok
    );
    
    if (ok && !newName.isEmpty()) {
        setEffectName(newName);
        emit renamed(newName);
    }
}
