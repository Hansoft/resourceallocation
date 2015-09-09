//
//  ConfigHelper.cpp
//
//  Created by Sitael S.p.A. on 18/02/15.
//

#include "ConfigHelper.h"

/**
* Get string parameter from config.ini
* @param string sectionName
* @param string parameter
* @return string param
*/
string ConfigHelper::getParamStringForSection(string section,string param)
{
    minIni parser = minIni("config.ini");
    string paramValue = parser.gets(section, param);
    
    return paramValue;
   
}

/**
* Get int parameter from config.ini
* @param string sectionName
* @param string parameter
* @return int param
*/
int ConfigHelper::getParamIntForSection(string section,string param)
{
    minIni parser = minIni("config.ini");
    int paramValue = parser.geti(section, param);
    
    return paramValue;
    
}



