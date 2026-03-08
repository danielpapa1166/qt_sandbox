#include "mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTabWidget>
#include <QHeaderView>
#include <QApplication>
#include <QScreen>
#include <QStatusBar>
#include <algorithm>
#include "bkk_api.hpp"

static std::unique_ptr<BkkApi> api;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), clickCount(0)
{
    setWindowTitle("🚌 Bangkok Transport Arrivals Monitor");
    resize(1000, 700);
    
    // Set window icon background with gradient
    setStyleSheet(
        "QMainWindow { background-color: #f5f7fa; }"
        "QWidget { background-color: #f5f7fa; }"
    );

    // Create central widget and main layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Title label with gradient styling
    label = new QLabel("🚌 Bangkok Transport Arrivals");
    label->setStyleSheet(
        "QLabel { "
        "font-size: 24px; "
        "font-weight: bold; "
        "color: #2c3e50; "
        "padding: 15px; "
        "background-color: rgba(52, 152, 219, 0.1); "
        "border-radius: 8px; "
        "border-left: 5px solid #3498db; "
        "}"
    );
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mainLayout->addWidget(label);

    // Control panel layout
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(10);

    // Fetch button with fancy styling
    QPushButton *button = new QPushButton("⟳ Refresh Arrivals", this);
    button->setStyleSheet(
        "QPushButton { "
        "font-size: 14px; "
        "font-weight: bold; "
        "color: white; "
        "background-color: #3498db; "
        "border: none; "
        "border-radius: 6px; "
        "padding: 12px 25px; "
        "min-width: 150px; "
        "} "
        "QPushButton:hover { "
        "background-color: #2980b9; "
        "} "
        "QPushButton:pressed { "
        "background-color: #1f618d; "
        "}"
    );
    controlLayout->addStretch();
    controlLayout->addWidget(button);
    controlLayout->addStretch();
    mainLayout->addLayout(controlLayout);

    // Create tab widget
    tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet(
        "QTabWidget::pane { "
        "border: 1px solid #bdc3c7; "
        "} "
        "QTabBar::tab { "
        "background-color: #ecf0f1; "
        "color: #2c3e50; "
        "padding: 8px 20px; "
        "margin-right: 2px; "
        "border: 1px solid #bdc3c7; "
        "border-bottom: none; "
        "} "
        "QTabBar::tab:selected { "
        "background-color: white; "
        "color: #3498db; "
        "font-weight: bold; "
        "} "
        "QTabBar::tab:hover { "
        "background-color: #d5dbdb; "
        "}"
    );

    // Station A Tab (two directions)
    stationA.station_ids = {"F02261", "F02262"};
    stationA.station_name = "Hollókő utca";
    stationA.tableWidget = new QTableWidget(this);
    stationA.tableWidget->setColumnCount(4);
    stationA.tableWidget->setHorizontalHeaderLabels({"Line", "Destination", "Departs In", "Departure Time"});
    setupTableWidget(stationA.tableWidget);
    tabWidget->addTab(stationA.tableWidget, "Hollókő utca");

    // Station B Tab (two directions)
    stationB.station_ids = {"F02121", "F02122"};
    stationB.station_name = "Rókaszalon";
    stationB.tableWidget = new QTableWidget(this);
    stationB.tableWidget->setColumnCount(4);
    stationB.tableWidget->setHorizontalHeaderLabels({"Line", "Destination", "Departs In", "Departure Time"});
    setupTableWidget(stationB.tableWidget);
    tabWidget->addTab(stationB.tableWidget, "Rókaszalon");

    mainLayout->addWidget(tabWidget);

    // Connect button clicked signal to slot
    connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

    setCentralWidget(centralWidget);

    // Add status bar
    statusBar()->setStyleSheet(
        "QStatusBar { "
        "background-color: #ecf0f1; "
        "border-top: 1px solid #bdc3c7; "
        "color: #2c3e50; "
        "padding: 5px; "
        "font-size: 11px; "
        "}"
    );
    statusBar()->showMessage("Ready to fetch arrivals");

    // create an instance of the BKK API
    api = std::make_unique<BkkApi>();
}

