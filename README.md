# Smart-temperature-monitoring-using-IoT

## Overview

This project is an IoT-based smart environmental monitoring system that uses an ESP8266 (NodeMCU) and DHT22 sensor to measure temperature and humidity. The data is uploaded to ThingSpeak and analyzed using Machine Learning to predict future temperature conditions and trigger alerts proactively.

## Features

Real-time temperature & humidity monitoring
Cloud integration using ThingSpeak
Automated alerts (LED + buzzer)
Temperature trend prediction using ML

## Tech Stack

Hardware: NodeMCU (ESP8266), DHT22, LEDs, Buzzer
Cloud: ThingSpeak
Programming: Arduino, Python
ML: Basic regression model

## System Architecture

Sensor collects temperature and humidity
ESP8266 sends data to ThingSpeak
Python script fetches historical data
ML model predicts future values
Decision model classifies condition
System triggers alert (LED + buzzer)

## Machine Learning Algorithm

1. Prediction Model

Algorithm: Linear Regression
Input: Previous temperature & humidity values
Output: Predicted next temperature & humidity

2. Decision Model

Algorithm: Decision Tree Classifier
Input: Predicted temperature & humidity
Output: Alert (ON/OFF)

3. Logic

High Temperature → Motor ON + Red LED + Buzzer
Normal Condition → Motor OFF + Green LED

## Procedure:
Step 1: Hardware Setup

Connect DHT22 to ESP8266 (Data pin to D4)
Connect LEDs and buzzer to digital pins

Step 2: Upload Arduino Code

Open Arduino IDE
Install required libraries:
ESP8266WiFi
ThingSpeak
DHT sensor library
Upload code to ESP8266

Step 3: Run Python Model

pip install pandas scikit-learn
python ml_model.py

## ThingSpeak Fields

Field 1 → Temperature
Field 2 → Humidity
Field 3 → Alert Status
Field 4 → Manual Buzzer Control

## Applications

Smart homes
Environmental monitoring
Industrial safety systems
Smart agriculture
