#include "effects/EffectWidget.h"
#include "ui_EffectWidget.h"
#include "effects/EffectManager.h"

#include <QResizeEvent>
#include <QDebug>
#include <QVBoxLayout>

EffectWidget::EffectWidget(DeviceManager* manager, SpatialGrid* grid, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EffectWidget)
    , deviceManager(manager)
    , spatialGrid(grid)
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
    updateEffectsList();
    updateDeviceList();
}

EffectWidget::~EffectWidget()
{
    clearDeviceList();
    delete ui;
}

void EffectWidget::setupUI()
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}

void EffectWidget::setupConnections()
{
    // Initialize effect manager
    auto& effectManager = Lightscape::EffectManager::getInstance();
    effectManager.initialize(deviceManager, spatialGrid);
    
    // Setup FPS reduction checkbox
    connect(ui->reduceFPSCheckBox, &QCheckBox::toggled, [&](bool checked) {
        effectManager.setReducedFps(checked);
    });
    
    // Preview toggle
    connect(ui->previewCheckBox, &QCheckBox::toggled, [&](bool checked) {
        effectManager.setPreviewEnabled(checked);
        ui->previewWidget->setVisible(checked);
        emit previewToggled(checked);
    });

    // Effects list
    connect(ui->effectsList, &QListWidget::currentItemChanged,
            this, &EffectWidget::onEffectSelected);

    // Effects list updates
    connect(&Lightscape::EffectList::getInstance(), &Lightscape::EffectList::effectAdded,
            this, &EffectWidget::updateEffectsList);
    connect(&Lightscape::EffectList::getInstance(), &Lightscape::EffectList::effectRemoved,
            this, &EffectWidget::updateEffectsList);

    // Select all button
    connect(ui->selectAllButton, &QPushButton::clicked,
            this, &EffectWidget::onSelectAllClicked);

    // Device manager connections
    if (deviceManager) {
        connect(deviceManager, &DeviceManager::deviceListChanged,
                this, &EffectWidget::updateDeviceList);
    }

    // Grid assignment changes
    if (spatialGrid) {
        connect(spatialGrid, &SpatialGrid::assignmentsChanged,
                this, &EffectWidget::updateDeviceList);
    }
}

// This has been moved to a lambda in setupConnections()
void EffectWidget::onPreviewToggled(bool checked)
{
    // Keep the method for compatibility but it's now handled in the lambda
    ui->previewWidget->setVisible(checked);
    emit previewToggled(checked);
}

void EffectWidget::updateEffectsList()
{
    ui->effectsList->clear();

    // Get categories and sort them
    QStringList categories = Lightscape::EffectList::getInstance().getCategories();
    categories.sort();

    for (const QString& category : categories)
    {
        // Add category header
        QListWidgetItem* header = new QListWidgetItem(category);
        header->setFlags(Qt::NoItemFlags);
        header->setBackground(QBrush(QColor(80, 80, 80)));
        ui->effectsList->addItem(header);

        // Add effects for this category
        Lightscape::EffectCategory cat;
        if (category == "Spatial") cat = Lightscape::EffectCategory::Spatial;
        else if (category == "Basic") cat = Lightscape::EffectCategory::Basic;
        else if (category == "Advanced") cat = Lightscape::EffectCategory::Advanced;
        else cat = Lightscape::EffectCategory::Custom;

        auto effects = Lightscape::EffectList::getInstance().getEffectsByCategory(cat);
        for (const auto& effect : effects)
        {
            QListWidgetItem* item = new QListWidgetItem(effect.name);
            item->setData(Qt::UserRole, effect.id);
            ui->effectsList->addItem(item);
        }
    }
}

void EffectWidget::updateDeviceList()
{
    if (!deviceManager || !spatialGrid) return;
    
    clearDeviceList();
    
    // Track unique devices to avoid duplicates
    QMap<QPair<int, Lightscape::DeviceType>, QString> uniqueDevices;

    // Get list of all grid positions
    GridDimensions dims = spatialGrid->GetDimensions();
    for (int z = 0; z < dims.depth; z++) {
        for (int y = 0; y < dims.height; y++) {
            for (int x = 0; x < dims.width; x++) {
                GridPosition pos(x, y, z);
                
                // Get assignments for this position
                const auto& assignments = spatialGrid->GetAssignments(pos);
                
                // Add each assigned device
                for (const auto& assignment : assignments) {
                    QPair<int, Lightscape::DeviceType> deviceKey(assignment.device_index, 
                                                               assignment.device_type);
                                                               
                    // Add to unique devices if not already present
                    if (!uniqueDevices.contains(deviceKey)) {
                        QString name = deviceManager->GetDeviceName(assignment.device_index, 
                                                                 assignment.device_type);
                        uniqueDevices[deviceKey] = name;
                    }
                }
            }
        }
    }

    // Create checkboxes for unique devices
    for (auto it = uniqueDevices.begin(); it != uniqueDevices.end(); ++it) {
        createDeviceCheckbox(it.value(), it.key().first, it.key().second);
    }
}

