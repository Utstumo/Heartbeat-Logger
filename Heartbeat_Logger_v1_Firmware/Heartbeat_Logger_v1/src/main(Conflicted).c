/**
 * The Heartbeat Logger Firmware by Ole Andreas Utstumo, 2016. 
 
 This file is part of Heartbeat Logger Firmware.

 Heartbeat Logger Firmware is free software: you can redistribute it 
 and/or modify it under the terms of the GNU General Public License 
 as published by the Free Software Foundation, either version 3 of 
 the License, or (at your option) any later version.

 Heartbeat Logger Firmware is distributed in the hope that it will 
 be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Heartbeat Logger Firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * TODO:
 * - Make device sleep with STANDBY instead of IDLE
 * - Divide logging operation into three modes:
 *   - SD card only
 *   - SD card and bluetooth
 *	 - Bluetooth only
 * - Better synchronisation when streaming with bluetooth
 *
*/

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "ff.h"

//Buffer size of the DMA channels. 1024 bytes = 512 samples / second, since a sample is 2 bytes.
#define BUFFER_SIZE 512

//Number of times the user can save a timestamp in a session
#define BUTTONCOUNT_LIMIT 50

#define BATTERY_CHECK_TIMER_MAX BUFFER_SIZE
#define BATTERY_VOLTAGE_LOW 350
#define BATTERY_VOLTAGE_SHUTDOWN 335

FATFS FatFs;
FIL Fil;

//Buffer in the SRAM to temporarily store the data for the DMA channels
char buffer_a[BUFFER_SIZE];
char buffer_b[BUFFER_SIZE];
char *buffer_current;
char *buffer_last;

//But since the D20 has no DMA, we'll use just one of the buffers and a counter instead
volatile uint16_t buffer_counter = 0;

//
uint16_t write_counter = 0;

//Battery voltage in V/100
uint16_t battery_voltage = 0;

//Battery checking timer, will redirect the ADC to poll the battery voltage when 
uint16_t battery_check_counter = 0;

//Variables for the timestamp system.
volatile uint32_t timestamp = 0;
uint32_t buttonstamps[BUTTONCOUNT_LIMIT];
uint16_t buttoncounter = 0;



//Function pointer for the states of the state machine.
uint8_t (*function_pointer)(void);

//Setup
void configure_extosc32k(void);
void configure_tc_sampler(void);
void configure_tc_batt_callbacks(void);
static void configure_event_user(struct events_resource *resource);

//Functions
void check_button(void);

//Callbacks
void adc_complete_callback(struct adc_module *const module);
void usart_write_callback(struct usart_module *const usart_module);
void ADC_Handler();

//External interrupts
void configure_extint_channel(void);
void configure_extint_callbacks(void);
void extint_detection_callback(void);

//States
void die(void);
void sampling(void);
void sleep(void);
void wake(void);

//Macros
#define afe_enable() ioport_set_pin_level(AFE_ENABLE_PIN, HIGH)
#define afe_disable() ioport_set_pin_level(AFE_ENABLE_PIN, LOW)
#define battsense_enable() ioport_set_pin_level(BATTSENSE_ENABLE_PIN, HIGH)
#define battsense_disable() ioport_set_pin_level(BATTSENSE_ENABLE_PIN, LOW)
#define sdcard_enable() ioport_set_pin_level(SDCARD_ENABLE_PIN, HIGH)
#define sdcard_disable() ioport_set_pin_level(SDCARD_ENABLE_PIN, LOW)
#define ext_enable() ioport_set_pin_level(EXT_ENABLE_PIN, HIGH)
#define ext_disable() ioport_set_pin_level(EXT_ENABLE_PIN, LOW)
#define chg_is_charging() !(ioport_get_pin_level(CHG_STATUS_PIN))
#define ext_is_connected() ioport_get_pin_level(EXT_STATUS_PIN)
#define button_is_pressed() !(ioport_get_pin_level(SW0_PIN))
#define RLED_on() ioport_set_pin_level(RLED_PIN, HIGH)
#define RLED_off() ioport_set_pin_level(RLED_PIN, LOW)
#define GLED_on() ioport_set_pin_level(GLED_PIN, HIGH)
#define GLED_off() ioport_set_pin_level(GLED_PIN, LOW)


