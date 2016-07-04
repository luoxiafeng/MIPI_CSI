/* **************************************************************************** 
 * ** 
 * ** Copyright (c) 2014~2112 ShangHai Infotm Ltd all rights reserved. 
 * ** 
 * ** Use of Infotm's code is governed by terms and conditions 
 * ** stated in the accompanying licensing statement. 
 * ** 
 * ** Soc FPGA verify: csi_core.c :offer core csi operation
 * **
 * ** Revision History: 
 * ** ----------------- 
 * ** v0.0.1	sam.zhou@2014/06/23: first version.
 * **
 * *****************************************************************************/
#include <camera/csi/csi_core.h>
#include <camera/csi/csi.h>
#include <camera/csi/csi_reg.h>
#include <camera/csi/csi_dma.h>
#include <irq.h>
#include <malloc.h>

#define CSI_TST_CODE (0x44)
#define dump_dat ("csi/csi_dump_data.dat")

extern volatile int ret_flag;
static int csi_fd = 0;
extern csi_dma_config_t gdcfg;

//这就是一个中断处理函数，而且这个函数仅仅是为了测试中断是否能产生
int csi_irq1_handler(int irq, void *args)
{
	//csi irq handler
	printf("in error1 interrupt . error status 0x%x\n", csi_core_read_reg(CSI2_ERR1_REG));
	return 0;
}
//这就是一个中断处理函数，而且这个函数仅仅是为了测试中断是否能产生
int csi_irq2_handler(int irq, void *args)
{
	//csi irq2 handler
	//printf("in error interrupt 2\n");
	printf("in error2 interrupt . error status 0x%x\n", csi_core_read_reg(CSI2_ERR2_REG));	
	return 0;
}
//相比较而言，这个中断处理函数就比较有意义。
int csi_irq0_handler(int irq, void *args)
{
	//csi irq0 for dma transfer data
	uint32_t status = 0;
	static	uint32_t nframe = 0;
	//get dma interrupt status
	status = csi_read(&g_host, CSIDMAREG_INTSTAT);
	//one frame dma transfer done
	if (status & (1 << CDRINT_BLOCKEND))
	{
		printf("csi in irq0 handler status =%x\n", status);

		nframe++;
		if(4 == nframe) {
			write(csi_fd, (void *)g_host.dma_buf_saddr, gdcfg.dma[0].length);
			csi_dma_stop();
			csi_core_close();
			close(csi_fd);
			printf("Get picture frame finished in interrupt\n");
			ret_flag = 1;
		}
	}
	//clear interrupt
	csi_dma_clean_intr();
	return 0;
}

uint32_t csi_core_get_version(void)
{
	return csi2_get_version();	
} 

uint32_t csi_core_get_state(void)
{
	return csi2_phy_get_state();
}

error_t csi_core_set_reg(uint32_t addr, uint32_t value)
{
	return csi2_set_reg(addr, value);
}

uint32_t csi_core_read_reg(uint32_t addr)
{
	return csi2_get_reg(addr);
}

