#pragma once

#include "grid/GridTypes.h"

namespace Lightscape {

enum class DeviceType {
    RGB,
    NonRGB
};

// Device info structure used throughout the application
struct DeviceInfo {
    int index;
    DeviceType type;
    int zoneIndex = -1;
    int ledIndex = -1;
    GridPosition position;
};

} // namespace Lightscape