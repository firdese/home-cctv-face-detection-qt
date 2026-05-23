#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buildDashboard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildDashboard()
{
    setWindowTitle("Home CCTV Face Detection");
    resize(1100, 700);

    auto *centralWidget = new QWidget(this);
    auto *rootLayout = new QHBoxLayout(centralWidget);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(16);

    auto *leftColumn = new QVBoxLayout();
    leftColumn->setSpacing(12);

    liveViewLabel = new QLabel("Camera feed will appear here", centralWidget);
    liveViewLabel->setAlignment(Qt::AlignCenter);
    liveViewLabel->setMinimumSize(640, 420);
    liveViewLabel->setStyleSheet(
        "QLabel {"
        "background-color: #111827;"
        "color: #d1d5db;"
        "border: 1px solid #374151;"
        "font-size: 18px;"
        "}"
    );
    leftColumn->addWidget(liveViewLabel, 1);

    auto *controls = new QHBoxLayout();
    startButton = new QPushButton("Start Monitoring", centralWidget);
    stopButton = new QPushButton("Stop", centralWidget);
    stopButton->setEnabled(false);
    controls->addWidget(startButton);
    controls->addWidget(stopButton);
    controls->addStretch();
    leftColumn->addLayout(controls);

    auto *rightColumn = new QVBoxLayout();
    rightColumn->setSpacing(12);

    auto *pipelineGroup = new QGroupBox("Frame Processing Pipeline", centralWidget);
    auto *pipelineLayout = new QGridLayout(pipelineGroup);
    captureStatusLabel = createStatusPill("Capture", "Idle");
    motionStatusLabel = createStatusPill("Motion", "Waiting");
    faceStatusLabel = createStatusPill("Face detection", "Waiting");
    matchStatusLabel = createStatusPill("Face matching", "Waiting");
    storageStatusLabel = createStatusPill("Storage", "Ready");
    pipelineLayout->addWidget(captureStatusLabel, 0, 0);
    pipelineLayout->addWidget(motionStatusLabel, 1, 0);
    pipelineLayout->addWidget(faceStatusLabel, 2, 0);
    pipelineLayout->addWidget(matchStatusLabel, 3, 0);
    pipelineLayout->addWidget(storageStatusLabel, 4, 0);
    rightColumn->addWidget(pipelineGroup);

    auto *knownFacesGroup = new QGroupBox("Known Faces Database", centralWidget);
    auto *knownFacesLayout = new QVBoxLayout(knownFacesGroup);
    auto *knownFacesLabel = new QLabel("No enrolled faces yet", knownFacesGroup);
    knownFacesLabel->setAlignment(Qt::AlignCenter);
    knownFacesLabel->setMinimumHeight(80);
    knownFacesLayout->addWidget(knownFacesLabel);
    rightColumn->addWidget(knownFacesGroup);

    auto *eventsGroup = new QGroupBox("Event History", centralWidget);
    auto *eventsLayout = new QVBoxLayout(eventsGroup);
    eventList = new QListWidget(eventsGroup);
    eventList->addItem("System ready");
    eventsLayout->addWidget(eventList);
    rightColumn->addWidget(eventsGroup, 1);

    rootLayout->addLayout(leftColumn, 3);
    rootLayout->addLayout(rightColumn, 2);
    setCentralWidget(centralWidget);

    connect(startButton, &QPushButton::clicked, this, [this]() {
        setMonitoringActive(true);
    });
    connect(stopButton, &QPushButton::clicked, this, [this]() {
        setMonitoringActive(false);
    });

    statusBar()->showMessage("Ready");
}

QLabel *MainWindow::createStatusPill(const QString &label, const QString &value)
{
    auto *statusLabel = new QLabel(label + ": " + value, this);
    statusLabel->setMinimumHeight(34);
    statusLabel->setStyleSheet(
        "QLabel {"
        "background-color: #f3f4f6;"
        "border: 1px solid #d1d5db;"
        "padding: 6px 10px;"
        "}"
    );
    return statusLabel;
}

void MainWindow::setMonitoringActive(bool active)
{
    startButton->setEnabled(!active);
    stopButton->setEnabled(active);

    captureStatusLabel->setText(active ? "Capture: Running" : "Capture: Idle");
    motionStatusLabel->setText(active ? "Motion: Watching" : "Motion: Waiting");
    faceStatusLabel->setText(active ? "Face detection: Queued" : "Face detection: Waiting");
    matchStatusLabel->setText(active ? "Face matching: Standby" : "Face matching: Waiting");
    liveViewLabel->setText(active ? "Monitoring active - camera module not connected yet" : "Camera feed will appear here");

    eventList->insertItem(0, active ? "Monitoring started" : "Monitoring stopped");
    statusBar()->showMessage(active ? "Monitoring active" : "Ready");
}

