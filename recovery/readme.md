## Rocket Radio/GPS Recovery System

This module of the rocket contains the microcontroller programming for the GPS system,
 the radio transmitter, and the data logging during a launch. The PCB design has provided the
 system required to ensure that these individual components, after construction, are operational.
 This document details the motivation for this package, how to implement it, any major design 
issues encountered, and finally any potential future work to build on.

### Purpose 

Amateur rockets get lost often. Typically, with larger scale rockets with possible 
ranges of the kilometres, there are serious issues with finding the package afterwards. To 
mitigate these long-range detection issues, a recovery system was implemented into the 
rocket package. This module provides convenient positional information of the rocket's whereabouts after a launch. Furthermore, it can log data into the micro SD card slot provided with the Teensy 3.6 microcontroller. Additionally, the system was made more robust to handle possible failures such as final orientation of the rocket after landing, long range radio systems for obstructive terrain, and simple ‘whip’ antennae for minimising directionality.

### Features:
* Long range RFM96W 433MHz radio system transmitting consistently and constantly to provide a signal to track and detect. At least 600 metres in an obstructive and involved environment (multiple concrete and metal buildings)
* Transmitted GPS coordinate data of the rocket system, as well as operational time and signal strength.
* GPS achieves lock within 10 minutes
* Dual antenna GPS system which is switched often to allow for maximal coverage and chance for the GPS system to get useable satellite signals.
* Micro SD card writing at every time a message is initialised, allowing for diagnostic information of the rocket to be tracked and maintained.
* Serial output receiver system that can pick up the transmitted data remotely from the rocket – includes remote diagnostics
* Passive antenna system on both the onboard_module and the home_base.

### Using it:
The system can be used by uploading the appropriate code to a constructed system – refer to the PCB documentation on construction. The system used can be the Arduino IDE (https://www.arduino.cc/en/Main/Software) with Teensyduino (https://www.pjrc.com/teensy/teensyduino.html) addon. If the Arduino IDE is being used for the system, then the required libraries need to be on /User/Arduino/libraries directory.

The following libraries were utilised for this module of the system:
* RH_RF95.h RadioHead library for Arduino radio interfacing (http://www.airspayce.com/mikem/arduino/RadioHead/classRH__RF95.html)
* SD.h provides functionality to write onto the SD card on the rocket module (https://www.arduino.cc/en/Reference/SD)
* SPI.h provides functionality on reading the radio on the system (https://www.arduino.cc/en/Reference/SPI)
* TimeLib.h for parsing the time elapsed on the system (https://github.com/PaulStoffregen/Time/blob/master/TimeLib.h)
* TinyGPS++ parses the Serial GPS data into usable data to transmit (http://arduiniana.org/libraries/tinygpsplus/)

To use the system, the documentation recovery guide details how to read data from the home_base system.

Upload the receiver (home_base) system:
* Upload home_base_rev2.ino onto the revision 2 PCB or home_base _rev3.ino onto the revision 3 PCB. Only difference is the wiring of the radio module through SPI (Serial Peripheral Interface) communication.
Upload the transmitter (onboard_module) system:
* Upload onboard_module_rev2.ino onto the revision 2 PCB or onboard_modul_rev3.ino onto the revision 3 PCB. Only difference is the wiring of the radio module through SPI (Serial Peripheral Interface) communication. Note that revision 2 does not have a GPS module onboard so it cannot give any numerical positional information.

### Developmental Notes/Issues:
* A GPS is inherently difficult to test without a source antenna indoors to take the place of a satellite. It requires making the system work outdoors to confirm operationality
* The considerable risk of the project was that the impedance of the track towards the GPS antenna would not be functional. Therefore, further PCB development needs to be very careful with that (and should consult the A2200-A datasheet).
* The antenna on the RFM96W radio is a simple quarter length whip antenna soldered to the antenna pad. This will need to be removed if anyone wants to use other antennas through the conventional SMA connector. The whip antenna is sufficient for sub 1km retrievals
* There were missing parts to revision 2 and revision 3 of the PCB such as required pull up resistors or required wire inputs. This was especially true for the GPS.
* Be careful with reading the datasheet and PCB circuit diagrams. This system currently only uses SPI for the radio and SD card, and serial for the GPS module.
* Utilising I2C for IMUs (inertial measurement units) is difficult and not typically reliable. The Teensy 3.6 will REQUIRE pullup resistors to successfully communicate on this protocol.
* The Arduino IDE with Teensyduino is reliable after getting the appropriate libraries.

### Potential Future Work
* Add IMU readings to change the antenna being used according to the orientation of the rocket. Will lead to more consistency than continual switching.
* The radio system can have an addressing system to make it more compatible with diverse types of 433MHz radios. Only using the RFM96W is a feature limitation in the development of the system.
* An external directional antenna would increase the range finding on the system significantly. Also, an active antenna can possibly lead to a more advantageous range.
* Fully encasing the radio system would make it more robust in outdoor conditions





