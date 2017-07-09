/*=============================================================================================*//**
 @file      logsys.cpp
 @brief     Logging system.
 @author    Mahdi Razavi............(Razavi.Dev@Gmail.com)
 @version   13920828
 *//*=============================================================================================*/
//_________________________________________________________________________________________ Includes
//
#include <stdlib.h>         // malloc,free
#include <time.h>           // time_t,struct tm,time,localtime,asctime
#include <stdio.h>          // snprintf
#include "logsys.h"
//__________________________________________________________________________________________ Defines
//
//________________________________________________________________________________________ New Types
//
//________________________________________________________________________________________ Constants
//
//________________________________________________________________________________________ Variables
//
//___________________________________________________________________________________________ Macros
//
//________________________________________________________________________________________ Functions
//
/*=============================================================================================*//**
 @brief     Return current date & time as a string.
 @param     [in]        logsys      Pointer to logging system data structure
 @return    Date & time string (should not be @p NULL)
 *//*=============================================================================================*/
static const char*
logsys_get_time_str(LogSys *logsys)
{
static char time_str[20];  // "yyyy/mm/dd hh:mm:ss"
time_t rawtime;
struct tm *timeinfo;

time(&rawtime);
timeinfo=localtime(&rawtime);

snprintf(time_str,sizeof(time_str),"%4d/%02d/%02d %02d:%02d:%02d",timeinfo->tm_year+1900,
        timeinfo->tm_mon+1,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

return time_str;
}

/*=============================================================================================*//**
 *//*=============================================================================================*/
LogSys*
logsys_open(void)
{
LogSys *logsys;

logsys=(LogSys*)malloc(sizeof(LogSys));
if(logsys!=NULL)
    {
    logsys->loggers=NULL;
    }

return logsys;
}
/*=============================================================================================*//**
 *//*=============================================================================================*/
void
logsys_close(LogSys *logsys)
{
LoggerNode *curr_logger;
LoggerNode *prev_logger;

if(logsys!=NULL)
    {
    curr_logger=logsys->loggers;
    while(curr_logger!=NULL)
        {
        curr_logger->api->close(curr_logger->data);
        prev_logger=curr_logger;
        curr_logger=prev_logger->next;
        free(prev_logger);
        }
    free(logsys);
    }
}
/*=============================================================================================*//**
 *//*=============================================================================================*/
LoggerNode*
logsys_insert_logger(LogSys *logsys,const LoggerApi *logger_api,const void *logger_info)
{
LoggerNode *logger;

if((logsys==NULL)||(logger_api==NULL))
    return NULL;

logger=(LoggerNode*)malloc(sizeof(LoggerNode));
if(logger!=NULL)
    {
    logger->api=logger_api;
    logger->data=logger_api->open(logger_info);
   // logger->level=(LogLevel)((int)LOG_NO_LEVELS)-1;
    logger->next=logsys->loggers;
    logsys->loggers=logger;
    }
return logger;
}
/*=============================================================================================*//**
 *//*=============================================================================================*/
void
logsys_remove_logger(LogSys *logsys,LoggerNode *logger)
{
LoggerNode *prev_logger;
LoggerNode *curr_logger;

if((logsys==NULL)||(logger==NULL))
    return;

prev_logger=NULL;
curr_logger=logsys->loggers;
while((curr_logger!=NULL)&&(curr_logger!=logger))
    {
    prev_logger=curr_logger;
    curr_logger=prev_logger->next;
    }
if(curr_logger!=NULL)
    {
    if(prev_logger!=NULL)
        prev_logger->next=curr_logger->next;
    else
        logsys->loggers=curr_logger->next;
    curr_logger->api->close(curr_logger->data);
    free(curr_logger);
    }
}
/*=============================================================================================*//**
 *//*=============================================================================================*/
void
logsys_set_log_level(LoggerNode *logger,LogLevel level)
{
if(level<LOG_NO_LEVELS)
    logger->level=level;
}
/*=============================================================================================*//**
 *//*=============================================================================================*/
LogLevel
logsys_get_log_level(LoggerNode *logger)
{
return logger->level;
}
/*=============================================================================================*//**
 *//*=============================================================================================*/
void
log_msg(LogSys *logsys,LogLevel level,const char *tag,const char *msg)
{
LoggerNode *curr_logger;
const char *time;

if((logsys==NULL)||(level>=LOG_NO_LEVELS))
    return;

time=NULL;
curr_logger=logsys->loggers;
while(curr_logger!=NULL)
    {
    if(level<=curr_logger->level)
        {
        if(time==NULL)
            time=logsys_get_time_str(logsys);
        curr_logger->api->log_msg(curr_logger->data,time,level,tag,msg);
        }
    curr_logger=curr_logger->next;
    }
}
//__________________________________________________________________________________________________
