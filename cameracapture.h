#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QTimer>

#include <opencv2/videoio.hpp>

class CameraCapture : public QObject
{
    Q_OBJECT

public:
    explicit CameraCapture(QObject *parent = nullptr);
    ~CameraCapture();

    bool start(int cameraIndex = 0);
    bool startVideoFile(const QString &filePath, bool loop = true);
    void stop();
    bool isRunning() const;

signals:
    void frameReady(const QImage &frame);
    void errorOccurred(const QString &message);

private:
    void grabFrame();
    void updateFrameInterval();

    cv::VideoCapture capture;
    QTimer frameTimer;
    bool loopVideo = false;
};

#endif // CAMERACAPTURE_H
