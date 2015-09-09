//
//  HansoftConnectionService.cpp
//
//  Created by Sitael S.p.A. on 20/02/15.
//

#include "HansoftConnectionService.h"

/**
* Connect to hansoft and store session
* @return bool result of connection
*/
bool HansoftConnectionService::connectToHansoftServer(){
    
    EHPMSdkDebugMode DebugMode = EHPMSdkDebugMode_Off;
#ifdef _DEBUG
    DebugMode = EHPMSdkDebugMode_Debug; // Set debug flag so we will get memory leak info.
#endif
    
    try
    {
        string hansoftUrlParam = ConfigHelper::getParamStringForSection(loginSection, hansoftAddress );
        int hansoftPortParam = ConfigHelper::getParamIntForSection(loginSection, hansoftPort );
        string hansoftDbParam = ConfigHelper::getParamStringForSection(loginSection, hansoftDatabase );
        string usernameParam = ConfigHelper::getParamStringForSection(loginSection, hansoftUser );
        string passwordParam = ConfigHelper::getParamStringForSection(loginSection, hansoftPassword );
        string libraryPath = PathHelper::getCurrentPath();

        
		m_pSession = HPMSdkSession::SessionOpen(wstring(hansoftUrlParam.begin(), hansoftUrlParam.end()), hansoftPortParam, wstring(hansoftDbParam.begin(), hansoftDbParam.end()), wstring(usernameParam.begin(), usernameParam.end()), wstring(passwordParam.begin(), passwordParam.end()), this, &m_ProcessCallbackInfo, true, DebugMode, NULL, 0, L"", wstring(libraryPath.begin(), libraryPath.end()), NULL);
        
        
    }
    catch (const HPMSdkException &_Error)
    {
        HPMSdk::HPMString SdkError = _Error.GetAsString();
        OutputLogger::logToConsole("SessionOpen failed with error:");
        return false;
    }
    catch (const HPMSdkCppException &_Error)
    {
		HPMSdk::HPMString SdkCppError = hpm_str("");
        OutputLogger::logToConsole("SessionOpen failed with error:");
        return false;
    }
    
    OutputLogger::logToConsole("Successfully opened session");
    return true;
    
}

void HansoftConnectionService::On_ProcessError(HPMSdk::EHPMError _Error)
{
	HPMSdk::HPMString SdkError = HPMSdkSession::ErrorToStr(_Error);
    OutputLogger::logToConsole("On_ProcessError: ");
}

void HansoftConnectionService::disconnect(){
    
    if (m_pSession)
    {
        HPMSdkSession::SessionDestroy(m_pSession);
        m_pSession = NULL;
    }
    
    
}