void EffectWidget::createDeviceCheckbox(const QString& name, int deviceIndex, 
                                      Lightscape::DeviceType type)
{
    QCheckBox* checkbox = new QCheckBox(name, ui->deviceScrollContent);
    connect(checkbox, &QCheckBox::toggled, this, &EffectWidget::onDeviceToggled);

    deviceCheckboxes[checkbox] = {deviceIndex, type, false};
    ui->deviceListLayout->addWidget(checkbox);
}

void EffectWidget::clearDeviceList()
{
    QList<QCheckBox*> checkboxes = deviceCheckboxes.keys();
    for (QCheckBox* checkbox : checkboxes) {
        ui->deviceListLayout->removeWidget(checkbox);
        checkbox->deleteLater();
    }
    deviceCheckboxes.clear();
}

void EffectWidget::onEffectSelected()
{
    QListWidgetItem* item = ui->effectsList->currentItem();
    if (item && !item->data(Qt::UserRole).toString().isEmpty())
    {
        QString effectId = item->data(Qt::UserRole).toString();
        
        // First, stop any existing effect
        auto& effectManager = Lightscape::EffectManager::getInstance();
        effectManager.stopEffect();
        
        // Start the new effect
        if (effectManager.startEffect(effectId))
        {
            // Get the effect widget and add it to the controls
            Lightscape::BaseEffect* effect = effectManager.getCurrentEffect();
            if (effect)
            {
                // Clear existing control widgets
                QLayoutItem* item;
                while ((item = ui->controlsLayout->takeAt(0)) != nullptr)
                {
                    if (item->widget())
                    {
                        item->widget()->hide();
                    }
                    delete item;
                }
                
                // Add the effect's control widget
                ui->controlsLayout->addWidget(effect);
                effect->show();
                
                // Update active devices
                updateDeviceSelections();
            }
        }
        
        emit effectSelected(effectId);
    }
}

void EffectWidget::updateDeviceSelections()
{
    QList<Lightscape::DeviceInfo> activeDevices;
    
    for (auto it = deviceCheckboxes.begin(); it != deviceCheckboxes.end(); ++it)
    {
        if (it.key()->isChecked())
        {
            Lightscape::DeviceInfo info;
            info.index = it.value().index;
            info.type = it.value().type;
            
            // Get position from spatial grid
            QString layer;
            GridPosition pos;
            if (deviceManager && deviceManager->GetDeviceAssignmentInfo(info.index, info.type, layer, pos))
            {
                info.position = pos;
                activeDevices.append(info);
            }
        }
    }
    
    // Set active devices in the effect manager
    Lightscape::EffectManager::getInstance().setActiveDevices(activeDevices);
}

void EffectWidget::onDeviceToggled()
{
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(sender());
    if (checkbox && deviceCheckboxes.contains(checkbox)) {
        deviceCheckboxes[checkbox].selected = checkbox->isChecked();
        updateDeviceSelections();
        emit deviceSelectionChanged();
    }
}

void EffectWidget::onSelectAllClicked()
{
    bool allChecked = true;
    for (const auto& pair : deviceCheckboxes) {
        if (!pair.selected) {
            allChecked = false;
            break;
        }
    }

    // Toggle all checkboxes to opposite state
    for (auto checkboxIt = deviceCheckboxes.begin(); checkboxIt != deviceCheckboxes.end(); ++checkboxIt) {
        checkboxIt.key()->setChecked(!allChecked);
    }
    
    // Update active devices
    updateDeviceSelections();
}

QSize EffectWidget::sizeHint() const
{
    return calculateNeededSize();
}

QSize EffectWidget::minimumSizeHint() const
{
    return QSize(250, 400);
}

QSize EffectWidget::calculateNeededSize() const
{
    return QSize(350, 600);
}

void EffectWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
}