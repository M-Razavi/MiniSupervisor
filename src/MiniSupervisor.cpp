/*
 * File:   MiniSupervisor.cpp
 * Author: Mahdi Razavi............(Razavi.Dev@Gmail.com)
 *
 * Created on: 	Nov 9, 2013
 * Edit Date :	
 * 				Nov 9, 2013
 *
 * It reads all folders in /proc
 * if the foldername is numeric, it cats cmdline to get the process name
 * the numeric foldername is the PID
 * if the process name coressponds with the searched process name (strstr or strcmp, case sensitive or not), it returns the pid
 * if it doesn't find the process, it returns -1, if it cannot access /proc it returns -2; if several processes with the same name run, it returns the one with the lower number
 *
 */

#ifndef __cplusplus
#define _GNU_SOURCE
#endif

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h> // for opendir(), readdir(), closedir()
#include <sys/stat.h> // for stat()

#ifdef __cplusplus
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#endif

#include "logger_screen.h"
#include "logger_txtfile.h"


#define PROC_DIRECTORY "/proc/"
#define CASE_SENSITIVE    1
#define CASE_INSENSITIVE  0
#define EXACT_MATCH       1
#define INEXACT_MATCH     0

using namespace std;


const char *LOG_LEVEL_STR[LOG_NO_LEVELS]=
{
		"ERROR",
		"WARNING",
		"INFO",
		"DEBUG",
		"VERBOSE",
};

/*=============================================================================================*//**
 @brief     Insert screen logger to the logging system.
 @param     [in,out]    logsys      Pointer to logging system data structure
 @param     [in]        level       Logging level
 *//*=============================================================================================*/
static void
add_logger_screen(LogSys *logsys,LogLevel level)
{
	LoggerNode *logger;

	logger=logsys_insert_logger(logsys,&LOGGER_SCREEN_API,&DEFAULT_LOGGER_SCREEN_INFO);
	logsys_set_log_level(logger,level);

	printf("%s v %s initialized. Maximum log level is %s.\n",
	       LOGGER_SCREEN_API.name,
	       LOGGER_SCREEN_API.version,
	       LOG_LEVEL_STR[level]);
}
/*=============================================================================================*//**
 @brief     Insert text file logger to the logging system.
 @param     [in,out]    logsys      Pointer to logging system data structure
 @param     [in]        level       Logging level
 *//*=============================================================================================*/
static void
add_logger_txtfile(LogSys *logsys,LogLevel level)
{
	LoggerTxtfileInfo info;
	LoggerNode *logger;

	info.filename="mylog.txt";

	logger=logsys_insert_logger(logsys,&LOGGER_TXTFILE_API,&info);
	logsys_set_log_level(logger,level);

	printf("%s v %s initialized. Maximum log level is %s.\n",
	       LOGGER_TXTFILE_API.name,
	       LOGGER_TXTFILE_API.version,
	       LOG_LEVEL_STR[level]);
}
/*=============================================================================================*//**
 @brief     Log some messages into the logging system.
 @param     [in]        logsys      Pointer to logging system data structure
 *//*=============================================================================================*/
static void
log_messages(LogSys *logsys)
{
	log_msg(logsys,LOG_ERROR,"TAG #1","Error #1");
	log_msg(logsys,LOG_WARNING,"TAG #1","Warning #1");
	log_msg(logsys,LOG_INFO,"TAG #1","Info #1");
	log_msg(logsys,LOG_DEBUG,"TAG #1","Debug #1");
	log_msg(logsys,LOG_VERBOSE,"TAG #1","Verbose #1");
}

/**
 * Get Current Date and Time in format yyyy-mm-dd hh:mm
 * @return
 */
string GetCurrentDateTime()
{
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);

	int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
	int year = aTime->tm_year + 1900; // Year is # years since 1900

	stringstream tmpDateTime;
	tmpDateTime << year << "-" << month << "-" << day << " " << aTime->tm_hour << ":" << aTime->tm_min << ":" << aTime->tm_sec;
	return tmpDateTime.str();
}

/**
 * Execute a system command in OS shell.
 * @param cmd
 * @return
 */
int systemExec( const char* cmd)
{
	int cmdResult = system( cmd);
	//writeDebugLog( cmd, cmdResult, __func__, __LINE__);
	return cmdResult;
}

/**
 * Convert string to number
 * Usage: StringToNumber<Type> ( String );
 * @param Text
 * @return
 */
template< typename T>
T StringToNumber( const string &Text)
{
	istringstream ss( Text);
	T result;
	return ss >> result ? result : 0;
}

/**
 * trim from start
 * @param s
 * @return
 */
static inline std::string &ltrim( std::string &s)
{
	s.erase(
			s.begin(),
			std::find_if(
					s.begin(), s.end(),
					std::not1( std::ptr_fun< int, int>( std::isspace))));
	return s;
}

