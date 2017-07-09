/*=============================================================================================*//**
 @file      logger_screen.h
 @brief     Screen logger.
 @author    Mahdi Razavi............(Razavi.Dev@Gmail.com)
 @version   13920828
 *//*=============================================================================================*/
#ifndef LOGGER_SCREEN_H
#define LOGGER_SCREEN_H
//_________________________________________________________________________________________ Includes
//
#include "logsys.h"
#include "terminal.h"
//__________________________________________________________________________________________ Defines
//
#define MAX_TEXT_ATTRIBS    4   ///< Maximum of number of text attributes
//________________________________________________________________________________________ New Types
//
//! Forgeround/Background color and general attributes of text messages (@ref terminal_text)
typedef unsigned char LoggerScreenTextAttribs[LOG_NO_LEVELS][MAX_TEXT_ATTRIBS];
//--------------------------------------------------------------------------------------------------
//! Keeping necessary information/data for screen logger
typedef struct LoggerScreenInfo
{
//! Text attributes of tags (use current attributes if it is @p NULL)
const LoggerScreenTextAttribs *tag_attribs;
//! Text attributes of messages (use tag attributes if it is @p NULL)
const LoggerScreenTextAttribs *msg_attribs;
const char *tag_prev_str;       ///< String before tag if not be @p NULL (use tag attributes)
const char *tag_next_str;       ///< String after tag if not be @p NULL (use tag attributes)
const char *msg_prev_str;       ///< String before message if not be @p NULL (use msg attributes)
const char *msg_next_str;       ///< String after message if not be @p NULL (use msg attributes)
} LoggerScreenInfo;
//________________________________________________________________________________________ Constants
//
//! Application programming interface for screen logger
extern const LoggerApi LOGGER_SCREEN_API;
//--------------------------------------------------------------------------------------------------
//! Default logger screen inforamtion
extern const LoggerScreenInfo DEFAULT_LOGGER_SCREEN_INFO;
//________________________________________________________________________________________ Variables
//
//___________________________________________________________________________________________ Macros
//
//________________________________________________________________________________________ Functions
//
//__________________________________________________________________________________________________
#endif  // LOGGER_SCREEN_H
