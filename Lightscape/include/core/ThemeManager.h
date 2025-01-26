#pragma once

#include <QString>

class ThemeManager
{
public:
    static QString getStyleSheet(bool darkTheme);

private:
    static QString getDarkTheme();
    static QString getLightTheme();
};