error_t csi_core_set_freq(uint32_t freq)
{
	if((freq >= 80) && (freq <= 90)) {
        csi2_phy_testcode(CSI_TST_CODE, 0x00);   //Range: 80-90MHz, default bit rate(90Mbps)
	}
	else if((freq > 90) && (freq <= 100)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x20);   //Range: 90-100MHz
	}
	else if((freq > 100) && (freq <= 110)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x40);   //Range: 100-110MHz
	}
    else if((freq > 110) && (freq <= 125)) {
        csi2_phy_testcode(CSI_TST_CODE, 0x02);   //Range: 110-125MHz
    }
    else if((freq > 125) && (freq <= 140)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x22);   //Range: 125-140MHz
	}
	else if((freq > 140) && (freq <= 150)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x42);   //Range: 140-150MHz
	}
	else if((freq > 150) && (freq <= 160)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x04);   //Range: 150-160MHz
	}
	else if((freq > 160) && (freq <= 180)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x24);   //Range: 160-180MHz
	}
	else if((freq > 180) && (freq <= 200)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x44);   //Range: 180-200MHz
	}
	else if((freq > 200) && (freq <= 210)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x06);   //Range: 200-210MHz
	}
    else if((freq > 210) && (freq <= 240)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x26);   //Range: 210-240MHz
	}
	else if((freq > 240) && (freq <= 250)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x46);   //Range: 240-250MHz
	}
	else if((freq > 250) && (freq <= 270)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x08/* 0x48*/);   //Range: 250-270MHz
	}
	else if((freq > 270) && (freq <= 300)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x28);   //Range: 270-300MHz
	}
	else if((freq > 300) && (freq <= 330)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x08);   //Range: 300-330MHz
	}
	else if((freq > 330) && (freq <= 360)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x2a);   //Range: 330-360MHz
	}
	else if((freq > 360) && (freq <= 400)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x4a);   //Range: 360-400MHz
	}
	else if((freq > 400) && (freq <= 450)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x0c);   //Range: 400-450MHz
	}
	else if((freq > 450) && (freq <= 500)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x2c);   //Range: 450-500MHz
	}
	else if((freq > 500) && (freq <= 550)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x0e);   //Range: 500-550MHz
	}
	else if((freq > 550) && (freq <= 600)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x2e);   //Range: 550-600MHz
	}
	else if((freq > 600) && (freq <= 650)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x10);   //Range: 600-650MHz
	}
	else if((freq > 650) && (freq <= 700)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x30);   //Range: 650-700MHz
	}
	else if((freq > 700) && (freq <= 750))
	{
		csi2_phy_testcode(CSI_TST_CODE, 0x12);   //Range: 700-750MHz
	}
	else if((freq > 750) && (freq <= 800)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x32);   //Range: 750-800MHz
	}
	else if((freq > 800) && (freq <= 850))
	{
		csi2_phy_testcode(CSI_TST_CODE, 0x14);   //Range: 800-850MHz
	}
	else if((freq > 850) && (freq <= 900)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x34);   //Range: 850-900MHz
	}
	else if((freq > 900) && (freq <= 950)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x54);   //Range: 900-950MHz
	}
	else if((freq > 950) && (freq <= 1000)) {
		csi2_phy_testcode(CSI_TST_CODE, 0x74);   //Range: 950-1000MHz
	}
	else {
		CSI_ERR("this frequency not support!\n");
		return EVALUE;
	}	
	return SUCCESS;
}
//initialize csi 
error_t csi_core_init(uint8_t log_level, uint8_t lanes, uint32_t freq)
{

	//prob device 	
	csi2_probe();
	g_host.log_level = log_level;
	g_host.lanes = lanes;
	g_host.freq = freq;
	//initialize log level
	set_log_verbase(g_host.log_level);

	//register intrrupte handle
	request_irq(CSI_IRQ1, csi_irq1_handler, "csi_irq1");
	request_irq(CSI_IRQ2, csi_irq2_handler, "csi_irq2");
	request_irq(CSI_IRQ0, csi_irq0_handler, "csi_irq0");
	//mipi module power on
	module_enable("mipi-csi");
	//mipi module reset
	module_reset("mipi-csi");
	int clk = 0;
	clk = module_get_clock("bus1");
	printf("get bus1 clk = %d\n", clk);
	return SUCCESS;
}

