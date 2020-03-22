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

/*
 * DW9714VAF voice coil motor driver
 *
 *
 */

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#include "lens_info.h"


#define AF_DRVNAME "DW9714VAF_DRV"
#define AF_I2C_SLAVE_ADDR        0x18

#define AF_DEBUG
#ifdef AF_DEBUG
#define LOG_INF(format, args...) pr_debug(AF_DRVNAME " [%s] " format, __func__, ##args)
#else
#define LOG_INF(format, args...)
#endif

typedef enum
{
    AF_disabled,
    AF_initializing,
    AF_driving,
    AF_releasing
}STATUS_NUM;
static struct i2c_client *g_pstAF_I2Cclient;
static int *g_pAF_Opened;
static spinlock_t *g_pAF_SpinLock;


static unsigned long g_u4AF_INF;
static unsigned long g_u4AF_MACRO = 1023;
static unsigned long g_u4TargetPosition;
static unsigned long g_u4CurrPosition;
static char status_AF; //[LGE_UPDATE] [kyunghun.oh@lge.com] [2017-04-20] Slope Controlling for reducing tick noise when open and release time
static char is_register_setted;

static int i2c_read(u8 a_u2Addr, u8 *a_puBuff)
{
    int i4RetValue = 0;
    char puReadCmd[1] = { (char)(a_u2Addr) };

    i4RetValue = i2c_master_send(g_pstAF_I2Cclient, puReadCmd, 1);

    if (i4RetValue != 1) {
        LOG_INF(" I2C write failed!!\n");
        return -1;
    }

    i4RetValue = i2c_master_recv(g_pstAF_I2Cclient, (char *)a_puBuff, 1);
    if (i4RetValue != 1) {
        LOG_INF(" I2C read failed!!\n");
        return -1;
    }

    return 0;
}

static u8 read_data(u8 addr)
{
    u8 get_byte = 0;

    i2c_read(addr, &get_byte);

    return get_byte;
}

static int s4DW9714AF_ReadReg(unsigned short *a_pu2Result)
{
    *a_pu2Result = (read_data(0x03) << 8) + (read_data(0x04) & 0xff);

    return 0;
}

static int s4AF_WriteReg(u16 a_u2Data)
{
    int i4RetValue = 0;
    char puSendCmd[2] = {(char)(a_u2Data >> 4), (char)((a_u2Data << 4) & 0xf0) };
    if(status_AF == AF_initializing)
        puSendCmd[1] |= 0x0F;		//4steps & 1088us per step
    else if(status_AF == AF_releasing)
        puSendCmd[1] |= 0x0D;		//4steps & 272us per steps
    else
        puSendCmd[1] |= 0x05;		//1steps & 162us per steps

    g_pstAF_I2Cclient->addr = AF_I2C_SLAVE_ADDR;

    g_pstAF_I2Cclient->addr = g_pstAF_I2Cclient->addr >> 1;

    i4RetValue = i2c_master_send(g_pstAF_I2Cclient, puSendCmd, 2);

    if (i4RetValue < 0) {
        LOG_INF("I2C send failed!!\n");
        return -1;
    }

    return 0;
}

static inline int getAFInfo(__user struct stAF_MotorInfo * pstMotorInfo)
{
    struct stAF_MotorInfo stMotorInfo;

    stMotorInfo.u4MacroPosition = g_u4AF_MACRO;
    stMotorInfo.u4InfPosition = g_u4AF_INF;
    stMotorInfo.u4CurrentPosition = g_u4CurrPosition;
    stMotorInfo.bIsSupportSR = 1;

    stMotorInfo.bIsMotorMoving = 1;

    if (*g_pAF_Opened >= 1)
        stMotorInfo.bIsMotorOpen = 1;
    else
        stMotorInfo.bIsMotorOpen = 0;

    if (copy_to_user(pstMotorInfo, &stMotorInfo, sizeof(struct stAF_MotorInfo)))
        LOG_INF("copy to user failed when getting motor information\n");

    return 0;
}



