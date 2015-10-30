//
//  main.cpp
//  Test
//
//  Created by masakazu nakazawa on 2015/10/27.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, const char * argv[])
{
    cv::Mat input1 = cv::imread("/Users/masakazu/Desktop/Opt/blocks1.png");
    if (input1.empty()) return -1;
    cv::Mat input2 = cv::imread("/Users/masakazu/Desktop/Opt/blocks2.png");
    if (input2.empty()) return -1;
    
    cv::Mat flowImg = input1.clone();
    
    cv::Mat gray1, gray2;
    cv::cvtColor(input1, gray1, CV_BGR2GRAY);
    cv::cvtColor(input2, gray2, CV_BGR2GRAY);
    
    cv::vector<cv::Point2f> prevPoints;
    cv::vector<cv::Point2f> nextPoints;
    
    cv::Mat status, error;
    
    cv::goodFeaturesToTrack(gray1, prevPoints, 100, 0.1, 3);
    cv::calcOpticalFlowPyrLK(gray1, gray2, prevPoints, nextPoints,
                             status, error);
    
    for(auto p=prevPoints.begin(), n=nextPoints.begin();
        p!=prevPoints.end(); p++, n++)
    {
        cv::line(flowImg, *p, *n, cv::Scalar(255,0,0), 2);
        cv::circle(flowImg, *n, 3, cv::Scalar(0,0,255), -1, 8);
    }
    
    cv::namedWindow("opticalflow", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    cv::imshow("opticalflow", flowImg);
    
    cv::imwrite("/Users/masakazu/Desktop/Opt/blocksFlow.png", flowImg);
    
    cv::waitKey(0);
    
    return 0;
}
