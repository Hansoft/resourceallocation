//
//  WorkDoneUpdatedTask.h
//
//  Created by Sitael S.p.A. on 18/02/15.
//

#ifndef __HansoftSDK_ResourceAllocation__WorkDoneUpdatedTask__
#define __HansoftSDK_ResourceAllocation__WorkDoneUpdatedTask__

#include <stdio.h>
#include <string>
#include <boost/date_time.hpp>

#endif /* defined(__HansoftSDK_ResourceAllocation__WorkDoneUpdatedTask__) */

using namespace std;
using namespace boost;

class WorkDoneUpdatedTask
{
    
public:
    int m_taskId;
    string m_taskDescription;
    double m_workDone;
    int m_projectId;
    string m_projectName;
    unsigned long long m_time;
    gregorian::date m_dateUpdate;
    double m_workDoneIncrement;
    string m_taskHyperlink;
	string m_taskContainerDescription;
	string m_taskContainerHyperlink;
	string m_jobId;
    
    
    
    bool operator < ( const WorkDoneUpdatedTask &_Right) const ;
    bool operator > ( const WorkDoneUpdatedTask &_Right) const;
    bool operator <= ( const WorkDoneUpdatedTask &_Right) const;
    bool operator >= ( const WorkDoneUpdatedTask &_Right) const;
    bool operator == (const WorkDoneUpdatedTask &_Right) const;
    bool operator != (const WorkDoneUpdatedTask &_Right) const;
    
	WorkDoneUpdatedTask() { m_taskId = 0; m_taskDescription = ""; m_workDone = 0; m_projectId = 0; m_projectName = ""; m_time = 0; m_workDoneIncrement = 0; m_taskHyperlink = ""; m_jobId = ""; }
    
};