error_t csi_iditoipi_open(int width, int height, int type)
{
	error_t ret = SUCCESS;
	csi2_phy_test_clear();
	
	ret = csi2_phy_set_lanes(g_host.lanes);
	if(SUCCESS != ret) {
	
		CSI_ERR("csi set phy lanes failed\n");
		goto lane_err;
	}
	csi_write(&g_host, CSIDMAREG_DATATYPE, type);
	csi_write(&g_host, CSIDMAREG_FRAMESIZE, (height << 16) | (width &0xffff));
#if 1
	csi2_phy_poweron(CSI_ENABLE);
	//reset phy
	ret = csi2_phy_reset();
	if(SUCCESS != ret) {
		CSI_ERR("csi reset phy failed\n");
		goto reset_err;
	}
	
	//reset controller
	ret = csi2_reset_ctrl();
	if(SUCCESS != ret) {
		CSI_ERR("csi reset controller failed\n");
		goto reset_err;
	}

	//power down first for calibration
//	ret = csi2_phy_poweron(CSI_DISABLE);
//	if(SUCCESS != ret) {
//		CSI_ERR("csi power on phy failed\n");
//		goto lane_err;
//	}
#endif
	//test clear
	//csi2_phy_test_clear();	
	csi_write(&g_host, CSI2_DPHY_TST_CRTL0_REG, CSI_DISABLE);

	//set freq
#if !defined(CONFIG_COMPILE_RTL) //for run rtl case ,comment
	ret = csi_core_set_freq(g_host.freq);
	if(SUCCESS != ret) {
		CSI_ERR("csi set phy freq failed\n");
		goto lane_err;

	}
	//calibration
	calibration();
#endif 	
#if 0
	//then power on
	ret = csi2_phy_poweron(CSI_ENABLE);
	if(SUCCESS != ret) {
		CSI_ERR("csi power on phy failed\n");
		goto lane_err;
	}

	//reset phy
	ret = csi2_phy_reset();
	if(SUCCESS != ret) {
		CSI_ERR("csi reset phy failed\n");
		goto reset_err;
	}

	//reset controller
	ret = csi2_reset_ctrl();
	if(SUCCESS != ret) {
		CSI_ERR("csi reset controller failed\n");
		goto reset_err;
	}
#endif
	//enable csi intrrupte: not need now
	//enable_irq(CSI_IRQ0);
	enable_irq(CSI_IRQ1);
	enable_irq(CSI_IRQ2);
	//later should mask error if ok
#if !defined(CONFIG_COMPILE_RTL)	
	csi_write(&g_host, CSI2_DATA_IDS_1_REG, 0x2b2b2b2b);
	csi_write(&g_host, CSI2_DATA_IDS_2_REG, 0x2b2b2b2b);
	csi2_mask_err1(0x00);
	csi2_mask_err2(0x00);
#else
	csi2_mask_err1(0xffffffff);
	csi2_mask_err2(0xffffffff);
#endif	

	return SUCCESS;
reset_err:
	csi2_phy_poweron(CSI_DISABLE);
lane_err:
	module_disable("mipi-csi");
	return ret;

}

error_t csi_core_open(void)
{
	error_t ret = SUCCESS;

	//set mipi controller clock
	//open file to store one frame
	csi_fd = open(dump_dat, O_WRONLY | O_CREAT);
	if(0 > csi_fd) {
		CSI_ERR("open dump.dat file error\n");
		return -1;
	}
	//csi2_set_clock();
	//set phy lane number
	csi2_phy_test_clear();
	
	ret = csi2_phy_set_lanes(g_host.lanes);
	if(SUCCESS != ret) {
	
		CSI_ERR("csi set phy lanes failed\n");
		goto lane_err;
	}
#if 1
	csi2_phy_poweron(CSI_ENABLE);
	//reset phy
	ret = csi2_phy_reset();
	if(SUCCESS != ret) {
		CSI_ERR("csi reset phy failed\n");
		goto reset_err;
	}
	
	//reset controller
	ret = csi2_reset_ctrl();
	if(SUCCESS != ret) {
		CSI_ERR("csi reset controller failed\n");
		goto reset_err;
	}

	//power down first for calibration
//	ret = csi2_phy_poweron(CSI_DISABLE);
//	if(SUCCESS != ret) {
//		CSI_ERR("csi power on phy failed\n");
//		goto lane_err;
//	}
#endif
	//test clear
	//csi2_phy_test_clear();	
	csi_write(&g_host, CSI2_DPHY_TST_CRTL0_REG, CSI_DISABLE);

	//set freq
#if !defined(CONFIG_COMPILE_RTL) //for run rtl case ,comment
	ret = csi_core_set_freq(g_host.freq);
	if(SUCCESS != ret) {
		CSI_ERR("csi set phy freq failed\n");
		goto lane_err;

	}
	//calibration
	calibration();
#endif 	
#if 0
	//then power on
	ret = csi2_phy_poweron(CSI_ENABLE);
	if(SUCCESS != ret) {
		CSI_ERR("csi power on phy failed\n");
		goto lane_err;
	}

	//reset phy
	ret = csi2_phy_reset();
	if(SUCCESS != ret) {
		CSI_ERR("csi reset phy failed\n");
		goto reset_err;
	}

	//reset controller
	ret = csi2_reset_ctrl();
	if(SUCCESS != ret) {
		CSI_ERR("csi reset controller failed\n");
		goto reset_err;
	}
#endif
	//enable csi intrrupte: not need now
	//enable_irq(CSI_IRQ0);
	enable_irq(CSI_IRQ1);
	enable_irq(CSI_IRQ2);
	//later should mask error if ok
#if !defined(CONFIG_COMPILE_RTL)	
	csi_write(&g_host, CSI2_DATA_IDS_1_REG, 0x2b2b2b2b);
	csi_write(&g_host, CSI2_DATA_IDS_2_REG, 0x2b2b2b2b);
	csi2_mask_err1(0x00);
	csi2_mask_err2(0x00);
#else
	csi2_mask_err1(0xffffffff);
	csi2_mask_err2(0xffffffff);
#endif	

	return SUCCESS;
reset_err:
	csi2_phy_poweron(CSI_DISABLE);
lane_err:
	module_disable("mipi-csi");
	return ret;
}