//Instances

extern struct adc_module adc_instance;
struct tc_module tc_sampler_instance;
struct adc_module adc_instance;
struct usart_module usart_instance;
struct events_resource tc_tick_event;
struct events_resource adc_resrdy_event;
struct events_hook     hook;

//Flags
volatile uint8_t flag_button = 0;
volatile uint8_t flag_buffer_ready = 0;
uint8_t flag_uart_sent = 0;
uint8_t flag_battery_read = 0;

DWORD get_fattime (void)
{
	/* Returns current time packed into a DWORD variable */
	return	  ((DWORD)(2013 - 1980) << 25)	/* Year 2013 */
	| ((DWORD)7 << 21)				/* Month 7 */
	| ((DWORD)28 << 16)				/* Mday 28 */
	| ((DWORD)0 << 11)				/* Hour 0 */
	| ((DWORD)0 << 5)				/* Min 0 */
	| ((DWORD)0 >> 1);				/* Sec 0 */
}

uint8_t sdcard_init(void){
	
	
	//Mount the memory card and create a file
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
	//Delete existing file
	f_unlink("ecgdata.bin");
	
	if (f_open(&Fil, "ecgdata.bin", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) 	/* Create a file */
	return 1;
	else
	return 0;
	
}

uint8_t sdcard_write(char *charbuffer, uint16_t size){
	
	UINT bw;
	//Write the data to the file
	
	f_write(&Fil, charbuffer, size, &bw);
	
	//set interrupt flag here
	if (bw == size)
	return 1;
	else
	return 0;
	
}

void sdcard_close(void){
	
	//Disable the analogue module
	
	//Write saved timestamps to the end of the file
	char stopbytes[2] = {0xFF, 0xFF};
	sdcard_write(stopbytes, 2);
	while(buttoncounter){
		//We better store the timestamps as 2 x uint16 little endian so Matlab can easily read the values along with the samples.
		//Endianess of the total uint32 doesn't matter.
		char timestamp_buffer[4] = {buttonstamps[buttoncounter-1]>>16&0xFF, buttonstamps[buttoncounter-1]>>24 & 0xFF,buttonstamps[buttoncounter-1]&0xFF,buttonstamps[buttoncounter-1]>>8&0xFF};
		buttoncounter--;
		sdcard_write(timestamp_buffer, 4);
	}
	//Close and sync the file. Must be done before powerdown or removal of sd card.
	f_close(&Fil);
}


void configure_extint_channel(void)
{
	//! [setup_1]
	struct extint_chan_conf config_extint_chan;
	//! [setup_1]
	//! [setup_2]
	extint_chan_get_config_defaults(&config_extint_chan);
	//! [setup_2]

	//! [setup_3]
	config_extint_chan.gpio_pin           = SW0_EIC_PIN;
	config_extint_chan.gpio_pin_mux       = SW0_EIC_MUX;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;
	config_extint_chan.wake_if_sleeping = true;
	//! [setup_3]
	//! [setup_4]
	extint_chan_set_config(SW0_EIC_LINE, &config_extint_chan);
	//! [setup_4]
	
	
}

void configure_extint_callbacks(void)
{
	//! [setup_5]
	extint_register_callback(extint_detection_callback,
	SW0_EIC_LINE,
	EXTINT_CALLBACK_TYPE_DETECT);
	//! [setup_5]
	//! [setup_6]
	extint_chan_enable_callback(SW0_EIC_LINE,
	EXTINT_CALLBACK_TYPE_DETECT);
	//! [setup_6]
}

void configure_usart(void){
	
		struct usart_config config_usart;
		//! [setup_config]
		//! [setup_config_defaults]
		usart_get_config_defaults(&config_usart);
		//! [setup_config_defaults]

		//! [setup_change_config]
		config_usart.baudrate    = 9600;
		config_usart.mux_setting = USART_RX_1_TX_0_XCK_1;
		config_usart.pinmux_pad0 = PINMUX_PA16C_SERCOM1_PAD0;
		config_usart.pinmux_pad1 = PINMUX_PA17C_SERCOM1_PAD1;
		config_usart.pinmux_pad2 = PINMUX_UNUSED;
		config_usart.pinmux_pad3 = PINMUX_UNUSED;
		//! [setup_change_config]

		//! [setup_set_config]
		while (usart_init(&usart_instance, SERCOM1, &config_usart) != STATUS_OK) {
		}
		//! [setup_set_config]

		//! [setup_enable]
		usart_enable(&usart_instance);
		//! [setup_enable]
}

void configure_usart_callback(void){
	//! [setup_register_callbacks]
	usart_register_callback(&usart_instance, usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	//! [setup_register_callbacks]

	//! [setup_enable_callbacks]
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	//! [setup_enable_callbacks]
	
}

void configure_adc(void){
	
		struct adc_config config_adc;
		adc_get_config_defaults(&config_adc);
		config_adc.gain_factor = ADC_GAIN_FACTOR_DIV2;
		config_adc.clock_prescaler = ADC_CLOCK_PRESCALER_DIV4;
		config_adc.reference = ADC_REFERENCE_INTVCC1;
		config_adc.positive_input = ADC_POSITIVE_INPUT_PIN1;
		config_adc.resolution = ADC_RESOLUTION_12BIT;
		
		config_adc.event_action = ADC_EVENT_ACTION_START_CONV;
		adc_init(&adc_instance, ADC, &config_adc);
		
		adc_enable(&adc_instance);
}

void configure_adc_callbacks(void){
	
	//adc_enable_interrupt(&adc_instance, ADC_INTERRUPT_RESULT_READY);
	REG_ADC_INTENSET = ADC_INTENSET_RESRDY;
	NVIC_EnableIRQ(ADC_IRQn);
	//NVIC_SetPriority(ADC_IRQn, 3);
	
}

void configure_extosc32k(void)
{

	struct system_clock_source_xosc32k_config config_ext32k;

	system_clock_source_xosc32k_get_config_defaults(&config_ext32k);

	config_ext32k.startup_time = SYSTEM_XOSC32K_STARTUP_4096;

	system_clock_source_xosc32k_set_config(&config_ext32k);
	
}

void configure_osc32k(void)
{
	//! [config_extosc32k_config]
	struct system_clock_source_osc32k_config config_32k;
	//! [config_extosc32k_config]
	//! [config_extosc32k_get_defaults]
	system_clock_source_osc32k_get_config_defaults(&config_32k);
	//! [config_extosc32k_get_defaults]
	config_32k.enable_1khz_output = false;
	//! [config_extosc32k_set_config]
	system_clock_source_osc32k_set_config(&config_32k);
	//! [config_extosc32k_set_config]
	
	volatile enum status_code osc32k_status = system_clock_source_enable(SYSTEM_CLOCK_SOURCE_OSC32K);

	if (osc32k_status != STATUS_OK) {
		ioport_set_pin_level(PIN_PA05, HIGH);
		/* Error enabling the clock source */
	}

	struct system_gclk_gen_config config_gclock_gen;
	system_gclk_gen_get_config_defaults(&config_gclock_gen);
	config_gclock_gen.source_clock    = SYSTEM_CLOCK_SOURCE_OSC32K;
	config_gclock_gen.division_factor = 16;
	config_gclock_gen.run_in_standby = true;
	
	system_gclk_gen_set_config(GCLK_GENERATOR_1, &config_gclock_gen);
}

void configure_tc_sampler(void)
{
	//! [setup_config]
	struct tc_config config_tc;
	struct tc_events config_events;
	//! [setup_config]
	//! [setup_config_defaults]
	tc_get_config_defaults(&config_tc);
	
	config_tc.clock_source = GCLK_GENERATOR_1;
	//! [setup_change_config]
	config_tc.counter_size    = TC_COUNTER_SIZE_8BIT;
	config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_FREQ;
	config_tc.counter_8_bit.period = 3;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
	
	//! [setup_change_config]

	//! [setup_change_config_pwm]
	config_tc.pwm_channel[0].enabled = false;
	//! [setup_change_config_pwm]

	//! [setup_set_config]
	tc_init(&tc_sampler_instance, TC0, &config_tc);
	//! [setup_set_config]

	config_events.generate_event_on_overflow = true;
	tc_enable_events(&tc_sampler_instance, &config_events);

	//! [setup_enable]
	tc_enable(&tc_sampler_instance);
	//! [setup_enable]
}

static void configure_event_channel(struct events_resource *resource)
{
	//! [setup_1]
	struct events_config config;
	//! [setup_1]

	//! [setup_2]
	events_get_config_defaults(&config);
	//! [setup_2]

	//! [setup_3]
	config.generator      = EVSYS_ID_GEN_TC0_OVF;
	config.edge_detect    = EVENTS_EDGE_DETECT_NONE;
	config.path           = EVENTS_PATH_ASYNCHRONOUS;
	config.clock_source   = GCLK_GENERATOR_0;

	//! [setup_3]

	//! [setup_4]
	events_allocate(resource, &config);
	//! [setup_4]
}


static void configure_event_channel_adc(struct events_resource *resource)
{
	//! [setup_1]
	struct events_config config;
	//! [setup_1]

	//! [setup_2]
	events_get_config_defaults(&config);
	//! [setup_2]

	//! [setup_3]
	config.generator      = EVSYS_ID_GEN_ADC_RESRDY;
	config.edge_detect    = EVENTS_EDGE_DETECT_NONE;
	config.path           = EVENTS_PATH_ASYNCHRONOUS;
	config.clock_source   = GCLK_GENERATOR_1;
	
	//! [setup_3]

	//! [setup_4]
	events_allocate(resource, &config);
	//! [setup_4]
}

static void configure_event_user(struct events_resource *resource)
{
	//! [setup_5]
	//An user for the event
	events_attach_user(resource, EVSYS_ID_USER_ADC_START);
	//! [setup_5]
}

// static void configure_event_interrupt(struct events_resource *resource,
// struct events_hook *hook)
// {
// 	//! [setup_12]
// 	events_create_hook(hook, &tc_ovf_callback);
// 	//! [setup_12]
// 	//! [setup_13]
// 	events_add_hook(resource, hook);
// 	events_enable_interrupt_source(resource, EVENTS_INTERRUPT_DETECT);
// 	//! [setup_13]
// }

int8_t add_timestamp(){
	if(buttoncounter<BUTTONCOUNT_LIMIT){
		buttonstamps[buttoncounter] = timestamp;
		buttoncounter++;
		return 1;
	}else{
		return -1;
	}
}

int8_t write_data(void){
	
	if(flag_buffer_ready){

		flag_buffer_ready = 0;
		
		if(battery_voltage > BATTERY_VOLTAGE_LOW)
			GLED_on();
		else
			RLED_on();		

		//write to sd card		
		if(!sdcard_write(buffer_last, BUFFER_SIZE))
			return -1;	
			
		if(battery_voltage > BATTERY_VOLTAGE_LOW)	
			GLED_off();
		else
			RLED_off();
		return 1;
	}
	return 0;
}

void check_button(void){
	
	if(flag_button){
		flag_button = 0;
		uint8_t i = 0;
		//Soft switch check
		//Button must be held x seconds to turn on / off
		//Otherwise, a timestamp will be saved while logging
		while(button_is_pressed() && i<255){
			for(int j = 0; j < 10000; j++);
			i++;
		}
		
		//Check duration and state
		if(i < 200 && function_pointer == sampling){
			add_timestamp();
		}else if(i >= 200){
			//Soft switch, selecting state
			if(function_pointer == sleep)
			function_pointer = &wake;
			else if(function_pointer == sampling || function_pointer == die)
			function_pointer = &sleep;
			else
			function_pointer = &sleep;
		}
	}
	
}

void update_battery_voltage(void){
	
	//We don't wanna store the ADC result in the buffers, so we disable interrupts
	cpu_irq_disable();
	battsense_enable();
	//swap ADC input, start ADC and wait till resrdy
	adc_set_positive_input(&adc_instance, ADC_POSITIVE_INPUT_PIN0);
	REG_ADC_SWTRIG |= ADC_SWTRIG_START;
	while(!(REG_ADC_INTFLAG & ADC_INTFLAG_RESRDY));
	
	//Interrupt flag is cleared by reading the result register
	battery_voltage = REG_ADC_RESULT;
	flag_battery_read = 1;
			
	//swap the ADC input back to the AFE
	adc_set_positive_input(&adc_instance, ADC_POSITIVE_INPUT_PIN1);
	
	battsense_disable();
	cpu_irq_enable();
	
}


int main (void)
{
	
	function_pointer = &wake;
	
	system_init();
	delay_init();
	configure_osc32k();
	configure_tc_sampler();	
	configure_adc();
	configure_usart();
	configure_usart_callback();
	configure_event_channel(&tc_tick_event);
	configure_event_user(&tc_tick_event);
	configure_adc_callbacks();
 	configure_extint_channel();
 	configure_extint_callbacks();
	 
	system_set_sleepmode(SYSTEM_SLEEPMODE_IDLE_2);
 	
 	system_interrupt_enable_global();
	 
	while (events_is_busy(&tc_tick_event)) {
		/* Wait for channel */
	};
	
	
	while(1){
		function_pointer();
		
	}
	/* Insert application code here, after the board has been initialized. */
}

void sleep(void){
	
	//Turn off everything
	sdcard_close();
	
	afe_disable();
	sdcard_disable();
	ext_disable();
	
	tc_stop_counter(&tc_sampler_instance);
	tc_disable(&tc_sampler_instance);
	adc_disable(&adc_instance);
	
	if(!chg_is_charging())
		RLED_off();
	GLED_off();
	RLED_off();
	
	//Go to sleep
	system_sleep();
	//On wakeup
	check_button();


}

void wake(void){
	
	function_pointer = sampling;
	
	GLED_on();
	
	afe_enable();
	sdcard_enable();
	ext_enable();
	
	tc_enable(&tc_sampler_instance);
	adc_enable(&adc_instance);
	
	if(!sdcard_init())
		function_pointer = die;
	
	buffer_counter = 0;
	timestamp = 0;
	buttoncounter = 0;	
	buffer_current = buffer_a;
	buffer_last = buffer_b;
	
	update_battery_voltage();

	tc_start_counter(&tc_sampler_instance);
	usart_write_buffer_job(&usart_instance, buffer_current, 2);

}

void sampling(void){
	
	write_data();
	check_button();
	
	if(flag_battery_read){
		flag_battery_read = 0;
		battery_voltage = battery_voltage * 2 * 33 * 100 / 40960;
	}
	
	//Power is low, save data and turn off
	if(battery_voltage < BATTERY_VOLTAGE_SHUTDOWN)
		function_pointer = die;
	
	if(chg_is_charging())
		RLED_on();
	else
		RLED_off();
}

void die(void){
	
	//Shutdown
	GLED_off();
	sdcard_close();
	tc_stop_counter(&tc_sampler_instance);
	
	//Todo: turn off unused peripherals
	while(function_pointer == die){
		delay_ms(500);
		ioport_toggle_pin_level(RLED_PIN);
		check_button();
	}
	
}

void ADC_Handler(){
	
	ioport_toggle_pin_level(EXT_STATUS_PIN);
	//Result is ready, put it in buffer
	if(buffer_counter < BUFFER_SIZE){
	
		uint16_t resultat = REG_ADC_RESULT;
		
		//store the result as little endian
		*(buffer_current + buffer_counter) = resultat;
		*(buffer_current + buffer_counter + 1) = 0xFF & (resultat>>8);
		buffer_counter+=2;
		
		if(buffer_counter >= BUFFER_SIZE){
			if(battery_check_counter++ > BATTERY_CHECK_TIMER_MAX){
				update_battery_voltage();
				battery_check_counter = 0;	
			}
			

			flag_buffer_ready = 1;
			buffer_counter = 0;
			
			char * buffer_temp;
			buffer_temp = buffer_current;
			buffer_current = buffer_last;
			buffer_last = buffer_temp;
						
		}	
		
	}else{
		
		//Buffer overflow
		RLED_on();
		
	}
		
	
}

void extint_detection_callback(void)
{
	if(button_is_pressed())
		flag_button = 1;
	else
		flag_button = 0;
}


void usart_write_callback(struct usart_module *const usart_module){
	char writeguard[3];
	if(buffer_counter >= 2)
		usart_write_buffer_job(&usart_instance, buffer_current + buffer_counter - 2, 2);
	else
		usart_write_buffer_job(&usart_instance, buffer_last + BUFFER_SIZE - 2, 2);
		
}

