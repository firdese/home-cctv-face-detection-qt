#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QListWidget;
class QPushButton;
class QString;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void buildDashboard();
    QLabel *createStatusPill(const QString &label, const QString &value);
    void setMonitoringActive(bool active);

    Ui::MainWindow *ui;
    QLabel *liveViewLabel = nullptr;
    QLabel *captureStatusLabel = nullptr;
    QLabel *motionStatusLabel = nullptr;
    QLabel *faceStatusLabel = nullptr;
    QLabel *matchStatusLabel = nullptr;
    QLabel *storageStatusLabel = nullptr;
    QListWidget *eventList = nullptr;
    QPushButton *startButton = nullptr;
    QPushButton *stopButton = nullptr;
};
#endif // MAINWINDOW_H
