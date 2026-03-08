#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTableWidget>
#include <QTabWidget>
#include <vector>
#include "bkk_api.hpp"

struct StationData {
    std::vector<std::string> station_ids;
    std::string station_name;
    QTableWidget *tableWidget;
    std::vector<Arrival> arrivals;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onButtonClicked();

private:
    void populateTable(StationData &station);
    void setupTableWidget(QTableWidget *table);
    QColor getColorForMinutes(int minutes);

    QLabel *label;
    QTabWidget *tabWidget;
    StationData stationA;
    StationData stationB;
    int clickCount;
};

#endif // MAINWINDOW_H
