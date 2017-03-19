#include <functional>
#include <deque>

#include <opencv2/opencv.hpp>

#include "video/camera.hpp"

#include <iostream>
#include <chrono>

bool iovirta_iot::video::Camera::capture_frame()
{
    return video_in.read(frame_);
}

bool iovirta_iot::video::Camera::check_motion()
{
    static bool first = true;
    cv::Mat smooth;
    //cv::blur(frame_, smooth, {5,5}, {-1,-1});
    //cv::GaussianBlur(frame_, smooth, cv::Size(5, 5), 0, 0);
    //cv::Mat gray(height_ / 2, width_, CV_8U);
    cv::Mat gray;
    cv::cvtColor(frame_, gray, CV_BGR2GRAY);
    if (first){
        prev_gray_ = gray; first = false; return false;}
    /* absdiff -> threshold -> findcontours */
    cv::Mat diff;
    cv::absdiff(prev_gray_, gray, diff);

    cv::Mat mask;
    cv::threshold(diff, mask, 30, 255, CV_THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    if (contours.size() > 0)
        frame_count_ = (frame_count_ == 25 ? 25 : frame_count_ + 1);
    else
        frame_count_ = (frame_count_ <= 10 ? 0 : frame_count_ - 1);

    prev_gray_ = gray.clone();

    return frame_count_ > 10;
}
