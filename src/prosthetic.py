import time
import board
import pwmio
import math
from adafruit_motor import servo
import neopixel
import time
import board
from digitalio import DigitalInOut, Direction
from analogio import AnalogIn

"""This program uses the output of an analog sensor to control the position of four servos.
The code uses the CircuitPython library for the RP2040 microcontroller. TODO: Writing the code
in C would allow for more precise sensor measurements and a more efficient program overall.
"""

def select(address):
    """Select the ADC channel to read from.

    Args:
        address (binary): The address of the ADC channel to read from.
    """
    addr0_pin.value = address & 0b001
    addr1_pin.value = address & 0b010
    addr2_pin.value = address & 0b100

def get_voltage(pin):
    """Get the voltage from the ADC pin.

    Args:
        pin (int): The ADC pin to read from.

    Returns:
        voltage: The adjusted (gain and bias) voltage reading from the ADC pin.
    """
    return (pin.value * 3.3) / 65536

# Initialize ADC pins
addr0_pin = DigitalInOut(board.ADC_ADDR_0)
addr0_pin.direction = Direction.OUTPUT
addr1_pin = DigitalInOut(board.ADC_ADDR_1)
addr1_pin.direction = Direction.OUTPUT
addr2_pin = DigitalInOut(board.ADC_ADDR_2)
addr2_pin.direction = Direction.OUTPUT
analog_in = AnalogIn(board.SHARED_ADC)

# Initialize servo objects and PWM cycle
pins = [board.SERVO_1, board.SERVO_2, board.SERVO_3, board.SERVO_4]
servos = [servo.Servo(pwmio.PWMOut(pin, duty_cycle=2 ** 15, frequency=50)) for pin in pins]

# Initialize main variables
frequency = 8
i = 0
j = 0

# Main loop
while(1):
    # TODO: Modularize code into functions [on/off switch, sensor reading, servo control]
    
    # Read sensor voltage on pin 1
    select(1)
    sensorvoltage = get_voltage(analog_in)
    sinmotorangle = int(90 + 80 * (math.sin(math.pi/180 * frequency * i)))
    # sensormotorangle = int(90 + 80 * (sensorvoltage))
    
    if j == 0:
        for servo in servos:
            servo.angle = sinmotorangle
            i += 1
            # servo.angle = sensormotorangle
    else:
        break
    
    # Print sensor voltage to serial monitor
    print((sensorvoltage,))
    
    ## Optional on/off switch by touching sensor pin 1 to ground [make separate function]
    # if sensorvoltage > 1:
    #     if j == 0:
    #         j = 1
    #     else:
    #         j = 0
    
    # Add in delay for stability
    time.sleep(0.05)