/**
 * trim from end
 * @param s
 * @return
 */
static inline std::string &rtrim( std::string &s)
{
	s.erase(
			std::find_if( s.rbegin(), s.rend(),
			              std::not1( std::ptr_fun< int, int>( std::isspace))).base(),
			              s.end());
	return s;
}

/**
 *  trim from both ends
 * @param s
 * @return
 */
static inline std::string &trim( std::string &s)
{
	return ltrim( rtrim( s));
}

/**
 * Search for last occurrence of chr character in str string
 * return index of founded character
 * if not found return -1.
 * @param str
 * @param chr
 * @return
 */
int LastIndexOf(const char* str, char chr)
{
	char * pch;
	pch=strrchr(str,chr);
	if (pch != NULL)
	{
		return (pch-str+1);
	}
	else
	{
		return -1;
	}
}

/**
 * Check input is numeric value
 * @param ccharptr_CharacterList
 * @return
 */
int IsNumeric(const char* ccharptr_CharacterList)
{
	for ( ; *ccharptr_CharacterList; ccharptr_CharacterList++)
		if (*ccharptr_CharacterList < '0' || *ccharptr_CharacterList > '9')
			return 0; // false
	return 1; // true
}


int strcmp_Wrapper(const char *s1, const char *s2, int intCaseSensitive)
{
	if (intCaseSensitive)
		return !strcmp(s1, s2);
	else
		return !strcasecmp(s1, s2);
}

int strstr_Wrapper(const char* haystack, const char* needle, int intCaseSensitive)
{
	if (intCaseSensitive)
		return (int) strstr(haystack, needle);
	else
		return (int) strcasestr(haystack, needle);
}


#ifdef __cplusplus
pid_t GetPIDbyName(const char* cchrptr_ProcessName, int intCaseSensitiveness, int intExactMatch)
#else
pid_t GetPIDbyName_implements(const char* cchrptr_ProcessName, int intCaseSensitiveness, int intExactMatch)
#endif
{
	char chrarry_CommandLinePath[100]  ;
	char chrarry_NameOfProcess[300]  ;
	char* chrptr_StringToCompare = NULL ;
	pid_t pid_ProcessIdentifier = (pid_t) -1 ;
	struct dirent* de_DirEntity = NULL ;
	DIR* dir_proc = NULL ;

	int (*CompareFunction) (const char*, const char*, int) ;

	if (intExactMatch)
		CompareFunction = &strcmp_Wrapper;
	else
		CompareFunction = &strstr_Wrapper;


	dir_proc = opendir(PROC_DIRECTORY) ;
	if (dir_proc == NULL)
	{
		perror("Couldn't open the " PROC_DIRECTORY " directory") ;
		return (pid_t) -2 ;
	}

	// Loop while not NULL
	while ( (de_DirEntity = readdir(dir_proc)) )
	{
		if (de_DirEntity->d_type == DT_DIR)
		{
			if (IsNumeric(de_DirEntity->d_name))
			{
				strcpy(chrarry_CommandLinePath, PROC_DIRECTORY) ;
				strcat(chrarry_CommandLinePath, de_DirEntity->d_name) ;
				strcat(chrarry_CommandLinePath, "/cmdline") ;
				FILE* fd_CmdLineFile = fopen (chrarry_CommandLinePath, "rt") ;  // open the file for reading text
				if (fd_CmdLineFile)
				{
					fscanf(fd_CmdLineFile, "%s", chrarry_NameOfProcess) ; // read from /proc/<NR>/cmdline
					fclose(fd_CmdLineFile);  // close the file prior to exiting the routine

					//cout << "chrarry_NameOfProcess=" << chrarry_NameOfProcess << endl;
					if (strrchr(chrarry_NameOfProcess, '/'))
						chrptr_StringToCompare = strrchr(chrarry_NameOfProcess, '/') +1 ;
					else
						chrptr_StringToCompare = chrarry_NameOfProcess ;

					//printf("Process name: %s\n", chrarry_NameOfProcess);
					//printf("Pure Process name: %s\n", chrptr_StringToCompare );

					if ( CompareFunction(chrptr_StringToCompare, cchrptr_ProcessName, intCaseSensitiveness) )
					{
						pid_ProcessIdentifier = (pid_t) atoi(de_DirEntity->d_name) ;
						closedir(dir_proc) ;
						return pid_ProcessIdentifier ;
					}
				}
			}
		}
	}
	closedir(dir_proc) ;
	return pid_ProcessIdentifier ;
}

