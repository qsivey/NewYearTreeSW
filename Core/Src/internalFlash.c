/** ____________________________________________________________________
 *
 * 	@file		internalFlash.c
 *
 *	GitHub:		qsivey
 *	Telegram:	@qsivey
 *	Email:		qsivey@gmail.com
 *	____________________________________________________________________
 */

#include "internalFlash.h"


/* ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 *														   Class Methods
 */
void InternalFlashUnlock (void)
{
	HAL_FLASH_Unlock();
}


void InternalFlashLock (void)
{
	HAL_FLASH_Lock();
}


void InternalFlashSaveWord (ui32 address, ui32 word)
{
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, (ui64)word);
}


ui32 InternalFlashReadWord (ui32 address)
{
	__IO ui32 word = *(__IO ui32*)address;
	return (ui32)word;
}


void InternalFlashEraseSector (ui32 sector, ui8 sectorsNumber)
{
	FLASH_EraseInitTypeDef EraseInit = { 0 };
	ui32 pagesError;

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_BSY);

	EraseInit.TypeErase		= FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress	= sector;
	EraseInit.NbPages		= sectorsNumber;

	if (HAL_FLASHEx_Erase(&EraseInit, &pagesError) != HAL_OK)
		while (1);

	/* FLASH_Erase_Sector(sector, voltageRange); */
	for (ui16 i = 0; i < 2000; i++)
		__NOP();
}


void InternalFlashWriteParameters (__IO ui32 *pData, ui32 wordCount)
{
	InternalFlashUnlock();

	/* Sector erasing */
	InternalFlashEraseSector(FLASH_SETTINGS_ADDRESS, 1);

	__IO ui32 pAddress = FLASH_SETTINGS_ADDRESS;

	while (wordCount)
	{
		InternalFlashSaveWord(pAddress, *(pData));
		pAddress += 4;
		pData++;
		wordCount -= 4;
	}

	/* Flash locking */
	InternalFlashLock();
}
