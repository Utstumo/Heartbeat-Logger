/**
 * \file
 *
 * \brief User board configuration template
 *
 * Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define F_CPU 1000000UL

#define SD_MOSI_PIN PIN_PA09
#define SD_MISO_PIN PIN_PA08
#define SD_CLK_PIN PIN_PA10
#define SD_SS_PIN PIN_PA11
#define SD_EN_PIN PIN_PA24
#define SD_DETECT_PIN PIN_PA15

//LEDs
#define RLED_PIN PIN_PA05
#define GLED_PIN PIN_PA06

//Other digital
#define EXT_ENABLE_PIN PIN_PA19
#define EXT_STATUS_PIN PIN_PA18
#define BATTSENSE_ENABLE_PIN PIN_PA22
#define CHG_STATUS_PIN PIN_PA23
#define SDCARD_ENABLE_PIN PIN_PA24
#define AFE_ENABLE_PIN PIN_PA27

//Analog
#define SIGNAL_PIN PIN_PA03
#define SIGNAL_ADC_INPUT ADC_POSITIVE_INPUT_PIN1
#define BATTSENSE_PIN PIN_PA02
#define BATTSENSE_ADC_INPUT ADC_POSITIVE_INPUT_PIN2

//Switch
#define SW0_PIN PIN_PA07
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_EIC_PIN               PIN_PA07A_EIC_EXTINT7
#define SW0_EIC_MUX               MUX_PA07A_EIC_EXTINT7
#define SW0_EIC_PINMUX            PINMUX_PA07A_EIC_EXTINT7
#define SW0_EIC_LINE              7


#define CHG_EIC_PIN               PIN_PA23A_EIC_EXTINT7
#define CHG_EIC_MUX               MUX_PA23A_EIC_EXTINT7
#define CHG_EIC_PINMUX            PINMUX_PA23A_EIC_EXTINT7
#define CHG_EIC_LINE              7


#endif // CONF_BOARD_H
