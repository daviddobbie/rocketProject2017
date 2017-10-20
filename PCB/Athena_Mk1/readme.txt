#ROCKET PCB
##FEATURES

- Teensy 3.6
- A2200A GPS
    - includes precise impedance-matched traces to the antennae (50ohm)
- RFM96W Radio
    - SPI connection
- MPU9250 IMU
    - I2C connection
- Two servo footprints (unused)
- Piezoelectric buzzer (unused)
- Igniter (unused)
- Regulated power supply
    - Includes bypass cap, diode, and 5V regulator for safety

##PCB EVALUATION
The embedded GPS was tested and found to work well (see Radio/GPS Recovery System Evaluation notes).
The board lacks buffer capacitors, adding these should improve robustness and help prevent brownouts.
A separate power supply should be considered for the igniter as it will currently have a large risk of browning out the board when used, as it is not current limited. 
Oversized for the small-scale (C class) cone rocket, so was not flight tested.

##POTENTIAL FUTURE WORK

Some greenwired parts can be given proper traces
    - GPS enable pin to a digital pin on teensy
    - Pull up resistors on gps spi pins
Isolated power supply for servos, igniter to prevent brownouts
Replace through hole components with SMD equivalents (especially the 7805 regulator) to save on space and weight
Replace IMU footprint with new IMU component
Add buffer / bypass caps 
Cut down size where possible.
