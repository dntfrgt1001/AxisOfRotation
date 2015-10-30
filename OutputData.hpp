//
//  OutputData.hpp
//  AxisOfRotation
//
//  Created by masakazu nakazawa on 2015/10/02.
//  Copyright © 2015年 masakazu. All rights reserved.
//

#ifndef OutputData_hpp
#define OutputData_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>

class OutputData {
public:
    OutputData(const std::string fileName, char seperater);
    ~OutputData();
    OutputData(const OutputData& od);
    
    void outputTwin(int first, int second);
    void outputTriple(int first, int second, int third);
    
private:
    OutputData& operator=(const OutputData& od);
    
    const std::string fileName;
    const char seperater;
    std::ofstream ofs;
    
};

#endif /* OutputData_hpp */