error_t csi_core_init_phy_only(uint8_t log_level, uint8_t lanes, uint32_t freq)
{

	//prob device 	
	csi2_probe();
	g_host.log_level = log_level;
	g_host.lanes = lanes;
	g_host.freq = freq;

	//initialize log level
	set_log_verbase(g_host.log_level);

	//mipi module power on
	module_enable("mipi-csi");
	//mipi module reset
	module_reset("mipi-csi");

	return SUCCESS;
}

#if defined(CONFIG_COMPILE_RTL)

error_t csi_core_open_phy_only(void)
{
	error_t ret = SUCCESS;

	// set mipi controller clock.
	//csi2_set_clock();

	// set phy lane number.
	ret = csi2_phy_set_lanes(g_host.lanes);
	if (SUCCESS != ret) {
		CSI_ERR("csi set phy lanes failed\n");
		goto lane_err;
	}
	//csi2_phy_poweron(CSI_ENABLE);
	//csi2_phy_reset();
	//csi2_reset_ctrl();

	// power down first for calibration.
	ret = csi2_phy_poweron(CSI_DISABLE);
	if (SUCCESS != ret) {
		CSI_ERR("csi power on phy failed\n");
		goto lane_err;
	}

	// test clear.
	csi2_phy_test_clear();	

	// set freq.
#if 0
	//for run rtl case ,comment
	ret = csi_core_set_freq(g_host.freq);
	if (SUCCESS != ret) {
		CSI_ERR("csi set phy freq failed\n");
		goto lane_err;
	}

	// calibration.
	calibration();
		
#endif 	
	// reset phy.
	ret = csi2_phy_reset();
	if (SUCCESS != ret) {
		CSI_ERR("csi reset phy failed\n");
		goto reset_err;
	}

	// then power on.
	ret = csi2_phy_poweron(CSI_ENABLE);
	if (SUCCESS != ret) {
		CSI_ERR("csi power on phy failed\n");
		goto lane_err;
	}

	return SUCCESS;
reset_err:
	csi2_phy_poweron(CSI_DISABLE);
lane_err:
	module_disable("mipi-csi");

	return ret;
}

#endif
//1:on 0:off
error_t csi_core_poweron(uint8_t flag)
{
	csi2_phy_poweron(flag);
	return SUCCESS;
}

error_t csi_core_close(void)
{
	csi2_phy_poweron(CSI_DISABLE);
#if 0	
	module_disable("mipi-csi");
	disable_irq(CSI_IRQ1);
	disable_irq(CSI_IRQ2);
	disable_irq(CSI_IRQ0);
	if(NULL != g_host.dma_buf_saddr) {
		free(g_host.dma_buf_saddr);
		g_host.dma_buf_saddr = NULL;
	}
#endif	
	if(0 != csi_fd) {
		close(csi_fd);
	}
	return SUCCESS;
}

error_t csi_core_set_bypass_decc(uint8_t flag)
{
	return csi2_set_bypass_decc(flag);
}

error_t csi_core_mask_err1(uint32_t mask)
{
	return csi2_mask_err1(mask);
}

error_t csi_core_umask_err1(uint32_t umask)
{
	return csi2_umask_err1(umask);
}

error_t csi_core_mask_err2(uint32_t mask)
{
	return csi2_mask_err2(mask);
}

error_t csi_core_umask_err2(uint32_t umask)
{
	return csi2_umask_err2(umask);
}

//id 0~7
error_t csi_core_set_data_ids(uint8_t id, uint8_t dt, uint8_t vc)
{
	return csi2_set_data_ids(id, dt, vc);
}

