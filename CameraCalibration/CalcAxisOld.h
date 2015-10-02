//
//  CalcAxis.h
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/09/11.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#ifndef __AxisOfRotation__CalcAxis__
#define __AxisOfRotation__CalcAxis__

#include <stdio.h>
#include <opencv2/opencv.hpp>

class CalcAxis{
public:
    CalcAxis(cv::Size frameSize, bool left, bool up,
             int dx, int cotNum, int shift, int thNum);
    ~CalcAxis();
    
    void setOpticalflow(cv::vector<cv::Point2f>* start,
                        cv::vector<cv::Point2f>* end);
    void removeNoise();
    bool isEnoughAllVector();
    bool isEnoughPartialVector();
    int calcAxis();
    int calcWindow();
    void incIter();
    bool isIterEnd();
    void resetIter();
    void drawRange(cv::Mat& flowImg);

    float calcOneAverage(bool positive);
    
    
private:
    cv::Size frameSize;
    const bool left;
    const bool up;
    cv::vector<cv::Point2f>* start;
    cv::vector<cv::Point2f>* end;
    int minAbsValue;
    int minAbsLeft;
    const int dx;
    const int shift;
    int curLeft;
    const int cotNum;
    const int thNum;
    int *cot;
};

#endif /* defined(__AxisOfRotation__CalcAxis__) */