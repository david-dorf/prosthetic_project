# A lightweight, actively controlled prosthetic with affordable sensors
Portfolio post: https://www.daviddorf.com/bionic-hand-armband-project

## Novel muscle activity detection
Electromyography (EMG) sensing technology is currently limited by its low signal to noise ratio, high cost, and scarce availability for quality products. Mechanomyography (MMG) and photoplethysmography (PPG) appear to be a promising alternatives that use affordable components to detect muscle activity. Previous studies suggest that MMG and PPG can potentially provide more accurate readings of muscular activity than EMG with less signal processing techniques. 

## Flexible materials and biomimetic joints
Traditional prosthetic devices using EMG control lack the ability of abduction. A new approach to prosthetics using flexible materials could allow for more complex gestures and improved dexterity when manipulating objects. Advancements in the field of 3D printing can allow for semi-hollow devices made of stretchable materials, like thermoplastic polyurethane (TPU), to be employed without sacrificing resilience to the elements. Combining a novel MMG/PPG control system with a prosthetic with hexagonal infill 3D printing pattern and biomimetic joints containing flexible MCP ligaments can allow for a lighter, more affordable, and durable prosthetic device.

## Using the code
Uploading the mux_ppg_dynamixel_control.ino file to the OpenRB-150 microcontroller through the Arduino IDE should be sufficient to use this system. Setup for the motors involves changing one of their ID numbers through the DYNAMIXEL WIZARD 2.0 so that the two motors can be moved independently of each other. Additionally, the firmware on the motors may need to be updated through the same client. Contact the manufacturer at www.robotis.us for further assistance if necessary.

## More information
Further details can be found at https://www.daviddorf.com/bionic-hand-armband-project - including CAD drawings, 3D renderings, and videos of the armband and bionic hand in action.
