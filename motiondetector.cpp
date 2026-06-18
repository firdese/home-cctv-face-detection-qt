#include "motiondetector.h"

#include <opencv2/imgproc.hpp>

std::vector<cv::Rect> MotionDetector::detect(const cv::Mat &frame)
{
    if (frame.empty()) {
        return {};
    }

    if (!backgroundSubtractor) {
        backgroundSubtractor = cv::createBackgroundSubtractorMOG2(500, 25.0, true);
    }

    cv::Mat foregroundMask;
    backgroundSubtractor->apply(frame, foregroundMask);

    cv::threshold(foregroundMask, foregroundMask, 200, 255, cv::THRESH_BINARY);
    cv::erode(foregroundMask, foregroundMask, cv::Mat(), cv::Point(-1, -1), 1);
    cv::dilate(foregroundMask, foregroundMask, cv::Mat(), cv::Point(-1, -1), 2);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(foregroundMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Rect> motionBoxes;
    for (const auto &contour : contours) {
        const double area = cv::contourArea(contour);
        if (area < 900.0) {
            continue;
        }

        motionBoxes.push_back(cv::boundingRect(contour));
    }

    return motionBoxes;
}

void MotionDetector::reset()
{
    backgroundSubtractor.release();
}
