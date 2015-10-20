//
//  CalcDeviationHalf.hpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/09.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#ifndef CalcDeviationHalf_hpp
#define CalcDeviationHalf_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "CalcDeviation.hpp"

class CalcDeviationHalf: public CalcDeviation{
public:
    CalcDeviationHalf(cv::Size frameSize,
                      cv::vector<cv::Point2f>& start,
                      cv::vector<cv::Point2f>& end,
                      int dx, int shift, bool left);
    bool isInWindow(const cv::Point2f& start);
    float calcValue(const cv::Point2f& start, const cv::Point2f& end);
    
protected:
    bool left;
};

#endif /* CalcDeviationHalf_hpp */
