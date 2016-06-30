/* **************************************************************************** 
 * ** 
 * ** Copyright (c) 2014~2112 ShangHai Infotm Ltd all rights reserved. 
 * ** 
 * ** Use of Infotm's code is governed by terms and conditions 
 * ** stated in the accompanying licensing statement. 
 * ** 
 * ** Soc FPGA verify: log.c :offer  csi module log operation
 * **
 * ** Revision History: 
 * ** ----------------- 
 * ** v0.0.1	sam.zhou@2014/06/23: first version.
 * **
 * *****************************************************************************/
#include <camera/csi/log.h>
#include <stdarg.h>
#include <common.h>
#define BUF_SIZE (256)
uint8_t g_level = VERBASE_LOG;

void set_log_verbase(uint8_t level)
{
	g_level = level;
}

void printK(uint8_t level, ...) 
{
	if(level <= g_level)
	{	
		char *fmt;
		char printbuffer[CONFIG_SYS_PBSIZE];	
		va_list args;			
		va_start(args, level);
		fmt = va_arg(args, char *);
		vsprintf(printbuffer, fmt, args);
		va_end(args);
		puts(printbuffer);
	}

	return ;
}
