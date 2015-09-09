//
//  HansoftLinkHelper.cpp
//
//  Created by Sitael S.p.A. on 26/02/15.
//

#include "HansoftLinkHelper.h"

/**
* Get hansoft link for task
* @param int idTask
* @param string hansoftAddress
* @param string hansoftDb
* @return string formatted link
*/
string HansoftLinkHelper::getHansoftTaskLink(int idTask,string hansoftAddr,string hansoftDb){
    
    stringstream ss;
    ss << idTask;
    string ourTaskIdString = ss.str();
    string taskHyperlink = hansoftLinkPrefix + hansoftAddr + ";" + hansoftDb + ";" + hansoftLinkPrefix2 + ourTaskIdString;
    
    return taskHyperlink;
}
