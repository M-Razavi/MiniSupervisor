/*=============================================================================================*//**
 @file      logsys.h
 @brief     Logging system.
 @author    Mahdi Razavi............(Razavi.Dev@Gmail.com)
 @version   13920828
 *//*=============================================================================================*/
#ifndef LOGSYS_H
#define LOGSYS_H
//_________________________________________________________________________________________ Includes
//
#include <linux/stddef.h>   // NULL
//__________________________________________________________________________________________ Defines
//
//________________________________________________________________________________________ New Types
//
//! Logging levels
typedef enum LogLevel
{
    LOG_ERROR=0,            ///< Errors
    LOG_WARNING=1,          ///< Warnings
    LOG_INFO=2,             ///< Informational messages
    LOG_DEBUG=3,            ///< Debug output
    LOG_VERBOSE=4,          ///< For development only!
    LOG_NO_LEVELS=5         ///< Number of levels
    ,
} LogLevel;
/*=============================================================================================*//**
 @brief     Open a logger and return an allocated data structure for future uses.
 @param     [in]    logger_info Special information for the corresponding logger if not be @p NULL
 @return    Allocated data structure for future uses by the corresponding logger (as @p logger_data)
 *//*=============================================================================================*/
typedef void*
(*LoggerOpen)(const void *logger_info);
/*=============================================================================================*//**
 @brief     Destroy the data structure that is allocated by ::LoggerOpen and close the logger.
 @param     [in]    logger_data Data strucure of logger that is returned by ::LoggerOpen
 *//*=============================================================================================*/
typedef void
(*LoggerClose)(void *logger_data);
/*=============================================================================================*//**
 @brief     Log the specified message string.
 @param     [in]    logger_data Data strucure of logger that is returned by ::LoggerOpen
 @param     [in]    time        Log date & time (time!=NULL)
 @param     [in]    level       Logging level (0 <= level < #LOG_NO_LEVELS)
 @param     [in]    tag         Tag string (maybe @p NULL)
 @param     [in]    msg         Message string to be logged (maybe @p NULL)
 *//*=============================================================================================*/
typedef void
(*LoggerLogMsg)(void *logger_data,const char *time,LogLevel level,const char *tag,const char *msg);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Logger Application Programming Interface
typedef struct LoggerApi
{
const char *name;           ///< Name of the corresponding logger
const char *version;        ///< Version of the corresponding logger (separated by dot)
LoggerOpen open;            ///< Open function of the corresponding logger
LoggerClose close;          ///< Close function of the corresponding logger
LoggerLogMsg log_msg;       ///< "Log Message" function of the corresponding logger
} LoggerApi;
//--------------------------------------------------------------------------------------------------
//! Logger node in linked list
typedef struct LoggerNode
{
const LoggerApi *api;       ///< Logger application programming interface
void *data;                 ///< Logger data
LogLevel level;             ///< Logging level
struct LoggerNode *next;    ///< Next logger if not be @p NULL
} LoggerNode;
//--------------------------------------------------------------------------------------------------
//! Logging system data structure
typedef struct LogSys
{
LoggerNode* loggers;        ///< Loggers linked list
} LogSys;
//________________________________________________________________________________________ Constants
//
//________________________________________________________________________________________ Variables
//
//___________________________________________________________________________________________ Macros
//
//________________________________________________________________________________________ Functions
//
/*=============================================================================================*//**
 @brief     Create a logging system and initialize it.
 @return    Pointer to logging system data structure if not be @p NULL
 *//*=============================================================================================*/
LogSys*
logsys_open(void);
/*=============================================================================================*//**
 @brief     Destroy the previously created logging system.
 @param     [in,out]    logsys      Pointer to logging system data structure
 *//*=============================================================================================*/
void
logsys_close(LogSys *logsys);
/*=============================================================================================*//**
 @brief     Insert a logger to the previously created logging system.
 @param     [in,out]    logsys      Pointer to logging system data structure
 @param     [in]        logger_api  Logger application programming interface
 @param     [in]        logger_info Logger information
 @return    Pointer to inserted logger if not be @p NULL
 *//*=============================================================================================*/
LoggerNode*
logsys_insert_logger(LogSys *logsys,const LoggerApi *logger_api,const void *logger_info);
/*=============================================================================================*//**
 @brief     Remove a previously inserted logger from the logging system.
 @param     [in,out]    logsys      Pointer to logging system data structure
 @param     [in]        logger      Logger that is returned by logsys_insert_logger()
 *//*=============================================================================================*/
void
logsys_remove_logger(LogSys *logsys,LoggerNode *logger);
/*=============================================================================================*//**
 @brief     Set logging level of a logger.
 @param     [in,out]    logger      Logger that is returned by logsys_insert_logger()
 @param     [in]        level       Logging level
 *//*=============================================================================================*/
void
logsys_set_log_level(LoggerNode *logger,LogLevel level);
/*=============================================================================================*//**
 @brief     Get logging level of a logger.
 @param     [in]        logger      Logger that is returned by logsys_insert_logger()
 @return    Logging level of the logger
 *//*=============================================================================================*/
LogLevel
logsys_get_log_level(LoggerNode *logger);
/*=============================================================================================*//**
 @brief     Log the specified message string in the logging system.
 @param     [in]        logsys      Pointer to logging system data structure
 @param     [in]        level       Logging level
 @param     [in]        tag         Tag string
 @param     [in]        msg         Message string to be logged
 *//*=============================================================================================*/
void
log_msg(LogSys *logsys,LogLevel level,const char *tag,const char *msg);
//__________________________________________________________________________________________________
#endif  // LOGSYS_H
