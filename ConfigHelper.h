//
// ConfigHelper.h
//
//  Created by Sitael S.p.A. on 18/02/15.
//

#ifndef __HansoftSDK_ResourceAllocation__PathHelper__
#define __HansoftSDK_ResourceAllocation__PathHelper__

#include <iostream>
#include <string>
#include "minIni.h"
#include "ConfigKeyConstants.h"

using namespace std;



#endif /* defined(__HansoftSDK_ResourceAllocation__PathHelper__) */

class ConfigHelper
{
    public :
    static string getParamStringForSection(string section,string param) ;
    static int getParamIntForSection(string section,string param) ;

} ;
