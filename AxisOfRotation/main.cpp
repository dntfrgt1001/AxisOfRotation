//
//  main.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/08/18.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "process.hpp"
#include "Calculator.hpp"
#include "CalcDeviationHalf.hpp"
#include "RemoveNoise.hpp"
#include "OutputData.hpp"
#include "Configure.hpp"

int main(int argc, const char * argv[])
{
    Configure conf("/Users/masakazu/Desktop/Opt/conf.ini");
    
    cv::Size frameSize(conf.getConfInt("universal.frameWidth"),
                       conf.getConfInt("universal.frameHeight"));
    const std::string dataPath = conf.getConfString("universal.dataPath");

    cv::Mat cameraMatrix, distCoeffs;
    
    calibrateProcess(conf, cameraMatrix, distCoeffs);
    
   
    const bool left = false;
    const bool up = true;
    const int threshNorm = 2;
    const int threshNum = 100;
    const int dx = frameSize.width / 12;
    const int shift = frameSize.width / 20;
    
    /*
    const int calcNum = 1;
    std::vector<Calculator> calcs;
    for (int i=0; i<calcNum; i++) {
        CalcDeviationHalf cdh = CalcDeviationHalf(frameSize, dx, shift, left);
        OutputData od(outputPath, ' ');
        Calculator calc(i, videoPath, frameSize, rn, cdf, od,
                        cameraMatrix, distCoeffs);
        calcs.push_back(calc);
    }
    */
    /*
    CalcDeviationHalf cdf1(frameSize, dx, shift, left);
    OutputData od1(dataPath+"output1.csv", ' ');
    Calculator calc1(1, videoPath, frameSize, rn, cdf1, od1,
                     cameraMatrix, distCoeffs);
    
    CalcDeviationHalf cdf2(frameSize, dx, shift, left);
    OutputData od2(dataPath+"output2.csv", ' ');
    Calculator calc2(2, videoPath, frameSize, rn, cdf2, od2,
                         cameraMatrix, distCoeffs);
    */

    RemoveNoise rn(frameSize, left, up, threshNorm, threshNum);
    
    const int calcNum = conf.getConfInt("flow.videoNum");
    std::vector<Calculator> calcs;
    for (int i=0; i<calcNum; i++) {
        CalcDeviationHalf cdh(frameSize, dx, shift, left);
        OutputData od(dataPath+"output"+std::to_string(i)+".csv", ' ');
        std::string videoName = dataPath +
                                conf.getConfString("flow.videoName"+std::to_string(i));
        Calculator calc(i, videoName, frameSize, rn, cdh, od,
                        cameraMatrix, distCoeffs);
        
        calcs.push_back(calc);
    }
    
//    CalcDeviationHalf cdf(frameSize, dx, shift, left);
//    RemoveNoise rn(frameSize, left, up, threshNorm, threshNum);
//    OutputData od(outputPath, ' ');
    
//    Calculator calc(0, videoPath, cdf, rn, od, frameSize,
//                    cameraMatrix, distCoeffs);

    
    while (true) {
        for (auto calcIter=calcs.begin(); calcIter!=calcs.end(); calcIter++) {
            try {
                calcIter->initialize();
                calcIter->calcFeature();
                calcIter->drawOpticalflow();
            }catch(int exception){
                switch (exception) {
                    case 0:
                        goto FINISH;
                        break;
                    case 1:
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
FINISH:
    
    return 0;
}

