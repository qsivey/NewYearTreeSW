/** ____________________________________________________________________
 *
 * 	@file		internalFlash.h
 *
 *	GitHub:		qsivey
 *	Telegram:	@qsivey
 *	Email:		qsivey@gmail.com
 *	____________________________________________________________________
 */

#ifndef		INC_INTERNAL_FLASH_H_
#define		INC_INTERNAL_FLASH_H_

/*  = = = = = = = = = = = = = = = = = = = = = = = */

#include	"stm32f0xx_hal.h"
#include	"adjunct.h"


#define		FLASH_SETTINGS_ADDRESS	0x0800FC00UL


/* ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 *					   Static Functions & Prototypes
 */
static inline ui32 AlignCountByWord (ui32 count)
{
	return ((count / 4) * 4) + ((count % 4) ? 4 : 0);
}


static inline void FinishWordWithZeroes (ui32 *address, ui32 size)
{
	ui8 remain = size % 4;

	memset(address + size, 0, remain);
}


void InternalFlashUnlock (void);
void InternalFlashLock (void);
void InternalFlashSaveWord (ui32 address, ui32 word);
ui32 InternalFlashReadWord (ui32 address);
void InternalFlashEraseSector (ui32 sector, ui8 sectorsNumber);
void InternalFlashWriteParameters (__IO ui32 *pData, ui32 wordCount);


/*  = = = = = = = = = = = = = = = = = = = = = = = */

#endif		/* INC_INTERNAL_FLASH_H_ */
