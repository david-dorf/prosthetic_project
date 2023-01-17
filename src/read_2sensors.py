# Adapted from Adafruit's CircuitPython analog input example
import time
import board
from digitalio import DigitalInOut, Direction
from analogio import AnalogIn

addr0_pin = DigitalInOut(board.ADC_ADDR_0)
addr0_pin.direction = Direction.OUTPUT

addr1_pin = DigitalInOut(board.ADC_ADDR_1)
addr1_pin.direction = Direction.OUTPUT

addr2_pin = DigitalInOut(board.ADC_ADDR_2)
addr2_pin.direction = Direction.OUTPUT

analog_in = AnalogIn(board.SHARED_ADC)


def get_voltage(pin):
    return (pin.value-12700)**2

def select(address):
    addr0_pin.value = address & 0b001
    addr1_pin.value = address & 0b010
    addr2_pin.value = address & 0b100


VOLTAGE_GAIN = 13.9 / 3.9
CURRENT_GAIN = 1 / (69 * 0.003)
CURRENT_OFFSET = -0.02
sensor1prev = 0
sensor2prev = 0

while True:
    # Read each sensor in turn and print its voltage
    #for i in range(2):
        #select(i)
        #print("S", i + 1, " = ", round(get_voltage(analog_in), 3), sep="", end=", ")
        #print((get_voltage(analog_in),))
    
    select(0)
    sensor1 = get_voltage(analog_in)
    smooth1 = 0.8 * sensor1 + 0.2 * sensor1prev
    sensor1prev = smooth1
    
    select(1)
    sensor2 = get_voltage(analog_in)
    smooth2 = 0.8 * sensor2 + 0.2 * sensor2prev
    sensor2prev = smooth2
    
    print((smooth1,smooth2))
    

    # Read the voltage sense and print the value
    select(board.VOLTAGE_SENSE_ADDR)
    voltage = get_voltage(analog_in) * VOLTAGE_GAIN
    #print("Voltage =", round(voltage, 4), end=", ")

    # Read the current sense and print the value
    select(board.CURRENT_SENSE_ADDR)
    current = (get_voltage(analog_in) + CURRENT_OFFSET) * CURRENT_GAIN
    #print("Current =", round(current, 4))

    time.sleep(0.02)