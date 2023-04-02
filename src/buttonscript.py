import serial
import tkinter as tk
from tkinter import ttk

# configure serial port
ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM3' # replace with your serial port name
ser.open()

# create GUI window
window = tk.Tk()
window.title("ORCO Hand Controller")
window.iconbitmap("icon.ico")  # replace "icon.ico" with the file path of your ICO file
window.configure(background='#333333') # set the background color to dark grey

# create button functions
def send_a():
    ser.write(b'a')
def send_b():
    ser.write(b'b')
def send_c():
    ser.write(b'c')
def send_d():
    ser.write(b'd')
def send_e():
    ser.write(b'e')
def send_f():
    ser.write(b'f')
def send_g():
    ser.write(b'g')

# create buttons
style = ttk.Style()
style.configure('TButton', font=('Segoe UI', 24), padding=20, borderwidth=4, relief='raised', background='#333333', foreground='black')
style.map('TButton', foreground=[('pressed', 'orange')])  # set the pressed color to orange
button_a = ttk.Button(window, text="Close", command=send_a)
button_b = ttk.Button(window, text="Open", command=send_b)
button_c = ttk.Button(window, text="Close side 1", command=send_c)
button_d = ttk.Button(window, text="Close side 2", command=send_d)
button_e = ttk.Button(window, text="Open side 1", command=send_e)
button_f = ttk.Button(window, text="Open side 2", command=send_f)
button_g = ttk.Button(window, text="Half close", command=send_g)

# add buttons to window
button_a.pack(padx=10, pady=10)
button_b.pack(padx=10, pady=10)
button_c.pack(padx=10, pady=10)
button_d.pack(padx=10, pady=10)
button_e.pack(padx=10, pady=10)
button_f.pack(padx=10, pady=10)
button_g.pack(padx=10, pady=10)

# run GUI loop
window.mainloop()

# close serial port
ser.close()
