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
LIBS:teensy
LIBS:rfm96w
LIBS:venus634_gps
LIBS:Athena_Mk1-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Teensy3.6 U1
U 1 1 5941C520
P 3200 3500
F 0 "U1" H 3200 5800 60  0000 C CNN
F 1 "Teensy3.6" H 3200 1200 60  0000 C CNN
F 2 "" H 3200 3500 60  0001 C CNN
F 3 "" H 3200 3500 60  0000 C CNN
	1    3200 3500
	1    0    0    -1  
$EndComp
Text GLabel 2200 1400 0    60   Input ~ 0
GND
Text GLabel 2200 1500 0    60   Input ~ 0
Rx
Text GLabel 2200 1600 0    60   Input ~ 0
Tx
Text GLabel 2200 2500 0    60   Input ~ 0
Rd_10
Text GLabel 2200 2600 0    60   Input ~ 0
Rd_11
Text GLabel 2200 2700 0    60   Input ~ 0
RD_12
Text GLabel 2200 5300 0    60   Input ~ 0
RD_13
$Comp
L D_ALT D1
U 1 1 59433AB8
P 5600 5000
F 0 "D1" H 5600 5100 50  0000 C CNN
F 1 "D_ALT" H 5600 4900 50  0000 C CNN
F 2 "Diodes_SMD:D_1206" H 5600 5000 50  0001 C CNN
F 3 "" H 5600 5000 50  0001 C CNN
	1    5600 5000
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X02 J1
U 1 1 59433C7B
P 5200 5350
F 0 "J1" H 5200 5500 49  0000 C CNN
F 1 "Power Supply" V 5300 5350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x02_Pitch2.54mm" H 5200 5350 50  0001 C CNN
F 3 "" H 5200 5350 50  0001 C CNN
	1    5200 5350
	-1   0    0    1   
$EndComp
Text GLabel 5900 5000 2    60   Input ~ 0
VIN
Text GLabel 5900 5700 2    60   Input ~ 0
GND
$Comp
L Buzzer BZ1
U 1 1 59433CFC
P 5750 1900
F 0 "BZ1" H 5900 1950 50  0000 L CNN
F 1 "Buzzer" H 5900 1850 50  0000 L CNN
F 2 "Buzzers_Beepers:BUZZER" V 5725 2000 50  0001 C CNN
F 3 "" V 5725 2000 50  0001 C CNN
	1    5750 1900
	1    0    0    -1  
$EndComp
$Comp
L CP1 C1
U 1 1 59433EAF
P 5800 5350
F 0 "C1" H 5825 5450 50  0000 L CNN
F 1 "CP1" H 5825 5250 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D10.0mm_P5.00mm" H 5800 5350 50  0001 C CNN
F 3 "" H 5800 5350 50  0001 C CNN
	1    5800 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5300 5400 5000
Wire Wire Line
	5400 5000 5450 5000
Wire Wire Line
	5750 5000 5900 5000
Wire Wire Line
	5800 5000 5800 5200
Wire Wire Line
	5400 5700 5400 5400
Wire Wire Line
	5400 5700 5900 5700
Wire Wire Line
	5800 5700 5800 5500
Connection ~ 5800 5000
Connection ~ 5800 5700
Text GLabel 2200 4000 0    60   Input ~ 0
GND
Text GLabel 4200 3900 2    60   Input ~ 0
GND
Text GLabel 4200 3800 2    60   Input ~ 0
GND
Text GLabel 4200 4700 2    60   Input ~ 0
VIN
Text GLabel 2200 5200 0    60   Input ~ 0
GND
Text GLabel 4200 2600 2    60   Input ~ 0
GND
Text GLabel 5650 2100 3    60   Input ~ 0
GND
$Comp
L BF244B Q1
U 1 1 5943505F
P 5550 1500
F 0 "Q1" H 5750 1575 50  0000 L CNN
F 1 "BF244B" H 5750 1500 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 5750 1425 50  0001 L CIN
F 3 "" H 5550 1450 50  0001 L CNN
	1    5550 1500
	1    0    0    -1  
$EndComp
Text GLabel 5250 1550 0    60   Input ~ 0
Bz_Ctrl
Text GLabel 5650 1200 1    60   Input ~ 0
5V
Wire Wire Line
	5650 2000 5650 2100
Wire Wire Line
	5650 1200 5650 1300
Wire Wire Line
	5350 1550 5250 1550
Text GLabel 4200 4200 2    60   Input ~ 0
5V
Text GLabel 2200 5500 0    60   Input ~ 0
Bz_Ctrl
$Comp
L RFM96W U2
U 1 1 59438706
P 5550 3050
F 0 "U2" H 5550 3500 60  0000 C CNN
F 1 "RFM96W" H 5550 2600 60  0000 C CNN
F 2 "TO_SOT_Packages_THT:TO-5-6" H 5550 3500 60  0001 C CNN
F 3 "" H 5550 3500 60  0001 C CNN
	1    5550 3050
	1    0    0    -1  
