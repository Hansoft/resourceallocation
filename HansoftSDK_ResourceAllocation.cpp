//
//  HansoftSDK_ResourceAllocation.cpp
//  Application Entry point
//
//  Created by Sitael S.p.A. on 20/02/2015.
//

#include "./hansoftSdk/HPMSdkCpp.h"

#ifdef _MSC_VER
#include <tchar.h>
#include <conio.h>
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/select.h>
#include <termios.h>
#ifdef __APPLE__
#include <errno.h>
#include <pthread.h>
#endif
#endif
#include <iostream>
#include <string>
#include <map>
#include "WorkDoneUpdatedTask.h"
#include "xlslib.h"
#include <boost/date_time.hpp>
#include "DateTimeHelper.h"
#include "XlsConstants.h"
#include "HansoftConnectionService.h"
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "HansoftLinkHelper.h"


using namespace std;
using namespace HPMSdk;
using namespace xlslib_core;
using namespace boost;


class HansoftSDK_ResourceAllocation
{
public:


	HansoftConnectionService hansoftService;
	HPMSdkSession *sdkSession;
	container::flat_map<string, int> mapActiveResource;
	container::flat_map <string, container::flat_map<unsigned long long, vector<WorkDoneUpdatedTask>>> mapTask;

	/**
	* Constructor
	*
	*
	*/
	HansoftSDK_ResourceAllocation()
	{
		sdkSession = NULL;
	}

	/**
	* Distructor
	*
	*
	*/
	~HansoftSDK_ResourceAllocation()
	{
		if (sdkSession)
		{
			hansoftService.disconnect();
		}
	}
	/**
	* Execute method.
	* Parse projects and output excel based on config.ini file
	*
	*/
	void Execute()
	{

		if (hansoftService.connectToHansoftServer())
		{
			try
			{
				sdkSession = hansoftService.m_pSession;
				printInputParameters();
				string projToAnalyze = ConfigHelper::getParamStringForSection(loginSection, projectNameToSearch);
				HPMProjectEnum projects = sdkSession->ProjectEnum();
				HPMResourceEnum resMemb = sdkSession->ResourceEnum();
				
				for (HPMUInt32 o = 0; o < resMemb.m_Resources.size(); ++o)
				{
					HPMUniqueID idRisorsa = resMemb.m_Resources[o];
					HPMResourceProperties propRes = sdkSession->ResourceGetProperties(idRisorsa);
					if (propRes.m_ResourceType != EHPMResourceType::EHPMResourceType_QAAccount )
					{
						
						HPMUInt64 resultDel = sdkSession->ResourceGetDeletedDate(idRisorsa);
						if (resultDel == 0)
						{
							string resource = string(propRes.m_Name.begin(), propRes.m_Name.end());
							mapActiveResource[resource] = 1;
						}
	
					}
				}
				for (HPMUInt32 i = 0; i < projects.m_Projects.size(); ++i)
				{
					HPMUniqueID projectUid = projects.m_Projects[i];
					HPMTaskEnum tasks = sdkSession->TaskEnum(projectUid);
					HPMProjectProperties projectProp = sdkSession->ProjectGetProperties(projectUid);
					HPMProjectCustomColumns columns = sdkSession->ProjectCustomColumnsGet(projectUid);
					HPMUniqueID customColumnDataHash;
					HPMProjectCustomColumnsColumn columnJob;
					string proj = string(projectProp.m_Name.begin(), projectProp.m_Name.end());
					
					if (!projectProp.m_bArchivedStatus && (projToAnalyze == "" || proj == projToAnalyze)){
						OutputLogger::logToConsole("Analyzing project:");
						OutputLogger::logToConsole(proj);
						for (HPMUInt32 k = 0; k < columns.m_ShowingColumns.size(); ++k)
						{
							HPMProjectCustomColumnsColumn info = columns.m_ShowingColumns[k];
							if (string(info.m_Name.begin(), info.m_Name.end()) == ConfigHelper::getParamStringForSection(customColumnSection, customColumnName)){
								customColumnDataHash = info.m_Hash;
							}
							else if (string(info.m_Name.begin(), info.m_Name.end()) == ConfigHelper::getParamStringForSection(customColumnSection, customColumnNameJobId)) {
								columnJob = info;
							}

						}
						for (HPMUInt32 j = 0; j < tasks.m_Tasks.size(); ++j)
						{
							string customColumnJobId = "";
							HPMUniqueID taskId = tasks.m_Tasks[j];
							HPMUniqueID clientId = sdkSession->TaskGetID(taskId);
							string taskDescriptionContainer = getContainerDescription(taskId);
							string taskHyperlinkContFinal = getContainerHyperlink(taskId);
							
							customColumnJobId = getCustomColumnValue(taskId, columnJob);
							
							parseHistoryForTask(taskId, customColumnDataHash, projectUid, projectProp, taskDescriptionContainer, taskHyperlinkContFinal, customColumnJobId);
						}
					}
				}
				writeResultsToExcel();

				
			}
			catch (HPMSdkException &_Error)
			{
				HPMString SdkError = _Error.GetAsString();
				OutputLogger::logToConsole("Exception in processing loop: " + string(SdkError.begin(), SdkError.end()));
			}
			catch (HPMSdkCppException _Error)
			{
				string SdkCppError = _Error.what();
				OutputLogger::logToConsole("Exception in processing loop: " + SdkCppError);
			}
		}
	}