static void set_reg_init(void)
{
    char puSendCmd0[2] = {0x00, 0x00};					//power up
    char puSendCmd_ENTRY_1_ON[2] = {0xEC, 0xA3};  	//Ringing setting ON
    char puSendCmd1[2] = {0xA1, 0x05};					//DLC mode OFF & MCLK = X1
    char puSendCmd2[2] = {0xF2, 0x80};					//T_SRC = 0b10000
    char puSendCmd_ENTRY_1_OFF[2] = {0xDC, 0x51}; 	//Ringing setting OFF

    i2c_master_send(g_pstAF_I2Cclient, puSendCmd0, 2); mdelay(1);
    i2c_master_send(g_pstAF_I2Cclient, puSendCmd_ENTRY_1_ON, 2); mdelay(1);
    i2c_master_send(g_pstAF_I2Cclient, puSendCmd1, 2); mdelay(1);
    i2c_master_send(g_pstAF_I2Cclient, puSendCmd2, 2); mdelay(1);
    i2c_master_send(g_pstAF_I2Cclient, puSendCmd_ENTRY_1_OFF, 2); mdelay(1);

}

static void set_reg_drive(void)
{
    char puSendCmd_ENTRY_1_ON[2] = {0xEC, 0xA3};  	//Ringing setting ON
    char puSendCmd1[2] = {0xA1, 0x05};					//DLC mode OFF & MCLK = X1
    char puSendCmd2[2] = {0xF2, 0x00};					//T_SRC = 0b10000
    char puSendCmd_ENTRY_1_OFF[2] = {0xDC, 0x51}; 	//Ringing setting OFF

    i2c_master_send(g_pstAF_I2Cclient, puSendCmd_ENTRY_1_ON, 2); mdelay(1);
    i2c_master_send(g_pstAF_I2Cclient, puSendCmd1, 2); mdelay(1);
    i2c_master_send(g_pstAF_I2Cclient, puSendCmd2, 2); mdelay(1);
    i2c_master_send(g_pstAF_I2Cclient, puSendCmd_ENTRY_1_OFF, 2); mdelay(1);

}


static void set_reg_release(void)
{
    //same as drive setting
}
static inline int moveAF(unsigned long a_u4Position)
{
    int ret = 0;
    if ((a_u4Position > g_u4AF_MACRO) || (a_u4Position < g_u4AF_INF)) {
        LOG_INF("out of range\n");
        return -EINVAL;
    }
    if (*g_pAF_Opened == 1) {
        unsigned short InitPos;
        status_AF = AF_initializing;//[LGE_UPDATE] [kyunghun.oh@lge.com] [2017-04-20] Slope Controlling for reducing tick noise when open and release time
        ret = s4DW9714AF_ReadReg(&InitPos);
        if (ret == 0) {
            LOG_INF("Init Pos %6d\n", InitPos);
            spin_lock(g_pAF_SpinLock);
            g_u4CurrPosition = (unsigned long)InitPos;
            spin_unlock(g_pAF_SpinLock);
        } else {
            spin_lock(g_pAF_SpinLock);
            g_u4CurrPosition = 0;
            spin_unlock(g_pAF_SpinLock);
        }

        spin_lock(g_pAF_SpinLock);
        *g_pAF_Opened = 2;
        spin_unlock(g_pAF_SpinLock);
    }

    if (g_u4CurrPosition == a_u4Position && status_AF != AF_initializing)
        return 0;
    spin_lock(g_pAF_SpinLock);
    g_u4TargetPosition = a_u4Position;
    spin_unlock(g_pAF_SpinLock);
    //LOG_INF("move [curr] %d [target] %d\n", g_u4CurrPosition, g_u4TargetPosition);
    if(status_AF == AF_initializing && is_register_setted != AF_initializing)
    {
        LOG_INF("[DW9714AF] set_initializing_mode\n");
        set_reg_init();
        is_register_setted = 0;
        if(g_u4TargetPosition == 0)
        {
            status_AF = AF_driving;
            return 0;
        }
        is_register_setted = AF_initializing;
    }
    else if(status_AF == AF_releasing && is_register_setted != AF_releasing)
    {
        LOG_INF("[DW9714AF] set_releasing_mode\n");
        set_reg_release();
        is_register_setted = AF_releasing;
    }
    else if(status_AF == AF_driving && is_register_setted != AF_driving)
    {
        LOG_INF("[DW9714AF] set_driving_mode\n");
        set_reg_drive();
        is_register_setted = AF_driving;
    }
    if (s4AF_WriteReg((unsigned short)g_u4TargetPosition) == 0) {
        spin_lock(g_pAF_SpinLock);
        g_u4CurrPosition = (unsigned long)g_u4TargetPosition;
        spin_unlock(g_pAF_SpinLock);
    } else {
        LOG_INF("set I2C failed when moving the motor\n");
    }

    if(status_AF == AF_initializing)
        status_AF = AF_driving;//[LGE_UPDATE] [kyunghun.oh@lge.com] [2017-04-20] Slope Controlling for reducing tick noise when open and release time

    return 0;
}

