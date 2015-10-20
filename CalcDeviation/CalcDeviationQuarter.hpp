//
//  CalcDeviationQuarter.hpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/18.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#ifndef CalcDeviationQuarter_hpp
#define CalcDeviationQuarter_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "CalcDeviationHalf.hpp"

class CalcDeviationQuarter: public CalcDeviationHalf{
public:
    CalcDeviationQuarter(cv::Size frameSize,
                         cv::vector<cv::Point2f>& start,
                         cv::vector<cv::Point2f>& end,
                         int dx, int shift, bool left, bool up);
    bool isInWindow(const cv::Point2f& start);
    float calcValue(const cv::Point2f& start, const cv::Point2f& end);
    
protected:
    bool up;
};

#endif /* CalcDeviationQuarter_hpp */