	string getContainerDescription(HPMUniqueID idTask)
	{
		string desc = "";
		HPMTaskEnum tasksEn = sdkSession->TaskEnumReferences(idTask);
		for (HPMUInt32 j = 0; j < tasksEn.m_Tasks.size(); ++j)
		{
			HPMUniqueID taskRefId = tasksEn.m_Tasks[j];
			HPMUniqueID idParent = sdkSession->TaskRefUtilGetParent(taskRefId);

			if (sdkSession->UtilIsIDTaskRef(idParent))
			{
				HPMUniqueID taskId = sdkSession->TaskRefGetTask(idParent);
				if (taskId != idTask)
				{
					HPMString descHpm = sdkSession->TaskGetDescription(taskId);
					desc = string(descHpm.begin(), descHpm.end());
					break;
				}
			}
			
			
		}
		return desc;
	}

	string getContainerHyperlink(HPMUniqueID idTask)
	{
		string taskHyperlinkContFinal = "";
		HPMTaskEnum tasksEn = sdkSession->TaskEnumReferences(idTask);
		for (HPMUInt32 j = 0; j < tasksEn.m_Tasks.size(); ++j)
		{
			HPMUniqueID taskRefId = tasksEn.m_Tasks[j];
			HPMUniqueID idParent = sdkSession->TaskRefUtilGetParent(taskRefId);

			if (sdkSession->UtilIsIDTaskRef(idParent))
			{
				HPMUniqueID taskId = sdkSession->TaskRefGetTask(idParent);
				if (taskId != idTask)
				{
					taskHyperlinkContFinal = HansoftLinkHelper::getHansoftTaskLink(taskId, ConfigHelper::getParamStringForSection(loginSection, hansoftAddress), ConfigHelper::getParamStringForSection(loginSection, hansoftDatabase));
					break;
				}
			}

		}
		return taskHyperlinkContFinal;
	}

	string getCustomColumnValue(HPMUniqueID idTask, HPMProjectCustomColumnsColumn customColumn) {
		string ret = "";
		HPMString value = sdkSession->TaskGetCustomColumnData(idTask, customColumn.m_Hash);
		string valueStr = string(value.begin(), value.end());
		if (valueStr != "") {
			int index = -1;
			HPMString valueCol;
			switch (customColumn.m_Type)
			{
			case EHPMProjectCustomColumnsColumnType_DropList:
				index = atoi(valueStr.c_str());
				ret = getDropListItemValue(index, customColumn.m_DropListItems);
				break;
			default:
				ret = "TODO";
			}
		}
		
		return ret;
	}

	string getDropListItemValue(int id, vector<HPMProjectCustomColumnsColumnDropListItem> itemList) {
		string ret = "";
		for (vector<HPMProjectCustomColumnsColumnDropListItem>::iterator it = itemList.begin(); it != itemList.end(); ++it) {
			HPMProjectCustomColumnsColumnDropListItem item = *it;
			if (item.m_Id == id)
				ret = string(item.m_Name.begin(), item.m_Name.end());
		}

		return ret;
	}

	/**
	* Execute method
	*
	*
	*/
	int Run()
	{
		Execute();
		return 0;

	}

