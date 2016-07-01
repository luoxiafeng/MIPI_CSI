/* Copyright (c) 2014~2112 ShangHai Infotm Ltd all rights reserved. 
** 
** Use of Infotm's code is governed by terms and conditions 
** stated in the accompanying licensing statement. 
** 
** FPGA soc verify mipi csi2 log header file     
**
** Revision History: 
** ----------------- 
** v0.0.1	sam.zhou@2014/06/23: first version.
**
*****************************************************************************/
#ifndef  __CSI_LOG_H_
#define  __CSI_LOG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <linux/types.h>
#include <common.h>

enum {
	LOG_ERR = 0,
	LOG_WARN,
	LOG_TRACE,
	LOG_INFO
};
extern uint8_t g_level;
#define VERBASE_LOG (2) //information

void set_log_verbase(uint8_t level);
void printK(uint8_t level, ...);
#ifdef __cplusplus
}
#endif

#endif



