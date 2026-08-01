#ifndef __SD_CARD_H__
#define __SD_CARD_H__

#include "platform_config.h"

#ifdef USING_SD_CARD

#include "xparameters.h"
#include "ff.h"
#include "xdevcfg.h"

FRESULT fatfs_init(FATFS *fatfs, TCHAR *path);
FRESULT sd_read_data(char *FileName, uint32_t DestinationAddress, uint32_t ByteLength);
FRESULT sd_write_data(char *FileName, uint32_t SourceAddress, uint32_t ByteLength);
FRESULT scan_files(char *path);

#endif
#endif
