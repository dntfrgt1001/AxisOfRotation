//
//  CalcDeviation.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/08.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "CalcDeviation.hpp"

CalcDeviation::CalcDeviation(cv::Size frameSize,
                   cv::vector<cv::Point2f>& start, cv::vector<cv::Point2f>& end,
                   int dx, int shift):
frameSize(frameSize), start(start), end(end), dx(dx), shift(shift),
curLeft(0), minLeft(0)
{
}


/*
 return the pixel of cotangent theta from the left of frame
 if not found in the frame, return INT_MAX, but minLeft is kept
 */
int CalcDeviation::calcPixel(){
    int prevLeft = minLeft;
    
    int tmpMinLeft = INT_MAX;
    float tmpMinValue = FLT_MAX;

    for(resetIter(); !isIterEnd(); incIter()){
        float curValue = calcWindow();
        
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
float CalcDeviation::calcWindow(){
    float posValue = 0.0f;
    float negValue = 0.0f;
    int posNum = 0;
    int negNum = 0;
    
    cv::vector<cv::Point2f>::const_iterator startIter = start.begin();
    cv::vector<cv::Point2f>::const_iterator endIter = end.begin();
    for(; startIter!=start.end(); startIter++, endIter++){
        if(isInWindow(*startIter)){
            float value = calcValue(*startIter, *endIter);
            
            if(0 <= value){
                posValue += value;
                posNum++;
            }else{
                negValue += value;
                negNum++;
            }
        }
    }
    
    if(posNum == 0 || negNum == 0){
        return FLT_MAX;
    }
    
    return posValue / posNum + negValue / negNum;
}


/*
bool CalcDeviation::isInWindow(const cv::Point2f& start){
}
 */

/*
float CalcDeviation::calcValue(const cv::Point2f& start, const cv::Point2f& end){
}
 */

void CalcDeviation::resetIter(){
    curLeft = 0;
}

bool CalcDeviation::isIterEnd(){
    return  frameSize.width < curLeft;
}

void CalcDeviation::incIter(){
    curLeft = curLeft + shift;
}

void CalcDeviation::drawRange(cv::Mat& flowImg)
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