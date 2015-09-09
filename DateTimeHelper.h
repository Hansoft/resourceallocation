//
//  DateTimeHelper.h
//
//  Created by Sitael S.p.A. on 19/02/15.
//

#ifndef __HansoftSDK_ResourceAllocation__DateTimeHelper__
#define __HansoftSDK_ResourceAllocation__DateTimeHelper__

#include <stdio.h>
#include <boost/date_time.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

#endif /* defined(__HansoftSDK_ResourceAllocation__DateTimeHelper__) */

using namespace boost;
using namespace std;

class DateTimeHelper
{
    public :
    static unsigned long long getUnixTimeFromString(string dateString) ;
    static string getDateStringFromUnixTimeStampMicroSeconds(unsigned long long timestamp);
    static string getDateStringWithHoursFromUnixTimeStampMicroSeconds(unsigned long long timestamp);
    static gregorian::date getDateFromUnixTimeStampMicroSeconds(unsigned long long timestamp);
	static bool getIsSaturdayOrSunday(unsigned long long timestamp);



};
