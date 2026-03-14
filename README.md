This is a highly professional, well-structured `README.md` file for your GitHub repository. 

Since you are applying to German universities, having a well-documented open-source project on your GitHub profile is a **massive advantage**. It shows practical engineering skills, problem-solving, and a structured mindset. 

Copy the text below and paste it into a file named `README.md` in your GitHub repository.

***

# 🌡️ EnvUnoSimple Logger: Arduino Environment Monitor & Visualizer

![Version](https://img.shields.io/badge/Version-2.1-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Arduino-00979C.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

**EnvUnoSimple Logger** is a lightweight, memory-efficient operating system for Arduino that monitors temperature and humidity, logs data over time using a circular buffer, and exports it as JSON for beautiful web-based visualization.

Built completely with non-blocking code (using `millis()` instead of `delay()`), it features a multi-view LCD interface, hardware debouncing, and a companion Glassmorphism HTML/JS dashboard.

---
![IMG_3268](https://github.com/user-attachments/assets/0968a3b7-a0e5-4299-876c-e0327c61e619)

## ✨ Key Features
* **1-Hour Data Logging:** Stores temperature data every minute locally in RAM using a memory-efficient circular buffer.
* **JSON Serial Export:** Exports logged data in a clean JSON format via USB with a 4-click hardware trigger.
* **Multi-Click UI Engine:** A single push-button controls the entire OS (change views, toggle backlight, export data) using click-counting logic.
* **Non-Blocking Architecture:** Continuous sensor reading and heartbeat LED blinking without pausing the main loop.
* **Web Dashboard (`index.html`):** A beautiful, responsive web visualizer built with Chart.js to graph the exported Arduino data.

---
![IMG_3270](https://github.com/user-attachments/assets/21e6ace4-1c14-4efd-9869-9c665057e4fa)

## 🛠️ Materials Required (BOM)
To build this project, you will need the following hardware:
1. **Arduino Uno / Nano / Mega** (Any ATmega328P based board)
2. **DHT11 or DHT22** Temperature and Humidity Sensor
3. **16x2 LCD Display** with **I2C Backpack** (PCF8574)
4. **Push Button** (Tactile Switch)
5. **LED** (Optional: the code uses the built-in LED on Pin 13)
6. **10kΩ Resistor** (For the push button pull-down, if not using internal pull-up)
7. Breadboard & Jumper Wires

---

## ⚡ Circuit Diagram & Wiring Guide

| Component | Arduino Pin | Notes |
| :--- | :--- | :--- |
| **DHT Sensor Data** | Digital Pin 2 | Requires a 10k pull-up resistor (most modules have this built-in). |
| **Push Button** | Digital Pin 3 | Connect one side to 5V, the other to Pin 3 (with a 10k resistor to GND). |
| **LED (+)** | Digital Pin 13 | Used as a "Heartbeat" / Error indicator. |
| **LCD I2C SDA** | Analog Pin A4 | Standard I2C Data line (SDA). |
| **LCD I2C SCL** | Analog Pin A5 | Standard I2C Clock line (SCL). |

*(Note: Power the DHT and LCD using the Arduino's 5V and GND pins).*

---

## 💻 Installation & Setup

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/YourUsername/EnvUnoSimple-Logger.git
   ```
2. **Install Required Arduino Libraries:**
   Open the Arduino IDE, go to *Sketch > Include Library > Manage Libraries*, and install:
   * `DHT sensor library` by Adafruit
   * `LiquidCrystal I2C` by Frank de Brabander
3. **Upload the Code:**
   * Open `arduino_code.ino` in the Arduino IDE.
   * Select your board and COM port.
   * Click **Upload**.

---

## 🕹️ How to Use the UI (Button Guide)

The system uses an advanced single-button interface. Press the button quickly to trigger different functions:

* 🟢 **1 Click:** Cycle through views (Dashboard -> Temp Focus -> Humid Focus -> Stats).
* 🔵 **2 Clicks:** Toggle LCD Backlight (ON/OFF).
* 🟡 **3 Clicks:** Show Developer Credits.
* 🔴 **4 Clicks:** **Export Data to Laptop** (Triggers JSON Serial Output).

---

## 📊 Data Visualization Guide

Don't want to read raw numbers? Use the included web dashboard!

1. Keep your Arduino plugged into your computer via USB.
2. Open the **Arduino IDE Serial Monitor** (Set Baud Rate to `9600`).
3. Press the hardware button **4 times**.
4. The Arduino will print a JSON string looking like this:
   ```text
   --- DATA START ---
   {"owner": "Panth", "unit": "Celsius", "count": 60, "data": [24.5, 24.6, ...]}
   --- DATA END ---
   ```
5. **Copy the entire output.**
6. Open the `visualizer.html` file (included in this repository) in any web browser.
7. Paste the copied text into the text box and click **Generate Graph**. 

Boom! Beautiful, smooth charts with automatic Max/Min/Avg calculations. 📈

---

## 🧠 Engineering Notes (For Developers)
* **Memory Management:** An Arduino Uno only has 2KB of SRAM. Storing a large JSON string would cause a stack crash. To solve this, the code stores raw `float` variables in an array and constructs the JSON string *on-the-fly* via the Serial port during export.
* **Debouncing:** Hardware debouncing is handled purely in software within the `handleButton()` function using `millis()` timing logic.

---

## 👨‍💻 Author
**Built by Panth**  
Open-source enthusiast, Maker, and Engineering Student.  
*(Feel free to reach out or open an issue if you have suggestions!)*

![IMG_3271](https://github.com/user-attachments/assets/97d98d26-43fa-4b53-afc6-c8995b8f18f8)
![IMG_3273](https://github.com/user-attachments/assets/fc3cdc8c-5216-422c-9772-da15ccdaf118)
![IMG_3274](https://github.com/user-attachments/assets/6ebc8c4f-09b6-433c-902d-0bdf4cf96bfb)
![IMG_3268](https://github.com/user-attachments/assets/0968a3b7-a0e5-4299-876c-e0327c61e619)

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
