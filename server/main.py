# Importing Libraries
import serial
import time

from flask import Flask, render_template
from flask_cors import CORS
from flask_restful import Resource, Api, reqparse

app = Flask(__name__)
api = CORS(app)

arduino = serial.Serial(port='COM4', baudrate=115200, timeout=30)

@app.route('/')
def render_page():
    return render_template("index.html")

@app.route('/controller/<direction>', methods=['GET'])
def hit_controller(direction):
    arduino.write(bytes(direction, 'utf-8'))
    return ""

@app.route('/fire', methods=['GET'])
def fire():
    arduino.reset_input_buffer()
    arduino.write(bytes('f', 'utf-8'))
    time.sleep(0.05)
    data = arduino.read()
    return data

@app.route('/zero', methods=['GET'])
def zero_motor():
    arduino.write(bytes('z', 'utf-8'))
    return ""

@app.route('/yrot/<angle>', methods=['GET'])
def send_yrot(angle):
    arduino.reset_input_buffer()
    arduino.write(bytes("y" + angle, 'utf-8'))
    time.sleep(0.05)
    data = arduino.read()
    return data

@app.route('/xrot/<angle>', methods=['GET'])
def send_xrot(angle):
    arduino.reset_input_buffer()
    arduino.write(bytes("x" + angle, 'utf-8'))
    time.sleep(0.05)
    data = arduino.read()
    return data

if __name__ == '__main__':
    app.run(host="0.0.0.0")