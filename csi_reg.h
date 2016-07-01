/* Copyright (c) 2014~2112 ShangHai Infotm Ltd all rights reserved. 
** 
** Use of Infotm's code is governed by terms and conditions 
** stated in the accompanying licensing statement. 
** 
** FPGA soc verify mipi csi2 defination header file     
**
** Revision History: 
** ----------------- 
** v0.0.1	sam.zhou@2014/06/23: first version.
**
*****************************************************************************/
#ifndef  __CSI_REG_H_
#define  __CSI_REG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <asm/io.h> 

//define csi register operation macro
#define csi_write(_host, _offset, _val)  writel((_val), (((_host)->base) + \
(_offset)))
#define csi_read(_host, _offset)  readl((((_host)->base) + (_offset)))

//#define csi register 
#define CSI_REG_BASE			(0x22200000)

#define CSI2_VERSION_REG 		(0x000) //32 bits R version of csi-2 host controller 表示硬件版本
#define CSI2_N_LANES_REG 		(0x004) //2 bits R/W num of active data lanes 表示LANE的个数
#define CSI2_DPHY_SHUTDOWN_REG  (0x008) //1 bit R/W PHY shutdown 这个寄存器用来关闭D-PHY的电源
#define CSI2_DPHYRST_REG        (0x00C) //1 bit R/W DPHY reset 这个寄存器用来对D-PHY进行复位
#define CSI2_RESET_REG			(0x010) //1 bit R/W CSI controller reset 这个寄存器用来对CSI-2控制器进行复位	
#define CSI2_DPHY_STATE_REG		(0x014) //12 bits R PHY state 获取D-PHY的状态寄存器。这是只读寄存器。
#define CSI2_DATA_IDS_1_REG     (0x018) //这个是LINE边界匹配失败时候报错的LINE id标识
//32 bits R/W list of which ids report line boundary error
#define CSI2_DATA_IDS_2_REG		(0x01C) //32 bits R/W 也就是说，哪个LINE的边界匹配发生了错误，其id号就会被报告
#define CSI2_ERR1_REG			(0x020) //29 bits R error state register 1 错误状态寄存器1
#define CSI2_ERR2_REG			(0x024) //24 bits R error state 2          错误状态寄存器2
#define CSI2_MASK1_REG			(0x028) //29 bits R/w mask for error 1   错误屏蔽寄存器1
#define CSI2_MASK2_REG			(0x02C) //24 bits R/W mask for error 2   错误屏蔽寄存器2
#define CSI2_DPHY_TST_CRTL0_REG	(0x030) //2 bits  R/W DPHY test interface control 0  D-PHY的测试控制接口0
#define CSI2_DPHY_TST_CRTL1_REG	(0x034) //17 bits R/W DPHY test interface control 1  D-PHY的测试控制接口1


/*
*下面定义的全部是代码中需要使用的宏定义
*/

#define CSI2_VERSION_ID			(0x3130322a)  //这个是版本号

//#define postion and width
#define CSI2_N_LANES				(0)
#define CSI2_PHY_SHUTDOWN			(0)
#define CSI2_PHY_RSTZ				(0)
#define CSI2_RESET_N				(0)
//PHY state
#define CSI2_PHY_RXULPSESC_0		(0) //lane 0 enter lps
#define CSI2_PHY_RXULPSESC_1		(1)
#define CSI2_PHY_RXULPSESC_2		(2)
#define CSI2_PHY_RXULPSESC_3		(3)
#define CSI2_PHY_STOPSTATEDATA_0	(4)//data laneX in stop mode
#define CSI2_PHY_STOPSTATEDATA_1	(5)
#define CSI2_PHY_STOPSTATEDATA_2	(6)
#define CSI2_PHY_STOPSTATEDATA_3	(7)
#define CSI2_PHY_RXCLKACTIVEHS		(8)
#define CSI2_PHY_RXULPSCLKNOT		(9)      //
#define CSI2_PHY_STOPSTATECLK       (10) //CLK停止
#define CSI2_PHY_BYPASS_2ECC_TST	(11)   //BY-PASS
//data ids 1
#define CSI2_DI0_DT					(0)
#define CSI2_DI0_VC					(6)
#define CSI2_DI1_DT					(8)
#define CSI2_DI1_VC					(14)
#define CSI2_DI2_DT					(16)
#define CSI2_DI2_VC					(22)
#define CSI2_DI3_DT					(24)
#define CSI2_DI3_VC					(30)
//data ids 2
#define CSI2_DI4_DT					(0)
#define CSI2_DI4_VC					(6)
#define CSI2_DI5_DT					(8)
#define CSI2_DI5_VC					(14)
#define CSI2_DI6_DT					(16)
#define CSI2_DI6_VC					(22)
#define CSI2_DI7_DT					(24)
#define CSI2_DI7_VC					(30)

