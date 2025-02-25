#include "core/ReferencePointSelector.h"
#include <QDataStream>
#include <QBuffer>

ReferencePointSelector::ReferencePointSelector(QWidget* parent)
    : QWidget(parent)
    , mainWidget(new QWidget(this))
    , mainLayout(new QVBoxLayout(mainWidget))
{
    setupUI();
}

ReferencePointSelector::~ReferencePointSelector()
{
    cleanupUI();
}

void ReferencePointSelector::setupUI()
{
    mainWidget->setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(4);
}

void ReferencePointSelector::cleanupUI()
{
    for(auto& ui : uiElements)
    {
        delete ui.comboBox;
        delete ui.checkBox;
    }
    uiElements.clear();
}

void ReferencePointSelector::rebuildUI()
{
    cleanupUI();

    for(const auto& point : referencePoints)
    {
        ReferencePointUI ui;
        
        // Create ComboBox
        ui.comboBox = new QComboBox(mainWidget);
        ui.comboBox->addItems(referencePoints.keys());
        ui.comboBox->setCurrentText(point.name);
        
        // Create CheckBox
        ui.checkBox = new QCheckBox(mainWidget);
        ui.checkBox->setText(tr("Enable"));
        ui.checkBox->setChecked(point.enabled);
        
        // Store current ID
        ui.currentId = point.deviceId;
        
        // Connect signals
        connect(ui.checkBox, &QCheckBox::stateChanged, this, &ReferencePointSelector::onCheckBoxStateChanged);
        connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
                this, &ReferencePointSelector::onComboBoxSelectionChanged);
        
        // Add to layout
        mainLayout->addWidget(ui.comboBox);
        mainLayout->addWidget(ui.checkBox);
        
        uiElements.append(ui);
    }
}

void ReferencePointSelector::addReferencePoint(const GridPosition& pos, bool isRGBDevice, 
                                             const QString& name, const QString& deviceId)
{
    if(!referencePoints.contains(deviceId))
    {
        referencePoints.insert(deviceId, ReferencePoint(pos, isRGBDevice, name, deviceId));
        updateUI();
        emit referencePointsChanged();
    }
}

void ReferencePointSelector::removeReferencePoint(const QString& deviceId)
{
    if(referencePoints.remove(deviceId) > 0)
    {
        updateUI();
        emit referencePointsChanged();
    }
}

void ReferencePointSelector::clearReferencePoints()
{
    referencePoints.clear();
    updateUI();
    emit referencePointsChanged();
}

QList<ReferencePointSelector::ReferencePoint> ReferencePointSelector::getEnabledReferencePoints() const
{
    QList<ReferencePoint> enabled;
    for(const auto& point : referencePoints)
    {
        if(point.enabled)
        {
            enabled.append(point);
        }
    }
    return enabled;
}

bool ReferencePointSelector::hasEnabledReferencePoints() const
{
    for(const auto& point : referencePoints)
    {
        if(point.enabled)
        {
            return true;
        }
    }
    return false;
}

void ReferencePointSelector::updateReferencePointPosition(const QString& deviceId, const GridPosition& newPos)
{
    if(referencePoints.contains(deviceId))
    {
        referencePoints[deviceId].position = newPos;
        emit referencePointsChanged();
    }
}

void ReferencePointSelector::setReferencePointEnabled(const QString& deviceId, bool enabled)
{
    if(referencePoints.contains(deviceId))
    {
        referencePoints[deviceId].enabled = enabled;
        updateUI();
        emit referencePointEnabled(deviceId, enabled);
        emit referencePointsChanged();
    }
}

void ReferencePointSelector::setTitle(const QString& title)
{
    mainWidget->setWindowTitle(title);
}

QByteArray ReferencePointSelector::saveState() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    
    // Save version
    stream << (quint32)1;
    
    // Save reference points
    stream << referencePoints;
    
    return data;
}

bool ReferencePointSelector::restoreState(const QByteArray& state)
{
    QDataStream stream(state);
    
    // Check version
    quint32 version;
    stream >> version;
    if(version != 1)
        return false;
    
    // Restore reference points
    stream >> referencePoints;
    
    updateUI();
    emit referencePointsChanged();
    
    return true;
}

void ReferencePointSelector::onCheckBoxStateChanged(int state)
{
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(sender());
    if(!checkbox)
        return;
        
    for(const auto& ui : uiElements)
    {
        if(ui.checkBox == checkbox)
        {
            setReferencePointEnabled(ui.currentId, state == Qt::Checked);
            break;
        }
    }
}

void ReferencePointSelector::onComboBoxSelectionChanged(int index)
{
    QComboBox* combobox = qobject_cast<QComboBox*>(sender());
    if(!combobox || index < 0)
        return;
        
    for(const auto& ui : uiElements)
    {
        if(ui.comboBox == combobox)
        {
            emit selectionChanged(ui.currentId);
            break;
        }
    }
}

void ReferencePointSelector::updateUI()
{
    rebuildUI();
}