/*=============================================================================================*//**
 @file      logger_screen.cpp
 @brief     Screen logger.
 @author    Mahdi Razavi............(Razavi.Dev@Gmail.com)
 @version   13920828
 *//*=============================================================================================*/
//_________________________________________________________________________________________ Includes
//
#include <stdio.h>          // printf
#include "logger_screen.h"
//__________________________________________________________________________________________ Defines
//
//________________________________________________________________________________________ New Types
//
//________________________________________________________________________________________ Constants
//
//! Default text attributes of tags
static const LoggerScreenTextAttribs DEFAULT_TAG_ATTRIBS=
        {
                {FG_RED,ATT_BOLD},                          // Error
                {FG_YELLOW,ATT_BOLD},                       // Warning
                {FG_GREEN,ATT_BOLD},                        // Information
                {FG_WHITE,ATT_BOLD},                        // Debug
                {FG_CYAN,ATT_BOLD},                         // Verbose
        };
//--------------------------------------------------------------------------------------------------
//! Default text attributes of messages
static const LoggerScreenTextAttribs DEFAULT_MSG_ATTRIBS=
        {
                {FG_RED},                                   // Error
                {FG_YELLOW},                                // Warning
                {FG_GREEN},                                 // Information
                {FG_WHITE},                                 // Debug
                {FG_CYAN},                                  // Verbose
        };
//--------------------------------------------------------------------------------------------------
const LoggerScreenInfo DEFAULT_LOGGER_SCREEN_INFO=
        {
                tag_attribs:&DEFAULT_TAG_ATTRIBS,
                msg_attribs:&DEFAULT_MSG_ATTRIBS,
                tag_prev_str:"<",
                tag_next_str:"> ",
                msg_prev_str:NULL,
                msg_next_str:"\n",
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
logger_screen_open(const void *logger_info)
{
return (void*)logger_info;
}
/*=============================================================================================*//**
 @brief     See ::LoggerClose
 *//*=============================================================================================*/
static void
logger_screen_close(void *logger_data)
{
}
/*=============================================================================================*//**
 @brief     Set text attributes for the corresponding log level.
 @param     [in]        attribs     Text attributes
 *//*=============================================================================================*/
void
logger_screen_set_text_attribs(const unsigned char *attribs)
{
int i;

SET_TEXT_ATTRIB(ATT_RESET);

for(i=0;(i<MAX_TEXT_ATTRIBS)&&(attribs[i]!=0);i++)
    SET_TEXT_ATTRIB(attribs[i]);
}
/*=============================================================================================*//**
 @brief     See ::LoggerLogMsg
 @todo      Checking!
 *//*=============================================================================================*/
static void
logger_screen_log_msg(void *logger_data,const char *time,LogLevel level,const char *tag,
                      const char *msg)
{
const LoggerScreenInfo *info;
int attribs_changed;

info=(LoggerScreenInfo*)logger_data;
attribs_changed=0;

if(info->tag_attribs!=NULL)
    {
    logger_screen_set_text_attribs(&((*(info->tag_attribs))[level][0]));
    attribs_changed=1;
    }

printf("%s%s%s",
        (info->tag_prev_str!=NULL) ? info->tag_prev_str : "",
        (tag!=NULL) ? tag : "",
        (info->tag_next_str!=NULL) ? info->tag_next_str : "");

if(info->msg_attribs!=NULL)
    {
    logger_screen_set_text_attribs(&((*(info->msg_attribs))[level][0]));
    attribs_changed=1;
    }

printf("%s%s%s",
       (info->msg_prev_str!=NULL) ? info->msg_prev_str : "",
       (msg!=NULL) ? msg : "",
       (info->msg_next_str!=NULL) ? info->msg_next_str : "");

if(attribs_changed)
    SET_TEXT_ATTRIB(ATT_RESET);
}
//________________________________________________________________________________________ Constants
//
const LoggerApi LOGGER_SCREEN_API=
        {
                name:"Screen Logger",
                version:"1.0.0",
                open:logger_screen_open,
                close:logger_screen_close,
                log_msg:logger_screen_log_msg,
        };
//__________________________________________________________________________________________________