//err1 state register
#define CSI2_PHY_ERRSOTSYNCHNS0		(0)
#define CSI2_PHY_ERRSOTSYNCHNS1		(1)
#define CSI2_PHY_ERRSOTSYNCHNS2		(2)
#define CSI2_PHY_ERRSOTSYNCHNS3		(3)
#define CSI2_ERR_F_BNDRY_MATCH_VC0	(4)
#define CSI2_ERR_F_BNDRY_MATCH_VC1	(5)
#define CSI2_ERR_F_BNDRY_MATCH_VC2	(6)
#define CSI2_ERR_F_BNDRY_MATCH_VC3	(7)
#define CSI2_ERR_F_SEQ_VC0			(8)
#define CSI2_ERR_F_SEQ_VC1			(9)
#define CSI2_ERR_F_SEQ_VC2			(10)
#define CSI2_ERR_F_SEQ_VC3			(11)
#define CSI2_ERR_FRAME_DATA_VC0		(12)
#define CSI2_ERR_FRAME_DATA_VC1		(13)
#define CSI2_ERR_FRAME_DATA_VC2		(14)
#define CSI2_ERR_FRAME_DATA_VC3		(15)
#define CSI2_ERR_L_BNDRY_MATCH_DI0	(16)
#define CSI2_ERR_L_BNDRY_MATCH_DI1	(17)
#define CSI2_ERR_L_BNDRY_MATCH_DI2	(18)
#define CSI2_ERR_L_BNDRY_MATCH_DI3	(19)
#define CSI2_ERR_L_SEQ_DI0			(20)
#define CSI2_ERR_L_SEQ_DI1			(21)
#define CSI2_ERR_L_SEQ_DI2			(22)
#define CSI2_ERR_L_SEQ_DI3			(23)
#define CSI2_VC0_ERR_CRC			(24)
#define CSI2_VC1_ERR_CRC			(25)
#define CSI2_VC2_ERR_CRC			(26)
#define CSI2_VC3_ERR_CRC			(27)
#define CSI2_ERR_ECC_DOUBLE			(28)
//err2
#define CSI2_PHY_ERRESC0 			(0)
#define CSI2_PHY_ERRESC1 			(1)
#define CSI2_PHY_ERRESC2 			(2)
#define CSI2_PHY_ERRESC3 			(3)
#define CSI2_PHY_ERRSOTHS0			(4)
#define CSI2_PHY_ERRSOTHS1			(5)
#define CSI2_PHY_ERRSOTHS2			(6)
#define CSI2_PHY_ERRSOTHS3			(7)
#define CSI2_VC0_ERR_ECC_CORRECTED	(8)
#define CSI2_VC1_ERR_ECC_CORRECTED	(9)
#define CSI2_VC2_ERR_ECC_CORRECTED	(10)
#define CSI2_VC3_ERR_ECC_CORRECTED	(11)
#define CSI2_ERR_ID_VC0				(12)
#define CSI2_ERR_ID_VC1				(13)
#define CSI2_ERR_ID_VC2				(14)
#define CSI2_ERR_ID_VC3				(15)
#define CSI2_ERR_L_BNDRY_MATCH_DI4	(16)
#define CSI2_ERR_L_BNDRY_MATCH_DI5	(17)
#define CSI2_ERR_L_BNDRY_MATCH_DI6	(18)
#define CSI2_ERR_L_BNDRY_MATCH_DI7	(19)
#define CSI2_ERR_L_SEQ_DI4			(20)
#define CSI2_ERR_L_SEQ_DI5			(21)
#define CSI2_ERR_L_SEQ_DI6			(22)
#define CSI2_ERR_L_SEQ_DI7			(23)
//mask1
#define CSI2_MASK_PHY_ERRSOTSYNCHNS0	(0)
#define CSI2_MASK_PHY_ERRSOTSYNCHNS1	(1)
#define CSI2_MASK_PHY_ERRSOTSYNCHNS2	(2)
#define CSI2_MASK_PHY_ERRSOTSYNCHNS3	(3)
#define CSI2_MASK_ERR_F_BNDRY_MATCH_VC0	(4)
#define CSI2_MASK_ERR_F_BNDRY_MATCH_VC1	(5)
#define CSI2_MASK_ERR_F_BNDRY_MATCH_VC2	(6)
#define CSI2_MASK_ERR_F_BNDRY_MATCH_VC3	(7)
#define CSI2_MASK_ERR_F_SEQ_VC0			(8)
#define CSI2_MASK_ERR_F_SEQ_VC1			(9)
#define CSI2_MASK_ERR_F_SEQ_VC2			(10)
#define CSI2_MASK_ERR_F_SEQ_VC3			(11)
#define CSI2_MASK_ERR_FRAME_DATA_VC0	(12)
#define CSI2_MASK_ERR_FRAME_DATA_VC1	(13)
#define CSI2_MASK_ERR_FRAME_DATA_VC2	(14)
#define CSI2_MASK_ERR_FRAME_DATA_VC3	(15)
#define CSI2_MASK_ERR_L_BNDRY_MATCH_DI0	(16)
#define CSI2_MASK_ERR_L_BNDRY_MATCH_DI1	(17)
#define CSI2_MASK_ERR_L_BNDRY_MATCH_DI2	(18)
#define CSI2_MASK_ERR_L_BNDRY_MATCH_DI3	(19)
#define CSI2_MASK_ERR_L_SEQ_DI0			(20)
#define CSI2_MASK_ERR_L_SEQ_DI1			(21)
#define CSI2_MASK_ERR_L_SEQ_DI2			(22)
#define CSI2_MASK_ERR_L_SEQ_DI3			(23)
#define CSI2_MASK_VC0_ERR_CRC			(24)
#define CSI2_MASK_VC1_ERR_CRC			(25)
#define CSI2_MASK_VC2_ERR_CRC			(26)
#define CSI2_MASK_VC3_ERR_CRC			(27)
#define CSI2_MASK_ERR_ECC_DOUBLE		(28)
//mask2
#define CSI2_MASK_PHY_ERRESC0 			(0)
#define CSI2_MASK_PHY_ERRESC1 			(1)
#define CSI2_MASK_PHY_ERRESC2 			(2)
#define CSI2_MASK_PHY_ERRESC3 			(3)
#define CSI2_MASK_PHY_ERRSOTHS0			(4)
#define CSI2_MASK_PHY_ERRSOTHS1			(5)
#define CSI2_MASK_PHY_ERRSOTHS2			(6)
#define CSI2_MASK_PHY_ERRSOTHS3			(7)
#define CSI2_MASK_VC0_ERR_ECC_CORRECTED	(8)
#define CSI2_MASK_VC1_ERR_ECC_CORRECTED	(9)
#define CSI2_MASK_VC2_ERR_ECC_CORRECTED	(10)
#define CSI2_MASK_VC3_ERR_ECC_CORRECTED	(11)
#define CSI2_MASK_ERR_ID_VC0			(12)
#define CSI2_MASK_ERR_ID_VC1			(13)
#define CSI2_MASK_ERR_ID_VC2			(14)
#define CSI2_MASK_ERR_ID_VC3			(15)
#define CSI2_MASK_ERR_L_BNDRY_MATCH_DI4	(16)
#define CSI2_MASK_ERR_L_BNDRY_MATCH_DI5	(17)
#define CSI2_MASK_ERR_L_BNDRY_MATCH_DI6	(18)
#define CSI2_MASK_ERR_L_BNDRY_MATCH_DI7	(19)
#define CSI2_MASK_ERR_L_SEQ_DI4			(20)
#define CSI2_MASK_ERR_L_SEQ_DI5			(21)
#define CSI2_MASK_ERR_L_SEQ_DI6			(22)
#define CSI2_MASK_ERR_L_SEQ_DI7			(23)

