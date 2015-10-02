//
//  CalcAxis.h
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/09/23.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#ifndef __AxisOfRotation__CalcAxis__
#define __AxisOfRotation__CalcAxis__

#include <stdio.h>
#include <opencv2/opencv.hpp>

class CalcAxis{
public:
    CalcAxis(cv::Size frameSize, bool left, bool up,
             float dx, float shift);
    ~CalcAxis(){};
    
    void setOpticalflow(cv::vector<cv::Point2f>* start,
                        cv::vector<cv::Point2f>* end);
    int calcAxis();
    float calcWindow();
    void incIter();
    bool isIterEnd();
    void resetIter();
    void drawRange(cv::Mat& flowImg);
    
private:
    cv::Size frameSize;
    const bool left;
    const bool up;
    cv::vector<cv::Point2f>* start;
    cv::vector<cv::Point2f>* end;
    const float dx;
    const float shift;
    int curLeft;
    int minLeft;
    float minValue;
};


#endif /* defined(__AxisOfRotation__CalcAxis__) */