void MainWindow::onButtonClicked()
{
    clickCount++;
    label->setText(QString("🚌 Bangkok Transport Arrivals - Fetching (attempt %1)...").arg(clickCount));
    statusBar()->showMessage("Loading arrivals from both stations...");
    
    // Clear both table widgets
    stationA.tableWidget->setRowCount(0);
    stationB.tableWidget->setRowCount(0);
    stationA.arrivals.clear();
    stationB.arrivals.clear();

    // measure the time taken to fetch and display arrivals
    auto start = std::chrono::high_resolution_clock::now();
    
    try {
        // Fetch arrivals for Station A (both directions)
        for (const auto& station_id : stationA.station_ids) {
            std::vector<Arrival> arrivals = api->get_arrivals_for_station(station_id);
            stationA.arrivals.insert(stationA.arrivals.end(), arrivals.begin(), arrivals.end());
        }
        populateTable(stationA);
        
        // Fetch arrivals for Station B (both directions)
        for (const auto& station_id : stationB.station_ids) {
            std::vector<Arrival> arrivals = api->get_arrivals_for_station(station_id);
            stationB.arrivals.insert(stationB.arrivals.end(), arrivals.begin(), arrivals.end());
        }
        populateTable(stationB);
        
        int totalArrivals = stationA.arrivals.size() + stationB.arrivals.size();
        label->setText(QString("🚌 Bangkok Transport Arrivals - Loaded %1 arrivals (A: %2, B: %3)")
            .arg(totalArrivals).arg(stationA.arrivals.size()).arg(stationB.arrivals.size()));
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        statusBar()->showMessage(QString("✓ Loaded %1 arrivals in %2 ms")
            .arg(totalArrivals).arg(duration));
        
    } catch (const std::exception& e) {
        printf("Error fetching arrivals: %s\n", e.what());
        
        stationA.tableWidget->setRowCount(1);
        QTableWidgetItem *errorItem = new QTableWidgetItem(QString("❌ Error: %1").arg(QString::fromStdString(std::string(e.what()))));
        errorItem->setForeground(Qt::red);
        stationA.tableWidget->setItem(0, 0, errorItem);
        
        label->setText("❌ Error fetching arrivals");
        statusBar()->showMessage("Error fetching data. Check console for details.");
    }
}

void MainWindow::populateTable(StationData &station)
{
    QTableWidget *table = station.tableWidget;
    
    // Sort arrivals by departure time (departs_in_min) in ascending order
    std::sort(station.arrivals.begin(), station.arrivals.end(),
        [](const Arrival& a, const Arrival& b) {
            return a.departs_in_min < b.departs_in_min;
        });
    
    table->setRowCount(station.arrivals.size());
    
    for (size_t i = 0; i < station.arrivals.size(); ++i) {
        const auto& arrival = station.arrivals[i];
        
        // Line number
        QTableWidgetItem *lineItem = new QTableWidgetItem(QString::fromStdString(arrival.line));
        lineItem->setFont(QFont("Segoe UI", 12, QFont::Bold));
        lineItem->setForeground(Qt::white);
        QColor bgColor = getColorForMinutes(arrival.departs_in_min);
        lineItem->setBackground(bgColor);
        lineItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 0, lineItem);
        
        // Destination
        QTableWidgetItem *destItem = new QTableWidgetItem(QString::fromStdString(arrival.destination));
        destItem->setFont(QFont("Segoe UI", 11));
        destItem->setBackground(getColorForMinutes(arrival.departs_in_min));
        destItem->setForeground(Qt::white);
        table->setItem(i, 1, destItem);
        
        // Departs in (with emoji indicator)
        QString minutesText = QString::number(arrival.departs_in_min);
        if (arrival.departs_in_min <= 3) {
            minutesText = "🔴 " + minutesText + " min";
        } else if (arrival.departs_in_min <= 10) {
            minutesText = "🟡 " + minutesText + " min";
        } else {
            minutesText = "🟢 " + minutesText + " min";
        }
        
        QTableWidgetItem *timeItem = new QTableWidgetItem(minutesText);
        timeItem->setFont(QFont("Segoe UI", 11, QFont::Bold));
        timeItem->setBackground(getColorForMinutes(arrival.departs_in_min));
        timeItem->setForeground(Qt::white);
        timeItem->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 2, timeItem);
        
        // Departure time
        QTableWidgetItem *depTimeItem = new QTableWidgetItem(QString::fromStdString(arrival.departure_time));
        depTimeItem->setFont(QFont("Segoe UI", 11));
        depTimeItem->setBackground(getColorForMinutes(arrival.departs_in_min));
        depTimeItem->setForeground(Qt::white);
        table->setItem(i, 3, depTimeItem);
    }
    
    table->resizeRowsToContents();
}

void MainWindow::setupTableWidget(QTableWidget *table)
{
    table->setStyleSheet(
        "QTableWidget { "
        "background-color: white; "
        "gridline-color: #ecf0f1; "
        "border: 1px solid #bdc3c7; "
        "border-radius: 6px; "
        "} "
        "QTableWidget::item { "
        "padding: 8px; "
        "} "
        "QHeaderView::section { "
        "background-color: #34495e; "
        "color: white; "
        "padding: 8px; "
        "border: none; "
        "font-weight: bold; "
        "font-size: 13px; "
        "}"
    );
    
    table->horizontalHeader()->setStretchLastSection(false);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setAlternatingRowColors(true);
    table->setFont(QFont("Segoe UI", 12));
}

QColor MainWindow::getColorForMinutes(int minutes)
{
    if (minutes <= 3) {
        return QColor("#e74c3c"); // Red - urgent
    } else if (minutes <= 10) {
        return QColor("#f39c12"); // Orange - medium
    } else {
        return QColor("#27ae60"); // Green - plenty of time
    }
}


