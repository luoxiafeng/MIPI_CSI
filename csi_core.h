/* Copyright (c) 2014~2112 ShangHai Infotm Ltd all rights reserved. 
** 
** Use of Infotm's code is governed by terms and conditions 
** stated in the accompanying licensing statement. 
** 
** FPGA soc verify mipi csi2 core header file     
**
** Revision History: 
** ----------------- 
** v0.0.1	sam.zhou@2014/06/23: first version.
**
*****************************************************************************/
#ifndef  __CSI_CORE_H_
#define  __CSI_CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <camera/csi/log.h>
#include <linux/types.h>
#include <camera/csi/csi.h> 
typedef enum {
	ENONE     = 0,
	SUCCESS	  = 0,	
	EVALUE    = -1,
	EPNULL    = -2,
	EUNKONE   = -3,

}error_t;

#define CSI_ERR(...)	printK(LOG_ERR,  __VA_ARGS__)
#define CSI_INFO(...)   printK(LOG_INFO, __VA_ARGS__)

#define CSI_IRQ1 	(MIPICSI1_INT_ID)	//(84)
#define CSI_IRQ2	(MIPICSI2_INT_ID)//(85)
#define CSI_IRQ0	(MIPICSI0_INT_ID)//(83)

uint32_t csi_core_get_version(void);
uint32_t csi_core_get_state(void);
uint32_t csi_core_read_reg(uint32_t addr);
error_t csi_core_set_reg(uint32_t addr, uint32_t value);
error_t csi_core_init(uint8_t log_level, uint8_t lanes, uint32_t freq);
error_t csi_core_open(void);
error_t csi_iditoipi_open(int width, int height, int type);
int csi_config_test(int width, int height, int csi_fmt);

error_t csi_core_init_phy_only(uint8_t log_level, uint8_t lanes, uint32_t freq);
#if defined(CONFIG_COMPILE_RTL)
error_t csi_core_open_phy_only(void);
#endif
error_t csi_core_close(void);
error_t csi_core_poweron(uint8_t flag);
error_t csi_core_set_bypass_decc(uint8_t flag);
error_t csi_core_mask_err1(uint32_t mask);
error_t csi_core_umask_err1(uint32_t umask);
error_t csi_core_mask_err2(uint32_t mask);
error_t csi_core_umask_err2(uint32_t umask);
error_t csi_core_set_data_ids(uint8_t id, uint8_t dt, uint8_t vc);
#ifdef __cplusplus
}
#endif

#endif



