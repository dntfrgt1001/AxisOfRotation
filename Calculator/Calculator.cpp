//
//  Calculator.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/22.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "Calculator.hpp"

Calculator::Calculator(int number, std::string videoName,
                       cv::Size& frameSize, RemoveNoise& rn,
                       CalcDeviation& cd, OutputData& od,
                       cv::Mat& cameraMatrix, cv::Mat& distCoeffs):
number(number), videoName(videoName), frameSize(frameSize),
rn(rn), cd(cd), od(od),
cameraMatrix(cameraMatrix), distCoeffs(distCoeffs),
frameNumber(0), sumPixel(0), minCenter(0)
{
    video = cv::VideoCapture(videoName);
    if(!video.isOpened()){
        std::cout << "video " << number << " cannot be opened" << std::endl;
        exit(1);
    }
    
    cv::namedWindow("opticalflow" + std::to_string(number),
                    CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("undistort" + std::to_string(number),
                    CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    initialize();
}

Calculator::~Calculator()
{
    std::string windowName = "opticalflow" + std::to_string(number);
    cv::destroyWindow(windowName);
}

Calculator::Calculator(const Calculator& calc):
number(calc.number), videoName(calc.videoName), frameSize(calc.frameSize),
rn(calc.rn), cd(calc.cd), od(calc.od),
cameraMatrix(calc.cameraMatrix), distCoeffs(calc.distCoeffs),
frameNumber(0), sumPixel(0), minCenter(0)
{
    video = cv::VideoCapture(videoName);
    if(!video.isOpened()){
        std::cout << "video " << number << " cannot be opened" << std::endl;
        exit(1);
    }
    
    cv::namedWindow("opticalflow" + std::to_string(number),
                    CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::namedWindow("undistort" + std::to_string(number),
                    CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    
    initialize();
}

void Calculator::initialize()
{
    flowImg = cv::Mat::zeros(frameSize.height, frameSize.width, CV_8UC1);
    prevImg = nextImg.clone();
    
    video >> inputImg;
    if(inputImg.empty()) throw 0;
    
    cv::resize(inputImg, inputImg, frameSize);
    cv::undistort(inputImg, undistortImg, cameraMatrix, distCoeffs);
    cv::cvtColor(undistortImg, nextImg, CV_BGR2GRAY);
}

void Calculator::calcFeature()
{
    int blankWidth = frameSize.width / 2;

    cv::Rect roi(cv::Point2f(blankWidth, 0),
                 cv::Point2f(frameSize.width, frameSize.height));

    cv::Mat prevImgRoi = prevImg(roi);
    cv::Mat nextImgRoi = nextImg(roi);
    
    cv::goodFeaturesToTrack(prevImgRoi, prevPoints, 1000, 0.1, 3);
    if(prevPoints.empty()) throw 1;
    
    cv::Mat status, error;
    cv::calcOpticalFlowPyrLK(prevImgRoi, nextImgRoi,
                             prevPoints, nextPoints, status, error);
    
    rn.removeOutlier(prevPoints, nextPoints);
   
    for(auto p=prevPoints.begin(),n=nextPoints.begin();
        p!=prevPoints.end();p++,n++)
    {
        *p = *p + cv::Point2f(blankWidth, 0);
        *n = *n + cv::Point2f(blankWidth, 0);
    }

    
    if(rn.isEnoughHalfVector(prevPoints)){
        int tmpCenter  = cd.calcPixel(prevPoints, nextPoints);
        
        if(0 <= tmpCenter && tmpCenter <= frameSize.width){
            minCenter = tmpCenter;
        
            od.outputTwin(frameNumber, minCenter-frameSize.width/2);
            frameNumber++;
        }
        
        std::cout << "minCenter = " << minCenter << std::endl;
        
        
        

    }
}

void Calculator::drawOpticalflow()
{
    cv::Scalar color(255);
    
    for(auto startIter=prevPoints.begin(), endIter=nextPoints.begin();
        startIter!=prevPoints.end(); startIter++,endIter++)
    {
        //draw Opticalflow on the gray flowImg
        cv::line(flowImg, *startIter, *endIter, color, 2, CV_AA);
        cv::circle(flowImg, *endIter, 3, color, -1, CV_AA);
        
        //draw Opticalflow on the color undistortImg
        cv::line(undistortImg, *startIter, *endIter,
                 cv::Scalar(255, 0, 0), 2, CV_AA);
        cv::circle(undistortImg, *endIter, 3, cv::Scalar(0, 0, 255), -1, CV_AA);
    }
    
    cd.drawRange(flowImg);
    
    cv::imshow("opticalflow"+std::to_string(number), flowImg);
    cv::imshow("undistort"+std::to_string(number), undistortImg);
    
    if(cv::waitKey(10) > 0) exit(0);
}