//phy test crtl
#define CSI2_PHY_TEST_CTRL 				(0)
#define CSI2_PHY_TEST_CLK				(1)
#define CSI2_PHY_TESTDIN				(0)
#define CSI2_PHY_TESTDOUT				(8)
#define CSI2_PHY_TESTEN					(16)

//systerm manager 
#define IMAPX_SYSMGR_BASE		(0x21E00000)
#define SYSMGR_MIPI_BASE		(IMAPX_SYSMGR_BASE + 0x24800)
#define SYSMGR_CLK_BASE			(IMAPX_SYSMGR_BASE + 0x0a000)

#define CSI_ENABLE     (1)
#define CSI_DISABLE    (0)

#define SYS_SOFT_RESET          	 (0x00)     /*  soft reset register */
#define SYS_CLOCK_GATE_EN	         (0x04)     /*  clock gated enable register */
#define SYS_POWER_CONTROL	         (0x08)     /*  power control register */
#define SYS_BUS_MANAGER		         (0x0c)     /*  bus manager register */
#define SYS_BUS_QOS_MANAGER0	     (0x10)     /*  bus QOS manager register */
#define SYS_BUS_QOS_MANAGER1   	   	 (0x14)     /*  bus QOS manager register */
#define SYS_BUS_ISOLATION_R       	 (0x18)     /*  bus signal isolation */

/*  power control */
#define MODULE_POWERON            0x11
#define MODULE_ISO_CLOSE          0x1
#define MODULE_POWERON_ACK        1
#define MODULE_POWERDOWN          0x10
#define MODULE_BUS_ENABLE         0x3

#ifdef __cplusplus
}
#endif

#endif



