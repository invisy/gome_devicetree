/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#define LOG_TAG "LCM"

#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif

#include "lcm_drv.h"


#ifdef BUILD_LK
#include <platform/upmu_common.h>
#include <platform/mt_gpio.h>
#include <platform/mt_i2c.h>
#include <platform/mt_pmic.h>
#include <string.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#endif

#ifdef BUILD_LK
#define LCM_LOGI(string, args...)  dprintf(0, "[LK/"LOG_TAG"]"string, ##args)
#define LCM_LOGD(string, args...)  dprintf(1, "[LK/"LOG_TAG"]"string, ##args)
#else
#define LCM_LOGI(fmt, args...)  pr_debug("[KERNEL/"LOG_TAG"]"fmt, ##args)
#define LCM_LOGD(fmt, args...)  pr_debug("[KERNEL/"LOG_TAG"]"fmt, ##args)
#endif

#define LCM_ID_NT35695 (0xf5)

static const unsigned int BL_MIN_LEVEL = 20;
static struct LCM_UTIL_FUNCS lcm_util;


#define SET_RESET_PIN(v)	(lcm_util.set_reset_pin((v)))
#define MDELAY(n)		(lcm_util.mdelay(n))
#define UDELAY(n)		(lcm_util.udelay(n))

#define dsi_set_cmdq_V22(cmdq, cmd, count, ppara, force_update) \
	lcm_util.dsi_set_cmdq_V22(cmdq, cmd, count, ppara, force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update) \
	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update) \
		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd) lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums) \
		lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd) \
	  lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size) \
		lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

#ifndef BUILD_LK
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/irq.h>
/* #include <linux/jiffies.h> */
/* #include <linux/delay.h> */
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#endif

/* static unsigned char lcd_id_pins_value = 0xFF; */
static const unsigned char LCD_MODULE_ID = 0x01;
#define LCM_DSI_CMD_MODE									0
#define FRAME_WIDTH										(1080)
#define FRAME_HEIGHT									(2160)

/* physical size in um */
#define LCM_PHYSICAL_WIDTH									(68040)
#define LCM_PHYSICAL_HEIGHT									(136080)

#define REGFLAG_DELAY		0xFFFC
#define REGFLAG_UDELAY	0xFFFB
#define REGFLAG_END_OF_TABLE	0xFFFD
#define REGFLAG_RESET_LOW	0xFFFE
#define REGFLAG_RESET_HIGH	0xFFFF

static struct LCM_DSI_MODE_SWITCH_CMD lcm_switch_mode_cmd;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

struct LCM_setting_table {
	unsigned int cmd;
	unsigned char count;
	unsigned char para_list[64];
};

static struct LCM_setting_table lcm_suspend_setting[] = {
	{0xFF, 1, {0x10}},
	{0x28, 0, {}},
	{REGFLAG_DELAY, 50, {}},
	{0x10, 0, {}},
	{REGFLAG_DELAY, 100, {}}
};

static struct LCM_setting_table init_setting[] = {
	{0xff, 1, {0x20}},
  {0xfb, 1, {0x01}},
  {0xaf, 1, {0x00}},
  
  {0xb0, 16, {0, 0xD, 0, 0x27, 0, 0x4F, 0, 0x6F, 0, 0x8B, 0, 0xA3, 0, 0xB8, 0, 0xCB}},
  {0xb1, 16, {0, 0xDC, 1, 0x14, 1, 0x3E, 1, 0x7C, 1, 0xAB, 1, 0xF1, 2, 0x28, 2, 0x2A}},
  {0xb2, 16, {2, 0x60, 2, 0x9E, 2, 0xC6, 2, 0xF8, 3, 0x17, 3, 0x44, 3, 0x50, 3, 0x5C}},
  {0xb3, 12, {3, 0x6A, 3, 0x79, 3, 0x89, 3, 0xA2, 3, 0xCF, 3, 0xD8}},
  {0xb4, 16, {0, 0xA8, 0, 0xB2, 0, 0xC6, 0, 0xD7, 0, 0xE6, 0, 0xF5, 1, 2, 1, 0xE}},
  {0xb5, 16, {1, 0x1A, 1, 0x41, 1, 0x62, 1, 0x95, 1, 0xBE, 1, 0xFC, 2, 0x31, 2, 0x33}},
  {0xb6, 16, {2, 0x67, 2, 0xA4, 2, 0xCC, 2, 0xFE, 3, 0x20, 3, 0x4B, 3, 0x57, 3, 0x65}},
  {0xb7, 12, {3, 0x74, 3, 0x86, 3, 0x9C, 3, 0xB7, 3, 0xD3, 3, 0xD8}},
  {0xb8, 16, {0, 0xD, 0, 0x2A, 0, 0x55, 0, 0x76, 0, 0x92, 0, 0xAB, 0, 0xC0, 0, 0xD3}},
  {0xb9, 16, {0, 0xE4, 1, 0x1B, 1, 0x44, 1, 0x81, 1, 0xAF, 1, 0xF3, 2, 0x2B, 2, 0x2D}},
  {0xba, 16, {2, 0x63, 2, 0xA1, 2, 0xC9, 2, 0xFD, 3, 0x20, 3, 0x51, 3, 0x61, 3, 0x77}},
  {0xbb, 12, {3, 0x9A, 3, 0xB8, 3, 0xBB, 3, 0xCA, 3, 0xD6, 3, 0xD8}},
  
