#include "effects/ui/EffectsPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>

EffectsPanel::EffectsPanel(EffectEngine* effectEngine, 
                         GameEffectManager* gameEffectManager, 
                         QWidget *parent)
    : QWidget(parent)
    , effectEngine(effectEngine)
    , gameEffectManager(gameEffectManager)
{
    setupUI();
    setupConnections();
    updatePresetsList();
}

EffectsPanel::~EffectsPanel()
{
}

void EffectsPanel::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Effect Selection Group
    QGroupBox* effectGroup = new QGroupBox("Effect Selection", this);
    QVBoxLayout* effectLayout = new QVBoxLayout(effectGroup);

    effectCombo = new QComboBox(this);
    effectCombo->addItem("None");
    effectCombo->addItem("Radial Fade");
    effectCombo->addItem("Wave");
    effectCombo->addItem("Ripple");
    effectCombo->addItem("Layer Cascade");

    toggleButton = new QPushButton("Start", this);
    toggleButton->setCheckable(true);

    effectLayout->addWidget(effectCombo);
    effectLayout->addWidget(toggleButton);

    // Effect Parameters Group
    QGroupBox* paramGroup = new QGroupBox("Effect Parameters", this);
    QVBoxLayout* paramLayout = new QVBoxLayout(paramGroup);

    // Speed Control
    QLabel* speedLabel = new QLabel("Speed:", this);
    speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(1, 100);
    speedSlider->setValue(50);

    // Intensity Control
    QLabel* intensityLabel = new QLabel("Intensity:", this);
    intensitySlider = new QSlider(Qt::Horizontal, this);
    intensitySlider->setRange(1, 100);
    intensitySlider->setValue(100);

    paramLayout->addWidget(speedLabel);
    paramLayout->addWidget(speedSlider);
    paramLayout->addWidget(intensityLabel);
    paramLayout->addWidget(intensitySlider);

    // Color Control Group
    QGroupBox* colorGroup = new QGroupBox("Color Control", this);
    QVBoxLayout* colorLayout = new QVBoxLayout(colorGroup);

    colorPickerButton = new QPushButton("Pick Color", this);
    clearEffectsButton = new QPushButton("Clear Effects", this);

    colorLayout->addWidget(colorPickerButton);
    colorLayout->addWidget(clearEffectsButton);

    // Presets Group
    QGroupBox* presetGroup = new QGroupBox("Presets", this);
    QVBoxLayout* presetLayout = new QVBoxLayout(presetGroup);

    presetsList = new QListWidget(this);
    
    QHBoxLayout* presetButtonLayout = new QHBoxLayout();
    savePresetButton = new QPushButton("Save", this);
    loadPresetButton = new QPushButton("Load", this);
    deletePresetButton = new QPushButton("Delete", this);
    
    presetButtonLayout->addWidget(savePresetButton);
    presetButtonLayout->addWidget(loadPresetButton);
    presetButtonLayout->addWidget(deletePresetButton);

    presetLayout->addWidget(presetsList);
    presetLayout->addLayout(presetButtonLayout);

    // Add all groups to main layout
    mainLayout->addWidget(effectGroup);
    mainLayout->addWidget(paramGroup);
    mainLayout->addWidget(colorGroup);
    mainLayout->addWidget(presetGroup);
    mainLayout->addStretch();

    // Set size policy to allow expansion
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void EffectsPanel::setupConnections()
{
    connect(effectCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &EffectsPanel::onEffectChanged);
    
    connect(toggleButton, &QPushButton::toggled,
            this, &EffectsPanel::onEffectToggled);
    
    connect(speedSlider, &QSlider::valueChanged,
            this, &EffectsPanel::onSpeedChanged);
    
    connect(intensitySlider, &QSlider::valueChanged,
            this, &EffectsPanel::onIntensityChanged);
    
    connect(colorPickerButton, &QPushButton::clicked,
            this, &EffectsPanel::onColorPickerClicked);
    
    connect(clearEffectsButton, &QPushButton::clicked,
            this, &EffectsPanel::onClearEffectsClicked);
    
    connect(savePresetButton, &QPushButton::clicked,
            this, &EffectsPanel::onSavePresetClicked);
    
    connect(loadPresetButton, &QPushButton::clicked,
            this, &EffectsPanel::onLoadPresetClicked);
    
    connect(deletePresetButton, &QPushButton::clicked,
            this, &EffectsPanel::onDeletePresetClicked);
    
    connect(presetsList, &QListWidget::itemDoubleClicked,
            this, &EffectsPanel::onPresetSelected);
}

void EffectsPanel::onEffectChanged(int index)
{
    if (!effectEngine) return;

    EffectEngine::Effect effect = static_cast<EffectEngine::Effect>(index);
    if (toggleButton->isChecked()) {
        effectEngine->startEffect(effect);
    }
}

void EffectsPanel::onEffectToggled(bool checked)
{
    if (!effectEngine) return;

    if (checked) {
        effectEngine->startEffect(static_cast<EffectEngine::Effect>(effectCombo->currentIndex()));
        toggleButton->setText("Stop");
    } else {
        effectEngine->stopEffect();
        toggleButton->setText("Start");
    }
}

void EffectsPanel::onSpeedChanged(int value)
{
    if (!effectEngine) return;
    effectEngine->setSpeed(value);
}

void EffectsPanel::onIntensityChanged(int value)
{
    if (!effectEngine) return;
    effectEngine->setIntensity(value);
}

void EffectsPanel::onColorPickerClicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Effect Color");
    if (color.isValid() && effectEngine) {
        // Convert QColor to OpenRGB format (0x00BBGGRR)
        RGBColor rgb_color = (color.blue() << 16) | (color.green() << 8) | color.red();
        effectEngine->setColor(rgb_color);
    }
}

void EffectsPanel::onClearEffectsClicked()
{
    if (!effectEngine) return;
    effectEngine->stopEffect();
    toggleButton->setChecked(false);
    toggleButton->setText("Start");
    effectCombo->setCurrentIndex(0);
}

void EffectsPanel::updatePresetsList()
{
    // TODO: Load presets from settings/file
    presetsList->clear();
}

void EffectsPanel::onSavePresetClicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Save Preset",
                                       "Enter preset name:", QLineEdit::Normal,
                                       "", &ok);
    if (ok && !name.isEmpty()) {
        savePreset(name);
        updatePresetsList();
    }
}

void EffectsPanel::onLoadPresetClicked()
{
    QListWidgetItem* item = presetsList->currentItem();
    if (item) {
        loadPreset(item->text());
    }
}

void EffectsPanel::onDeletePresetClicked()
{
    QListWidgetItem* item = presetsList->currentItem();
    if (item) {
        if (QMessageBox::question(this, "Delete Preset",
                                "Are you sure you want to delete this preset?",
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            // TODO: Implement preset deletion
            delete item;
        }
    }
}

void EffectsPanel::onPresetSelected(QListWidgetItem* item)
{
    if (item) {
        loadPreset(item->text());
    }
}

void EffectsPanel::savePreset(const QString& name)
{
    // TODO: Implement preset saving
    // Save current effect settings to a preset file or settings
}

void EffectsPanel::loadPreset(const QString& name)
{
    // TODO: Implement preset loading
    // Load effect settings from a preset file or settings
}