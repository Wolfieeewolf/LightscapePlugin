#include "effects/TestEffect/TestEffect.h"
#include "effects/SpatialControllerZone.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QColorDialog>
#include <QDebug>

namespace Lightscape {

TestEffect::TestEffect(QWidget *parent)
    : BaseEffect(parent)
{
    // Set minimum size for the widget
    setMinimumSize(350, 300);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    
    // Explicitly set the name on the object
    setObjectName("TestEffect");
    
    printf("[Lightscape][TestEffect] Test Effect created\n");
    
    // Create a basic UI layout for the test effect
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(8);
    
    // Create a header/label
    QLabel* header = new QLabel("Test Effect Settings", this);
    header->setStyleSheet("font-weight: bold; font-size: 14px;");
    
    // Create some basic controls
    QLabel* speedLabel = new QLabel("Speed:", this);
    speedLabel->setStyleSheet("font-weight: bold;");
    QSlider* speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(10, 100);
    speedSlider->setValue(speed);
    speedSlider->setMinimumHeight(24);
    speedSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    border: 1px solid #999999;"
        "    height: 8px;"
        "    background: #333333;"
        "    margin: 2px 0;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: #0088ff;"
        "    border: 1px solid #5c5c5c;"
        "    width: 18px;"
        "    margin: -2px 0;"
        "    border-radius: 3px;"
        "}"
    );
    
    QLabel* brightnessLabel = new QLabel("Brightness:", this);
    brightnessLabel->setStyleSheet("font-weight: bold;");
    QSlider* brightnessSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setRange(0, 100);
    brightnessSlider->setValue(brightness);
    brightnessSlider->setMinimumHeight(24);
    brightnessSlider->setStyleSheet(speedSlider->styleSheet()); // Use same style
    
    // Connect signals
    connect(speedSlider, &QSlider::valueChanged, this, [this](int value) {
        speed = value;
        emit settingsChanged();
    });
    
    connect(brightnessSlider, &QSlider::valueChanged, this, [this](int value) {
        brightness = value;
        emit settingsChanged();
    });
    
    // Add widgets to layout
    layout->addWidget(header);
    layout->addWidget(speedLabel);
    layout->addWidget(speedSlider);
    layout->addWidget(brightnessLabel);
    layout->addWidget(brightnessSlider);
    
    // Add some spacing
    layout->addSpacing(10);
    
    // Add a color picker section
    QLabel* colorLabel = new QLabel("Color:", this);
    colorLabel->setStyleSheet("font-weight: bold;");
    QPushButton* colorButton = new QPushButton("Select Color", this);
    colorButton->setMinimumHeight(28);
    colorButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #444444;"
        "    border: 1px solid #666666;"
        "    border-radius: 4px;"
        "    padding: 6px 12px;"
        "    color: white;"
        "}"
        "QPushButton:hover {"
        "    background-color: #555555;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #333333;"
        "}"
    );
    
    connect(colorButton, &QPushButton::clicked, this, [this]() {
        QColor initial = userColors.isEmpty() ? 
                        QColor(255, 0, 0) : 
                        QColor(RGBGetRValue(userColors.first()), 
                               RGBGetGValue(userColors.first()), 
                               RGBGetBValue(userColors.first()));
                               
        QColor color = QColorDialog::getColor(initial, this, "Select Effect Color");
        
        if (color.isValid()) {
            userColors.clear();
            userColors.append(ToRGBColor(color.red(), color.green(), color.blue()));
            emit settingsChanged();
        }
    });
    
    layout->addWidget(colorLabel);
    layout->addWidget(colorButton);
    
    // Add stretch at the end to push everything to the top
    layout->addStretch();
    
    // Set the layout
    setLayout(layout);
    
    // Debug output
    printf("[Lightscape][TestEffect] TestEffect UI created\n");
}

EffectInfo TestEffect::GetStaticInfo()
{
    EffectInfo info;
    info.name = QString("Test Effect");
    info.id = QString("test_effect");
    info.description = QString("A simple test effect that changes color based on position");
    info.category = EffectCategory::Basic;
    info.requiresReferencePoint = false;
    info.supportsPreview = true;
    printf("[Lightscape][TestEffect] GetStaticInfo created with name: %s, ID: %s\n", 
           info.name.toStdString().c_str(), 
           info.id.toStdString().c_str());
    return info;
}

RGBColor TestEffect::getColorForPosition(const GridPosition& pos, float time)
{
    // Modify calculations based on selected settings
    float speedFactor = speed / 50.0f; // Normalize to make 50 the default speed
    
    // If user has selected colors, use them instead of position-based colors
    if (!userColors.isEmpty()) {
        RGBColor baseColor = userColors.first();
        
        // Create a pulsing effect based on time and speed
        float pulse = (sin(time * speedFactor * 3.0f) + 1.0f) / 2.0f;
        
        // Apply brightness
        float brightnessFactor = brightness / 100.0f;
        int r = RGBGetRValue(baseColor) * brightnessFactor * pulse;
        int g = RGBGetGValue(baseColor) * brightnessFactor * pulse;
        int b = RGBGetBValue(baseColor) * brightnessFactor * pulse;
        
        printf("[Lightscape] TestEffect: Using user color with speed=%d, brightness=%d\n", speed, brightness);
        return ToRGBColor(r, g, b);
    }
    
    // Position-based colors with speed and brightness controls
    int r = (pos.x * 20 + static_cast<int>(time * speedFactor * 50)) % 255;
    int g = (pos.y * 20 + static_cast<int>(time * speedFactor * 30)) % 255;
    int b = (pos.z * 20 + static_cast<int>(time * speedFactor * 70)) % 255;
    
    // Apply brightness
    float brightnessFactor = brightness / 100.0f;
    r = static_cast<int>(r * brightnessFactor);
    g = static_cast<int>(g * brightnessFactor);
    b = static_cast<int>(b * brightnessFactor);
    
    return ToRGBColor(r, g, b);
}

void TestEffect::StepEffect(std::vector<ControllerZone*> zones)
{
    // Call the base class StepEffect to use our getColorForPosition method
    // This demonstrates using the existing spatial calculation for both interfaces
    BaseEffect::StepEffect(zones);
    
    // If we wanted custom behavior for non-spatial zones, we could add it here
    // For example, we could identify special devices and treat them differently
}

} // namespace Lightscape