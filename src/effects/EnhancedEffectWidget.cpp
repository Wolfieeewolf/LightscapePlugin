/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EnhancedEffectWidget.cpp                                  |
|                                                           |
| Enhanced widget for managing multiple effects             |
\*---------------------------------------------------------*/

#include "effects/EnhancedEffectWidget.h"
#include "ui_EnhancedEffectWidget.h"
#include "effects/EffectManager.h"
#include "effects/EffectRegistry.h"
#include "effects/EffectSelectorDialog.h"
#include "effects/EffectStateManager.h"

#include <QResizeEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QInputDialog>
#include <QLabel>
#include <QTimer>
#include <QDebug>

namespace Lightscape {

EnhancedEffectWidget::EnhancedEffectWidget(::DeviceManager* deviceManager, ::SpatialGrid* spatialGrid, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EnhancedEffectWidget)
    , _deviceManager(deviceManager)
    , _spatialGrid(spatialGrid)
    , _effectsListPanel(nullptr)
    , _effectsControlPanel(nullptr)
    , _effectsPreviewPanel(nullptr)
    , _deviceListPanel(nullptr)
    , _currentEffect(nullptr)
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
}

EnhancedEffectWidget::~EnhancedEffectWidget()
{
    try {
        // Get a reference to the EffectManager
        auto& effectManager = EffectManager::getInstance();
        
        // Before we delete anything, ensure all effects are properly stopped
        if (_effectsListPanel) {
            QList<BaseEffect*> effects = _effectsListPanel->getAllEffects();
            for (BaseEffect* effect : effects) {
                if (!effect) continue;
                
                // Stop in EffectManager if running
                if (effectManager.isEffectRunning(effect)) {
                    effectManager.stopEffect(effect);
                }
                
                // Also stop directly
                if (effect->getEnabled()) {
                    effect->stop();
                }
            }
        }
        
        // Clear current effect in preview panel
        if (_effectsPreviewPanel) {
            _effectsPreviewPanel->clearEffect();
        }
        
        // Clear references to avoid dangling pointer issues
        _currentEffect = nullptr;
        
        // Clear maps before deleting UI
        _effectToDevices.clear();
        
        // Now safe to delete UI
        delete ui;
        ui = nullptr;
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in ~EnhancedEffectWidget:" << e.what();
    }
    catch (...) {
        qWarning() << "Unknown exception in ~EnhancedEffectWidget";
    }
}

