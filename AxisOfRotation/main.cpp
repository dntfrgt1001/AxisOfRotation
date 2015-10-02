//
//  main.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/08/18.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#include <iostream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

int main(int argc, char* argv[])
{
    int frameWidth = 640;
    int frameHeight = 480;
    
    cv::VideoCapture cap(0);
    if(!cap.isOpened()) return -1;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);
    
    cv::namedWindow("Previous", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("Next", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("OpticalFlow", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    cv::Mat input;
    cv::Mat flowImg;
    cv::Mat prev, next;
    cv::Mat status, error;
    
    std::vector<cv::Point2f> prev_pts;
    std::vector<cv::Point2f> next_pts;

    cap >> input;
    cv::cvtColor(input, next, CV_BGR2GRAY);
    while(true){
        prev = next.clone();
        cap >> input;
//      flowImg = input.clone();
        flowImg = cv::Mat::zeros(frameHeight, frameWidth, CV_8UC1);
        cv::cvtColor(input, next, CV_BGR2GRAY);
        
        cv::goodFeaturesToTrack(prev, prev_pts, 50, 0.1, 10, cv::Mat(), 3, true, 0.04);
        
        cv::calcOpticalFlowPyrLK(prev, next, prev_pts, next_pts, status, error);
    
        std::vector<cv::Point2f>::const_iterator p = prev_pts.begin();
        std::vector<cv::Point2f>::const_iterator n = next_pts.begin();
        for(; n!=next_pts.end(); p++, n++){
            cv::line(flowImg, *p, *n, cv::Scalar(255, 0, 0), 2);
            cv::circle(flowImg, *n, 5, cv::Scalar(0, 0, 255), -1, 8);
        }
    
        cv::imshow("Previous", prev);
        cv::imshow("Next", next);
        cv::imshow("OpticalFlow", flowImg);
        
        if(cv::waitKey(50) > 0) break;
    }
}