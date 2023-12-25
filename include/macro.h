/****************************************************************************/
/*

DESCRIPTION
    Macros used by CodeWarriorTRK.

COPYRIGHT

    Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc.
    All other product or service names are the property of their respective owners.
    (C) Freescale Semiconductor, Inc. 2000-2009
    All rights reserved.

*/
/****************************************************************************/


#ifndef __MACRO_H__
#define __MACRO_H__


/****************************************************************************/
/*
** The following 4 macros are used to build strings based
** on other macro values.
** val_str() string-izes the value of a macro
** val_paste() pastes the values of 2 macros together
*/
#define macro_str(x)                #x
#define val_str(x)                  macro_str(x)
#define macro_paste(x, y)           x##y
#define val_paste(x, y)             macro_paste(x, y)

/*
** The version_str(major,minor) macro creates a version
** string based on the version major and minor values.
** For example, if major=3 and minor=1, the resulting
** string would be "3.1".
*/
#define version_str(major, minor)   val_str(val_paste(val_paste(major, .), minor))

/*
** The following macros are used to create function name with the defined
** suffix. This feature is especially useful in the following example.
**
** For example, on an evaluation board, if there are two serial chips and CodeWarriorTRK
** is going to use both serial chips, then the UART function names need to be
** different since the UART interfaces for both chip are the same.
*/
#define FUNC_NAME(x)                val_paste(x, SUFFIX)

/*
** Macro FUNC_HEADER creates function name string based on
** the macro argument value, x, y, z and the defined macro SUFFIX.
**
** x : return type
** y : function name
** z : function argument
*/
#define FUNC_HEADER(x, y, z)        x FUNC_NAME(y)(z)

/*
** Macro FUNC_CALL creates function call name string based on
** the macro argument value, x, and the defined macro SUFFIX.
**
** x : function name
*/
#define FUNC_CALL(x)                FUNC_NAME(x)

/*
** Macro IPTOL converts a 4-part IP address to a long value.
** Example: IPTOL(192, 168, 0, 2) yields 0xc0a80002.
*/
#define IPTOL(a, b, c, d)           ((((a) & 0xff) << 24) | (((b) & 0xff) << 16) | (((c) & 0xff) << 8) | ((d) & 0xff))


#endif /* #ifndef __MACRO_H__ */