$EndComp
Text GLabel 6050 3000 2    60   Input ~ 0
3.3V
Text GLabel 4200 2500 2    60   Input ~ 0
3.3V
Text GLabel 5050 2700 0    60   Input ~ 0
GND
Text GLabel 5050 2800 0    60   Input ~ 0
Rd_12
Text GLabel 5050 2900 0    60   Input ~ 0
Rd_11
Text GLabel 5050 3000 0    60   Input ~ 0
Rd_13
Text GLabel 5050 3100 0    60   Input ~ 0
Rd_10
Text GLabel 5050 3400 0    60   Input ~ 0
GND
Text GLabel 6050 3300 2    60   Input ~ 0
GND
$Comp
L Venus634_GPS U3
U 1 1 5943917E
P 5550 4200
F 0 "U3" H 5550 4500 60  0000 C CNN
F 1 "Venus634_GPS" H 5550 3900 60  0000 C CNN
F 2 "" H 5550 4200 60  0001 C CNN
F 3 "" H 5550 4200 60  0001 C CNN
	1    5550 4200
	1    0    0    -1  
$EndComp
Text GLabel 6100 4050 2    60   Input ~ 0
GND
Text GLabel 6100 4150 2    60   Input ~ 0
Rx
Text GLabel 6100 4250 2    60   Input ~ 0
Tx
Text GLabel 6100 4350 2    60   Input ~ 0
3.3V
Text GLabel 5050 3200 0    60   Input ~ 0
3.3V
NoConn ~ 4200 1400
NoConn ~ 4200 1500
NoConn ~ 4200 1600
NoConn ~ 4200 1700
NoConn ~ 4200 1800
NoConn ~ 4200 1900
NoConn ~ 4200 2000
NoConn ~ 4200 2100
NoConn ~ 4200 2200
NoConn ~ 4200 2300
NoConn ~ 4200 2400
NoConn ~ 4200 2700
NoConn ~ 4200 2800
NoConn ~ 4200 2900
NoConn ~ 4200 3000
NoConn ~ 6050 2700
NoConn ~ 6050 2800
NoConn ~ 6050 2900
NoConn ~ 6050 3100
NoConn ~ 6050 3200
NoConn ~ 6050 3400
NoConn ~ 5050 3300
Wire Wire Line
	5650 1800 5650 1700
NoConn ~ 5000 4400
NoConn ~ 5000 4300
NoConn ~ 5000 4200
NoConn ~ 5000 4100
NoConn ~ 5000 4000
NoConn ~ 4200 5600
NoConn ~ 4200 5500
NoConn ~ 4200 5400
NoConn ~ 4200 5300
NoConn ~ 4200 5200
NoConn ~ 4200 5100
NoConn ~ 4200 5000
NoConn ~ 4200 4900
NoConn ~ 4200 4800
NoConn ~ 4200 4600
NoConn ~ 4200 4500
NoConn ~ 4200 4400
NoConn ~ 4200 4300
NoConn ~ 4200 4100
NoConn ~ 4200 4000
NoConn ~ 4200 3700
NoConn ~ 4200 3600
NoConn ~ 4200 3500
NoConn ~ 4200 3400
NoConn ~ 4200 3300
NoConn ~ 4200 3200
NoConn ~ 4200 3100
NoConn ~ 2200 1700
NoConn ~ 2200 1800
NoConn ~ 2200 1900
NoConn ~ 2200 2000
NoConn ~ 2200 2100
NoConn ~ 2200 2200
NoConn ~ 2200 2300
NoConn ~ 2200 2400
NoConn ~ 2200 2800
NoConn ~ 2200 5600
NoConn ~ 2200 5400
NoConn ~ 2200 5100
NoConn ~ 2200 5000
NoConn ~ 2200 4900
NoConn ~ 2200 4800
NoConn ~ 2200 4700
NoConn ~ 2200 4600
NoConn ~ 2200 4500
NoConn ~ 2200 4400
NoConn ~ 2200 4300
NoConn ~ 2200 4200
NoConn ~ 2200 4100
NoConn ~ 2200 3900
NoConn ~ 2200 3800
NoConn ~ 2200 3700
NoConn ~ 2200 3600
NoConn ~ 2200 3500
NoConn ~ 2200 3400
NoConn ~ 2200 3300
NoConn ~ 2200 3200
NoConn ~ 2200 3100
NoConn ~ 2200 3000
NoConn ~ 2200 2900
NoConn ~ 1350 3700
$EndSCHEMATC
