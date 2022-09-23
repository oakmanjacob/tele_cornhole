# Importing Libraries
import serial
import time

from flask import Flask, render_template
from flask_restful import Resource, Api, reqparse

app = Flask(__name__)
api = Api(app)

arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.1)

@app.route('/')
def render_page():
    return render_template("index.html")

@app.route('/controller/<direction>', methods=['GET'])
def hit_controller(direction):
    arduino.write(bytes(direction, 'utf-8'))
    return ""

@app.route('/fire', methods=['GET'])
def fire():
    arduino.write(bytes('f', 'utf-8'))
    return ""

@app.route('/zero', methods=['GET'])
def zero_motor():
    arduino.write(bytes('z', 'utf-8'))
    return ""

# @app.route('/yrot/<angle>', methods=['GET'])
# def send_yrot(angle):

#     arduino.write(bytes(angle, 'utf-8'))
#     return ""

# @app.route('/xrot/<angle>', methods=['GET'])
# def send_xrot(angle):

#     arduino.write(bytes(angle, 'utf-8'))
#     return ""

if __name__ == '__main__':
    app.run()