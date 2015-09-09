//
//  OutputLogger.cpp
//
//  Created by Sitael S.p.A. on 26/02/15.
//

#include "OutputLogger.h"

/**
* Write to output message
* @param string stringToOuput string to write on console
*/
void OutputLogger::logToConsole(string stringToOutput){
    std::wstring wstr = std::wstring(stringToOutput.begin(), stringToOutput.end());
    wcout << wstr;
    wcout << "\r\n";
    
}