	/**
	* Print on console input parameters on config.ini
	*
	*
	*/
	void printInputParameters(){

		OutputLogger::logToConsole("Input parameter");
		OutputLogger::logToConsole("Selected Project");
		OutputLogger::logToConsole(ConfigHelper::getParamStringForSection(loginSection, projectNameToSearch));
		OutputLogger::logToConsole("Name Custom Column Work Done");
		OutputLogger::logToConsole(ConfigHelper::getParamStringForSection(customColumnSection, customColumnName));
		OutputLogger::logToConsole("Name Custom Column Job ");
		OutputLogger::logToConsole(ConfigHelper::getParamStringForSection(customColumnSection, customColumnNameJobId));
		OutputLogger::logToConsole("Date Start");
		OutputLogger::logToConsole(ConfigHelper::getParamStringForSection(reportSection, reportFromTime));
		OutputLogger::logToConsole("Date End");
		OutputLogger::logToConsole(ConfigHelper::getParamStringForSection(reportSection, reportToTime));
	};

	/**
	* Get history entries for task by querying hansoft sdk
	* @param HPMUniqueId taskId
	* @return HPMDataHistory historyObject
	*/
	HPMDataHistory getHistoryForTask(HPMUniqueID taskId){
		HPMDataHistoryGetHistoryParameters params;
		params.m_DataID = taskId;
		params.m_FieldID = EHPMStatisticsField_NoStatistics;
		params.m_FieldData = 0;
		params.m_DataIdent0 = EHPMStatisticsScope_NoStatisticsScope;
		params.m_DataIdent1 = 0;
		bool error;
		HPMDataHistory historyTask;
		historyTask = sdkSession->DataHistoryGetHistory(params, error);
		while (error){
			historyTask = sdkSession->DataHistoryGetHistory(params, error);
		}
		return historyTask;
	}

