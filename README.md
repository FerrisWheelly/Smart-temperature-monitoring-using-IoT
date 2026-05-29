# Smart-temperature-monitoring-using-IoT

**Overview
**
This project is an IoT-based smart environmental monitoring system that uses an ESP8266 (NodeMCU) and DHT22 sensor to measure temperature and humidity. The data is uploaded to ThingSpeak and analyzed using Machine Learning to predict future temperature conditions and trigger alerts proactively.

**Features**

Real-time temperature & humidity monitoring
Cloud integration using ThingSpeak
Automated alerts (LED + buzzer)
Temperature trend prediction using ML

**Tech Stack
**
Hardware: NodeMCU (ESP8266), DHT22, LEDs, Buzzer
Cloud: ThingSpeak
Programming: Arduino, Python
ML: Basic regression model

**System Architecture**

Sensor collects temperature and humidity
ESP8266 sends data to ThingSpeak
Python script fetches historical data
ML model predicts future values
Decision model classifies condition
System triggers alert (LED + buzzer)
**
Machine Learning Algorithm**

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