  {0xff, 1, {0x21}},
  {0xfb, 1, {0x1}},
  
  {0xb0, 16, {0, 0xD, 0, 0x27, 0, 0x4F, 0, 0x6F, 0, 0x8B, 0, 0xA3, 0, 0xB8, 0, 0xCB}},
  {0xb1, 16, {0, 0xDC, 1, 0x14, 1, 0x3E, 1, 0x7C, 1, 0xAB, 1, 0xF1, 2, 0x28, 2, 0x2A}},
  {0xb2, 16, {2, 0x60, 2, 0x9E, 2, 0xC6, 2, 0xF8, 3, 0x17, 3, 0x44, 3, 0x50, 3, 0x5C}},
  {0xb3, 12, {3, 0x6A, 3, 0x79, 3, 0x89, 3, 0xA2, 3, 0xCF, 3, 0xD8}},
  {0xb4, 16, {0, 0xA8, 0, 0xB2, 0, 0xC6, 0, 0xD7, 0, 0xE6, 0, 0xF5, 1, 2, 1, 0xE}},
  {0xb5, 16, {1, 0x1A, 1, 0x41, 1, 0x62, 1, 0x95, 1, 0xBE, 1, 0xFC, 2, 0x31, 2, 0x33}},
  {0xb6, 16, {2, 0x67, 2, 0xA4, 2, 0xCC, 2, 0xFE, 3, 0x20, 3, 0x4B, 3, 0x57, 3, 0x65}},
  {0xb7, 12, {3, 0x74, 3, 0x86, 3, 0x9C, 3, 0xB7, 3, 0xD3, 3, 0xD8}},
  {0xb8, 16, {0, 0xD, 0, 0x2A, 0, 0x55, 0, 0x76, 0, 0x92, 0, 0xAB, 0, 0xC0, 0, 0xD3}},
  {0xb9, 16, {0, 0xE4, 1, 0x1B, 1, 0x44, 1, 0x81, 1, 0xAF, 1, 0xF3, 2, 0x2B, 2, 0x2D}},
  {0xba, 16, {2, 0x63, 2, 0xA1, 2, 0xC9, 2, 0xFD, 3, 0x20, 3, 0x51, 3, 0x61, 3, 0x77}},
  {0xbb, 12, {3, 0x9A, 3, 0xB8, 3, 0xBB, 3, 0xCA, 3, 0xD6, 3, 0xD8}},
  
  {0xff, 1, {0x10}},
  {0xfb, 1, {0x1}},
  
  {0x35, 1, {}},
  {0x11, 0, {}},
  
  {REGFLAG_DELAY, 120, {}},
  
  {0x29, 0, {}}
};

#if 0
static struct LCM_setting_table lcm_set_window[] = {
	{0x2A, 4, {0x00, 0x00, (FRAME_WIDTH >> 8), (FRAME_WIDTH & 0xFF)} },
	{0x2B, 4, {0x00, 0x00, (FRAME_HEIGHT >> 8), (FRAME_HEIGHT & 0xFF)} },
	{REGFLAG_END_OF_TABLE, 0x00, {} }
};
#endif
#if 0
static struct LCM_setting_table lcm_sleep_out_setting[] = {
	/* Sleep Out */
	{0x11, 1, {0x00} },
	{REGFLAG_DELAY, 120, {} },

	/* Display ON */
	{0x29, 1, {0x00} },
	{REGFLAG_DELAY, 20, {} },
	{REGFLAG_END_OF_TABLE, 0x00, {} }
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	/* Display off sequence */
	{0x28, 1, {0x00} },
	{REGFLAG_DELAY, 20, {} },

