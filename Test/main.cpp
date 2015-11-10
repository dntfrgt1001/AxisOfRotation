//
//  main.cpp
//  Test
//
//  Created by masakazu nakazawa on 2015/10/27.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "RemoveNoise.hpp"

int main(int argc, const char * argv[])
{
    /*
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
    */
    
    cv::Size frameSize(480, 640);
    const std::string dataPath = "/Users/masakazu/Desktop/OptTest/";
    const std::string checkerPrefix = "AZ1-";
    const std::string checkerSuffix = ".jpg";
    const int checkerNum = 5;
    const cv::Size checkerSize(10, 7);
    cv::vector<cv::Mat> checkerImgs;
    
    for (int i=0; i<checkerNum; i++) {
        std::string fileName = checkerPrefix + std::to_string(i+1) +
                               checkerSuffix;
        
        cv::Mat tmp = cv::imread(dataPath +fileName, 0);
        cv::resize(tmp, tmp, frameSize);
        checkerImgs.push_back(tmp);
    }
    
    cv::vector<cv::vector<cv::Point2f>> imagePoints(checkerNum);
    cv::vector<cv::vector<cv::Point3f>> worldPoints(checkerNum);
    
    cv::TermCriteria criteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.001);
    
    cv::namedWindow("Source", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    for (int i=0; i<checkerNum; i++) {
        if (cv::findChessboardCorners(checkerImgs[i], checkerSize,
                                      imagePoints[i]))
        {
            cv::cornerSubPix(checkerImgs[i], imagePoints[i], cv::Size(5, 5),
                             cv::Size(-1, -1), criteria);
            cv::drawChessboardCorners(checkerImgs[i], checkerSize,
                                      (cv::Mat)(imagePoints[i]), true);
            cv::imshow("Source", checkerImgs[i]);
            
            std::cout << "corner found" << std::endl;
            
            cv::waitKey(500);
        } else {
            std::cout << "corner not found in image" << i+1 << std::endl;
            exit(1);
        }
    }
    
    cv::destroyWindow("Source");
    
    for (int i=0; i<checkerNum; i++) {
        for (int j=0; j<checkerSize.height; j++) {
            for (int k=0; k<checkerSize.width; k++) {
                worldPoints[i].push_back(cv::Point3f(j, k, 0.0));
            }
        }
    }
    
    cv::Mat cameraMatrix, distCoeffs;
    cv::vector<cv::Mat> rotationVectors, translationVectors;
    
    cv::calibrateCamera(worldPoints, imagePoints, frameSize,
                        cameraMatrix, distCoeffs,
                        rotationVectors, translationVectors);
    
    /******************************************************************/
    
    
    cv::VideoCapture cap(dataPath + "MAF000852.mp4");

    if (!cap.isOpened()) {
        std::cout << "video not found." << std::endl;
        exit(1);
    }
    
    RemoveNoise rn(frameSize, false, true, 5, 100);
    
    //cv::namedWindow("input", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    //cv::namedWindow("undistort", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("opticalflow", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    cv::Mat prevImg, nextImg;
    while (true) {
        cv::Mat flowImg = cv::Mat::zeros(frameSize.height, frameSize.width,
                                         CV_8UC1);
        prevImg = nextImg.clone();
        
        cv::Mat inputImg;
        
        cap >> inputImg;
        if (inputImg.empty()) break;
        
        cv::Mat undistortImg;
        
        resize(inputImg, inputImg, frameSize);
        cv::undistort(inputImg, undistortImg, cameraMatrix, distCoeffs);
        cv::cvtColor(undistortImg, nextImg, CV_BGR2GRAY);
        
        cv::vector<cv::Point2f> prevPoints;
        cv::vector<cv::Point2f> nextPoints;
        
        cv::goodFeaturesToTrack(prevImg, prevPoints, 500, 0.01, 3);
        if(prevPoints.empty()) continue;
        
        cv::vector<uchar> status;
        cv::vector<float> error;
        cv::calcOpticalFlowPyrLK(prevImg, nextImg, prevPoints, nextPoints,
                                 status, error);
        
        rn.removeOutlier(prevPoints, nextPoints);
        
        cv::vector<cv::Point2f>::iterator p,n;
        cv::vector<uchar>::iterator s;
        for (p=prevPoints.begin(),n=nextPoints.begin(), s=status.begin();
             p!=prevPoints.end(); p++,n++,s++)
        {
            cv::Scalar color(255);
            
            if (*s==1) {
                cv::line(flowImg, *p, *n, color, 2, CV_AA);
                cv::circle(flowImg, *n, 3, color, -1, CV_AA);
            }
        }
        
        //cv::imshow("input", inputImg);
        //cv::imshow("undistort", undistortImg);
        cv::imshow("opticalflow", flowImg);
        
        if (cv::waitKey(10) > 0) {
            break;
        }
        
    }
    
    return 0;
}
