/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

//#include "Spi_sd.h"
//#include "common.h"
#include "diskio.h"
#include "SPI_SD_driver.h"
#include  "rtc.h"



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize_SD (
    BYTE drv				/* Physical drive nmuber (0..) */
)
{
    u8 state;

    if(drv != MMC)
    {
        return STA_NOINIT;  //��֧�ִ���0�Ĳ���
    }

    state = SD_Init();
    if(state == STA_NODISK)
    {
        return STA_NODISK;
    }
    else if(state != 0)
    {
        return STA_NOINIT;  //�������󣺳�ʼ��ʧ��
    }
    else
    {
        return 0;           //��ʼ���ɹ�
    }
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status_SD (
    BYTE drv		/* Physical drive nmuber (0..) */
)
{
    if(drv != MMC)
    {
        return STA_NOINIT;  //��֧�ִ���0����
    }

    //���SD���Ƿ����
    if(!SD_DET())
    {
        return STA_NODISK;
    }
    return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read_SD (
    BYTE drv,		/* Physical drive nmuber (0..) */
    BYTE *buff,		/* Data buffer to store read data */
    DWORD sector,	/* Sector address (LBA) */
    BYTE count		/* Number of sectors to read (1..255) */
)
{
    u8 res = 0;
    if (drv != MMC || !count)
    {
        return RES_PARERR;  //��֧�ֵ����̲�����count���ܵ���0�����򷵻ز�������
    }
    if(!SD_DET())
    {
        return RES_NOTRDY;  //û�м�⵽SD������NOT READY����
    }



    if(count == 1)          //1��sector�Ķ�����
    {
        res = SD_ReadSingleBlock(sector, buff);
    }
    else                    //���sector�Ķ�����
    {
        res = SD_ReadMultiBlock(sector, buff, count);
    }
    /*
    do
    {
        if(SD_ReadSingleBlock(sector, buff)!=0)
        {
            res = 1;
            break;
        }
        buff+=512;
    }while(--count);
    */
    //��������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res == 0x00)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write_SD (
    BYTE drv,			/* Physical drive nmuber (0..) */
    const BYTE *buff,	/* Data to be written */
    DWORD sector,		/* Sector address (LBA) */
    BYTE count			/* Number of sectors to write (1..255) */
)
{
    u8 res;

    if (drv != MMC || !count)
    {
        return RES_PARERR;  //��֧�ֵ����̲�����count���ܵ���0�����򷵻ز�������
    }
    if(!SD_DET())
    {
        return RES_NOTRDY;  //û�м�⵽SD������NOT READY����
    }

    // ��д����
    if(count == 1)
    {
        res = SD_WriteSingleBlock(sector, buff);
    }
    else
    {
        res = SD_WriteMultiBlock(sector, buff, count);
    }
    // ����ֵת��
    if(res == 0)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl_SD (
    BYTE drv,		/* Physical drive nmuber (0..) */
    BYTE ctrl,		/* Control code */
    void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT res;


    if (drv != MMC)
    {
        return RES_PARERR;  //��֧�ֵ����̲��������򷵻ز�������
    }

    //FATFSĿǰ�汾���账��CTRL_SYNC��GET_SECTOR_COUNT��GET_BLOCK_SIZ��������
    switch(ctrl)
    {
    case CTRL_SYNC:
        SD_CS_ENABLE();
        if(SD_WaitReady() == 0)
        {
            res = RES_OK;
        }
        else
        {
            res = RES_ERROR;
        }
        SD_CS_DISABLE();
        break;

    case GET_BLOCK_SIZE:
        *(WORD *)buff = 512;
        res = RES_OK;
        break;

    case GET_SECTOR_COUNT:
        *(DWORD *)buff = SD_GetCapacity();
        res = RES_OK;
        break;

    default:
        res = RES_PARERR;
        break;
    }

    return res;
}


/*-----------------------------------------------------------------------*/
/* User defined function to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */
//DWORD get_fattime (void)
//{
/*  DWORD date;
  RTC_TimeTypeDef ts;
  RTC_DateTypeDef ds;
  unsigned int	year, month, day;
  unsigned int	hour, minute, sec;
  uint8_t			buf[4];
  uint32_t		utc_bkp, utc;

  RTC_GetTime( RTC_Format_BIN, &ts );
  RTC_GetDate( RTC_Format_BIN, &ds );
  year	= ds.RTC_Year + 2000;
  month	= ds.RTC_Month;
  day		= ds.RTC_Date;
  hour	= ts.RTC_Hours;
  minute	= ts.RTC_Minutes;
  sec		= ts.RTC_Seconds;

  year -= 1980;		//��ݸ�Ϊ1980����
  sec /= 2;      	//��������Ϊ0-29

  date = 0;
  date = (year << 25) | (month << 21) | (day << 16) | \
         (hour << 11) | (minute << 5) | (sec);

  return date;*/


DWORD get_fattime(void)
{

    /* u32  res_u32=0;

        year=time_now.year+2000-1980;
      res_u32=(year<<1); // ����1 λ ����1λ
      res_u32=(res_u32<<4)+(time_now.month&0x0f);//  left   4  bit
      res_u32=(res_u32<<5)+(time_now.day&0x1f);// left    5
      res_u32=(res_u32<<5)+(time_now.hour&0x1f);// left  5
      res_u32=(res_u32<<6)+(time_now.min&0x3f);// left  6
      res_u32=(res_u32<<4)+((time_now.sec/2)&0x0f);// left  4

    return res_u32;*/

    return ((time_now.year + 2000 - 1980) << 25) /* Year = 2010 */
           | ((u32)time_now.month << 21) /* Month = 11 */
           | ((u32)time_now.day << 16) /* Day = 2 */
           | ((u32)time_now.hour << 11) /* Hour = 15 */
           | ((u32)time_now.min << 5) /* Min = 0 */
           | ((u32)time_now.sec >> 1) /* Sec = 0 */
           ;
}






