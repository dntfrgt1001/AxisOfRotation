//
//  Calculator.hpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/22.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#ifndef Calculator_hpp
#define Calculator_hpp

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "CalcDeviationHalf.hpp"
#include "RemoveNoise.hpp"
#include "OutputData.hpp"

class Calculator{
public:
    Calculator(int number, std::string videoName,
               cv::Size& frameSize, RemoveNoise& rn,
               CalcDeviation& cd, OutputData& od,
               cv::Mat& cameraMatrix, cv::Mat& distCoeffs);
    ~Calculator();
    Calculator(const Calculator& calc);
    
    int calcOneFrame();
    void initialize();
    void calcFeature();
    void drawOpticalflow();
    
private:
    Calculator& operator=(const Calculator& calc);
    
    int number;
    std::string videoName;
    cv::Size& frameSize;
    RemoveNoise& rn;
    CalcDeviation cd;
    OutputData od;
    
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;

    cv::VideoCapture video;
    cv::Mat inputImg;
    cv::Mat undistortImg;
    cv::Mat prevImg, nextImg;
    cv::Mat flowImg;
    
    cv::vector<cv::Point2f> prevPoints;
    cv::vector<cv::Point2f> nextPoints;

    int frameNumber;
    int sumPixel;
    int minCenter;
};


#endif /* Calculator_hpp */
