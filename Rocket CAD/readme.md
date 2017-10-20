# Rocket CAD
All the rocket CAD files were designed in OnShape, a cloud based, open-source CAD software.    
The basic designs modelled with OpenRocket were converted to CAD models that could be 3D printed.
## Using OnShape
OnShape is similar to standard CAD software (Sloidworks, AutoCAD) except with the addition of version control an the ability to link between documents.   
Parts and assemblies are included in OnShape but it is advisable to contain all your parts in a single Part Studio to reduece complexity.
## Designs
### Athena Mk 1.0
Original team design that was discontinued in favor of the cone rocket.   
[OnShape Document](https://cad.onshape.com/documents/2019db2de40d4090ff84086a/w/bdaca4e6df1e9552dd54dee6/e/34c61646b283667b1d52ec77)
### Athena Mk 2.0
The Mk 2.0 is the passively stable, single stage, C-motor rocket.  
The rocket is aerodynamically stable due to the large “cone” that greatly increases the drag force that the rocket experiences. This was done as it reduced the overall rocket size as well as the maximum height the rocket would reach. Limiting the rocket apogee height with aerodynamic drag was chosen over limiting by increasing the rocket’s weight because it also improved the rocket’s aerodynamic stability.  
The motor mount is incorporated directly into the cone and a small hole is in the cone directly above the motor to vent the ejection charge. A second (square) hole was placed adjacent to the motor mount to allow the Athena Mk 2.0 to be placed on the launch rail. This did reduce the aerodynamic stability of the rocket but due to the low apogee point this was deemed to be an acceptable drawback.
### Athena Mk 2.1
The Mk 2.1 is the single stage, C-motor rocket with active stabilization in the form of the gimbal.   
The motor mount was removed from the rocket body, now being a component of the gimbal. The opening in the cone from the launch rail was removed as the active stabilization system allowed the rocket to be launched without a launch rail.   
Approximately 41 mm from the base of the cone, six evenly distributed structural supports extend from the cone to a ring in the center to which the Gimbal is mounted. Sections of the ring were removed, between two of the support structures on opposite sides, so that three support beams were connected together on each side of the cone. This was done to allow the gimbal to be placed within easily and to create space for the electronics.   
The electronics placed within the Athena Mk 2.1 was a Teensy 3.2 and an MPU 9250 IMU. A mount for the Teensy 3.2 is placed on the inner wall of the cone.  
[OnShape Document](https://cad.onshape.com/documents/0f010e1928b5ff53af9e6f65/w/b7a54311fe5ab080fbc05722/e/73f9c92d5f48f3ef2f847937)

### Athena Mk 3.0
The Mk 3.0 is the two stage, actively stable G-motor (with an I-motor second stage) rocket.   
The first stage can be used with smaller motors, such as a C or D class, by incorporating the “booster seat” which is a motor mount that can be placed within the gimbal.   
The first stage is actively stable with the use of the MK4.2 Gimbal with its outer ring and support structures now being directly connected with the rocket body. This removes the need to screw the gimbals outer ring in reduces the risk of the gimbal disconnecting. It contains a mount for the PCB (revision 3) and a 7.2V 2-cell LiPo battery.    
The top of the first stage body is coupled to the bottom of the second stage with a friction fit. The first stage is designed to be much wider than the next stage, also incorporating a nose-cone, to allow for the rocket to drag separate. The connecting section includes shunts directly beneath the nose-cone so the larger motor will not damage the lower stage when it ignites.   
The second stage is a more standard rocket which is passively stable. It contains electronics for radio and GPS communication. The PCB used in both sections are designed to be of equal size, however the first stage can use the Teensy 3.2 instead of the larger Teensy 3.6 to drive the Gimbal.   
[OnShape Document](https://cad.onshape.com/documents/9c18878e6faebb54262b731f/w/ca7579d91de786a386be370d/e/f7db08edaa3e677c4bf4a746)
