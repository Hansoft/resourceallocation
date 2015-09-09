//
//  HansoftConnectionService.h
//
//  Created by Sitael S.p.A. on 20/02/15.
//

#ifndef __HansoftSDK_ResourceAllocation__HansoftConnectionService__
#define __HansoftSDK_ResourceAllocation__HansoftConnectionService__

#include <stdio.h>
#include <string>
#include "./hansoftSdk/HPMSdkCpp.h"
#include <iostream>
#include "ConfigHelper.h"
#include "PathHelper.h"
#include "OutputLogger.h"





#endif /* defined(__HansoftSDK_ResourceAllocation__HansoftConnectionService__) */

using namespace std;
using namespace HPMSdk;


class HansoftConnectionService: public HPMSdkCallbacks{
public:
    HPMNeedSessionProcessCallbackInfo m_ProcessCallbackInfo;
    HPMSdkSession *m_pSession;
    
    bool connectToHansoftServer() ;
    
    void disconnect();
    
    virtual void On_ProcessError(EHPMError _Error);

   
    HansoftConnectionService(){};

    
};

