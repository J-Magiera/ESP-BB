from PyQt5.QtWidgets import (
    QApplication,
    QHBoxLayout,
    QVBoxLayout,
    QLabel,
    QLineEdit)
from PyQt5 import QtWidgets, QtCore
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg, NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
import paho.mqtt.client as mqtt
import sys
import random

class Graph(FigureCanvasQTAgg):
    def __init__(self, parent=None, width=5, height=4, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(111)
        self.axes.grid()
        super(Graph, self).__init__(fig)



class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.setWindowTitle("MQTT app")
        self.client = mqtt.Client(client_id="mqtt")
        self.canvas = Graph(self, width=5, height=4, dpi=100)

        n_data = 100
        self.xdata = list(range(n_data))
        self.ydata = list(range(n_data))
        self.plot_ref = None
        self.updatePlot()
        
        layout = QtWidgets.QVBoxLayout()
        top_layout =  QtWidgets.QHBoxLayout()
        bottom_layout = QtWidgets.QHBoxLayout()
    
        self.connectButton = QtWidgets.QPushButton(text="Connect", checkable = True, enabled=True, clicked = self.connectMQTT)
        self.sendButton = QtWidgets.QPushButton(text="Send")
        self.dataTable = QtWidgets.QLineEdit()
        self.chooseSensor = QtWidgets.QComboBox()
        self.chooseSensor.addItems(["Sensor 1", "Sensor 2"])
       

        top_layout.addWidget(self.connectButton)
        top_layout.addWidget(self.sendButton)
        top_layout.addWidget(self.dataTable)
        top_layout.addWidget(self.chooseSensor)


        bottom_layout.addWidget(self.canvas)
        layout.addLayout(top_layout)
        layout.addLayout(bottom_layout)
        widget = QtWidgets.QWidget()
        widget.setLayout(layout)
        self.setCentralWidget(widget)
        self.show()

        self.timer = QtCore.QTimer()
        self.timer.setInterval(100)
        self.timer.timeout.connect(self.updatePlot)

    def connectMQTT(self):
        if self.connectButton.isChecked():
            try:
                self.client.connect("192.168.0.157", 1883, 60)
                self.client.username_pw_set(username="mqtt",password="passwd")
                self.client.loop_start()
                self.client.on_connect = self.on_connect
                #self.client.on_message = self.readSensor
                self.client.message_callback_add("topic/2/CLI/return", self.readSensor)
                self.connectButton.setChecked(True)
            except:
                print("Error #0001")
    
    def on_connect(self, client, userdata, flags, rc):
        self.client.subscribe("topic/"+ str(self.chooseSensor.currentIndex()+1) +"/CLI/return")
        self.client.subscribe("topic/"+ str(self.chooseSensor.currentIndex()+1) +"/CLI")
        self.client.publish("topic/"+ str(self.chooseSensor.currentIndex()+1) +"/CLI", 5)
        self.timer.start()

    def updatePlot(self):
        self.client.loop()
        if self.plot_ref is None:
            plot_refs = self.canvas.axes.plot(self.xdata, self.ydata, 'r')

            self.plot_ref = plot_refs[0]
        else:
            self.plot_ref.set_ydata(self.ydata)
        self.canvas.axes.set_ylim([min(self.ydata),max(self.ydata)])
        self.canvas.draw()

    def readSensor(self, client, userdata, message):
        print("xd")
        print(message.payload)
        self.ydata = self.ydata[1:] + [int(message.payload)]

if __name__ == "__main__":  
    app = QApplication(sys.argv)
    app.setStyle('Breeze')
    window = MainWindow()
    window.show()
    app.exec_()