	/**
	* Write results to excel
	*
	*
	*/
	void writeResultsToExcel(){
		OutputLogger::logToConsole("Writing excel file");



		workbook wb;
		worksheet* sh = wb.sheet(xlsSheetName);

		sh->defaultColwidth(xlsColWidth);

		int startingRow = xlsStartingRow;

		sh->label(0, xlsResourceColumnNumber, xlsResourceColumnHeader);
		sh->label(0, xlsProjectNameColumnNumber, xlsProjectNameColumnHeader);
		sh->label(0, xlsTaskIdColumnNumber, xlsTaskIdColumnHeader);
		sh->label(0, xlsTaskContainerDescriptionColumnNumber, xlsTaskContainerDescriptionColumnHeader);
		sh->label(0, xlsTaskDescriptionColumnNumber, xlsTaskDescriptionColumnHeader);
		sh->label(0, xlsWorkDoneIncrementColumnNumber, xlsWorkDoneIncrementColumnHeader);
		sh->label(0, xlsTimeModificationColumnNumber, xlsTimeModificationColumnHeader);
		sh->label(0, xlsJobIdColumnNumber, xlsJobIdColumnHeader);
		for (container::flat_map<string, container::flat_map<unsigned long long, vector<WorkDoneUpdatedTask>>>::iterator it = mapTask.begin(); it != mapTask.end(); ++it)
		{
			container::flat_map<unsigned long long, vector<WorkDoneUpdatedTask>> mapForDateUser = it->second;
			startingRow = startingRow + xlsRowInterval;
			checkForMissingDays(mapForDateUser,it->first);
			//check sulla data mapForDateUser chiave giorno valore vettore task
			bool firstTimeForUser = true;
			for (container::flat_map<unsigned long long, vector<WorkDoneUpdatedTask>>::iterator it2 = mapForDateUser.begin(); it2 != mapForDateUser.end(); ++it2){
				double counterDailyWork = 0;

				vector<WorkDoneUpdatedTask> resourceList = it2->second;
				startingRow = !firstTimeForUser ? startingRow + xlsSecondRowInterval : startingRow;
				firstTimeForUser = false;
				format_t *myFormat = wb.format("#0.00");
				xf_t *x_myFormat = wb.xformat(myFormat);


				for (int l = 0; l < resourceList.size(); ++l)
				{
					WorkDoneUpdatedTask task = resourceList[l];
					sh->label(startingRow, xlsResourceColumnNumber, it->first);
					sh->label(startingRow, xlsProjectNameColumnNumber, task.m_projectName);
					sh->number(startingRow, xlsTaskIdColumnNumber, task.m_taskId);
					cell_t *cellHyper = sh->label(startingRow, xlsTaskDescriptionColumnNumber, task.m_taskDescription);
					sh->hyperLink(cellHyper, task.m_taskHyperlink);
					sh->number(startingRow, xlsWorkDoneIncrementColumnNumber, task.m_workDoneIncrement, x_myFormat);
					string completeDate = DateTimeHelper::getDateStringWithHoursFromUnixTimeStampMicroSeconds(task.m_time);
					sh->label(startingRow, xlsTimeModificationColumnNumber, completeDate.substr(0, 11));
					sh->label(startingRow, xlsHourModificationColumnNumber, completeDate.substr(12));
					cell_t *cellHyperCont = sh->label(startingRow, xlsTaskContainerDescriptionColumnNumber, task.m_taskContainerDescription);
					sh->hyperLink(cellHyperCont, task.m_taskContainerHyperlink);
					sh->label(startingRow, xlsJobIdColumnNumber, task.m_jobId);
					++startingRow;
					counterDailyWork += task.m_workDoneIncrement;
				}
				sh->label(startingRow, xlsResourceColumnNumber, it->first);
				string dateString = DateTimeHelper::getDateStringFromUnixTimeStampMicroSeconds((it2->first * 1000000ull));
				string dailyHourLabel = xlsDailyWorkColumnLabel;
				cell_t *cellDailyLabel = sh->label(startingRow, xlsDailyWorkColumnNumberLabel, dailyHourLabel);
				cell_t *cellDaily = sh->number(startingRow, xlsDailyWorkColumnNumber, counterDailyWork, x_myFormat);
				sh->label(startingRow, xlsTimeModificationColumnNumber, dateString);
				if (counterDailyWork < xlsMinHours){
					cellDaily->fillstyle(FILL_SOLID);
					cellDailyLabel->fillstyle(FILL_SOLID);
					cellDaily->fillfgcolor(CLR_RED);
					cellDailyLabel->fillfgcolor(CLR_RED);
				}
				++startingRow;
			}
		}
		string filenamePrefix = xlsFileNamePrefix;
		string fromDateMod = ConfigHelper::getParamStringForSection(reportSection, reportFromTime);
		boost::erase_all(fromDateMod, "/");
		string toDateMod = ConfigHelper::getParamStringForSection(reportSection, reportToTime);
		boost::erase_all(toDateMod, "/");
		string finalFilename = filenamePrefix + fromDateMod + "-" + toDateMod + xlsFileNameExtension;
		wb.Dump(finalFilename);
		OutputLogger::logToConsole("File saved: " + finalFilename);
	};

	void checkForMissingDays(container::flat_map<unsigned long long, vector<WorkDoneUpdatedTask>> &mapForDateUser, string nomeRisorsa)
	{
		if (mapActiveResource.find(nomeRisorsa) != mapActiveResource.end())
		{
			string fromDateConf = ConfigHelper::getParamStringForSection(reportSection, reportFromTime);
			unsigned long long startTimeStamp = DateTimeHelper::getUnixTimeFromString(fromDateConf);
			string toDateConf = ConfigHelper::getParamStringForSection(reportSection, reportToTime);
			unsigned long long endTimeStamp = DateTimeHelper::getUnixTimeFromString(toDateConf);
			unsigned long long currentTimestamp = startTimeStamp;
			while (currentTimestamp <= endTimeStamp)
			{
				if (!(DateTimeHelper::getIsSaturdayOrSunday(currentTimestamp)))
				{
					if ((mapForDateUser.find(currentTimestamp) == mapForDateUser.end()))
					{
						vector<WorkDoneUpdatedTask> emptyVectorUpdate;
						mapForDateUser[currentTimestamp] = emptyVectorUpdate;
					}
				}
				
				currentTimestamp += 86400ull;
			}
		}


	}

