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
    int frameWidth = 480;
    int frameHeight = 640;
    
    cv::VideoCapture cap(0);
    if(!cap.isOpened()) return -1;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);
    
    cv::namedWindow("OpticalFlow", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    cv::Size flowSize(10, 10);
    cv::Point2f center = cv::Point(frameWidth/2, frameHeight/2);
    
    cv::Mat input;
    cv::Mat flowImg;
    cv::Mat prev, next;
    cv::Mat status, error;
    
    std::vector<cv::Point2f> prev_pts;
    std::vector<cv::Point2f> next_pts;
    
    for(int i=0; i<flowSize.width; i++){
        for(int j=0; j<flowSize.height; j++){
            cv::Point2f p(i*float(frameWidth)/(flowSize.width-1),
                          j*float(frameHeight)/(flowSize.height-1));
            prev_pts.push_back((p-center)*0.8f+center);
        }
    }
    
    cv::namedWindow("Previous", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("Next", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    cap >> input;
    cv::cvtColor(input, next, CV_BGR2GRAY);
    while(true){
        prev = next.clone();
        cap >> input;
        flowImg = input.clone();
        cv::cvtColor(input, next, CV_BGR2GRAY);
        
        cv::calcOpticalFlowPyrLK(prev, next, prev_pts, next_pts, status, error);
        
        for(auto p=prev_pts.begin(),n=next_pts.begin();
            n!=next_pts.end(); p++, n++)
        {
            cv::line(flowImg, *p, *n, cv::Scalar(255, 0, 0), 2);
            cv::circle(flowImg, *n, 3, cv::Scalar(0, 0, 255), -1, 8);
        }
        
        cv::imshow("OpticalFlow", flowImg);
        cv::imshow("Previous", prev);
        cv::imshow("Next", next);
        
        if(cv::waitKey(10) > 0) break;
    }
}