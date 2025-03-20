/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| PreviewSettings.h                                         |
|                                                           |
| Settings management for preview renderer                  |
\*---------------------------------------------------------*/

#pragma once

#include <QObject>
#include <QWidget>
#include <QPoint>

namespace Lightscape {

class PreviewSettings : public QObject {
    Q_OBJECT

public:
    PreviewSettings(QWidget* parent = nullptr);
    ~PreviewSettings() = default;

    // Load/save view settings
    void loadSettings(float& rotationX, float& rotationY, float& rotationZ, float& zoom);
    void saveSettings(float rotationX, float rotationY, float rotationZ, float zoom);
    void clearSettings();
    
    // Factory defaults
    static constexpr float DEFAULT_ROTATION_X = -1.16f;
    static constexpr float DEFAULT_ROTATION_Y = -0.61f;
    static constexpr float DEFAULT_ROTATION_Z = 0.0f;
    static constexpr float DEFAULT_ZOOM = 1.0f;

private:
    QWidget* _parent;
};

} // namespace Lightscape