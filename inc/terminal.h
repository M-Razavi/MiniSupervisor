/*=============================================================================================*//**
 @file      terminal.h
 @brief     Terminal control commands.
 @author    Mahdi Razavi............(Razavi.Dev@Gmail.com)
 @version   13920826
 @see       http://en.wikipedia.org/wiki/ANSI_escape_code
 @see       http://wiki.bash-hackers.org/scripting/terminalcodes
 *//*=============================================================================================*/
#ifndef TERMINAL_H
#define TERMINAL_H
//_________________________________________________________________________________________ Includes
//
#include <stdio.h>              // printf
//__________________________________________________________________________________________ Defines
//
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*//**
 @defgroup terminal_text                                                        Text Attributes
 @{ *//*------------------------------------------------------------------------------------------*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*//**
 @defgroup terminal_fg                                    Foreground colors
 @{ *//*----------------------------------------------------------------------*/
#define FG_BLACK            30  ///< Set foreground to color #0 - black
#define FG_RED              31  ///< Set foreground to color #1 - red
#define FG_GREEN            32  ///< Set foreground to color #2 - green
#define FG_YELLOW           33  ///< Set foreground to color #3 - yellow
#define FG_BLUE             34  ///< Set foreground to color #4 - blue
#define FG_MAGENTA          35  ///< Set foreground to color #5 - magenta
#define FG_CYAN             36  ///< Set foreground to color #6 - cyan
#define FG_WHITE            37  ///< Set foreground to color #7 - white
#define FG_DEFAULT          39  ///< Set default color as foreground color
/** @} *//*-------------------------------------------------------------------*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*//**
 @defgroup terminal_bg                                    Background colors
 @{ *//*----------------------------------------------------------------------*/
#define BG_BLACK            40  ///< Set background to color #0 - black
#define BG_RED              41  ///< Set background to color #1 - red
#define BG_GREEN            42  ///< Set background to color #2 - green
#define BG_YELLOW           43  ///< Set background to color #3 - yellow
#define BG_BLUE             44  ///< Set background to color #4 - blue
#define BG_MAGENTA          45  ///< Set background to color #5 - magenta
#define BG_CYAN             46  ///< Set background to color #6 - cyan
#define BG_WHITE            47  ///< Set background to color #7 - white
#define BG_DEFAULT          49  ///< Set default color as background color
/** @} *//*-------------------------------------------------------------------*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*//**
 @defgroup terminal_att                                  General Attributes
 @{ *//*----------------------------------------------------------------------*/
#define ATT_RESET           0   ///< Reset all attributes
#define ATT_BOLD            1   ///< Set "bright" attribute
#define ATT_UNDERLINE       4   ///< Set "underscore" (underlined text) attribute
#define ATT_BLINK           5   ///< Set "blink" attribute
#define ATT_REVERSE         7   ///< Set "reverse" attribute (swap foreground and background)
#define ATT_NO_BOLD         22  ///< Clear "bright" attribute
#define ATT_NO_UNDERLINE    24  ///< Clear "underscore" attribute
#define ATT_NO_BLINK        25  ///< Clear "blink" attribute
#define ATT_NO_REVERSE      27  ///< Clear "reverse" attribute
/** @} *//*-------------------------------------------------------------------*/
/** @} *//*---------------------------------------------------------------------------------------*/
//________________________________________________________________________________________ New Types
//
//________________________________________________________________________________________ Constants
//
//________________________________________________________________________________________ Variables
//
//___________________________________________________________________________________________ Macros
//
/*=============================================================================================*//**
 @brief     Set one of the attributes of the text (@ref terminal_text).
 @param     [in]    code        Text attribute
 *//*=============================================================================================*/
#define SET_TEXT_ATTRIB(code)   printf("\x1B[%dm",code)
/*=============================================================================================*//**
 @brief     Set foreground color.
 @param     [in]    fg          Foreground color
 *//*=============================================================================================*/
#define SET_FGCOLOR(fg)         SET_TEXT_ATTRIB(fg)
/*=============================================================================================*//**
 @brief     Set background color.
 @param     [in]    bg          Background color
 *//*=============================================================================================*/
#define SET_BGCOLOR(bg)         SET_TEXT_ATTRIB(bg)
/*=============================================================================================*//**
 @brief     Set one of the general attributes of the text.
 @param     [in]    att         General attribute
 *//*=============================================================================================*/
#define SET_ATTRIB(att)         SET_TEXT_ATTRIB(att)
//________________________________________________________________________________________ Functions
//
//__________________________________________________________________________________________________
#endif  // TERMINAL_H