#ifdef __cplusplus
pid_t GetPIDbyName(const char* cchrptr_ProcessName)
{
	return GetPIDbyName(cchrptr_ProcessName, CASE_INSENSITIVE, EXACT_MATCH) ;
}
#else
// C cannot overload functions - fixed
pid_t GetPIDbyName_Wrapper(const char* cchrptr_ProcessName, ... )
{
	int intTempArgument ;
	int intInputArguments[2] ;
	// intInputArguments[0] = 0 ;
	// intInputArguments[1] = 0 ;
	memset(intInputArguments, 0, sizeof(intInputArguments) ) ;
	int intInputIndex ;
	va_list argptr;

	va_start( argptr, cchrptr_ProcessName );
	for (intInputIndex = 0;  (intTempArgument = va_arg( argptr, int )) != 15; ++intInputIndex)
	{
		intInputArguments[intInputIndex] = intTempArgument ;
	}
	va_end( argptr );
	return GetPIDbyName_implements(cchrptr_ProcessName, intInputArguments[0], intInputArguments[1]);
}

#define GetPIDbyName(ProcessName,...) GetPIDbyName_Wrapper(ProcessName, ##__VA_ARGS__, (int) 15)

#endif

#ifdef __cplusplus
int GetRunningProcessCountbyName(const char* cchrptr_ProcessName, int intCaseSensitiveness, int intExactMatch)
#else
int GetRunningProcessCountbyName_implements(const char* cchrptr_ProcessName, int intCaseSensitiveness, int intExactMatch)
#endif
{
	char chrarry_CommandLinePath[100]  ;
	char chrarry_NameOfProcess[300]  ;
	char* chrptr_StringToCompare = NULL ;
	//pid_t pid_ProcessIdentifier = (pid_t) -1 ;
	struct dirent* de_DirEntity = NULL ;
	DIR* dir_proc = NULL ;
	int counter = 0;

	int (*CompareFunction) (const char*, const char*, int) ;

	if (intExactMatch)
		CompareFunction = &strcmp_Wrapper;
	else
		CompareFunction = &strstr_Wrapper;


	dir_proc = opendir(PROC_DIRECTORY) ;
	if (dir_proc == NULL)
	{
		perror("Couldn't open the " PROC_DIRECTORY " directory") ;
		return (pid_t) -2 ;
	}


	// Loop while not NULL
	while ( (de_DirEntity = readdir(dir_proc)) )
	{
		if (de_DirEntity->d_type == DT_DIR)
		{
			if (IsNumeric(de_DirEntity->d_name))
			{
				strcpy(chrarry_CommandLinePath, PROC_DIRECTORY) ;
				strcat(chrarry_CommandLinePath, de_DirEntity->d_name) ;
				strcat(chrarry_CommandLinePath, "/cmdline") ;
				FILE* fd_CmdLineFile = fopen (chrarry_CommandLinePath, "rt") ;  // open the file for reading text
				if (fd_CmdLineFile)
				{
					fscanf(fd_CmdLineFile, "%s", chrarry_NameOfProcess) ; // read from /proc/<NR>/cmdline
					fclose(fd_CmdLineFile);  // close the file prior to exiting the routine

					//					cout << "chrarry_NameOfProcess=" << chrarry_NameOfProcess << endl;
					if (strrchr(chrarry_NameOfProcess, '/'))
					{
						chrptr_StringToCompare = strrchr(chrarry_NameOfProcess, '/') +1 ;
					}
					else
					{
						chrptr_StringToCompare = chrarry_NameOfProcess ;
					}

					if ( CompareFunction(chrptr_StringToCompare, cchrptr_ProcessName, intCaseSensitiveness) )
					{
						counter++;
						//						cout << atoi(de_DirEntity->d_name) << endl;
						//closedir(dir_proc) ;
					}
				}
			}
		}
	}
	closedir(dir_proc) ;
	return counter ;
}

#ifdef __cplusplus
int GetRunningProcessCountbyName(const char* cchrptr_ProcessName)
{
	return GetRunningProcessCountbyName(cchrptr_ProcessName, CASE_INSENSITIVE, EXACT_MATCH);
}
#endif



