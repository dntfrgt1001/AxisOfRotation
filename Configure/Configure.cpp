//
//  Configure.cpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/22.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#include "Configure.hpp"

Configure::Configure(std::string fileName):
fileName(fileName)
{
    boost::property_tree::read_ini(fileName, pt);
}

int Configure::getConfInt(std::string confName)
{
    boost::optional<int> value = pt.get_optional<int>(confName);
    
    if(!value){
        printError(confName);
        exit(1);
        //return -1;
    }

    return value.get();
}

std::string Configure::getConfString(std::string confName)
{
    boost::optional<std::string> value = pt.get_optional<std::string>(confName);
    
    if(!value){
        printError(confName);
        exit(1);
        //        return std::string();
    }
    
    return value.get();
}

void Configure::printError(std::string confName){
    std::cout << "\"" << confName << "\"" << " is not found." << std::endl;
}