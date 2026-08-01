#include "app_sd_card.h"

#ifdef USING_SD_CARD
#ifdef USING_SD_CARD_TEST

#define USING_ULOG
#include "dbg/ulog.h"

#define SD_FILE "0:SD_TEST.txt"

static FRESULT sd_rw_test(void)
{
    FRESULT res;
    const char src_str[] = "ZYNQ test SD card write and read!";
    char dest_str[33];
    uint32_t len = strlen(src_str);

    res = sd_write_data(SD_FILE, (uint32_t)src_str, len);
    if (XST_SUCCESS != res)
    {
        kprintf("ERROR: fail to write SD Card.\r\n");
        return res;
    }
    else
    {
        kprintf("Success to write SD Card.\r\n");
    }

    res = sd_read_data(SD_FILE, (uint32_t)dest_str, len);
    if (XST_SUCCESS != res)
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

int sd_card_test_do_once(void)
{
    app_sd_init();
    sd_rw_test();
    return 0;
}

#endif
#endif
