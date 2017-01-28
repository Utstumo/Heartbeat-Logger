EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:utstumo
LIBS:IFS-Special
LIBS:ok-74logic
LIBS:ok-con-generic
LIBS:ok-diodes
LIBS:ok-generic
LIBS:ok-ic-analog
LIBS:ok-ic-atmel
LIBS:ok-ic-com
LIBS:ok-ic-memory
LIBS:ok-ic-power
LIBS:ok-ic-special
LIBS:ok-ic-stm32
LIBS:ok-jacks
LIBS:ok-opto
LIBS:ok-power
LIBS:ok-relay
LIBS:ok-switches
LIBS:ok-transformers
LIBS:ok-transistors
LIBS:IFS-Capacitors
LIBS:IFS-Connectors
LIBS:IFS-Diodes
LIBS:IFS-Displays
LIBS:IFS-Microcontrollers
LIBS:IFS-Motor.Drivers
LIBS:IFS-Power
LIBS:IFS-Resistors
LIBS:IFS-Sensors
LIBS:IFS-Switches
LIBS:IFS-Transformers
LIBS:IFS-Transistors
LIBS:IFS-Voltage.Regulators
LIBS:IFS-XTALs
LIBS:Heartbeat_Logger-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L SAMD21E17A U2
U 1 1 581223D3
P 5350 3400
F 0 "U2" H 4300 4800 50  0000 C CNN
F 1 "SAMD21E17A" H 6200 2000 50  0000 C CNN
F 2 "Utstumo:SAMD21_QFP_32_handsolder" H 5350 2400 50  0001 C CIN
F 3 "" H 5350 3400 50  0000 C CNN
	1    5350 3400
	-1   0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 581225BF
P 6950 1900
F 0 "R7" H 6800 1959 50  0000 L BNN
F 1 "10k" H 6800 1770 50  0000 L BNN
F 2 "Resistors_SMD:R_0603_HandSoldering" H 6950 2050 50  0001 C CNN
F 3 "" H 6950 1900 60  0000 C CNN
	1    6950 1900
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR027
U 1 1 58122626
P 8400 1900
F 0 "#PWR027" H 8400 1750 50  0001 C CNN
F 1 "+3V3" H 8400 2040 50  0000 C CNN
F 2 "" H 8400 1900 50  0000 C CNN
F 3 "" H 8400 1900 50  0000 C CNN
	1    8400 1900
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR028
U 1 1 58122651
P 6950 1700
F 0 "#PWR028" H 6950 1550 50  0001 C CNN
F 1 "+3V3" H 6950 1840 50  0000 C CNN
F 2 "" H 6950 1700 50  0000 C CNN
F 3 "" H 6950 1700 50  0000 C CNN
	1    6950 1700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 58122690
P 6700 4800
F 0 "#PWR029" H 6700 4550 50  0001 C CNN
F 1 "GND" H 6700 4650 50  0000 C CNN
F 2 "" H 6700 4800 50  0000 C CNN
F 3 "" H 6700 4800 50  0000 C CNN
	1    6700 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 4550 6700 4800
Wire Wire Line
	6700 4650 6600 4650
Wire Wire Line
	6600 4550 6700 4550
Connection ~ 6700 4650
$Comp
L Crystal X1
U 1 1 58122953
P 7700 5550
F 0 "X1" H 7625 5650 50  0000 L CNN
F 1 "XTAL-32.768kHz-Cilindrical-SMD" H 7700 5250 50  0001 L CNN
F 2 "Nova:ABS07-32.768KHZ-T" H 7700 5150 30  0001 L CNN
F 3 "" V 7575 5600 60  0000 C CNN
F 4 "32.678kHz" H 7700 5450 50  0000 C CNN "E.Value"
	1    7700 5550
	1    0    0    -1  
$EndComp
Text GLabel 3850 2150 0    60   Input ~ 0
XIN32
Text GLabel 7050 5550 0    60   Input ~ 0
XIN32
Wire Wire Line
	7050 5550 7550 5550
Text GLabel 8400 5550 2    60   Input ~ 0
XOUT32
Wire Wire Line
	7850 5550 8400 5550
Text GLabel 3850 2250 0    60   Input ~ 0
XOUT32
Wire Wire Line
	3850 2250 4100 2250
Wire Wire Line
	4100 2150 3850 2150
$Comp
L C C7
U 1 1 58122DF7
P 7200 5900
F 0 "C7" H 7300 6100 50  0000 L BNN
F 1 "6.8pF" H 7300 6000 50  0000 L BNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 7200 6050 50  0001 C CNN
F 3 "" H 7200 5900 60  0000 C CNN
	1    7200 5900
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 58122E49
P 8200 5900
F 0 "C8" H 8300 6100 50  0000 L BNN
F 1 "6.8pF" H 8300 6000 50  0000 L BNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 8200 6050 50  0001 C CNN
F 3 "" H 8200 5900 60  0000 C CNN
	1    8200 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 5550 8200 5750
Connection ~ 8200 5550
Wire Wire Line
	7200 5550 7200 5750
Connection ~ 7200 5550
$Comp
L GND #PWR030
U 1 1 58122ECB
P 8200 6050
F 0 "#PWR030" H 8200 5800 50  0001 C CNN
F 1 "GND" H 8200 5900 50  0000 C CNN
F 2 "" H 8200 6050 50  0000 C CNN
F 3 "" H 8200 6050 50  0000 C CNN
	1    8200 6050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 58122EF5
