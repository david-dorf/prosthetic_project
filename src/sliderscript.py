import sys
import serial
import pyqtgraph as pg
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QSlider, QGridLayout
from PyQt5.QtCore import Qt


class SliderWindow(QWidget):
    def __init__(self):
        super().__init__()

        # Set up serial communication
        self.ser = serial.Serial('COM3', 9600)

        # Set up the first slider
        self.slider1 = QSlider(Qt.Horizontal)
        self.slider1.setMinimum(90)
        self.slider1.setMaximum(270)
        self.slider1.setValue(180)
        self.slider1.setTickPosition(QSlider.TicksBelow)
        self.slider1.setTickInterval(10)
        self.slider1.valueChanged.connect(self.send_values)

        # Set up the second slider
        self.slider2 = QSlider(Qt.Horizontal)
        self.slider2.setMinimum(90)
        self.slider2.setMaximum(270)
        self.slider2.setValue(180)
        self.slider2.setTickPosition(QSlider.TicksBelow)
        self.slider2.setTickInterval(10)
        self.slider2.valueChanged.connect(self.send_values)

        # Set up the labels
        self.label1 = QLabel('Slider 1: 180')
        self.label2 = QLabel('Slider 2: 180')

        # Set up the plot widget
        self.plot_widget = pg.PlotWidget(title="Motor Positions")
        self.plot_widget.setYRange(90, 270)
        self.plot_widget.setMouseEnabled(x=False, y=False)
        self.bar_plot = pg.BarGraphItem(x=[0, 1], height=[0, 0], width=0.6)
        self.plot_widget.addItem(self.bar_plot)

        # Set up the layout
        vbox = QVBoxLayout()
        hbox = QHBoxLayout()
        grid = QGridLayout()
        grid.setSpacing(20)
        grid.addWidget(self.label1, 0, 0)
        grid.addWidget(self.slider1, 1, 0)
        grid.addWidget(self.label2, 0, 1)
        grid.addWidget(self.slider2, 1, 1)
        hbox.addLayout(grid)
        hbox.addWidget(self.plot_widget)
        vbox.addLayout(hbox)
        self.setLayout(vbox)

        # Set up the window
        self.setWindowTitle('Slider Window')
        self.setGeometry(100, 100, 600, 400)
        self.show()

    def send_values(self):
        # Send the values over serial
        value1 = str(self.slider1.value()).encode('utf-8')
        value2 = str(self.slider2.value()).encode('utf-8')
        # self.ser.write(value1 + b',' + value2)
        self.ser.write(value1)
        print(value1)

        # Update the labels
        self.label1.setText('Motor 1: {}'.format(self.slider1.value()))
        self.label2.setText('Motor 2: {}'.format(self.slider2.value()))

        # Update the plot
        x = [1, 2]
        y = [self.slider1.value(), self.slider2.value()]
        self.bar_plot.setOpts(x=x, height=y, width=0.6, brush=(10, 10, 255, 255))


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = SliderWindow()
    sys.exit(app.exec_())