static inline int setAFInf(unsigned long a_u4Position)
{
    spin_lock(g_pAF_SpinLock);
    g_u4AF_INF = a_u4Position;
    spin_unlock(g_pAF_SpinLock);
    return 0;
}

static inline int setAFMacro(unsigned long a_u4Position)
{
    spin_lock(g_pAF_SpinLock);
    g_u4AF_MACRO = a_u4Position;
    spin_unlock(g_pAF_SpinLock);
    return 0;
}

/* ////////////////////////////////////////////////////////////// */
long DW9714VAF_Ioctl(struct file *a_pstFile, unsigned int a_u4Command, unsigned long a_u4Param)
{
    long i4RetValue = 0;

    switch (a_u4Command) {
        case AFIOC_G_MOTORINFO:
            i4RetValue = getAFInfo((__user struct stAF_MotorInfo *) (a_u4Param));
            break;

        case AFIOC_T_MOVETO:
            i4RetValue = moveAF(a_u4Param);
            break;

        case AFIOC_T_SETINFPOS:
            i4RetValue = setAFInf(a_u4Param);
            break;

        case AFIOC_T_SETMACROPOS:
            i4RetValue = setAFMacro(a_u4Param);
            break;

        default:
            LOG_INF("No CMD\n");
            i4RetValue = -EPERM;
            break;
    }

    return i4RetValue;
}

/* Main jobs: */
/* 1.Deallocate anything that "open" allocated in private_data. */
/* 2.Shut down the device on last close. */
/* 3.Only called once on last time. */
/* Q1 : Try release multiple times. */
int DW9714VAF_Release(struct inode *a_pstInode, struct file *a_pstFile)
{
    LOG_INF("Start\n");
    status_AF = AF_releasing;//[LGE_UPDATE] [kyunghun.oh@lge.com] [2017-04-20] Slope Controlling for reducing tick noise when open and release time

    if (*g_pAF_Opened == 2)
        LOG_INF("Wait\n");

    if (*g_pAF_Opened) {
        LOG_INF("Free\n");
        spin_lock(g_pAF_SpinLock);
        *g_pAF_Opened = 0;
        spin_unlock(g_pAF_SpinLock);
    }

    if (*g_pAF_Opened == 0)
    {
        char puSendCmd1[2] = {0x80, 0x00};
        unsigned long release_position;
        while(g_u4CurrPosition > 300)
        {
            release_position = g_u4CurrPosition;
            release_position -= 30;
            moveAF(release_position);
            msleep(5);
        }

        while(g_u4CurrPosition > 5)
        {
            release_position = g_u4CurrPosition;
            release_position -= 5;
            moveAF(release_position);
            msleep(2);
        }
        status_AF = AF_disabled;//[LGE_UPDATE] [kyunghun.oh@lge.com] [2017-04-20] Slope Controlling for reducing tick noise when open and release time
        i2c_master_send(g_pstAF_I2Cclient, puSendCmd1, 2);	//Power down mode
    }
    LOG_INF("End\n");

	return 0;
}

int DW9714VAF_SetI2Cclient(struct i2c_client *pstAF_I2Cclient, spinlock_t *pAF_SpinLock, int *pAF_Opened)
{
	g_pstAF_I2Cclient = pstAF_I2Cclient;
	g_pAF_SpinLock = pAF_SpinLock;
	g_pAF_Opened = pAF_Opened;
	
	return 1;
}
