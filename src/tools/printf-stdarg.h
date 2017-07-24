/*
 * printf-stdarg.h
 *
 *  Created on: 2017年7月21日
 *      Author: Cotyledon
 */

#ifndef TOOLS_PRINTF_STDARG_H_
#define TOOLS_PRINTF_STDARG_H_

extern int debug_printf(const char *format, ...);
extern int debug_sprintf(char *out, const char *format, ...);
extern int debug_snprintf( char *buf, unsigned int count, const char *format, ... );

#endif /* TOOLS_PRINTF_STDARG_H_ */
