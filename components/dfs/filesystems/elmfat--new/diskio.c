/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: USB drive control */
//#include "atadrive.h"	/* Example: ATA drive control */
//#include "sdcard.h"		/* Example: MMC/SDC contorl */

/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA drive to drive number 0 */
#define MMC		1	/* Example: Map MMC/SD card to drive number 1 */
#define USB		2	/* Example: Map USB drive to drive number 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */

DSTATUS disk_initialize_USB (BYTE drv);
DSTATUS disk_status_USB (BYTE drv);
DSTATUS disk_initialize_USB (BYTE drv);
DRESULT disk_read_USB (BYTE drv, BYTE *buff, DWORD sector, BYTE count);
DRESULT disk_write_USB (BYTE drv, const BYTE *buff, DWORD sector, BYTE count);
DRESULT disk_ioctl_USB(BYTE drv, BYTE ctrl, void *buff);

/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    int result;

    switch (pdrv)
    {
    case ATA :
        //result = ATA_disk_status();

        // translate the reslut code here

        return stat;

    case MMC :
        result = disk_status_SD(pdrv); // MMC_disk_status();

        // translate the reslut code here

        return stat;

    case USB :
        result = disk_status_USB(pdrv);//USB_disk_status();

        // translate the reslut code here

        return stat;
    }
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    int result;

    switch (pdrv)
    {
    case ATA :
        //result = ATA_disk_initialize();

        // translate the reslut code here

        return stat;

    case MMC :
        result = disk_initialize_SD(pdrv); // MMC_disk_initialize();

        // translate the reslut code here

        return stat;

    case USB :
        result = disk_initialize_USB(pdrv);// USB_disk_initialize();

        // translate the reslut code here

        return stat;
    }
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,		/* Physical drive nmuber to identify the drive */
    BYTE *buff,		/* Data buffer to store read data */
    DWORD sector,	/* Sector address in LBA */
    UINT count		/* Number of sectors to read */
)
{
    DRESULT res;
    int result;

    switch (pdrv)
    {
    case ATA :
        // translate the arguments here

        //result = ATA_disk_read(buff, sector, count);

        // translate the reslut code here

        return res;

    case MMC :
        // translate the arguments here

        result = disk_read_SD(pdrv, buff, sector, count);//MMC_disk_read(buff, sector, count);

        // translate the reslut code here

        return res;

    case USB :
        // translate the arguments here

        result = disk_read_USB(pdrv, buff, sector, count);//USB_disk_read(buff, sector, count);

        // translate the reslut code here

        return res;
    }

    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
    BYTE pdrv,			/* Physical drive nmuber to identify the drive */
    const BYTE *buff,	/* Data to be written */
    DWORD sector,		/* Sector address in LBA */
    UINT count			/* Number of sectors to write */
)
{
    DRESULT res;
    int result;

    switch (pdrv)
    {
    case ATA :
        // translate the arguments here

        //result = ATA_disk_write(buff, sector, count);

        // translate the reslut code here

        return res;

    case MMC :
        // translate the arguments here

        result =  disk_write_SD(pdrv, buff, sector, count);//MMC_disk_write(buff, sector, count);

        // translate the reslut code here

        return res;

    case USB :
        // translate the arguments here

        result = disk_write_USB(pdrv, buff, sector, count); //USB_disk_write(buff, sector, count);

        // translate the reslut code here

        return res;
    }

    return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
    BYTE pdrv,		/* Physical drive nmuber (0..) */
    BYTE ctrl,		/* Control code */
    void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT res;
    int result;

    switch (pdrv)
    {
    case ATA :

        // Process of the command for the ATA drive

        return res;

    case MMC :

        res = disk_ioctl_SD(pdrv, ctrl, buff);
        // Process of the command for the MMC/SD card

        return res;

    case USB :

        res = disk_ioctl_USB(pdrv, ctrl, buff);
        // Process of the command the USB drive

        return res;
    }

    return RES_PARERR;
}
#endif