int main(int argc, char *argv[])
{
	//To prevent mistake in some OS in startup
	sleep(2);

	//only one instance of application can run
	{
		string appSelfName = string(argv[0]);
		unsigned found = appSelfName.find_last_of("/\\");
		appSelfName = appSelfName.substr(found+1);

		int selfPid = GetPIDbyName(appSelfName.c_str());
		int processCnt = GetRunningProcessCountbyName(appSelfName.c_str());
		//		cout << " file: " << appSelfName << '\n';
		//		cout << "pid=" << selfPid << endl;
		//		cout << "processCnt=" << processCnt << endl;
		if(processCnt > 1)
		{
			cout << "Another instance of " << argv[0] << " with PID " << selfPid << " is running.\nExit in progress..." << endl;
			exit(-1);
		}
	}

	if(argc < 2)
	{
		cout << "Please Enter Command Line Like This:" << endl;
		cout << "ReRuner.mng -t timeout  InputAppFilesName OutputLogFile" << endl;
		cout << "Or ReRuner.mng -h for help. " << endl;
		exit(0);
	}


	if(!strcmp(argv[1],"-v"))
	{
		cout << "MiniSupervisor Ver 1.0.0 - compile time:" << __DATE__ << " " << __TIME__ << endl;
		return 0;
	}

	if(!strcmp(argv[1],"-h"))
	{
		cout << "Usage like one of this:" << endl;
		cout << "ReRuner.mng -t timeout  InputAppFilesName OutputLogFile\t Check every application in timeout(s) period." << endl;
		cout << "ReRuner.mng -v \t show version" << endl;
		cout << "ReRuner.mng -h \t show help" << endl;
		return 0;
	}

	if(argc < 5)
	{
		cout << "Please Enter Command Line Like This:" << endl;
		cout << "ReRuner.mng -t timeout  InputAppFilesName OutputLogFile" << endl;
		exit(0);
	}

	// Declare Variables
	string line,resultExec;
	char appName[30],appExecName[30];
	size_t colonPosition;
	int timeout;

	// Declare Output & input file
	ifstream inputAppFilesName;
	ofstream outputLogFile;

	timeout = StringToNumber<int>(argv[2]);

	inputAppFilesName.open(argv[3]);
	outputLogFile.open(argv[4], ios::out | ios::app);

	if (outputLogFile.is_open())
	{
		//OutputLogFile.clear();
		outputLogFile << GetCurrentDateTime() << " MiniSupervisor Initialized for " << timeout << " sec timeout." << endl;
	}
	else
	{
		cout << "Unable to open Output file";
	}

	//int lineCnt = 0;
	//Main Loop
	while(true)
	{
		// Check input File Exist Correctly --------------------------------------
		if (inputAppFilesName.is_open())
		{
			// Read Line by Line Input File ---------------------------------------
			while (inputAppFilesName.good())
			{
				getline(inputAppFilesName,line);

				line = trim(line);

				//cout << "line=" << ++lineCnt << "--" << line << endl;

				//if line started with #, assumed as comment
				if(line[0] == '#' || line.length() < 3)
				{
					continue;
				}

				//cout << "processing=" <<  endl;

				// Clear Array for Each Line --------------------------------------
				memset(appName,0,30);
				memset(appExecName,0,30);
				// different member versions of find in the same order as above:
				colonPosition = line.find(":");

				if(colonPosition != string::npos)
				{
					// Extract application name for Passing to GetPIDbyName() function
					line.copy(appName,(colonPosition),0);
					pid_t pid = GetPIDbyName(appName) ; // If -1 = not found, if -2 = proc fs access error

					if(pid < 0)
					{
						// Extract application execution name for Passing to exec() function
						line.copy(appExecName, (line.length() - colonPosition),(colonPosition + 1));

						//cout <<__DATE__<< " " << __TIME__ << " " << appExecName << " started. Execution result= "<< resultExec << "."<< endl;

						//Call system() function for apply bash command in OS --------------
						resultExec = systemExec(appExecName);
						// Write Software Started in Output File -------------------------
						if (outputLogFile.is_open())
						{
							outputLogFile << GetCurrentDateTime() << " " << appExecName << " started. Execution result= "<< resultExec << "."<< endl;
						}
						else
						{
							cout << "Unable to open Output file";
						}
					}
				}
			}

			//cout << "file reading finished.\n";

			//Clear error state flags
			inputAppFilesName.clear();
			inputAppFilesName.seekg(0, ios::beg);

			if(!inputAppFilesName.good())
			{
				cout << " good()=" << inputAppFilesName.good() << endl;
				cout << " eof()=" << inputAppFilesName.eof() << endl;
				cout << " fail()=" << inputAppFilesName.fail() << endl;
				cout << " bad()=" << inputAppFilesName.bad() << endl;
			}
		}
		else
		{
			cout << "Unable to open input file";
			if (outputLogFile.is_open())
			{
				outputLogFile << "Unable to open input file";
			}
			return -1;
		}

		LogSys *logsys;

		//		logsys=logsys_open();
		//		add_logger_screen(logsys,LOG_VERBOSE);
		//		add_logger_txtfile(logsys,LOG_INFO);
		//
		//
		//		log_messages(logsys);
		//		logsys_close(logsys);


		//cout << "File Flush\n";

		//outputLogFile.flush();
		//cout << "go sleep\n";
		sleep(timeout);
		//cout << "exit sleep\n";
	}

	return EXIT_SUCCESS ;
}




