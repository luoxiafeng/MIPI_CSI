/* **************************************************************************** 
 * ** 
 * ** Copyright (c) 2014~2112 ShangHai Infotm Ltd all rights reserved. 
 * ** 
 * ** Use of Infotm's code is governed by terms and conditions 
 * ** stated in the accompanying licensing statement. 
 * ** 
 * ** Soc FPGA verify: csi_main.c :offer  csi module test items operation
 * **
 * ** Revision History: 
 * ** ----------------- 
 * ** v0.0.1	sam.zhou@2014/06/23: first version.
 * **
 * *****************************************************************************/
#include <common.h>
#include <command.h>
#include <testbench.h>
#include <camera/camif/camif_core.h>
#include <camera/csi/csi_core.h>
#include <iic.h>
#include <camera/sensor/ar0330.h>
#include <camera/sensor/ov2710.h>
#include <camera/isp/isp.h>
#include <irq.h>
#include <camera/csi/csi_dma.h>
#include <isp/V2500/isp_define.h>

#define CSI_LOG_LEVEL (LOG_ERR)

#if defined(CONFIG_COMPILE_FPGA)
#ifdef MIPI_ONE_LANES
#define CSI_LANES     (1)
#else
#define CSI_LANES	 (2)
#endif
#else
#define CSI_LANES     (4)
#endif

#define CSI_CMD_LEN	  (2)
#define CSI_FREQ	  (380) //mipi data rate clock
//#define CSI_USE_OV2710
#ifndef I2C_CHN_IM
#if defined(CONFIG_COMPILE_FPGA)
#define I2C_CHN_IM    (1)
#else
#define I2C_CHN_IM	  (4)
#endif
#endif
//define csi test command
#define HP	("hp")
#define VR	("vs")
#define ST	("st")
#define OP	("op")
#define CL	("cl")
#define MI	("mi")
//#define ME  ("me")
#define SG	("sg")
#define MR	("mr")
#define RW  ("rw")
#define GP	("gp")
#define GP1 ("g1")
#define GP2 ("g2")
#define IP	("ip")
//The test run items
//No command: print csi test case help
#define exp1 ("display all test command")
#define exp2 ("get csi controller version")
#define exp3 ("get phy state")
#define exp4 ("open csi controller")
#define exp5 ("close csi controller")
#define exp6 ("mask csi cpu interrupt")
#define exp7 ("set csi register")
#define exp8 ("mask error")
#define exp9 ("read write csi register")
#define exp10 ("get picture data rgb565")
#define exp11 ("get picture data raw12")
#define exp12 ("get picture data yuv422 8bits")
#define exp13 ("verify idi to ipi link.")

//sg: set register

#define COMMAND_LIST_ENTRY(cmd, exp) {cmd, exp},  
#define COMMAND_LIST \
	COMMAND_LIST_ENTRY(HP, exp1) \
	COMMAND_LIST_ENTRY(VR, exp2) \
	COMMAND_LIST_ENTRY(ST, exp3) \
	COMMAND_LIST_ENTRY(OP, exp4) \
	COMMAND_LIST_ENTRY(CL, exp5) \
	COMMAND_LIST_ENTRY(MI, exp6) \
	COMMAND_LIST_ENTRY(SG, exp7) \
	COMMAND_LIST_ENTRY(MR, exp8) \
	COMMAND_LIST_ENTRY(RW, exp9) \
	COMMAND_LIST_ENTRY(GP, exp10) \
	COMMAND_LIST_ENTRY(GP1, exp11) \
	COMMAND_LIST_ENTRY(GP2, exp12) \
	COMMAND_LIST_ENTRY(IP, exp13)

#define CMD_NUM (10)
#define CMD_WID (2)
extern csi_dma_config_t gdcfg; 
extern volatile int ret_flag;
static const char *g_csi_cmd[][2] = {COMMAND_LIST};

void csi_usage(void)
{
	printf("Usage for csi testbench:\n");
	printf("\tcsi command [argument]:\n");
	printf("\texample: csi set_lanes 2\n");
}

