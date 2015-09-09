//
//  DateTimeHelper.cpp
//
//  Created by Sitael S.p.A. on 19/02/15.
//

#include "DateTimeHelper.h"

/**
* Get unix time from string 
* @param string dateString 
* @return unsigned long long seconds from 1970 for date
*/
unsigned long long DateTimeHelper::getUnixTimeFromString(string dateString)
{

    if(dateString == "")
        return 0;
    else{
        gregorian::date date(gregorian::from_simple_string(dateString));
        posix_time::ptime posTime(date);
        posix_time::ptime epochEnd(boost::gregorian::date(1970, 1, 1));
        posix_time::time_duration diffDuration(posTime - epochEnd);
        return (diffDuration.ticks() / diffDuration.ticks_per_second());
    }
}

/**
* Get date string from microseconds time
* @param unsigned long long timestamp
* @return string formatted date
*/
string DateTimeHelper::getDateStringFromUnixTimeStampMicroSeconds(unsigned long long timestamp){
    gregorian::date dateFinal = posix_time::from_time_t(timestamp/1000000).date();
    return gregorian::to_simple_string(dateFinal);
}

/**
* Get date string with hours from microseconds time
* @param unsigned long long timestamp
* @return string formatted date
*/
string DateTimeHelper::getDateStringWithHoursFromUnixTimeStampMicroSeconds(unsigned long long timestamp){
    boost::posix_time::ptime pTimeFinal;
    pTimeFinal = posix_time::from_time_t(timestamp/1000000);
    
    return boost::posix_time::to_simple_string(pTimeFinal);
    
}
/**
* Get gregorian::date from microseconds timestamp
* @param unsigned long long timestamp
* @return gregorian::date date Object
*/
gregorian::date DateTimeHelper::getDateFromUnixTimeStampMicroSeconds(unsigned long long timestamp){
    gregorian::date dateFinal = posix_time::from_time_t(timestamp/1000000).date();
    return dateFinal;
}

bool DateTimeHelper::getIsSaturdayOrSunday(unsigned long long timestamp)
{
	gregorian::date dateFinal = posix_time::from_time_t(timestamp).date();
	return (dateFinal.day_of_week().as_short_string() == std::string("Sat") || dateFinal.day_of_week().as_short_string() == std::string("Sun"));


}



