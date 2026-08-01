#include "app_sd_card.h"

#ifdef USING_SD_CARD

#define USING_ULOG
#include "dbg/ulog.h"

static FATFS sd_fatfs;

int app_sd_init(void)
{
    FRESULT res;

    res = fatfs_init(&sd_fatfs, SD_FS);
    if (XST_SUCCESS != res)
    {
        kprintf("ERROR: fail to open SD Card.\r\n");
    }
    else
    {
        kprintf("Success to open SD Card.\r\n");
    }

    kprintf("Scan Files in %s: \r\n", SD_FS);

    scan_files(SD_FS);

    return 0;
}

#endif
