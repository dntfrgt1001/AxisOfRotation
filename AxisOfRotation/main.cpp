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
    cv::Mat img1 = cv::imread("/Users/masakazu/Desktop/Opt/blocks1.png");
    if(img1.empty()) return -1;
    cv::Mat img2 = cv::imread("/Users/masakazu/Desktop/Opt/blocks2.png");
    if(img2.empty()) return -1;
    
    cv::Mat flowImg = img1.clone();
    
    cv::Mat prev, next;
    cv::cvtColor(img1, prev, CV_BGR2GRAY);
    cv::cvtColor(img2, next, CV_BGR2GRAY);
    
    std::vector<cv::Point2f> prev_pts;
    std::vector<cv::Point2f> next_pts;
    
    cv::Size flowSize(10, 10);
    cv::Point2f center = cv::Point(prev.cols/2, prev.rows/2);
    
    for(int i=0; i<flowSize.width; i++){
        for(int j=0; j<flowSize.height; j++){
            cv::Point2f p(i*float(prev.cols)/(flowSize.width-1),
                          j*float(prev.rows)/(flowSize.height-1));
            prev_pts.push_back((p-center)*0.8f+center);
        }
    }
    
    cv::Mat status, error;
    
    cv::calcOpticalFlowPyrLK(prev, next, prev_pts, next_pts, status, error);
    
    std::vector<cv::Point2f>::const_iterator p = prev_pts.begin();
    std::vector<cv::Point2f>::const_iterator n = next_pts.begin();
    

    for(; n!=next_pts.end(); p++, n++){
        cv::line(flowImg, *p, *n, cv::Scalar(255, 0, 0), 2);
        cv::circle(flowImg, *n, 3, cv::Scalar(0, 0, 255), -1, 8);
    }
    
    cv::namedWindow("Previous", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("Next", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("OpticalFlow", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    cv::imshow("Previous", img1);
    cv::imshow("Next", img2);
    cv::imshow("OpticalFlow", flowImg);
    
    cv::waitKey(0);
}