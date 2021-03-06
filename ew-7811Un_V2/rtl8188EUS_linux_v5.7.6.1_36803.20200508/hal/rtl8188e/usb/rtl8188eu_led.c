/******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/

#include <drv_types.h>
#include <rtl8188e_hal.h>
#ifdef CONFIG_RTW_SW_LED

/* ********************************************************************************
 * LED object.
 * ******************************************************************************** */


/* ********************************************************************************
 *	Prototype of protected function.
 * ******************************************************************************** */


/* ********************************************************************************
 * LED_819xUsb routines.
 * ******************************************************************************** */

/*
 *	Description:
 *		Turn on LED according to LedPin specified.
 *   */
static void
SwLedOn_8188EU(
	_adapter			*padapter,
	PLED_USB		pLed
)
{
	u8	LedCfg, gpio_mode;
	u32 gpio_value;
	/* HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter); */

	if (RTW_CANNOT_RUN(padapter))
		return;

	LedCfg = rtw_read8(padapter, REG_LEDCFG2);
	gpio_mode = rtw_read8(padapter, 0x40);
	gpio_value = rtw_read32(padapter, 0x44);
#if 0
	switch (pLed->LedPin) {
	case LED_PIN_LED0:
		rtw_write8(padapter, REG_LEDCFG2, (LedCfg & 0xf0) | BIT5 | BIT6); /* SW control led0 on. */
		break;

	case LED_PIN_LED1:
		rtw_write8(padapter, REG_LEDCFG2, (LedCfg & 0x0f) | BIT5); /* SW control led1 on. */
		break;

	default:
		break;
	}
#endif
	rtw_write8(padapter, REG_LEDCFG2, LedCfg & (~BIT5));
	rtw_write8(padapter, 0x40, gpio_mode & (~(BIT0 | BIT1)));
	gpio_value |= (BIT5 << 24);
	gpio_value |= (BIT5 << 16);
	gpio_value &= (~(BIT5 << 8));
	rtw_write32(padapter, 0x44, gpio_value);

	pLed->bLedOn = _TRUE;
}


/*
 *	Description:
 *		Turn off LED according to LedPin specified.
 *   */
static void
SwLedOff_8188EU(
	_adapter			*padapter,
	PLED_USB		pLed
)
{
	u8	LedCfg, gpio_mode;
	u32 gpio_value;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);

	if (RTW_CANNOT_RUN(padapter))
		goto exit;


	LedCfg = rtw_read8(padapter, REG_LEDCFG2);/* 0x4E */

	gpio_mode = rtw_read8(padapter, 0x40);
	gpio_value = rtw_read32(padapter, 0x44);
#if 0
	switch (pLed->LedPin) {
	case LED_PIN_LED0:
		if (pHalData->bLedOpenDrain == _TRUE) { /* Open-drain arrangement for controlling the LED) */
			LedCfg &= 0x90; /* Set to software control.				 */
			rtw_write8(padapter, REG_LEDCFG2, (LedCfg | BIT3));
			LedCfg = rtw_read8(padapter, REG_MAC_PINMUX_CFG);
			LedCfg &= 0xFE;
			rtw_write8(padapter, REG_MAC_PINMUX_CFG, LedCfg);
		} else
			rtw_write8(padapter, REG_LEDCFG2, (LedCfg | BIT3 | BIT5 | BIT6));
		break;

	case LED_PIN_LED1:
		LedCfg &= 0x0f; /* Set to software control. */
		rtw_write8(padapter, REG_LEDCFG2, (LedCfg | BIT3));
		break;

	default:
		break;
	}
#endif
	rtw_write8(padapter, REG_LEDCFG2, LedCfg & (~BIT5));
	rtw_write8(padapter, 0x40, gpio_mode & (~(BIT0 | BIT1)));
	gpio_value |= (BIT5 << 24);
	gpio_value |= (BIT5 << 16);
	gpio_value |= (BIT5 << 8);
	rtw_write32(padapter, 0x44, gpio_value);

exit:
	pLed->bLedOn = _FALSE;

}

/* ********************************************************************************
 * Interface to manipulate LED objects.
 * ******************************************************************************** */


/* ********************************************************************************
 * Default LED behavior.
 * ******************************************************************************** */

/*
 *	Description:
 *		Initialize all LED_871x objects.
 *   */
void
rtl8188eu_InitSwLeds(
	_adapter	*padapter
)
{
	struct led_priv *pledpriv = adapter_to_led(padapter);

	pledpriv->LedControlHandler = LedControlUSB;

	pledpriv->SwLedOn = SwLedOn_8188EU;
	pledpriv->SwLedOff = SwLedOff_8188EU;

	InitLed(padapter, &(pledpriv->SwLed0), LED_PIN_LED0);

	InitLed(padapter, &(pledpriv->SwLed1), LED_PIN_LED1);
}


/*
 *	Description:
 *		DeInitialize all LED_819xUsb objects.
 *   */
void
rtl8188eu_DeInitSwLeds(
	_adapter	*padapter
)
{
	struct led_priv	*ledpriv = adapter_to_led(padapter);

	DeInitLed(&(ledpriv->SwLed0));
	DeInitLed(&(ledpriv->SwLed1));
}
#endif