static int atoi(int *pint, const char *pchar, uint8_t hex)
{
	int sum = 0;
	if(10 == hex) {
		while(*pchar != '\0') {

			sum = sum*10 + (*pchar - 48);
			pchar++;
		}
		*pint = sum;
	}
	else if (16 == hex) {
		if(0 == strncmp("0x", pchar, 2)) {
			pchar += 2;
			while(*pchar != '\0') {
				switch (*pchar) {
					case 'A':
				    case 'B':
					case 'C':
					case 'D':
					case 'E':
					case 'F':
					 	{
							sum = sum*16 + (*pchar - 55);
							break;
						}
					case 'a':
					case 'b':
					case 'c':
					case 'd':
					case 'e':
				    case 'f':

						{
							sum = sum*16 + (*pchar - 87);
							break;
						}
					default:
						{
							sum = sum*16 + (*pchar - 48);
							break;
						}
				}
				pchar++;

			}
			*pint = sum;

		}
		else {
			printf("Hex is not valid: suffix:[0x] example: 0x12344\n");

			return -1;
		}
	}
	printf("dec sum = %d ;;;; hex sum =%x\n", sum ,sum);
	return 0;
}

int csi_hp_command(void)
{
	uint8_t i,j;
	for(i = 0; i < (sizeof(g_csi_cmd)/sizeof(char *))/2; i++) {
		for(j = 0; j < CMD_WID; j++) {
			printf("%s	", g_csi_cmd[i][j]);
		}
		printf("\n");
	}

	return 0;
}

int csi_sg_command(const char *paddr, const char *pvalue)
{
	int value;
	int addr; 
	error_t ret;
	if(atoi(&addr, paddr, 16))
	{
		CSI_ERR("address is invalid\n");
		return -1;
	}
	if(atoi(&value, pvalue, 16))
	{
		CSI_ERR("value is invalid\n");
		return -1;
	}
	//	printf("addr =%x, value=%x\n", addr, value);
	ret = csi_core_set_reg(addr, (uint32_t)value); 
	if(SUCCESS != ret) {
		CSI_ERR("set register csi failure\n");
	}
	printf("is running here\n");
	
	value = csi_core_read_reg(addr);
	printf("after setting ,the value :0x%x\n", value);

	return ret;
}

int csi_mr_command(const char *pmask, const char *preg_num)
{
	int reg_num = 0, mask = 0;
	error_t ret = SUCCESS;
	atoi(&reg_num, preg_num, 10);
	if(atoi(&mask, pmask, 16)) {
		CSI_ERR("mask value is invalid\n");
		return -1;
	}
	if(1 == reg_num) {
		ret = csi_core_mask_err1(mask); 
		if(SUCCESS != ret) {
			CSI_ERR("mask error1 csi failure\n");
		}

	}
	else if(2 == reg_num) {
		ret = csi_core_mask_err2(mask); 
		if(SUCCESS != ret) {
			CSI_ERR("mask error2 csi failure\n");
		}
	
	}
	return ret;

}

static int run_csi_rtl_case(void)
{
#if !defined(CONFIG_COMPILE_RTL)
#if !defined(CSI_USE_OV2710)	
	ar0330_power_on();
	if(ar0330_id_check()) {
		ar0330_init();
	}
	else {
		CSI_ERR("ar0330 image sensor doesn't work\n");
	}
#else
	ov2710_power_on();
	if(ov2710_id_check()) {
		ov2710_init();
	}
	else {
		CSI_ERR("ov2710 image sensor doesn't work\n");
	}

#endif

#endif	
	//open csi interface 
	csi_core_open();
	//initialize csi dma 
	csi_dma_init(&gdcfg);
	//start dma to transfer data
#if !defined(CONFIG_COMPILE_RTL)
#if !defined(CSI_USE_OV2710)	
	ar0330_enable(1);
#else
	ov2710_enable(1);
#endif	
#endif	
	csi_dma_start();
	while(1) {
		if(1 == ret_flag){
			printf("dump picture data from memory to rtl_data.bin\n");
			return 1;
			
		}	
	}
	
	return 0;
}

