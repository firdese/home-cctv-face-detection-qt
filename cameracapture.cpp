#include "cameracapture.h"

#include <algorithm>

#include <opencv2/imgproc.hpp>

CameraCapture::CameraCapture(QObject *parent)
    : QObject(parent)
{
    frameTimer.setInterval(33);
    connect(&frameTimer, &QTimer::timeout, this, &CameraCapture::grabFrame);
}

CameraCapture::~CameraCapture()
{
    stop();
}

bool CameraCapture::start(int cameraIndex)
{
    if (isRunning()) {
        return true;
    }

    loopVideo = false;

    if (!capture.open(cameraIndex, cv::CAP_ANY)) {
        emit errorOccurred(QStringLiteral("Could not open camera %1").arg(cameraIndex));
        return false;
    }

    updateFrameInterval();
    frameTimer.start();
    return true;
}

bool CameraCapture::startVideoFile(const QString &filePath, bool loop)
{
    if (isRunning()) {
        return true;
    }

    loopVideo = loop;

    if (filePath.trimmed().isEmpty()) {
        emit errorOccurred(QStringLiteral("Choose a video file before starting"));
        return false;
    }

    if (!capture.open(filePath.toStdString(), cv::CAP_ANY)) {
        emit errorOccurred(QStringLiteral("Could not open video file: %1").arg(filePath));
        return false;
    }

    updateFrameInterval();
    frameTimer.start();
    return true;
}

void CameraCapture::stop()
{
    frameTimer.stop();

    if (capture.isOpened()) {
        capture.release();
    }
}

bool CameraCapture::isRunning() const
{
    return capture.isOpened() && frameTimer.isActive();
}

void CameraCapture::grabFrame()
{
    cv::Mat frame;
    if (!capture.read(frame) || frame.empty()) {
        if (loopVideo) {
            capture.set(cv::CAP_PROP_POS_FRAMES, 0);
            if (capture.read(frame) && !frame.empty()) {
                // Continue below with the first frame after looping.
            } else {
                emit errorOccurred(QStringLiteral("Video file could not be looped"));
                stop();
                return;
            }
        } else {
            emit errorOccurred(QStringLiteral("Camera frame could not be read"));
            stop();
            return;
        }
    }

    cv::Mat rgbFrame;
    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);

    QImage image(
        rgbFrame.data,
        rgbFrame.cols,
        rgbFrame.rows,
        static_cast<int>(rgbFrame.step),
        QImage::Format_RGB888
    );

    emit frameReady(image.copy());
}

void CameraCapture::updateFrameInterval()
{
    const double fps = capture.get(cv::CAP_PROP_FPS);
    if (fps > 0.0) {
        frameTimer.setInterval(std::max(1, static_cast<int>(1000.0 / fps)));
        return;
    }

    frameTimer.setInterval(33);
}
