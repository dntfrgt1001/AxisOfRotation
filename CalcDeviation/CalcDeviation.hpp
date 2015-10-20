//
//  CalcDeviation.hpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/08.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#ifndef CalcDeviation_hpp
#define CalcDeviation_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

class CalcDeviation{
public:
    CalcDeviation(cv::Size frameSize,
             cv::vector<cv::Point2f>& start, cv::vector<cv::Point2f>& end,
             int dx, int shift);
    ~CalcDeviation(){};
    
    int calcPixel();
    float calcWindow();
    virtual bool isInWindow(const cv::Point2f& start) = 0;
    virtual float calcValue(const cv::Point2f& start, const cv::Point2f& end) = 0;
    void incIter();
    bool isIterEnd();
    void resetIter();
    void drawRange(cv::Mat& flowImg);
    
protected:
    cv::Size frameSize;
    cv::vector<cv::Point2f>& start;
    cv::vector<cv::Point2f>& end;
    const int dx;
    const int shift;
    int curLeft;
    int minLeft;
};

#endif /* CalcDeviation_hpp */
