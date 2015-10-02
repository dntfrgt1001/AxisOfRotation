//
//  main.cpp
//  CameraCalibration
//
//  Created by masakazu nakazawa on 2015/08/28.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "CalcAxis.h"
#include "RemoveNoise.h"
#include "CalcAxis.h"

int main(int argc, const char * argv[])
{
    const std::string imgPath = "/Users/masakazu/Desktop/Opt/";
    const std::string windowNameSrc = "Source";
    const std::string windowNameUnd = "Undistorted Image";
    const int numberOfCheckerPatterns = 4;
    
    cv::Size frameSize(480, 640);
    
    cv::vector<cv::Mat> checkerImgs;
    const cv::Size  patternSize(10, 7);
    cv::vector<cv::vector<cv::Point3f>> worldPoints(numberOfCheckerPatterns);
    cv::vector<cv::vector<cv::Point2f>> imagePoints(numberOfCheckerPatterns);
    
    cv::TermCriteria criteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.001);
    
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;
    cv::vector<cv::Mat> rotationVectors;
    cv::vector<cv::Mat> translationVectors;
    
    for(int i=0; i<numberOfCheckerPatterns; i++){
        std::stringstream stream;
        stream << "calib" << i+1 << ".jpg";
        std::string fileName = stream.str();
        cv::Mat tmp = cv::imread(imgPath+fileName, 0);
        cv::resize(tmp, tmp, frameSize);
        checkerImgs.push_back(tmp);
        std::cout << "load checker image: " << fileName << std::endl;
    }
    
    cv::namedWindow(windowNameSrc, CV_WINDOW_AUTOSIZE);
    for(int i=0; i<numberOfCheckerPatterns; i++){
        std::cout << "find corners from image " << i+1;
        if(cv::findChessboardCorners(checkerImgs[i],
                                     patternSize, imagePoints[i])){
            std::cout << " ... all corners found." << std::endl;
            cv::cornerSubPix(checkerImgs[i], imagePoints[i], cv::Size(5, 5),
                             cv::Size(-1, -1), criteria);
            cv::drawChessboardCorners(checkerImgs[i], patternSize,
                                      (cv::Mat)(imagePoints[i]), true);
            cv::imshow(windowNameSrc, checkerImgs[i]);
            cv::waitKey(100);
        }else{
            std::cout << " ... at least 1 corner not found." << std::endl;
            cv::waitKey(0);
            return -1;
        }
    }
    cv::destroyWindow(windowNameSrc);
    
    for(int i=0; i<numberOfCheckerPatterns; i++){
        for(int j=0; j<patternSize.area(); j++){
            worldPoints[i].push_back(
                    cv::Point3f(static_cast<float>(j%patternSize.width*10),
                                static_cast<float>(j/patternSize.width*10),
                                0.0));
        }
    }
    
    cv::calibrateCamera(worldPoints, imagePoints, checkerImgs[0].size(),
                        cameraMatrix, distCoeffs, rotationVectors,
                        translationVectors);
    
    std::cout << "camera matrix" << std::endl;
    std::cout << cameraMatrix << std::endl;
    std::cout << "dist coeffs" << std::endl;
    std::cout << distCoeffs << std::endl;
    
    cv::VideoCapture video("/Users/masakazu/Desktop/Opt/GOPR00192.mp4");
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

    
    bool left = false;
    bool up = true;
    int threshNorm = 1;
    int threshNum = 100;
    int dx = frameSize.width / 12;
    int shift = frameSize.width / 20;
    
    
    RemoveNoise removeNoise(frameSize, left, up,
                            threshNorm, threshNum);
    
    
    CalcAxis calcAxis(frameSize, left, up, dx, shift);
    
/*
    int rest = 50;
    for(int i=0, widthNum=10; i<widthNum; i++){
        for(int j=0, heightNum=8; j<heightNum; j++){
            prevPoints.push_back(
                cv::Point2f(rest+i*(frameSize.width-2*rest)/(widthNum-1),
                            rest+j*(frameSize.height-2*rest)/(heightNum-1)));
        }
    }
*/
    
    int minLeft = 0;
    video >> input;
    cv::resize(input, input, frameSize);
    cv::undistort(input, undistortImg, cameraMatrix, distCoeffs);
    cv::cvtColor(undistortImg, nextImg, CV_BGR2GRAY);
    cv::Size undistFrameSize = nextImg.size();
    while(true){
        prevImg = nextImg.clone();
        video >> input;
        if(input.empty()) break;
        cv::resize(input, input, frameSize);
        cv::undistort(input, undistortImg, cameraMatrix, distCoeffs);
        
        cv::cvtColor(undistortImg, nextImg, CV_BGR2GRAY);
   
        flowImg = cv::Mat::zeros(undistFrameSize.height,
                                 undistFrameSize.width, CV_8UC1);
        
        cv::goodFeaturesToTrack(prevImg, prevPoints, 150, 0.1, 3);
        
        if(prevPoints.empty())
            continue;
            
        cv::calcOpticalFlowPyrLK(prevImg, nextImg, prevPoints, nextPoints,
                                 status, error);
        
        removeNoise.setOpticalflow(&prevPoints, &nextPoints);
        removeNoise.removeOutlier();
        if(removeNoise.isEnoughQuarterVector()){
            calcAxis.setOpticalflow(&prevPoints, &nextPoints);
            int tmpLeft = calcAxis.calcAxis();
            if(0 <= tmpLeft && tmpLeft <= frameSize.width)
                minLeft = tmpLeft;
            std::cout << "minLeft = " << minLeft << std::endl;
        }
        
        calcAxis.drawRange(flowImg);
        
        cv::vector<cv::Point2f>::const_iterator p = prevPoints.begin();
        cv::vector<cv::Point2f>::const_iterator n = nextPoints.begin();
        for(; n!=nextPoints.end(); p++, n++){
            cv::line(undistortImg, *p, *n, cv::Scalar(255, 0, 0), 2);
            cv::circle(undistortImg, *n, 2, cv::Scalar(0, 0, 255), -1, 8);
            cv::line(flowImg, *p, *n, cv::Scalar(255), 2);
            cv::circle(flowImg, *n, 2, cv::Scalar(255), -1, 8);
        }

        cv::imshow("Input", input);
        cv::imshow("Undistort", undistortImg);
        cv::imshow("GrayOpticalflow", flowImg);
        
        if(cv::waitKey(10) > 0) break;
    }
    
    return 0;
}
