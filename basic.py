#!/usr/bin/python
# coding: utf-8
from flask import Flask, request, jsonify

app = Flask(__name__)

data = []

@app.route('/')
def home():
    return "Welcome to Home"

@app.route('/sensor-data', methods=['POST'])
def post_data():
    data = request.json
    temperature = data.get('temperature')
    humidity = data.get('humidity')
    print(f"Received Temperature: {temperature}")
    print(f"Received Humidity: {humidity}")

    response = {
        "status": "success",
        "data_received": data
    }
    return jsonify(response)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)