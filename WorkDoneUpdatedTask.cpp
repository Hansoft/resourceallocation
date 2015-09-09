//
//  WorkDoneUpdatedTask.cpp
//
//  Created by Sitael S.p.A. on 18/02/15.
//

#include "WorkDoneUpdatedTask.h"


bool WorkDoneUpdatedTask::operator < ( const WorkDoneUpdatedTask &_Right) const
{
    if (m_time < _Right.m_time)
        return true;
    return false;
}

bool WorkDoneUpdatedTask::operator > ( const WorkDoneUpdatedTask &_Right) const
{
    return _Right < *this;
}
bool WorkDoneUpdatedTask::operator <= ( const WorkDoneUpdatedTask &_Right) const
{
    return !(_Right < *this);
}
bool WorkDoneUpdatedTask::operator >= ( const WorkDoneUpdatedTask &_Right) const
{
    return !(*this < _Right);
}

bool WorkDoneUpdatedTask::operator == (const WorkDoneUpdatedTask &_Right) const
{
    if (m_time != _Right.m_time)
        return false;
    if (m_projectId != _Right.m_projectId)
        return false;
    if (m_projectName != _Right.m_projectName)
        return false;
    if (m_taskId != _Right.m_taskId)
        return false;
    if (m_taskDescription != _Right.m_taskDescription)
        return false;
    if (m_workDone != _Right.m_workDone)
        return false;
    if (m_time != _Right.m_time)
        return false;
	if (m_jobId != _Right.m_jobId)
		return false;
    return true;
}
bool WorkDoneUpdatedTask::operator != (const WorkDoneUpdatedTask &_Right) const
{
    return !(*this == _Right);
}