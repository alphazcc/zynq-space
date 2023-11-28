#include "sd_card.h"
#include "xil_printf.h"

#define kprintf xil_printf

#define SD_FS   "0:/"
#define SD_FILE "0:SD_TEST.txt"

static FATFS sd_fatfs;

static FRESULT sd_rw_test(void)
{
    FRESULT res;
    const char src_str[] = "ZYNQ test SD card write and read!";
    char dest_str[33];
    uint32_t len = strlen(src_str);

    res = sd_write_data(SD_FILE, (uint32_t)src_str, len);
    if(XST_SUCCESS != res)
    {
        kprintf("ERROR: fail to write SD Card.\r\n");
        return res;
    }
    else
    {
        kprintf("Success to write SD Card.\r\n");
    }

    res = sd_read_data(SD_FILE, (uint32_t)dest_str, len);
    if(XST_SUCCESS != res)
    {
        kprintf("ERROR: fail to read SD Card.\r\n");
        return res;
    }
    else
    {
        kprintf("Success to read SD Card; data: %s \r\n", dest_str);
    }

    kprintf("SD Card Write and Read test end.\r\n");
    return res;
 }

int sd_cmd(void)
{
    FRESULT res;

    res = fatfs_init(&sd_fatfs, SD_FS);
    if(XST_SUCCESS != res)
    {
        kprintf("ERROR: fail to open SD Card.\r\n");
    }
    else
    {
        kprintf("Success to open SD Card.\r\n");
    }

    sd_rw_test();

    kprintf("Scan Files in %s: \r\n", SD_FS);
    
    scan_files(SD_FS);

    return 0;
}
