/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| HashFunctions.h                                           |
|                                                           |
| Hash functions for custom types                           |
\*---------------------------------------------------------*/

#pragma once

#include <QtGlobal>
#include <QtCore/qhash.h>
#include <QPair>
#include "core/Types.h"

// Define a qHash function for DeviceType enum
inline uint qHash(const Lightscape::DeviceType &key, uint seed = 0) noexcept
{
    return qHash(static_cast<int>(key), seed);
}

// Define a qHash function for QPair<int, DeviceType>
inline uint qHash(const QPair<int, Lightscape::DeviceType> &key, uint seed = 0) noexcept
{
    return qHash(key.first, seed) ^ (qHash(key.second, seed) << 1);
}
