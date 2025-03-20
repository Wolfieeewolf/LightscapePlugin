/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| PreviewSettings.cpp                                       |
|                                                           |
| Settings management for preview renderer                  |
\*---------------------------------------------------------*/

#include "effects/PreviewSettings.h"
#include <QSettings>
#include <QMessageBox>

namespace Lightscape {

PreviewSettings::PreviewSettings(QWidget* parent)
    : QObject(parent)
    , _parent(parent)
{
    // Nothing to initialize
}

void PreviewSettings::loadSettings(float& rotationX, float& rotationY, float& rotationZ, float& zoom)
{
    QSettings settings("OpenRGB", "Lightscape");
    settings.beginGroup("PreviewSettings");
    
    if (settings.contains("DefaultRotationX")) {
        // Load saved orientation
        rotationX = settings.value("DefaultRotationX").toFloat();
        rotationY = settings.value("DefaultRotationY").toFloat();
        rotationZ = settings.value("DefaultRotationZ").toFloat();
        zoom = settings.value("DefaultZoom").toFloat();
    } else {
        // Use the hard-coded default values
        rotationX = DEFAULT_ROTATION_X;
        rotationY = DEFAULT_ROTATION_Y;
        rotationZ = DEFAULT_ROTATION_Z;
        zoom = DEFAULT_ZOOM;
    }
    
    settings.endGroup();
}

void PreviewSettings::saveSettings(float rotationX, float rotationY, float rotationZ, float zoom)
{
    QSettings settings("OpenRGB", "Lightscape");
    settings.beginGroup("PreviewSettings");
    settings.setValue("DefaultRotationX", rotationX);
    settings.setValue("DefaultRotationY", rotationY);
    settings.setValue("DefaultRotationZ", rotationZ);
    settings.setValue("DefaultZoom", zoom);
    settings.endGroup();
    
    if (_parent) {
        // Show a message box to confirm the settings were saved
        QMessageBox::information(_parent, "Default View Saved", 
                               "The current view has been saved as the default orientation.");
    }
}

void PreviewSettings::clearSettings()
{
    QSettings settings("OpenRGB", "Lightscape");
    settings.beginGroup("PreviewSettings");
    settings.remove(""); // Remove all keys in this group
    settings.endGroup();
    
    if (_parent) {
        QMessageBox::information(_parent, "Settings Reset", 
                               "All view settings have been reset to factory defaults.");
    }
}

} // namespace Lightscape