void EnhancedEffectWidget::setupUI()
{
    try {
        // Create and add the panels
        
        // 1. Effects List Panel
        _effectsListPanel = new EffectsListPanel(this);
        if (ui->effectsListLayout) {
            ui->effectsListLayout->addWidget(_effectsListPanel);
        }
        
        // 2. Effects Control Panel
        _effectsControlPanel = new EffectsControlPanel(this);
        if (ui->effectsControlLayout) {
            ui->effectsControlLayout->addWidget(_effectsControlPanel);
            _effectsControlPanel->setMinimumHeight(250);
            printf("[Lightscape] Created EffectsControlPanel with minimum height of 250\n");
        }
        
        // 3. Effects Preview Panel
        _effectsPreviewPanel = new EffectsPreviewPanel(this);
        if (_effectsPreviewPanel && _spatialGrid) {
            _effectsPreviewPanel->setGrid(_spatialGrid);
            
            // Make sure header is not oversized
            QLabel* titleLabel = _effectsPreviewPanel->findChild<QLabel*>("titleLabel");
            if (titleLabel) {
                titleLabel->setMaximumHeight(25); // Limit the header height
            }
        }
        if (ui->effectsPreviewLayout) {
            ui->effectsPreviewLayout->addWidget(_effectsPreviewPanel);
        }
        
        // 4. Device List Panel
        _deviceListPanel = new DeviceListPanel(this);
        if (_deviceListPanel && _deviceManager && _spatialGrid) {
            _deviceListPanel->init(_deviceManager, _spatialGrid);
        }
        if (ui->deviceListLayout) {
            ui->deviceListLayout->addWidget(_deviceListPanel);
        }
        
        // Set size policy
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        
        // Set initial splitter sizes
        if (ui->mainSplitter) {
            // Set the distribution to match the mockup (roughly 1:2:1)
            ui->mainSplitter->setSizes(QList<int>() << 200 << 600 << 200);
        }
        
        // Set initial middle splitter sizes if it exists (control panel vs preview)
        if (ui->middleSplitter) {
            // Control panel gets a larger portion now for better visibility
            ui->middleSplitter->setSizes(QList<int>() << 250 << 350); // Increased control panel size
            printf("[Lightscape] Set middle splitter sizes for control panel visibility\n");
            
            // Force the middle splitter to respect the minimum sizes
            ui->middleSplitter->setCollapsible(0, false); // Don't allow control panel to collapse
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in setupUI:" << e.what();
    }
    catch (...) {
        qWarning() << "Unknown exception in setupUI";
    }
}

void EnhancedEffectWidget::setupConnections()
{
    try {
        // Initialize core components
        if (!_deviceManager) {
            printf("[Lightscape] WARNING: Device manager is null, some features may not work\n");
        }
        
        if (!_spatialGrid) {
            printf("[Lightscape] WARNING: Spatial grid is null, some features may not work\n");
        }
        
        // Get state manager instance and connect to its signals
        auto& stateManager = EffectStateManager::getInstance();
        
        // Connect state manager signals to our UI components
        connect(&stateManager, &EffectStateManager::selectedEffectChanged,
                this, &EnhancedEffectWidget::onEffectChanged);
                
        connect(&stateManager, &EffectStateManager::effectRunningStateChanged,
                this, [this](BaseEffect* effect, bool running) {
                    // Update UI state when effect running state changes
                    if (_effectsListPanel) {
                        _effectsListPanel->updateEffectRunningState(effect, running);
                    }
                    
                    // Update preview if needed
                    if (_effectsPreviewPanel && effect == _currentEffect) {
                        _effectsPreviewPanel->setEffect(effect);
                        _effectsPreviewPanel->getRenderer()->update();
                    }
                });
        
        connect(&stateManager, &EffectStateManager::effectDevicesChanged,
                this, [this](BaseEffect* effect, const QList<DeviceInfo>& devices) {
                    // Update device list selection if this is the current effect
                    if (effect == _currentEffect && _deviceListPanel) {
                        _deviceListPanel->setSelectedDevices(devices);
                    }
                    
                    // Update device positions in preview
                    updateDevicePositionsForPreview();
                });
                
        connect(&stateManager, &EffectStateManager::previewUpdateRequested, 
                this, [this]() {
                    if (_effectsPreviewPanel) {
                        _effectsPreviewPanel->getRenderer()->update();
                    }
                });
        
        // Initialize effect manager
        auto& effectManager = EffectManager::getInstance();
        
        // Make sure we have components before initializing
        if (_deviceManager && _spatialGrid) {
            effectManager.initialize(_deviceManager, _spatialGrid);
        } else {
            printf("[Lightscape] Cannot initialize EffectManager: Missing dependencies\n");
        }
        
        // Make sure preview is visible right from the start
        if (_effectsPreviewPanel) {
            if (_spatialGrid) {
                // Set the grid in the preview renderer
                _effectsPreviewPanel->setGrid(_spatialGrid);
                
                // Clear device positions initially
                _effectsPreviewPanel->setDevicePositions(QSet<GridPosition>());
            } else {
                printf("[Lightscape] Cannot set grid in preview panel: Grid is null\n");
            }
            
            // Force update of the preview
            _effectsPreviewPanel->getRenderer()->update();
            
            printf("[Lightscape] Preview initialized with grid\n");
        } else {
            printf("[Lightscape] WARNING: Effects preview panel is null\n");
        }
        
        // Connect effects list panel signals
        if (_effectsListPanel) {
            connect(_effectsListPanel, &EffectsListPanel::addEffectClicked,
                    this, &EnhancedEffectWidget::onAddEffectClicked);
            
            connect(_effectsListPanel, &EffectsListPanel::saveProfileClicked,
                    this, &EnhancedEffectWidget::onSaveProfileClicked);
            
            connect(_effectsListPanel, &EffectsListPanel::loadProfileClicked,
                    this, &EnhancedEffectWidget::onLoadProfileClicked);
                    
            connect(_effectsListPanel, &EffectsListPanel::effectChanged,
                    this, [&stateManager](BaseEffect* effect) {
                        // Update selected effect in state manager
                        stateManager.setSelectedEffect(effect);
                    });
            
            connect(_effectsListPanel, &EffectsListPanel::effectClosed,
                    this, &EnhancedEffectWidget::onEffectClosed);
            
            connect(_effectsListPanel, &EffectsListPanel::effectStartStopChanged,
                    this, &EnhancedEffectWidget::onEffectStartStopChanged);
            
            connect(_effectsListPanel, &EffectsListPanel::effectRenamed,
                    this, &EnhancedEffectWidget::onEffectRenamed);
        } else {
            printf("[Lightscape] WARNING: Effects list panel is null\n");
        }
        
        // Connect device list panel signals
        if (_deviceListPanel) {
            connect(_deviceListPanel, &DeviceListPanel::selectionChanged,
                    this, &EnhancedEffectWidget::onDeviceSelectionChanged);
        } else {
            printf("[Lightscape] WARNING: Device list panel is null\n");
        }
        
        // Give effect manager the preview renderer
        if (_effectsPreviewPanel) {
            PreviewRenderer* renderer = _effectsPreviewPanel->getRenderer();
            if (renderer) {
                effectManager.setPreviewRenderer(renderer);
                printf("[Lightscape] Set preview renderer in effect manager\n");
            } else {
                printf("[Lightscape] WARNING: Preview renderer is null\n");
            }
            
            // Connect to device-only mode toggled signal
            connect(_effectsPreviewPanel, &EffectsPreviewPanel::deviceOnlyModeToggled, this, [this](bool enabled) {
                // Log for debugging
                printf("[Lightscape] Device-only mode %s\n", enabled ? "enabled" : "disabled");
                
                // Update device positions when device-only mode is toggled
                if (enabled) {
                    updateDevicePositionsForPreview();
                }
            });
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in setupConnections:" << e.what();
        printf("[Lightscape] EXCEPTION in setupConnections: %s\n", e.what());
    }
    catch (...) {
        qWarning() << "Unknown exception in setupConnections";
        printf("[Lightscape] UNKNOWN EXCEPTION in setupConnections\n");
    }
}

void EnhancedEffectWidget::refresh()
{
    // Refresh the device list widget
    if (_deviceListPanel) {
        _deviceListPanel->refresh();
    }
    
    // Refresh the preview panel
    if (_effectsPreviewPanel) {
        _effectsPreviewPanel->getRenderer()->update();
    }
}

void EnhancedEffectWidget::migrateFromExistingEffect(BaseEffect* effect)
{
    if (!effect) return;
    
    try {
        // Create a new instance of the same effect
        QString effectId = effect->GetStaticInfo().id;
        addEffect(effectId);
        
        // TODO: Copy settings from the old effect to the new one
        // This would be a more complex operation requiring effect-specific handling
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in migrateFromExistingEffect:" << e.what();
    }
    catch (...) {
        qWarning() << "Unknown exception in migrateFromExistingEffect";
    }
}

void EnhancedEffectWidget::addEffect(const QString& effectId)
{
    if (effectId.isEmpty()) {
        qWarning() << "Attempted to add effect with empty ID";
        return;
    }
    
    try {
        auto& registry = EffectRegistry::getInstance();
        auto& stateManager = EffectStateManager::getInstance();
        
        if (!registry.hasEffect(effectId)) {
            qWarning() << "Effect not found:" << effectId;
            printf("[Lightscape][EnhancedEffectWidget] Effect not found: %s\n", effectId.toStdString().c_str());
            return;
        }
        
        // Create new effect instance
        void* rawEffect = registry.createEffect(effectId);
        if (!rawEffect) {
            qWarning() << "Failed to create effect:" << effectId;
            printf("[Lightscape][EnhancedEffectWidget] Failed to create effect: %s\n", effectId.toStdString().c_str());
            return;
        }
        
        BaseEffect* effect = static_cast<BaseEffect*>(rawEffect);
        if (!effect) {
            qWarning() << "Effect cast failed for:" << effectId;
            printf("[Lightscape][EnhancedEffectWidget] Effect cast failed for: %s\n", effectId.toStdString().c_str());
            return;
        }
        
        // Debug effect info
        printf("[Lightscape][EnhancedEffectWidget] Created effect - Name: %s, ID: %s\n", 
               effect->GetStaticInfo().name.toStdString().c_str(),
               effect->GetStaticInfo().id.toStdString().c_str());
        
        // Initialize effect but ensure it's stopped initially
        if (_deviceManager && _spatialGrid) {
            effect->initialize(_deviceManager, _spatialGrid);
            
            // Explicitly ensure the effect is stopped
            effect->stop();
            effect->setEnabled(false);
            printf("[Lightscape][EnhancedEffectWidget] Ensuring effect starts in stopped state: %s\n", 
                   effectId.toStdString().c_str());
        }
        
        // Get reference point from the grid's user position
        if (_spatialGrid && _spatialGrid->HasUserPosition() &&
            effect->GetStaticInfo().requiresReferencePoint) {
            auto userPos = _spatialGrid->GetUserPosition();
            if (userPos.has_value()) {
                effect->setReferencePoint(userPos.value());
            }
        }
        
        // Register with the state manager first
        stateManager.addEffect(effect);
        
        // Add to the UI panels 
        if (_effectsListPanel) {
            printf("[Lightscape][EnhancedEffectWidget] Adding effect to UI: %s\n", 
                   effect->GetStaticInfo().name.toStdString().c_str());
            _effectsListPanel->addEffect(effect);
        }
        
        // Make sure it gets selected
        stateManager.setSelectedEffect(effect);
        
        // Debug output
        printf("[Lightscape][EnhancedEffectWidget] Added effect: %s\n", effectId.toStdString().c_str());
        
        // Ensure this effect gets devices
        if (_deviceListPanel) {
            // Create an empty device list for the new effect
            // We won't select any devices by default - let the user choose
            QList<DeviceInfo> devices;
            
            // Set devices for the effect (empty list)
            printf("[Lightscape][EnhancedEffectWidget] Setting empty device list for new effect\n");
            stateManager.setDevicesForEffect(effect, devices);
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in addEffect:" << e.what();
        printf("[Lightscape][EnhancedEffectWidget] Exception in addEffect: %s\n", e.what());
    }
    catch (...) {
        qWarning() << "Unknown exception in addEffect";
        printf("[Lightscape][EnhancedEffectWidget] Unknown exception in addEffect\n");
    }
}

void EnhancedEffectWidget::onAddEffectClicked()
{
    try {
        // Show effect selector dialog
        EffectSelectorDialog dialog(this);
        
        if (dialog.exec() == QDialog::Accepted) {
            QString effectId = dialog.getSelectedEffectId();
            
            if (!effectId.isEmpty()) {
                addEffect(effectId);
            }
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in onAddEffectClicked:" << e.what();
    }
    catch (...) {
        qWarning() << "Unknown exception in onAddEffectClicked";
    }
}

void EnhancedEffectWidget::onSaveProfileClicked()
{
    try {
        if (!_effectsListPanel) {
            qWarning() << "Cannot save profile: _effectsListPanel is null";
            return;
        }
        
        // Get all effects
        QList<BaseEffect*> effects = _effectsListPanel->getAllEffects();
        
        if (effects.isEmpty()) {
            QMessageBox::information(this, "Save Profile", "No effects to save.");
            return;
        }
        
        // Ask for file location
        QString filename = QFileDialog::getSaveFileName(
            this,
            "Save Profile",
            "",
            "Lightscape Profiles (*.json)"
        );
        
        if (filename.isEmpty()) return;
        
        // Add .json extension if not present
        if (!filename.endsWith(".json")) {
            filename += ".json";
        }
        
        // Create profile JSON
        QJsonObject profileObject;
        QJsonArray effectsArray;
        
        // Save each effect
        for (BaseEffect* effect : effects) {
            if (!effect) continue;
            
            QJsonObject effectObject;
            
            // Save effect ID
            effectObject["id"] = effect->GetStaticInfo().id;
            
            // Save effect settings
            effectObject["settings"] = effect->saveSettings();
            
            // Save device selections
            QJsonArray devicesArray;
            QList<DeviceInfo> devices = _effectToDevices.value(effect, QList<DeviceInfo>());
            
            for (const DeviceInfo& device : devices) {
                QJsonObject deviceObject;
                deviceObject["index"] = device.index;
                deviceObject["type"] = static_cast<int>(device.type);
                
                QJsonObject posObject;
                posObject["x"] = device.position.x;
                posObject["y"] = device.position.y;
                posObject["z"] = device.position.z;
                deviceObject["position"] = posObject;
                
                devicesArray.append(deviceObject);
            }
            
            effectObject["devices"] = devicesArray;
            
            // Add to effects array
            effectsArray.append(effectObject);
        }
        
        profileObject["effects"] = effectsArray;
        profileObject["version"] = 1.0;
        
        // Write to file
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(profileObject);
            file.write(doc.toJson());
            file.close();
            
            QMessageBox::information(this, "Save Profile", "Profile saved successfully.");
        } else {
            QMessageBox::warning(this, "Save Profile", "Failed to save profile: " + file.errorString());
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in onSaveProfileClicked:" << e.what();
    }
    catch (...) {
        qWarning() << "Unknown exception in onSaveProfileClicked";
    }
}

void EnhancedEffectWidget::onLoadProfileClicked()
{
    try {
        if (!_effectsListPanel) {
            qWarning() << "Cannot load profile: _effectsListPanel is null";
            return;
        }
        
        // Ask for file location
        QString filename = QFileDialog::getOpenFileName(
            this,
            "Load Profile",
            "",
            "Lightscape Profiles (*.json)"
        );
        
        if (filename.isEmpty()) return;
        
        // Read file
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Load Profile", "Failed to open profile: " + file.errorString());
            return;
        }
        
        QByteArray data = file.readAll();
        file.close();
        
        // Parse JSON
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        
        if (error.error != QJsonParseError::NoError) {
            QMessageBox::warning(this, "Load Profile", "Failed to parse profile: " + error.errorString());
            return;
        }
        
        QJsonObject profileObject = doc.object();
        
        // Check version
        if (!profileObject.contains("version") || !profileObject.contains("effects")) {
            QMessageBox::warning(this, "Load Profile", "Invalid profile format.");
            return;
        }
        
        // Close all existing effects
        QList<BaseEffect*> existingEffects = _effectsListPanel->getAllEffects();
        for (BaseEffect* effect : existingEffects) {
            if (!effect) continue;
            
            _effectsListPanel->closeEffect(effect);
        }
        
        // Load effects
        QJsonArray effectsArray = profileObject["effects"].toArray();
        bool anyEffectLoaded = false;
        
        for (const QJsonValue& effectValue : effectsArray) {
            QJsonObject effectObject = effectValue.toObject();
            
            // Check required fields
            if (!effectObject.contains("id") || !effectObject.contains("settings")) {
                continue;
            }
            
            // Create effect
            QString effectId = effectObject["id"].toString();
            if (effectId.isEmpty()) continue;
            
            void* rawEffect = nullptr;
            try {
                rawEffect = EffectRegistry::getInstance().createEffect(effectId);
            } 
            catch (...) {
                qWarning() << "Exception creating effect:" << effectId;
                continue;
            }
            
            if (!rawEffect) {
                qWarning() << "Failed to create effect:" << effectId;
                continue;
            }
            
            BaseEffect* effect = static_cast<BaseEffect*>(rawEffect);
            if (!effect) {
                qWarning() << "Failed to cast effect:" << effectId;
                continue;
            }
            
            // Initialize effect
            if (_deviceManager && _spatialGrid) {
                effect->initialize(_deviceManager, _spatialGrid);
            }
            
            // Load settings
            QJsonObject settingsObject = effectObject["settings"].toObject();
            effect->loadSettings(settingsObject);
            
            // Add to tab widget
            _effectsListPanel->addEffect(effect);
            
            // Load device selections
            QList<DeviceInfo> devices;
            
            if (effectObject.contains("devices")) {
                QJsonArray devicesArray = effectObject["devices"].toArray();
                
                for (const QJsonValue& deviceValue : devicesArray) {
                    QJsonObject deviceObject = deviceValue.toObject();
                    
                    DeviceInfo device;
                    device.index = deviceObject["index"].toInt();
                    device.type = static_cast<DeviceType>(deviceObject["type"].toInt());
                    
                    if (deviceObject.contains("position")) {
                        QJsonObject posObject = deviceObject["position"].toObject();
                        device.position.x = posObject["x"].toInt();
                        device.position.y = posObject["y"].toInt();
                        device.position.z = posObject["z"].toInt();
                    }
                    
                    devices.append(device);
                }
            }
            
            // Store device selections
            _effectToDevices[effect] = devices;
            
            // Update preview if this is the first effect
            if (!anyEffectLoaded && _effectsPreviewPanel) {
                _effectsPreviewPanel->setEffect(effect);
                anyEffectLoaded = true;
            }
            
            // Emit signal
            emit effectAdded(effect);
        }
        
        QMessageBox::information(this, "Load Profile", "Profile loaded successfully.");
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in onLoadProfileClicked:" << e.what();
    }
    catch (...) {
        qWarning() << "Unknown exception in onLoadProfileClicked";
    }
}

void EnhancedEffectWidget::onResetViewClicked()
{
    if (_effectsPreviewPanel) {
        _effectsPreviewPanel->resetView();
    }
}

void EnhancedEffectWidget::onToggleViewClicked()
{
    if (_effectsPreviewPanel) {
        _effectsPreviewPanel->toggleViewMode();
    }
}

void EnhancedEffectWidget::onChangeLayerClicked()
{
    if (_effectsPreviewPanel) {
        _effectsPreviewPanel->showLayerDialog();
    }
}

void EnhancedEffectWidget::onSelectAllClicked()
{
    if (_deviceListPanel && _currentEffect) {
        // Find the button within DeviceListWidget and simulate a click
        DeviceListWidget* deviceListWidget = _deviceListPanel->getDeviceListWidget();
        if (deviceListWidget) {
            QPushButton* selectAllButton = deviceListWidget->findChild<QPushButton*>("selectAllButton");
            if (selectAllButton) {
                selectAllButton->click();
            }
        }
        // The selection change signal will update our mapping
    }
}

void EnhancedEffectWidget::onClearAllClicked()
{
    if (_deviceListPanel && _currentEffect) {
        // Find the button within DeviceListWidget and simulate a click
        DeviceListWidget* deviceListWidget = _deviceListPanel->getDeviceListWidget();
        if (deviceListWidget) {
            QPushButton* clearAllButton = deviceListWidget->findChild<QPushButton*>("clearAllButton");
            if (clearAllButton) {
                clearAllButton->click();
            }
        }
        // The selection change signal will update our mapping
    }
}

void EnhancedEffectWidget::onEffectChanged(BaseEffect* effect)
{
    // Update the current effect reference
    _currentEffect = effect;
    
    if (!effect) {
        // Clear current selection but keep grid displayed
        printf("[Lightscape] No effect selected, clearing UI\n");
        
        // Important: We still want to show the grid even with no effect
        if (_effectsPreviewPanel) {
            _effectsPreviewPanel->clearEffect();
            // Force update to ensure grid is displayed
            _effectsPreviewPanel->getRenderer()->update();
        }
        
        // Note: We no longer clear the control panel since we're using tabs
        // This preserves all effect settings
        
        if (_deviceListPanel) {
            _deviceListPanel->setSelectedDevices(QList<DeviceInfo>());
        }
        
        return;
    }
    
    try {
        printf("[Lightscape] Effect selected: %s (running: %s)\n", 
               effect->GetStaticInfo().name.toStdString().c_str(), 
               effect->getEnabled() ? "true" : "false");
        
        // Update UI components with the selected effect
        
        // 1. Control panel - add effect controls to the control panel
        if (_effectsControlPanel) {
            printf("[Lightscape] Adding/updating effect in EffectsControlPanel: %s\n", 
                   effect->GetStaticInfo().name.toStdString().c_str());
                
            // Always select the tab if it exists or create it if it doesn't
            // Instead of clearing existing tabs, we just select or create
            if (_effectsControlPanel->hasEffect(effect)) {
                _effectsControlPanel->setCurrentEffect(effect);
            } else {
                _effectsControlPanel->setEffect(effect);
            }
        }
        
        // 2. Preview panel
        if (_effectsPreviewPanel) {
            _effectsPreviewPanel->setEffect(effect);
            updateDevicePositionsForPreview(); // Update device positions when effect changes
            _effectsPreviewPanel->getRenderer()->update();
        }
        
        // 3. Device list selection
        if (_deviceListPanel) {
            // Get devices from state manager
            QList<DeviceInfo> devices = EffectStateManager::getInstance().getDevicesForEffect(effect);
            
            // Only update the device list if there are devices assigned to this effect
            if (!devices.isEmpty()) {
                printf("[Lightscape] Setting device selection from effect (found %d devices)\n", devices.size());
                _deviceListPanel->setSelectedDevices(devices);
            } else {
                printf("[Lightscape] No devices assigned to effect, keeping current device selection\n");
                // Don't clear the device selection if the effect has no devices
                // Instead, update the effect with the current device selection
                QList<DeviceInfo> currentDevices = _deviceListPanel->getSelectedDevices();
                if (!currentDevices.isEmpty()) {
                    printf("[Lightscape] Applying current device selection to effect (%d devices)\n", 
                           currentDevices.size());
                    EffectStateManager::getInstance().setDevicesForEffect(effect, currentDevices);
                    
                    // Update preview
                    updateDevicePositionsForPreview();
                }
            }
        }
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in onEffectChanged:" << e.what();
    }
    catch (...) {
        qWarning() << "Unknown exception in onEffectChanged";
    }
}

void EnhancedEffectWidget::onEffectClosed(BaseEffect* effect)
{
    if (!effect) return;
    
    printf("[Lightscape][EnhancedEffectWidget] Effect closed: %s\n", 
           effect->GetStaticInfo().name.toStdString().c_str());
    
    // Remove the effect's tab from the control panel
    if (_effectsControlPanel) {
        _effectsControlPanel->clearEffect(effect);
    }
    
    // The state manager will handle all of the state updates and proper cleanup
    EffectStateManager::getInstance().removeEffect(effect);
    
    // Force an UI update
    QTimer::singleShot(100, [this, effect]() {
        if (_effectsListPanel) {
            _effectsListPanel->updateGeometry();
        }
        
        // If this was the current effect, clear it
        if (_currentEffect == effect) {
            _currentEffect = nullptr;
            
            // Select a new effect if available
            QList<BaseEffect*> effects = _effectsListPanel->getAllEffects();
            if (!effects.isEmpty()) {
                EffectStateManager::getInstance().setSelectedEffect(effects.first());
            }
        }
    });
}

void EnhancedEffectWidget::onEffectStartStopChanged(BaseEffect* effect, bool running)
{
    if (!effect) return;
    
    printf("[Lightscape][EnhancedEffectWidget] Effect start/stop changed: %s -> %s\n",
           effect->GetStaticInfo().name.toStdString().c_str(),
           running ? "RUNNING" : "STOPPED");
    
    // Let the state manager handle everything
    EffectStateManager::getInstance().setEffectRunning(effect, running);
    
    // Force an effect update if the effect is now running
    if (running) {
        // Wait a short time to let the effect initialize
        QTimer::singleShot(100, [effect]() {
            if (effect->getEnabled()) {
                printf("[Lightscape][EnhancedEffectWidget] Forcing initial effect update\n");
                // Get devices for this effect from state manager
                auto& stateManager = EffectStateManager::getInstance();
                QList<DeviceInfo> devices = stateManager.getDevicesForEffect(effect);
                if (!devices.isEmpty()) {
                    effect->applyToDevices(devices);
                }
            }
        });
    }
}

void EnhancedEffectWidget::onEffectRenamed(BaseEffect* effect, const QString& newName)
{
    // Nothing needs to be done here beyond the tab changes already handled by EffectTabWidget
    Q_UNUSED(effect);
    Q_UNUSED(newName);
}

void EnhancedEffectWidget::onDeviceSelectionChanged(const QList<DeviceInfo>& devices)
{
    if (!_currentEffect) return;
    
    try {
        printf("[Lightscape][EnhancedEffectWidget] Device selection changed: %d devices for effect %s\n", 
               devices.size(), 
               _currentEffect->GetStaticInfo().name.toStdString().c_str());
        
        // Let the state manager handle device assignment
        EffectStateManager::getInstance().setDevicesForEffect(_currentEffect, devices);
        
        // Update preview
        updateDevicePositionsForPreview();
        
        // Update active devices for current effect in effect manager
        updateActiveDevicesForCurrentEffect();
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in onDeviceSelectionChanged:" << e.what();
        printf("[Lightscape][EnhancedEffectWidget] Exception in onDeviceSelectionChanged: %s\n", e.what());
    }
    catch (...) {
        qWarning() << "Unknown exception in onDeviceSelectionChanged";
        printf("[Lightscape][EnhancedEffectWidget] Unknown exception in onDeviceSelectionChanged\n");
    }
}

void EnhancedEffectWidget::updateActiveDevicesForCurrentEffect()
{
    if (!_currentEffect) return;
    
    try {
        // Get devices from the state manager for the current effect
        QList<DeviceInfo> devices = EffectStateManager::getInstance().getDevicesForEffect(_currentEffect);
        
        printf("[Lightscape][EnhancedEffectWidget] Updating active devices for current effect: %d devices\n", devices.size());
        
        // Update active devices in the effect manager
        EffectManager::getInstance().setActiveDevicesForEffect(_currentEffect, devices);
    }
    catch (const std::exception& e) {
        qWarning() << "Exception in updateActiveDevicesForCurrentEffect:" << e.what();
        printf("[Lightscape][EnhancedEffectWidget] Exception in updateActiveDevicesForCurrentEffect: %s\n", e.what());
    }
    catch (...) {
        qWarning() << "Unknown exception in updateActiveDevicesForCurrentEffect";
        printf("[Lightscape][EnhancedEffectWidget] Unknown exception in updateActiveDevicesForCurrentEffect\n");
    }
}

QSize EnhancedEffectWidget::sizeHint() const
{
    return calculateNeededSize();
}

QSize EnhancedEffectWidget::minimumSizeHint() const
{
    return QSize(800, 600);
}

QSize EnhancedEffectWidget::calculateNeededSize() const
{
    return QSize(1000, 700);
}

void EnhancedEffectWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
}

void Lightscape::EnhancedEffectWidget::updateDevicePositionsForPreview()
{
    if (!_effectsPreviewPanel || !_currentEffect) return;
    
    // Create a set of all device positions for the preview
    QSet<GridPosition> devicePositions;
    
    // Get the selected devices from the current effect
    QList<DeviceInfo> selectedDevices = EffectStateManager::getInstance().getDevicesForEffect(_currentEffect);
    
    // Add only the positions of selected devices
    for (const DeviceInfo& device : selectedDevices) {
        devicePositions.insert(device.position);
        printf("[Lightscape] Adding selected device position to preview: (%d, %d, %d)\n",
               device.position.x, device.position.y, device.position.z);
    }
    
    // Set the device positions in the preview renderer
    _effectsPreviewPanel->setDevicePositions(devicePositions);
    printf("[Lightscape] Total selected device positions: %d\n", devicePositions.size());
}

} // namespace Lightscape