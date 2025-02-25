#include "core/ThemeManager.h"

QString ThemeManager::getStyleSheet(bool darkTheme)
{
    return darkTheme ? getDarkTheme() : getLightTheme();
}

QString ThemeManager::getDarkTheme()
{
    return QString(R"(
        QWidget {
            background-color: #2D2D2D;
            color: #FFFFFF;
        }

        QFrame {
            border: 1px solid #404040;
        }

        QPushButton {
            background-color: #404040;
            border: 1px solid #505050;
            padding: 5px;
            border-radius: 3px;
        }

        QPushButton:hover {
            background-color: #454545;
        }

        QPushButton:pressed {
            background-color: #353535;
        }

        QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox {
            background-color: #353535;
            border: 1px solid #505050;
            padding: 3px;
            border-radius: 2px;
        }

        QScrollBar:vertical {
            border: none;
            background: #2D2D2D;
            width: 10px;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical {
            background: #404040;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical:hover {
            background: #454545;
        }

        QMenu {
            background-color: #2D2D2D;
            border: 1px solid #404040;
        }

        QMenu::item {
            padding: 5px 20px;
        }

        QMenu::item:selected {
            background-color: #404040;
        }

        QTabWidget::pane {
            border: 1px solid #404040;
        }

        QTabBar::tab {
            background-color: #353535;
            padding: 5px 10px;
            border-top-left-radius: 3px;
            border-top-right-radius: 3px;
        }

        QTabBar::tab:selected {
            background-color: #404040;
        }

        QGroupBox {
            border: 1px solid #404040;
            margin-top: 1em;
            padding-top: 0.5em;
        }

        QGroupBox::title {
            color: #FFFFFF;
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 0 3px;
        }
    )");
}

QString ThemeManager::getLightTheme()
{
    return QString(R"(
        QWidget {
            background-color: #F0F0F0;
            color: #000000;
        }

        QFrame {
            border: 1px solid #D0D0D0;
        }

        QPushButton {
            background-color: #E0E0E0;
            border: 1px solid #C0C0C0;
            padding: 5px;
            border-radius: 3px;
        }

        QPushButton:hover {
            background-color: #D5D5D5;
        }

        QPushButton:pressed {
            background-color: #CACACA;
        }

        QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox {
            background-color: #FFFFFF;
            border: 1px solid #C0C0C0;
            padding: 3px;
            border-radius: 2px;
        }

        QScrollBar:vertical {
            border: none;
            background: #F0F0F0;
            width: 10px;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical {
            background: #D0D0D0;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical:hover {
            background: #C0C0C0;
        }

        QMenu {
            background-color: #F0F0F0;
            border: 1px solid #D0D0D0;
        }

        QMenu::item {
            padding: 5px 20px;
        }

        QMenu::item:selected {
            background-color: #E0E0E0;
        }

        QTabWidget::pane {
            border: 1px solid #D0D0D0;
        }

        QTabBar::tab {
            background-color: #E5E5E5;
            padding: 5px 10px;
            border-top-left-radius: 3px;
            border-top-right-radius: 3px;
        }

        QTabBar::tab:selected {
            background-color: #F0F0F0;
        }

        QGroupBox {
            border: 1px solid #D0D0D0;
            margin-top: 1em;
            padding-top: 0.5em;
        }

        QGroupBox::title {
            color: #000000;
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 0 3px;
        }
    )");
}