	/* Sleep Mode On */
	{0x10, 1, {0x00} },
	{REGFLAG_DELAY, 120, {} },
	{REGFLAG_END_OF_TABLE, 0x00, {} }
};
#endif
static struct LCM_setting_table bl_level[] = {
	{0x51, 1, {0xFF} },
	{REGFLAG_END_OF_TABLE, 0x00, {} }
};

static void push_table(void *cmdq, struct LCM_setting_table *table,
	unsigned int count, unsigned char force_update)
{
	unsigned int i;
	unsigned cmd;

	for (i = 0; i < count; i++) {
		cmd = table[i].cmd;

		switch (cmd) {

		case REGFLAG_DELAY:
			if (table[i].count <= 10)
				MDELAY(table[i].count);
			else
				MDELAY(table[i].count);
			break;

		case REGFLAG_UDELAY:
			UDELAY(table[i].count);
			break;

		case REGFLAG_END_OF_TABLE:
			break;

		default:
			dsi_set_cmdq_V22(cmdq, cmd, table[i].count, table[i].para_list, force_update);
		}
	}
}


static void lcm_set_util_funcs(const struct LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(struct LCM_UTIL_FUNCS));
}


static void lcm_get_params(struct LCM_PARAMS *params)
{
	memset(params, 0, sizeof(struct LCM_PARAMS));
	params->type   = LCM_TYPE_DSI;

	params->width  = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;

	params->physical_width=62;
	params->physical_height=124;


#if (LCM_DSI_CMD_MODE)
	params->dsi.mode   = CMD_MODE;
#else
	params->dsi.mode   = SYNC_PULSE_VDO_MODE;  //SYNC_EVENT_VDO_MODE  BURST_VDO_MODE    SYNC_PULSE_VDO_MODE
#endif

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM		= 4;	// 4
	params->dsi.PLL_CLOCK 		= 481;	//371/2;  //4lane

#if 1
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
#else
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_LSB_FIRST;
	params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_MSB;
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
#endif

	// Highly depends on LCD driver capability.
	// Not support in MT6578
	params->dsi.packet_size=256;
	params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;

	params->dsi.vertical_sync_active	= 2;
	params->dsi.vertical_backporch		= 30;
	params->dsi.vertical_frontporch		= 30;
	params->dsi.vertical_active_line	= FRAME_HEIGHT;

	params->dsi.horizontal_sync_active	= 4;
	params->dsi.horizontal_backporch	= 54;
	params->dsi.horizontal_frontporch	= 64;
	params->dsi.horizontal_active_pixel	= FRAME_WIDTH;

	//improve clk quality
	//params->dsi.PLL_CLOCK 		= 390; 	  //3lane
	//params->dsi.PLL_CLOCK 		= 257;    //4lane

	params->dsi.compatibility_for_nvk 	= 1;
	params->dsi.ssc_disable 		= 1;

	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 1;
	params->dsi.lcm_esd_check_table[0].cmd = 0x0A;
	params->dsi.lcm_esd_check_table[0].count = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9C;
#ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
	params->corner_pattern_width = 1080;
	params->corner_pattern_height = 32;
#endif
}

static void lcm_init_power(void)
{
	display_bias_enable();
}

static void lcm_suspend_power(void)
{
	display_bias_disable();
}

static void lcm_resume_power(void)
{
	SET_RESET_PIN(0);
	display_bias_enable();
}

