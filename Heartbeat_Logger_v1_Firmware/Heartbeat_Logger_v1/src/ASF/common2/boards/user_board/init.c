/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
#elif defined(__ICCARM__)
void board_init(void);
#  pragma weak board_init=system_board_init
#endif

void system_board_init(void)

{
	
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);

	/* Configure LEDs as outputs, turn them off */
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(GLED_PIN, &pin_conf);
	port_pin_set_config(RLED_PIN, &pin_conf);
	port_pin_set_output_level(GLED_PIN, LOW);
	port_pin_set_output_level(RLED_PIN, LOW);
	
	//Analogue front end
	port_pin_set_config(AFE_ENABLE_PIN, &pin_conf);
	port_pin_set_output_level(AFE_ENABLE_PIN, LOW);
	
	//SD card enable
	port_pin_set_config(SDCARD_ENABLE_PIN, &pin_conf);
	port_pin_set_output_level(SDCARD_ENABLE_PIN, LOW);
	
	//Battsense
	port_pin_set_config(BATTSENSE_ENABLE_PIN, &pin_conf);
	port_pin_set_output_level(BATTSENSE_ENABLE_PIN, LOW);
	
	
	//External addons, like HC06
	port_pin_set_config(EXT_ENABLE_PIN, &pin_conf);
	port_pin_set_output_level(EXT_ENABLE_PIN, LOW);
	
	//Sd card enable (not the same as SS or CS)
	port_pin_set_config(SDCARD_ENABLE_PIN, &pin_conf);
	port_pin_set_output_level(SDCARD_ENABLE_PIN, HIGH);
	
	//Inputs
	port_get_config_defaults(&pin_conf);
	pin_conf.direction = PORT_PIN_DIR_INPUT;
	
	port_pin_set_config(SW0_PIN, &pin_conf);
	port_pin_set_config(CHG_STATUS_PIN, &pin_conf);
	
	pin_conf.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(EXT_STATUS_PIN, &pin_conf);

	
			
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
}