int csi_test_main(int argc, char *argv[])
{
	if(1 >= argc) {
		csi_usage();
		return 0;
	}
	static uint8_t flag = 0;
	error_t ret;
	if(0 == flag) {							 //initialize once
		//initialize i2c for sensor 
#if !defined(CONFIG_COMPILE_RTL)		
		i2c_init(I2C_CHN_IM, 100000);
		i2c_init(PMU_I2C_CHN, 100000);
#endif
		//initialize csi module
		csi_core_init(CSI_LOG_LEVEL, CSI_LANES, CSI_FREQ);
		// open csi
		//csi_core_open();	
		CSI_INFO("csi module testbench initial\n");
		//enable_irq for get a frame
		enable_irq(CSI_IRQ0);
		//isp_clock_cfg(0x00, 0x00);
		//module_enable("isp");
		//power on image sensor
		//sensor_power_on();
		//csi_dma_config(&gdcfg, CSI_FRAME_SIZE);

		flag++;
	}

	if(0 == strncmp(HP, argv[1], CSI_CMD_LEN)) {
		//display command help
		//TODO
		csi_hp_command();
	}
	else if(0 == strncmp(VR, argv[1], CSI_CMD_LEN)) {
		
		printf("Get the CSI controller version :0x%x\n", csi_core_get_version());	
	}
	else if(0 == strncmp(ST, argv[1], CSI_CMD_LEN)) {
		printf("Current CSI lanes status :0x%x\n", csi_core_get_state());
	}
	else if(0 == strncmp(OP, argv[1], CSI_CMD_LEN)) {
		ret = csi_core_open();
		if(SUCCESS != ret) {
			CSI_ERR("open csi failure\n");
		}
	}
	else if(0 == strncmp(CL, argv[1], CSI_CMD_LEN)) {
		ret = csi_core_close();
		if(SUCCESS != ret) {
			CSI_ERR("close csi failure\n");
		}

	}	
	else if(0 == strncmp(MI, argv[1], CSI_CMD_LEN)) {
		uint32_t ret = 0;
		ret = csi_core_set_reg(0x4, (uint32_t)0x201);
		ret= csi_core_read_reg(0x4);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x34, (uint32_t)0x0);
		ret= csi_core_read_reg(0x34);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x30, (uint32_t)0x1);
		ret= csi_core_read_reg(0x30);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x8, (uint32_t)0x1);
		ret= csi_core_read_reg(0x8);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0xc, (uint32_t)0x1);
		ret= csi_core_read_reg(0xc);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x10, (uint32_t)0x1);
		ret= csi_core_read_reg(0x10);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x30, (uint32_t)0x0);
		ret= csi_core_read_reg(0x30);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x30, (uint32_t)0x0);
		ret= csi_core_read_reg(0x30);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x34, (uint32_t)0x10044);
		ret= csi_core_read_reg(0x34);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x30, (uint32_t)0x2);
		ret= csi_core_read_reg(0x30);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x30, (uint32_t)0x0);
		ret= csi_core_read_reg(0x30);
		 printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x34, (uint32_t)0x14);
		 ret= csi_core_read_reg(0x34);
		 printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x30, (uint32_t)0x2);
		 ret= csi_core_read_reg(0x30);
		printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x30, (uint32_t)0x0);
		 ret= csi_core_read_reg(0x30);
		  printf("Get csi register data:0x%x\n", ret);
		ret = csi_core_set_reg(0x10, (uint32_t)0x1);
		 ret= csi_core_read_reg(0x10);
		printf("Get csi register data:0x%x\n", ret);
	}
	else if(0 == strncmp(MR, argv[1], CSI_CMD_LEN)) {
		if(argc < 3) {
			CSI_ERR("mr command must point mask and which error: example mr, 0x10ff, 1\n");
			return -1 ;
		}
		//csi pmask preg_num
		csi_mr_command(argv[3], argv[2]);	
	}
	else if(0 == strncmp(SG, argv[1], CSI_CMD_LEN)) {
		if(argc < 3) {
			CSI_ERR("sg command must point reg value, reg address offset:\
					example: sg 0x0 0x10ff\n");
			return -1 ;
		}
		csi_sg_command(argv[2], argv[3]);
	}
	else if(0 == strncmp(RW, argv[1], CSI_CMD_LEN)) {
		uint32_t addr;
		printf("read default value of registers\n");
		for(addr = 0; addr <= 0x34; addr +=4) {
			printf("address offset: 0x%x	0x%x\n", addr, csi_core_read_reg(addr));
		}

		for(addr = 0; addr <= 0x34; addr +=4) {
			csi_core_set_reg(addr, 0xffffffff);
		}
		printf("read value after set 0xffffffff to the registers\n");
		for(addr = 0; addr <= 0x34; addr +=4) {
			printf("address offset: 0x%x	0x%x\n", addr, csi_core_read_reg(addr));
		}

	}
	else if(0 == strncmp(GP, argv[1], CSI_CMD_LEN)) {
			//TODO
		csi_dma_config(&gdcfg, CSI_FRAME_SIZE *2);
		return run_csi_rtl_case(); //data type :rgb565	
	}
	else if (0 == strncmp(GP1, argv[1], CSI_CMD_LEN)) {
		printf("test case :csi raw12.\n");
#if defined(CONFIG_COMPILE_RTL)		
		csi_dma_config(&gdcfg, (CSI_FRAME_SIZE *3)/2);
#else
		csi_dma_config(&gdcfg, (CSI_SENSOR_FRAME_SIZE*5)/4);
#endif		
		return run_csi_rtl_case(); //data type :raw12	

	}
	else if (0 == strncmp(GP2, argv[1], CSI_CMD_LEN)) {
		csi_dma_config(&gdcfg, CSI_FRAME_SIZE *2);
		return run_csi_rtl_case(); //data type :yuv422:8bits	

	}
	else if (0 == strncmp(IP, argv[1], CSI_CMD_LEN)) { //case for idi to ipi verify.
		printf("test case : idi to ipi link.\n");
		int width = 0, height = 0, type = 0; 
		atoi(&width, argv[2], 10);
		printf("width = %d\n", width);
		atoi(&height, argv[3], 10);
		printf("height = %d\n", height);
		atoi(&type, argv[4], 16);
		printf("type = 0x%x\n", type);
		csi_iditoipi_open(width, height, type);
		int count = 0;
				
		while(count <= 2)
		{
			count = csi_read(&g_host, CSIDMAREG_VSYNCCNT);
			udelay(500);
		}
	}

	return 0;
}

