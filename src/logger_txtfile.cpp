/*=============================================================================================*//**
 @file      logger_txtfile.cpp
 @brief     Text file logger.
 @author    Mahdi Razavi............(Razavi.Dev@Gmail.com)
 @version   13920828
 *//*=============================================================================================*/
//_________________________________________________________________________________________ Includes
//
#include <stdio.h>          // FILE,fopen,fclose,fprintf
#include <stdlib.h>         // malloc,free
#include "logger_txtfile.h"
//__________________________________________________________________________________________ Defines
//
//________________________________________________________________________________________ New Types
//
//! Keeping necessary data for text file logger
typedef struct LoggerTxtfileData
{
FILE *txtfile;
} LoggerTxtfileData;
//________________________________________________________________________________________ Constants
//
//! Level identification strings
const char* LoggerTxtfileIDs[LOG_NO_LEVELS]=
        {
                "E",            // Error
                "W",            // Warning
                "I",            // Information
                "D",            // Debug
                "V",            // Verbose
        };
//________________________________________________________________________________________ Variables
//
//___________________________________________________________________________________________ Macros
//
//________________________________________________________________________________________ Functions
//
/*=============================================================================================*//**
 @brief     See ::LoggerOpen
 *//*=============================================================================================*/
static void*
logger_txtfile_open(const void *logger_info)
{
const LoggerTxtfileInfo *info;
LoggerTxtfileData *data;

if(logger_info==NULL)
    return NULL;

info=(const LoggerTxtfileInfo*)logger_info;
if(info->filename==NULL)
    return NULL;

data=(LoggerTxtfileData*)malloc(sizeof(LoggerTxtfileData));
if(data==NULL)
    return NULL;

data->txtfile=fopen(info->filename,"a");
if(data->txtfile==NULL)
    {
    free(data);
    return NULL;
    }

return data;
}
/*=============================================================================================*//**
 @brief     See ::LoggerClose
 *//*=============================================================================================*/
static void
logger_txtfile_close(void *logger_data)
{
LoggerTxtfileData *data;

data=(LoggerTxtfileData*)logger_data;
if(data!=NULL)
    {
    if(data->txtfile!=NULL)
        fclose(data->txtfile);
    free(data);
    }
}
/*=============================================================================================*//**
 @brief     See ::LoggerLogMsg
 *//*=============================================================================================*/
static void
logger_txtfile_log_msg(void *logger_data,const char *time,LogLevel level,const char *tag,
                       const char *msg)
{
LoggerTxtfileData *data;

data=(LoggerTxtfileData*)logger_data;
if(data==NULL)
    return;

if(data->txtfile!=NULL)
    {
    fprintf(data->txtfile,"%s %s: <%s> %s\n",
            time,
            LoggerTxtfileIDs[level],
            (tag!=NULL) ? tag : "",
            (msg!=NULL) ? msg : "");
    }
}
//________________________________________________________________________________________ Constants
//
const LoggerApi LOGGER_TXTFILE_API=
        {
                name:"TxtFile Logger",
                version:"1.0.0",
                open:logger_txtfile_open,
                close:logger_txtfile_close,
                log_msg:logger_txtfile_log_msg,
        };
//__________________________________________________________________________________________________
