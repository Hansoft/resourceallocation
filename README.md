# Hansoft Resource Allocation

###Overview
Hansoft Resource Allocation is a tool that exports an excel file where is calculated for each day the time spent by each user.
In order to calculate this data is necessary to create a Custom Column for your project, named "Work done", where each user will add the hours spent on a task.
This application will notify with a red colour when a resource have not added at least 8 hours of work done for a specific day.
There is also the possibility to specify another custom field in the configuration file that will be exported in the output excel file.
Example:
Task assigned to:
	Programmer ABC
Project:
	VideoGame AAA
Task Container:
	Database Development
Task:
	Database Design
Task Id:
	585
Work Done:
	8,00
Date and Time:
	2015-Aug-31	15:17:10
Second Custom value Field:
	53001A

###Terms and conditions
Hansoft Resource Allocation is licensed under a MIT License as stated in the [LICENSE.md].
This tool is not part of the official Hansoft product or subject to its license agreement. The program is provided as is and there is no obligation on Hansoft AB to provide support, update or enhance this program.


###Building the program
Hansoft Resource Allocation can be built with e.g the freely available [Visual Studio Express 2013 for Desktop (Windows)]. 
You will need the Hansoft SDK to be able to build the program. 
Make sure to include the:
- HPMSdkCpp.h and HPMSdkCpp.cpp files
- Boost header
- XlsLib Header
- minGlue.h
- minIni.c and minIni.h

 You also need to add:
- HPMSdk.x86.dll(HPMSdk.x64.dll)
- HPMSdkManaged_4_5.x86.dll (HPMSdkManaged_4_5.x64.dll)
- Boost lib
	- libboost_date_time-vc120-mt-1_57.lib
	- libboost_date_time-vc120-mt-gd-1_57
	- libboost_date_time-vc120-mt-s-1_57.lib
	- libboost_date_time-vc120-mt-sgd-1_57.lib
	- libboost_date_time-vc120-s-1_57.lib
	- libboost_date_time-vc120-sgd-1_57.lib
	- libboost_filesystem-vc120-mt-1_57.lib
	- libboost_filesystem-vc120-mt-gd-1_57.lib
	- libboost_filesystem-vc120-mt-s-1_57.lib
	- libboost_filesystem-vc120-mt-sgd-1_57.lib
	- libboost_filesystem-vc120-s-1_57.lib
	- libboost_filesystem-vc120-sgd-1_57.lib
	- libboost_system-vc120-mt-1_57.lib
	- libboost_system-vc120-mt-gd-1_57.lib
	- libboost_system-vc120-mt-s-1_57.lib
	- libboost_system-vc120-mt-sgd-1_57.lib
	- libboost_system-vc120-s-1_57.lib
	- libboost_system-vc120-sgd-1_57.lib
- XlsLib
	- xlslib_lib.lib
	
You eventually need to update the project references.

###Configuration file settings
The configuration file is config.ini and requires the following settings:

[Login]
user = Hansoft User with SDK license
pass = Hansoft User Password with SDK license
hansoftUrl = Hansoft Server Url
hansoftPort = Hansoft Server Url
hansoftDatabase = Hansoft Database Name
projectName = (if left empty it will analyze all the project, otherwise can be specified a single project name)
[CustomColumn]
columnName = "Work done" (Custom column name that keep track of the hours spent on a task)
jobId = "Commessa" (Additional Custom column that we want to be visible on the excel output file)
[Report]
fromTime = 2015/08/25 
toTime = 2015/08/31

[LICENSE.md]:https://github.com/Hansoft/resourceallocation/master/LICENSE.md
[Visual Studio Express 2013 for Desktop (Windows)]:https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx
