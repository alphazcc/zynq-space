#include "sd_card.h"
#include "xil_printf.h"
#include "stdio.h"

#define kprintf xil_printf

FRESULT fatfs_init(FATFS *fatfs, TCHAR *path)
{
    FRESULT res;

    res = f_mount(fatfs, path, 1);
    if(res != FR_OK)
    {
        res = f_mkfs(path, 0, 0);
        if (res != FR_OK)
        {
            kprintf("ERROR: Unable to format FATfs.\r\n");
            return res;
        }

        res = f_mount(fatfs, path, 1);
        if(res != FR_OK)
        {
            kprintf("ERROR: f_mount returned %d.\r\n", res);
            return res;
        }
    }
    return res;
}

FRESULT sd_read_data(char *FileName, uint32_t DestinationAddress, uint32_t ByteLength)
{
    FIL fil;
    FRESULT res;
    UINT br;

    res = f_open(&fil, FileName, FA_READ);
    if(res)
    {
        kprintf("ERROR: %s f_open returned %d\r\n", FileName, res);
        return res;
    }

    res = f_lseek(&fil, 0);
    if(res)
    {
        kprintf("ERROR: %s f_lseek returned %d\r\n", FileName, res);
        return res;
    }

    res = f_read(&fil, (void*)DestinationAddress, ByteLength, &br);
    if(res)
    {
        kprintf("ERROR: %s f_read returned %d\r\n", FileName, res);
        return res;
    }

    res = f_close(&fil);
    if(res)
    {
        kprintf("ERROR: %s f_close returned %d\r\n", FileName, res);
        return res;
    }
    return res;
}

FRESULT sd_write_data(char *FileName, uint32_t SourceAddress, uint32_t ByteLength)
{
    FIL fil;
    FRESULT res;
    UINT bw;

    res = f_open(&fil, FileName, FA_CREATE_ALWAYS | FA_WRITE);
    if(res)
    {
        kprintf("ERROR: %s f_open returned %d.\r\n", FileName, res);
        return res;
    }

    res = f_lseek(&fil, 0);
    if(res)
    {
        kprintf("ERROR: %s f_lseek returned %d.\r\n", FileName, res);
        return res;
    }

    res = f_write(&fil, (void*) SourceAddress, ByteLength, &bw);
    if(res)
    {
        kprintf("ERROR: %s f_write returned %d.\r\n", FileName, res);
        return res;
    }

    res = f_close(&fil);
    if(res)
    {
        kprintf("ERROR: %s f_close returned %d.\r\n", FileName, res);
        return res;
    }
    return res;
}


FRESULT scan_files(char *path)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path);

    char pathBuff[256];

    if(res == FR_OK)
    {
        for( ; ; )
        {
            res = f_readdir(&dir, &fno);
            if(res != FR_OK || fno.fname[0] == 0)
            {
                break;
            }

            if(fno.fattrib & AM_DIR)
            {
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                kprintf("%s \r\n", path);
                res = scan_files(path);
                if(res != FR_OK)
                {
                    break;
                }
                path[i] = 0;
            }
            else
            {
                kprintf("%s/%s \r\n", path, fno.fname);
                strcpy(pathBuff, fno.fname);
            }
        }
    }
    else
    {
        kprintf("Failed - %s", &res);
    }
    f_closedir(&dir);
    return res;
}
