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
    CalcDeviationQuarter(cv::Size& frameSize, int dx, int shift,
                         bool left, bool up);
    CalcDeviationQuarter(const CalcDeviationQuarter& cdq);

    bool isInWindow(const cv::Point2f& start) const;
    float calcValue(const cv::Point2f& start, const cv::Point2f& end) const;
    
protected:
    bool up;
};

#endif /* CalcDeviationQuarter_hpp */
