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
    CalcDeviation(cv::Size& frameSize, int dx, int shift);
    CalcDeviation(const CalcDeviation& cd);
    
    int calcPixel(const cv::vector<cv::Point2f>& start,
                  const cv::vector<cv::Point2f>& end);
    float calcWindow(const cv::vector<cv::Point2f>& start,
                     const cv::vector<cv::Point2f>& end);
    
    virtual bool isInWindow(const cv::Point2f& start) const;
    virtual float calcValue(const cv::Point2f& start,
                            const cv::Point2f& end) const;
    
    virtual void resetIter();
    virtual bool isIterEnd() const;
    void incIter();
    
    void drawRange(cv::Mat& flowImg) const;
    
protected:
    CalcDeviation& operator=(const CalcDeviation& cd);
    cv::Size& frameSize;
    const int dx;
    const int shift;
    int curLeft;
    int minLeft;
};

#endif /* CalcDeviation_hpp */