	/**
	* Parse history entries for task
	* @param HPMUniqueId taskId
	* @param HPMUniqueId hash for Work done column
	* @param HPMUniqueId projectId
	* @param HPMProjectProperties project properties
	*
	*/
	void parseHistoryForTask(HPMUniqueID taskId, HPMUniqueID customColumnDataHash, HPMUniqueID projectId, HPMProjectProperties prop,
		string taskContainerDescription, string taskContainerHyperlink, string customColumnJobId){
		string fromDate = ConfigHelper::getParamStringForSection(reportSection, reportFromTime);
		string toDate = ConfigHelper::getParamStringForSection(reportSection, reportToTime);
		HPMDataHistory history = getHistoryForTask(taskId);
		HPMString taskDescriptionW = sdkSession->TaskGetDescription(taskId);
		string taskDescription = string(taskDescriptionW.begin(), taskDescriptionW.end());
		int clientId = sdkSession->TaskGetID(taskId);
		std::vector<HPMDataHistoryEntry> sortedEntries = history.m_HistoryEntries;
		double incrementHistory = 0;
		for (HPMUInt32 m = 0; m < sortedEntries.size(); ++m)
		{
			HPMDataHistoryEntry entry = sortedEntries[m];
			if (entry.m_FieldData == customColumnDataHash){
				HPMVariantData dataForColumn = sdkSession->DataHistoryGetEntryData(history, m);
				if ((mapTask.find(string(entry.m_Resource.begin(), entry.m_Resource.end())) == mapTask.end())){
					container::flat_map<unsigned long long, vector<WorkDoneUpdatedTask>> emptyTaskMap;
					mapTask[string(entry.m_Resource.begin(), entry.m_Resource.end())] = emptyTaskMap;

				}
				double workDone = sdkSession->VariantDecode_HPMFP32(dataForColumn);
				workDone = round(workDone * 100) / 100.00;
				container::flat_map<unsigned long long, vector<WorkDoneUpdatedTask>> dateMapForUser = mapTask[string(entry.m_Resource.begin(), entry.m_Resource.end())];
				gregorian::date dateForUpdate = DateTimeHelper::getDateFromUnixTimeStampMicroSeconds(entry.m_Time);
				string dateForUpdateString = DateTimeHelper::getDateStringFromUnixTimeStampMicroSeconds(entry.m_Time);
				unsigned long long uxTimestamp = DateTimeHelper::getUnixTimeFromString(dateForUpdateString);
				if (dateMapForUser.find(uxTimestamp) == dateMapForUser.end()){
					vector<WorkDoneUpdatedTask> emptyVectorUpdate;
					dateMapForUser[uxTimestamp] = emptyVectorUpdate;
				}
				vector<WorkDoneUpdatedTask> vectorUpdate = dateMapForUser[uxTimestamp];
				double increment = incrementHistory == 0 ? workDone : workDone - incrementHistory;
				increment = round(increment * 100) / 100.00;
				if ((entry.m_Time >= (DateTimeHelper::getUnixTimeFromString(fromDate) * 1000000ull)) && (entry.m_Time <= (DateTimeHelper::getUnixTimeFromString(toDate) * 1000000ull) + (86400ull * 1000000))){
					WorkDoneUpdatedTask task;
					task.m_taskId = clientId;
					task.m_taskDescription = taskDescription;
					task.m_workDoneIncrement = increment;
					task.m_workDone = workDone;
					task.m_projectId = projectId;
					task.m_projectName = string(prop.m_Name.begin(), prop.m_Name.end());
					task.m_time = entry.m_Time;
					task.m_dateUpdate = dateForUpdate;
					string taskHyperlinkFinal;
					taskHyperlinkFinal = HansoftLinkHelper::getHansoftTaskLink(taskId, ConfigHelper::getParamStringForSection(loginSection, hansoftAddress), ConfigHelper::getParamStringForSection(loginSection, hansoftDatabase));
					task.m_taskHyperlink = taskHyperlinkFinal;
					task.m_taskContainerDescription = taskContainerDescription;
					task.m_taskContainerHyperlink = taskContainerHyperlink;
					task.m_jobId = customColumnJobId;
					vectorUpdate.push_back(task);
					dateMapForUser[uxTimestamp] = vectorUpdate;
					mapTask[string(entry.m_Resource.begin(), entry.m_Resource.end())] = dateMapForUser;
				}
				incrementHistory = workDone;
			}

		}
	};

};

#ifdef _MSC_VER
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, const char * argv[])
#endif
{
	HansoftSDK_ResourceAllocation resourceAllocation;

	return resourceAllocation.Run();
}