P 7200 6050
F 0 "#PWR031" H 7200 5800 50  0001 C CNN
F 1 "GND" H 7200 5900 50  0000 C CNN
F 2 "" H 7200 6050 50  0000 C CNN
F 3 "" H 7200 6050 50  0000 C CNN
	1    7200 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 2100 8600 2050
Wire Wire Line
	8600 2050 8250 2050
Wire Wire Line
	8250 2050 8250 2100
Wire Wire Line
	8400 1900 8400 2050
Connection ~ 8400 2050
$Comp
L +3.3VADC #PWR032
U 1 1 5812335D
P 6850 2950
F 0 "#PWR032" H 7000 2900 50  0001 C CNN
F 1 "+3.3VADC" H 6850 3050 50  0000 C CNN
F 2 "" H 6850 2950 50  0000 C CNN
F 3 "" H 6850 2950 50  0000 C CNN
	1    6850 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 2950 6850 3050
Wire Wire Line
	6850 3050 6600 3050
$Comp
L C C10
U 1 1 5812350C
P 8600 2250
F 0 "C10" H 8700 2450 50  0000 L BNN
F 1 "100nF" H 8700 2350 50  0000 L BNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 8600 2400 50  0001 C CNN
F 3 "" H 8600 2250 60  0000 C CNN
	1    8600 2250
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 5812358D
P 8250 2250
F 0 "C9" H 8100 2450 50  0000 L BNN
F 1 "100nF" H 8000 2350 50  0000 L BNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 8250 2400 50  0001 C CNN
F 3 "" H 8250 2250 60  0000 C CNN
	1    8250 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 2050 6950 2150
Wire Wire Line
	6600 2150 7250 2150
$Comp
L GND #PWR033
U 1 1 58123D29
P 8400 2500
F 0 "#PWR033" H 8400 2250 50  0001 C CNN
F 1 "GND" H 8400 2350 50  0000 C CNN
F 2 "" H 8400 2500 50  0000 C CNN
F 3 "" H 8400 2500 50  0000 C CNN
	1    8400 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 2400 8600 2450
Wire Wire Line
	8600 2450 8250 2450
Wire Wire Line
	8250 2450 8250 2400
Wire Wire Line
	8400 2450 8400 2500
Connection ~ 8400 2450
$Comp
L +3V3 #PWR034
U 1 1 58123E99
P 6850 2450
F 0 "#PWR034" H 6850 2300 50  0001 C CNN
F 1 "+3V3" H 6850 2590 50  0000 C CNN
F 2 "" H 6850 2450 50  0000 C CNN
F 3 "" H 6850 2450 50  0000 C CNN
	1    6850 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2550 6850 2550
Wire Wire Line
	6850 2450 6850 2700
Wire Wire Line
	6850 2700 6600 2700
Connection ~ 6850 2550
Text GLabel 3900 4550 0    60   Input ~ 0
SWDCLK
Wire Wire Line
	3900 4550 4100 4550
Text GLabel 3900 4650 0    60   Input ~ 0
SWDIO
Wire Wire Line
	3900 4650 4100 4650
Text GLabel 7250 2150 2    60   Input ~ 0
nRESET
Connection ~ 6950 2150
Text GLabel 3900 2950 0    60   Input ~ 0
SPI_MOSI
Wire Wire Line
	3900 2950 4100 2950
Text GLabel 3900 3050 0    60   Input ~ 0
SPI_SCK
Text GLabel 3900 3150 0    60   Input ~ 0
SPI_MISO
Wire Wire Line
	3900 3050 4100 3050
Wire Wire Line
	4100 3150 3900 3150
Text GLabel 3900 3250 0    60   Input ~ 0
SPI_nSS
Wire Wire Line
	3900 3250 4100 3250
Text GLabel 3900 3550 0    60   Input ~ 0
BT_TX
Text GLabel 3900 3650 0    60   Input ~ 0
BT_RX
Text GLabel 3900 3750 0    60   Input ~ 0
BT_status
Text GLabel 3900 3850 0    60   Input ~ 0
BT_enable
Wire Wire Line
	3900 3550 4100 3550
Wire Wire Line
	4100 3650 3900 3650
Wire Wire Line
	3900 3750 4100 3750
Wire Wire Line
	4100 3850 3900 3850
Text GLabel 3900 3450 0    60   Input ~ 0
LED0
Text GLabel 3900 3350 0    60   Input ~ 0
LED1
Wire Wire Line
	3900 3450 4100 3450
Wire Wire Line
	3900 3350 4100 3350
Text GLabel 3850 2750 0    60   Input ~ 0
BATTSENSE_output
Text GLabel 3850 2850 0    60   Input ~ 0
AFE_output
Wire Wire Line
	3850 2750 4100 2750
Wire Wire Line
	4100 2850 3850 2850
Text GLabel 3900 4450 0    60   Input ~ 0
Button
Wire Wire Line
	3900 4450 4100 4450
Text GLabel 3900 3950 0    60   Input ~ 0
BATTSENSE_enable
Text GLabel 3900 4350 0    60   Input ~ 0
CHG_status
Text GLabel 3900 4150 0    60   Input ~ 0
SD_enable
Wire Wire Line
	3900 3950 4100 3950
Wire Wire Line
	4100 4350 3900 4350
Wire Wire Line
	3900 4150 4100 4150
Text GLabel 3900 4250 0    60   Input ~ 0
AFE_enable
Wire Wire Line
	4100 4250 3900 4250
NoConn ~ 4100 2350
NoConn ~ 4100 2450
NoConn ~ 4100 2550
NoConn ~ 4100 2650
Wire Wire Line
	6950 1700 6950 1750
$EndSCHEMATC
