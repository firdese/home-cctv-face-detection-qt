#ifndef MOTIONDETECTOR_H
#define MOTIONDETECTOR_H

#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/video/background_segm.hpp>

class MotionDetector
{
public:
    std::vector<cv::Rect> detect(const cv::Mat &frame);
    void reset();

private:
    cv::Ptr<cv::BackgroundSubtractor> backgroundSubtractor;
};

#endif // MOTIONDETECTOR_H