static void lcm_init(void)
{
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(20);

	push_table(NULL, init_setting, sizeof(init_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
	push_table(NULL, lcm_suspend_setting, sizeof(lcm_suspend_setting) / sizeof(struct LCM_setting_table), 1);
	MDELAY(10);
	/* SET_RESET_PIN(0); */
}

static void lcm_resume(void)
{
	lcm_init();
}

static void lcm_update(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0 >> 8) & 0xFF);
	unsigned char x0_LSB = (x0 & 0xFF);
	unsigned char x1_MSB = ((x1 >> 8) & 0xFF);
	unsigned char x1_LSB = (x1 & 0xFF);
	unsigned char y0_MSB = ((y0 >> 8) & 0xFF);
	unsigned char y0_LSB = (y0 & 0xFF);
	unsigned char y1_MSB = ((y1 >> 8) & 0xFF);
	unsigned char y1_LSB = (y1 & 0xFF);

	unsigned int data_array[16];

	data_array[0] = 0x00053902;
	data_array[1] = (x1_MSB << 24) | (x0_LSB << 16) | (x0_MSB << 8) | 0x2a;
	data_array[2] = (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00053902;
	data_array[1] = (y1_MSB << 24) | (y0_LSB << 16) | (y0_MSB << 8) | 0x2b;
	data_array[2] = (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x002c3909;
	dsi_set_cmdq(data_array, 1, 0);
}

static unsigned int lcm_compare_id(void)
{
	unsigned int id = 0, version_id = 0;
	unsigned char buffer[2];
	unsigned int array[16];

	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(1);

	SET_RESET_PIN(1);
	MDELAY(20);

	array[0] = 0x00023700;	/* read id return two byte,version and id */
	dsi_set_cmdq(array, 1, 1);

	read_reg_v2(0xF4, buffer, 2);
	id = buffer[0];     /* we only need ID */

	read_reg_v2(0xDB, buffer, 1);
	version_id = buffer[0];

	LCM_LOGI("%s,nt35695_id=0x%08x,version_id=0x%x\n", __func__, id, version_id);

	if (id == LCM_ID_NT35695 && version_id == 0x81)
		return 1;
	else
		return 0;

}

static void lcm_setbacklight_cmdq(void *handle, unsigned int level)
{

	LCM_LOGI("%s,nt35695 backlight: level = %d\n", __func__, level);

	bl_level[0].para_list[0] = level;

	push_table(handle, bl_level, sizeof(bl_level) / sizeof(struct LCM_setting_table), 1);
}

static void *lcm_switch_mode(int mode)
{
#ifndef BUILD_LK
/* customization: 1. V2C config 2 values, C2V config 1 value; 2. config mode control register */
	if (mode == 0) {	/* V2C */
		lcm_switch_mode_cmd.mode = CMD_MODE;
		lcm_switch_mode_cmd.addr = 0xBB;	/* mode control addr */
		lcm_switch_mode_cmd.val[0] = 0x13;	/* enabel GRAM firstly, ensure writing one frame to GRAM */
		lcm_switch_mode_cmd.val[1] = 0x10;	/* disable video mode secondly */
	} else {		/* C2V */
		lcm_switch_mode_cmd.mode = SYNC_PULSE_VDO_MODE;
		lcm_switch_mode_cmd.addr = 0xBB;
		lcm_switch_mode_cmd.val[0] = 0x03;	/* disable GRAM and enable video mode */
	}
	return (void *)(&lcm_switch_mode_cmd);
#else
	return NULL;
#endif
}

#if (LCM_DSI_CMD_MODE)

/* partial update restrictions:
 * 1. roi width must be 1080 (full lcm width)
 * 2. vertical start (y) must be multiple of 16
 * 3. vertical height (h) must be multiple of 16
 */
static void lcm_validate_roi(int *x, int *y, int *width, int *height)
{
	unsigned int y1 = *y;
	unsigned int y2 = *height + y1 - 1;
	unsigned int x1, w, h;

	x1 = 0;
	w = FRAME_WIDTH;

	y1 = round_down(y1, 16);
	h = y2 - y1 + 1;

	/* in some cases, roi maybe empty. In this case we need to use minimu roi */
	if (h < 16)
		h = 16;

	h = round_up(h, 16);

	/* check height again */
	if (y1 >= FRAME_HEIGHT || y1 + h > FRAME_HEIGHT) {
		/* assign full screen roi */
		pr_debug("%s calc error,assign full roi:y=%d,h=%d\n", __func__, *y, *height);
		y1 = 0;
		h = FRAME_HEIGHT;
	}

	/*pr_debug("lcm_validate_roi (%d,%d,%d,%d) to (%d,%d,%d,%d)\n",*/
	/*	*x, *y, *width, *height, x1, y1, w, h);*/

	*x = x1;
	*width = w;
	*y = y1;
	*height = h;
}
#endif

struct LCM_DRIVER nt36672_fhd_dsi_vdo_rt5081_lcm_drv = {
	.name = "nt36672_fhd_dsi_vdo_rt5081_drv",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params = lcm_get_params,
	.init = lcm_init,
	.suspend = lcm_suspend,
	.resume = lcm_resume,
	.compare_id = lcm_compare_id,
	.init_power = lcm_init_power,
	.resume_power = lcm_resume_power,
	.suspend_power = lcm_suspend_power,
	.set_backlight_cmdq = lcm_setbacklight_cmdq,
	.update = lcm_update,
	.switch_mode = lcm_switch_mode,
#if (LCM_DSI_CMD_MODE)
	.validate_roi = lcm_validate_roi,
#endif

};
