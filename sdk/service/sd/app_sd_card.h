#ifndef __APP_SD_H__
#define __APP_SD_H__

#include "sd/sd_card.h"

#ifdef USING_SD_CARD

#define SD_FS "0:/"

int app_sd_init(void);

#endif
#endif