int csi_config_test(int width, int height, int csi_fmt)
{

	static uint8_t flag = 0;
	error_t ret = 0;
	printf("test case : idi to ipi link.\n");
	if(0 == flag) {
		//initialize once
		//initialize i2c for sensor
#if !defined(CONFIG_COMPILE_RTL)
		i2c_init(I2C_CHN_IM, 100000);
		i2c_init(PMU_I2C_CHN, 100000);
#endif
		//initialize csi module
		csi_core_init(CSI_LOG_LEVEL, CSI_LANES, CSI_FREQ);
		CSI_INFO("csi module testbench initial\n");
		enable_irq(CSI_IRQ0);
		flag++;
	}
#if !defined(CONFIG_COMPILE_RTL)                      
#if !defined(CSI_USE_OV2710)                          
    ar0330_power_on();                                
    if(ar0330_id_check()) {                           
        ar0330_init();                                
    }                                                 
    else {                                            
        CSI_ERR("ar0330 image sensor doesn't work\n");
    }                                                 
#else                                                 
    ov2710_power_on();                                
    if(ov2710_id_check()) {                           
        ov2710_init();                                
    }                                                 
    else {                                            
        CSI_ERR("ov2710 image sensor doesn't work\n");
    }                                                 
                                                      
#endif                                                
#endif                                                
	csi_iditoipi_open(width, height, csi_fmt);
#if !defined(CONFIG_COMPILE_RTL) 
#if !defined(CSI_USE_OV2710)     
	ar0330_enable(1);            
#else                            
	ov2710_enable(1);            
#endif                           
#endif                           
	return ret;
}


int csi_test_rtl(int id)
{
	CSI_INFO("invoking id: %d\n", id);
	return 0;
}

void csi_testbench_init(void)
{
	register_testbench("csi", csi_test_main, csi_test_rtl);
}

testbench_init(csi_testbench_init);


