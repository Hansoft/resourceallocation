//
//  HansoftLinkHelper.h
//  HansoftSDK_ResourceAllocation
//
//  Created by Sitael S.p.A. on 26/02/15.
//

#ifndef __HansoftSDK_ResourceAllocation__HansoftLinkHelper__
#define __HansoftSDK_ResourceAllocation__HansoftLinkHelper__

#include <stdio.h>
#include <string>
#include "HansoftLinkConstants.h"
#include <sstream>


#endif /* defined(__HansoftSDK_ResourceAllocation__HansoftLinkHelper__) */

using namespace std;

class HansoftLinkHelper{
    
public:
    static string getHansoftTaskLink(int idTask,string hansoftAddr,string hansoftDb);
};