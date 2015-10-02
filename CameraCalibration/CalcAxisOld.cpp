//
//  CalcAxis.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/09/11.
//  Copyright (c) 2015年 masakazu. All rights reserved.
//

#include "CalcAxis.h"

CalcAxis::CalcAxis(cv::Size frameSize, bool left, bool up,
                   int dx, int cotNum, int shift, int thNum):
frameSize(frameSize), left(left), up(up),
dx(dx), cotNum(cotNum), shift(shift), thNum(thNum)
{
    cot = new int[cotNum];
}

CalcAxis::~CalcAxis()
{
    delete cot;
}

void CalcAxis::setOpticalflow(cv::vector<cv::Point2f>* start,
                              cv::vector<cv::Point2f>* end)
{
    this->start = start;
    this->end = end;
}

void CalcAxis::removeNoise()
{
    cv::vector<cv::Point2f>::iterator startIter = start->begin();
    
    cv::vector<cv::Point2f>::iterator endIter = end->begin();
    
    float norm = 0.0f;
    for(; startIter!=start->end(); startIter++, endIter++){
        norm += cv::norm(*startIter-*endIter);
    }
    norm /= start->size();
    
//    std::cout << "average norm = " << norm
//    << ", vector number = " << start->size() << std::endl;
    
    startIter = start->begin();
    endIter = end->begin();
    for(; startIter!=start->end(); ){
        if(cv::norm(*startIter-*endIter) > 2.0f * norm){
            startIter = start->erase(startIter);
            endIter = end->erase(endIter);
            
            continue;
        }
        
        startIter++;
        endIter++;
    }
}

bool CalcAxis::isEnoughAllVector()
{
    int center = frameSize.width / 2;
    
    int count = 0;
    cv::vector<cv::Point2f>::const_iterator startIter = start->begin();
    for(; startIter!=start->end(); startIter++){
        if(center <= startIter->x && startIter->x <= frameSize.width){
            count++;
        }
    }
    
//    std::cout << "count = " << count << std::endl;
    
    if(count < thNum)
        return false;
    
    return true;
}

bool CalcAxis::isEnoughPartialVector()
{
    int xcenter = frameSize.width / 2;
    int ycenter = frameSize.height / 2;
    
    int count = 0;
    cv::vector<cv::Point2f>::const_iterator startIter = start->begin();
    for(; startIter!=start->end(); startIter++){
        if((left? startIter->x <= xcenter: xcenter < startIter->x) &&
           (up?   startIter->y <= ycenter: ycenter < startIter->y)){
            count++;
        }
    }
  
    std::cout << "count = " << count << std::endl;
    
    if(count < thNum / 4)
        return false;
    
    return true;
}

int CalcAxis::calcAxis()
{
    minAbsValue = INT_MAX / 2;
    minAbsLeft = INT_MAX / 2;

    for(resetIter(); !isIterEnd(); incIter()){
        int curValue = calcWindow();
        if(abs(minAbsValue) > abs(curValue)){
            minAbsValue = curValue;
            minAbsLeft = curLeft;
        }
    }
    
    return minAbsLeft + dx / 2;
}

int CalcAxis::calcWindow()
{
    float posSum = 0;
    float negSum = 0;
    int posNum = 0;
    int negNum = 0;
    
    cv::vector<cv::Point2f>::const_iterator startIter = start->begin();
    cv::vector<cv::Point2f>::const_iterator endIter = end->begin();
    for(; startIter!=start->end(); startIter++, endIter++){
        if(curLeft <= startIter->x && startIter->x < curLeft+dx &&
           up?
            startIter->y <= frameSize.height/2:
            startIter->y >  frameSize.height/2){
            
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
    
    std::cout << posSum << ", " << posNum << ", " << negSum << ", " << negNum;
    
    if(posNum == 0 || negNum == 0){
        std::cout << std::endl;
        return INT_MAX;
    }
    
    std::cout << ", " << (posSum/posNum+negSum/negNum) <<  std::endl;
    
    
    return posSum/posNum + negSum/negNum;
}

void CalcAxis::resetIter()
{
    curLeft = left? 0: frameSize.width / 2;
}

bool CalcAxis::isIterEnd()
{
    return (left?
                frameSize.width / 2 < curLeft:
                frameSize.width < curLeft);
}

void CalcAxis::incIter()
{
    curLeft = curLeft + shift;
}

void CalcAxis::drawRange(cv::Mat& flowImg)
{
    int minAbsRight = minAbsLeft + dx;
    
    if(minAbsLeft < frameSize.width - dx){

        cv::line(flowImg, cv::Point(minAbsLeft, 0),
                 cv::Point(minAbsLeft, frameSize.height), cv::Scalar(127));
    
        cv::line(flowImg, cv::Point(minAbsRight, 0),
                 cv::Point(minAbsRight, frameSize.height), cv::Scalar(127));
    }
}
