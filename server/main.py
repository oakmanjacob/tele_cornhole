# Importing Libraries
import serial
import time

from flask import Flask, render_template
from flask_restful import Resource, Api, reqparse

app = Flask(__name__)
api = Api(app)

arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)

@app.route('/')
def render_page():
    return render_template("index.html")

@app.route('/controller/<direction>', methods=['GET'])
def hit_controller(direction):
    arduino.write(bytes(direction, 'utf-8'))
    return ""

if __name__ == '__main__':
    app.run()