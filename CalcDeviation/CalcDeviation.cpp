//
//  CalcDeviation.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/08.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "CalcDeviation.hpp"

CalcDeviation::CalcDeviation(cv::Size& frameSize, int dx, int shift):
frameSize(frameSize), dx(dx), shift(shift),
curLeft(0), minLeft(0)
{
}

CalcDeviation::CalcDeviation(const CalcDeviation& cd):
frameSize(cd.frameSize), dx(cd.dx), shift(cd.shift),
curLeft(0), minLeft(0)
{
}

/*
 return the pixel of cotangent theta from the left of frame
 if not found in the frame, return INT_MAX, but minLeft is kept
 */
int CalcDeviation::calcPixel(const cv::vector<cv::Point2f>& start,
                             const cv::vector<cv::Point2f>& end)
{
    int prevLeft = minLeft;
    
    int tmpMinLeft = INT_MAX;
    float tmpMinValue = FLT_MAX;

    for(resetIter(); !isIterEnd(); incIter()){
        float curValue = calcWindow(start, end);
        
        if(std::abs(tmpMinValue) > std::abs(curValue)){
            tmpMinLeft = curLeft;
            tmpMinValue = curValue;
        }
    }
    
    if(tmpMinValue == FLT_MAX){
        minLeft = prevLeft;
        return INT_MAX;
    }
    
    minLeft = tmpMinLeft;
    
    return minLeft + dx / 2;
}

/*
 return the value of the current window
 if cannnot calculate, return FLT_MAX
 */
float CalcDeviation::calcWindow(const cv::vector<cv::Point2f>& start,
                                const cv::vector<cv::Point2f>& end)
{
    float posValue = 0.0f;
    float negValue = 0.0f;
    int posCount = 0;
    int negCount = 0;
    
    for(auto startIter=start.begin(),endIter=end.begin();
        startIter!=start.end(); startIter++, endIter++)
    {
        if(isInWindow(*startIter)){
            float value = calcValue(*startIter, *endIter);
            
            if(0 <= value){
                posValue += value;
                posCount++;
            }else{
                negValue += value;
                negCount++;
            }
        }
    }
    
    if(posCount == 0 || negCount == 0){
        return FLT_MAX;
    }
    
    int comp = 10;
    if((posCount > negCount * comp) || (negCount > posCount * comp))
        return FLT_MAX;
    
    return posValue / posCount + negValue / negCount;
}

bool CalcDeviation::isInWindow(const cv::Point2f &start) const
{
    /*
    cv::Rect window(curLeft, 0, dx, frameSize.height);
    
    if (window.contains(start)) {
        return true;
    }
    */
    
    int blankHeight = frameSize.height / 4;
    cv::Rect window1(curLeft, 0, dx, (frameSize.height-blankHeight)/2);
    cv::Rect window2(curLeft, (frameSize.height+blankHeight)/2,
                     dx, (frameSize.height-blankHeight)/2);
    
    if(window1.contains(start) || window2.contains(start))
        return true;
    
    return false;
}

float CalcDeviation::calcValue(const cv::Point2f& start,
                               const cv::Point2f& end) const
{
    float value = end.x - start.x;
    
    if(frameSize.height/2 < start.y){
        value *= -1.0f;
    }
    
    return value;
}

void CalcDeviation::resetIter()
{
    curLeft = 0;
}

bool CalcDeviation::isIterEnd() const
{
    return  frameSize.width < curLeft;
}

void CalcDeviation::incIter()
{
    curLeft = curLeft + shift;
}
 
void CalcDeviation::drawRange(cv::Mat& flowImg) const
{
    int minRight = minLeft + dx;
    int minCenter = minLeft + dx / 2;
    cv::Scalar color(127);
    
    if(0 <= minLeft && minLeft <= frameSize.width){
        cv::line(flowImg,
                 cv::Point(minLeft, 0), cv::Point(minLeft, frameSize.height),
                 color, 3);
    }
    
    if(0 <= minRight && minRight <= frameSize.width){
        cv::line(flowImg,
                 cv::Point(minRight, 0), cv::Point(minRight, frameSize.height),
                 color, 3);
    }
    
    if(0 <= minCenter && minCenter <= frameSize.width){
        cv::line(flowImg,
                 cv::Point(minCenter, 0), cv::Point(minCenter, frameSize.height),
                 color, 6);
    }
}

