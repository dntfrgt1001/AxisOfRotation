//
//  CalcAxis.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/09/23.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#include "CalcAxis.h"

CalcAxis::CalcAxis(cv::Size frameSize,
                   cv::vector<cv::Point2f>* start,
                   cv::vector<cv::Point2f>* end,
                   bool left, bool up, float dx, float shift):
frameSize(frameSize), start(start), end(end),
left(left), up(up), dx(dx), shift(shift)
{
    
}

void CalcAxis::setOpticalflow(cv::vector<cv::Point2f>* start,
                              cv::vector<cv::Point2f>* end){
    this->start = start;
    this->end = end;
}


int CalcAxis::calcAxis(){
    int prevLeft = minLeft;

    minValue = FLT_MAX;
    minLeft = INT_MAX;
    
    for(resetIter(); !isIterEnd(); incIter()){
        float curValue = calcWindow();
        
        if(std::abs(minValue) > std::abs(curValue)){
            minValue = curValue;
            minLeft = curLeft;
        }
    }
    
    if(minValue == FLT_MAX){
        minLeft = prevLeft;
        return INT_MAX;
    }
        
    return minLeft + dx / 2;
}

float CalcAxis::calcWindow(){
    int yCenter = frameSize.height / 2;
    
    float posSum = 0.0f;
    float negSum = 0.0f;
    int posNum = 0;
    int negNum = 0;
    
    cv::vector<cv::Point2f>::const_iterator startIter = start->begin();
    cv::vector<cv::Point2f>::const_iterator endIter = end->begin();
    for(; startIter!=start->end(); startIter++, endIter++){
        if(curLeft <= startIter->x && startIter->x < curLeft + dx &&
           (up? startIter->y <= yCenter: yCenter < startIter->y)){
            float xvector = endIter->x - startIter->x;
            
            if(0 <= xvector){
                posSum += xvector;
                posNum++;
            }else{
                negSum += xvector;
                negNum++;
            }
        }
    }
    
    if(posNum == 0 || negNum == 0){
        return FLT_MAX;
    }
    
    return posSum / posNum + negSum / negNum;
}

void CalcAxis::incIter(){
    curLeft = curLeft + shift;
}

bool CalcAxis::isIterEnd(){
    return (left?
                frameSize.width / 2 < curLeft:
                frameSize.width < curLeft);
}

void CalcAxis::resetIter(){
    curLeft = left? 0: frameSize.width / 2;
}

void CalcAxis::drawRange(cv::Mat& flowImg){
    int minRight = minLeft + dx;
    
    if(minLeft < frameSize.width){
        cv::line(flowImg, cv::Point(minLeft, 0),
                 cv::Point(minLeft, frameSize.height), cv::Scalar(127), 3);
        
        if(minRight < frameSize.width){
            cv::line(flowImg, cv::Point(minRight, 0),
                     cv::Point(minRight, frameSize.height), cv::Scalar(127), 3);
        
            cv::line(flowImg,
                     cv::Point(minLeft, 1),
                     cv::Point(minRight, 1),
                     cv::Scalar(127), 3);
            cv::line(flowImg,
                     cv::Point(minLeft, frameSize.height-1),
                     cv::Point(minRight, frameSize.height-1), cv::Scalar(127), 3);
        }else{
            cv::line(flowImg, cv::Point(minLeft, 1),
                     cv::Point(frameSize.width, 1),
                     cv::Scalar(127), 3);
            cv::line(flowImg, cv::Point(minLeft, frameSize.height-1),
                     cv::Point(frameSize.width, frameSize.height-1),
                     cv::Scalar(127), 3);
        }
    }
}


