//
//  main.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/08/18.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#include <iostream>

#include <opencv2/opencv.hpp> 

#include "CalcDeviationHalf.hpp"
#include "CalcDeviationQuarter.hpp"
#include "RemoveNoise.h"
#include "OutputData.hpp"

int main(int argc, const char * argv[])
{
    const std::string dataPath = "/Users/masakazu/Desktop/Opt/";
    const std::string checkerPrefix = "calib";
    const std::string checkerSuffix = ".jpg";
    const int checkerNum = 4;
    const cv::Size frameSize(480, 640);
    const cv::Size checkerSize(10, 7);
    
    cv::vector<cv::Mat> checkerImgs;
    cv::vector<cv::vector<cv::Point3f>> worldPoints(checkerNum);
    cv::vector<cv::vector<cv::Point2f>> imagePoints(checkerNum);
    
    cv::TermCriteria criteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.001);
    
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;
    cv::vector<cv::Mat> rotationVectors;
    cv::vector<cv::Mat> translationVectors;
    
    for(int i=0; i<checkerNum; i++){
        std::stringstream stream;
        stream << checkerPrefix << i+1 << checkerSuffix;
        std::string fileName = stream.str();
        cv::Mat tmp = cv::imread(dataPath + fileName, 0);
        cv::resize(tmp, tmp, frameSize);
        checkerImgs.push_back(tmp);
        std::cout << "load checker image: " << fileName << std::endl;
    }
    
    cv::namedWindow("Source", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    for(int i=0; i<checkerNum; i++){
        std::cout << "find corners from image " << i+1;
        if(cv::findChessboardCorners(checkerImgs[i], checkerSize,
                                     imagePoints[i])){
            std::cout << " ... all corners found." << std::endl;
            cv::cornerSubPix(checkerImgs[i], imagePoints[i], cv::Size(5, 5),
                             cv::Size(-1, -1), criteria);
            cv::drawChessboardCorners(checkerImgs[i], checkerSize,
                                      (cv::Mat)(imagePoints[i]), true);
            cv::imshow("Source", checkerImgs[i]);
            cv::waitKey(200);
        }else{
            std::cout << " ... at least 1 corner not found." << std::endl;
            cv::waitKey(0);
            return -1;
        }
    }
    cv::destroyWindow("Source");
    
    for(int i=0; i<checkerNum; i++){
        for(int j=0; j<checkerSize.area(); j++){
            worldPoints[i].push_back(
                cv::Point3f(static_cast<float>(j % checkerSize.width * 10),
                            static_cast<float>(j / checkerSize.width * 10),
                            0.0));
        }
    }
    
    cv::calibrateCamera(worldPoints, imagePoints, frameSize, cameraMatrix,
                        distCoeffs, rotationVectors, translationVectors);
    
    std::cout << "camera matrix" << std::endl;
    std::cout << cameraMatrix << std::endl;
    std::cout << "dist coeffs" << std::endl;
    std::cout << distCoeffs << std::endl;
    
    const cv::string videoName = "GOPR00312.mp4";
    cv::VideoCapture video(dataPath + videoName);
    if(!video.isOpened()){
        std::cout << "video cannot be opened" << std::endl;
        return -1;
    }
    
    cv::Mat input;
    cv::Mat undistortImg;
    cv::Mat flowImg;
    cv::Mat prevImg, nextImg;
    cv::Mat status, error;
    cv::vector<cv::Point2f> prevPoints;
    cv::vector<cv::Point2f> nextPoints;
    
    cv::namedWindow("Input",
                    CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("Undistort",
                    CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("GrayOpticalflow",
                    CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    const bool left = false;
    const bool up = true;
    const int threshNorm = 1;
    const int threshNum = 100;
    const int dx = frameSize.width / 12;
    const int shift = frameSize.width / 20;
    
    RemoveNoise removeNoise(frameSize, &prevPoints, &nextPoints,left, up,
                            threshNorm, threshNum);
    
    CalcDeviationHalf calculator(frameSize, prevPoints, nextPoints,
                                 dx, shift, left);
    
    //CalcDeviationQuarter calculator(frameSize, prevPoints, nextPoints,
    //                                dx, shift, left, up);
    
    OutputData outputData("/Users/masakazu/Desktop/Opt/output.csv", ' ');
    
    
    cv::Rect roi(frameSize.width/2, 0, frameSize.width/2, frameSize.height);
    
    int frameNumber = 0;
    int averagePixel = 0;
    int minCenter= 0;
    video >> input;
    cv::resize(input, input, frameSize);
    cv::undistort(input, undistortImg, cameraMatrix, distCoeffs);
    cv::cvtColor(undistortImg, nextImg, CV_BGR2GRAY);
    while(true){
        flowImg = cv::Mat::zeros(frameSize.height, frameSize.width, CV_8UC1);
        
        prevImg = nextImg.clone();
        video >> input;
        if(input.empty()) break;
        cv::resize(input, input, frameSize);
        cv::undistort(input, undistortImg, cameraMatrix, distCoeffs);
        cv::cvtColor(undistortImg, nextImg, CV_BGR2GRAY);

        

        cv::goodFeaturesToTrack(prevImg(roi), prevPoints, 4000, 0.1, 3);
        if(prevPoints.empty()) continue;
        cv::calcOpticalFlowPyrLK(prevImg(roi), nextImg(roi), prevPoints,
                                 nextPoints, status, error);
        
        cv::vector<cv::Point2f>::iterator p = prevPoints.begin();
        cv::vector<cv::Point2f>::iterator n = nextPoints.begin();

        for (; p!=prevPoints.end(); p++, n++) {
            *p = *p + cv::Point2f(frameSize.width/2, 0);
            *n = *n + cv::Point2f(frameSize.width/2, 0);
        }
        removeNoise.removeOutlier();
        if(removeNoise.isEnoughQuarterVector()){
            int tmpCenter = calculator.calcPixel();
            if(0 <= tmpCenter && tmpCenter <= frameSize.width)
                minCenter = tmpCenter;
            
            frameNumber++;
            
            averagePixel += minCenter;
            
            std::cout << "minCenter = " << minCenter << std::endl;

            outputData.outputTwin(frameNumber, minCenter);
        }
        
        calculator.drawRange(flowImg);
        
//        cv::vector<cv::Point2f>::const_iterator p = prevPoints.begin();
//        cv::vector<cv::Point2f>::const_iterator n = nextPoints.begin();

        p = prevPoints.begin();
        n = nextPoints.begin();
        for(; n!=nextPoints.end(); p++, n++){
            cv::line(undistortImg, *p, *n, cv::Scalar(255, 0, 0), 2, CV_AA);
            cv::circle(undistortImg, *n, 3, cv::Scalar(0, 0, 255), -1, CV_AA);
            cv::line(flowImg, *p, *n, cv::Scalar(255), 2, CV_AA);
            cv::circle(flowImg, *n, 3, cv::Scalar(255), -1, CV_AA);
        }
        
        cv::imshow("Input", input);
        cv::imshow("Undistort", undistortImg);
        cv::imshow("GrayOpticalflow", flowImg);
        
        if(frameNumber > 100)
            break;
        
        if(cv::waitKey(10) > 0) break;
    }
    
    std::cout << "average = " << averagePixel / frameNumber << std::endl;
    
    return 0;
}
