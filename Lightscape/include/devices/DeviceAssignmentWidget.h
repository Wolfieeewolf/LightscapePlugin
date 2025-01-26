#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>

namespace Ui {
class DeviceAssignmentWidget;
}

class DeviceAssignmentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceAssignmentWidget(QWidget *parent = nullptr);
    ~DeviceAssignmentWidget();

private slots:
    void onAssignButtonClicked();
    void onDeviceSelectionChanged(int index);

private:
    Ui::DeviceAssignmentWidget *ui;
    void populateDeviceComboBox();
    void setupAssignmentTable();
};
