# TOBB ETÜ ELE495 - Capstone Project
## Design of an SMD Pick and Place Machine

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Screenshots](#screenshots)
- [Acknowledgements](#acknowledgements)

---

## Introduction

This repository contains the complete implementation of an autonomous 
SMD (Surface Mount Device) pick-and-place machine developed as a 
capstone project at TOBB University of Economics and Technology 
(ELE 495, 2025–2026).

The system picks electronic components (resistors and diodes) from 
a feeder platform, transports them to a test station for electrical 
measurement, and places them onto a PCB at predefined positions. 
The project integrates mechanical design, embedded firmware, computer 
vision, and a web-based control interface into a single autonomous 
platform.

---

## Features

### Hardware

| Component | Description |
|---|---|
| [Raspberry Pi 4 (4GB)](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/) | Main controller, runs Flask server inside Docker |
| [Arduino Uno — GRBL](https://www.arduino.cc/en/Guide/ArduinoUno) | CNC motor controller, receives G-code over serial |
| [Arduino Uno — Test Station](https://www.arduino.cc/en/Guide/ArduinoUno) | Measures resistance and diode direction via voltage divider |
| [Raspberry Pi Camera Module V2](https://www.raspberrypi.com/products/camera-module-v2/) | Live MJPEG stream and YOLO inference input |
| NEMA 17 Stepper Motors (×4) | X, Y, Z axis motion |
| CNC Shield V3 + A4988 Drivers | Stepper motor drive |
| 12V Vacuum Pump + IRF520 MOSFET | Component pick and release |
| Sigma Aluminum Profiles | Machine frame |

### Operating System and Packages

- **OS:** Raspberry Pi OS (64-bit)
- **Runtime:** Docker (python:3.13-slim base image)
- **Python packages:** Flask, Flask-SocketIO, OpenCV, 
  ONNX Runtime, NumPy (see `requirements.txt`)
- **Arduino firmware:** GRBL (motor controller), 
  custom test station firmware

### Applications

- Autonomous 3-axis pick-and-place cycle (8 component slots)
- Electrical component testing via voltage divider circuit
  (resistance measurement and diode polarity detection)
- Real-time browser dashboard (HTML5 + CSS3 + vanilla JavaScript)
- WebSocket-based bidirectional communication (Flask-SocketIO)
- Live MJPEG camera stream with YOLOv8 / ONNX Runtime overlay
- Placement accuracy verification (IoU, pixel distance, 
  error percentage)
- REST API (`/api/status`, `/api/camera/stream`, 
  `/api/vision/detect`, etc.)

### Services

- **camera_server.py** — runs on host OS, captures frames via 
  `rpicam-vid`, serves MJPEG on port 5001
- **Flask app (Docker)** — serves dashboard and handles all 
  WebSocket events on port 5000
- **ZeroTier VPN** — optional remote access without port forwarding

---

## Repository Structure
```
ELE-495-cnc-pick-place/
├── app/
│   ├── main.py              # Flask server, REST API, WebSocket events
│   ├── process.py           # Pick-and-place process loop
│   ├── grbl.py              # GRBL serial controller
│   ├── tester.py            # Test station serial controller
│   ├── camera_service.py    # Camera stream client
│   ├── camera_server.py     # Host-side MJPEG server (rpicam-vid)
│   ├── config.py            # Coordinates, port settings, constants
│   ├── vision/
│   │   ├── yolo_runtime.py  # ONNX inference runtime
│   │   ├── placement_verify.py  # IoU-based placement check
│   │   └── best.onnx        # Trained YOLOv8n model
│   └── templates/
│       └── index.html       # Single-page dashboard
├── arduino/
│   ├── test_station/        # Test station Arduino firmware
│   └── grbl/                # GRBL configuration
├── Dockerfile
├── docker-compose.yml
├── entrypoint.sh
└── requirements.txt
```

---

## Installation

### Prerequisites

- Raspberry Pi 4 with Raspberry Pi OS (64-bit)
- Docker and Docker Compose installed
- ZeroTier client installed (optional, for remote access)
- Arduino Uno ×2 — one flashed with GRBL firmware, 
  one with test station firmware

### Flash Arduino Firmware
```bash
# Test station firmware
# Open arduino/test_station/ in Arduino IDE and upload to Arduino Uno

# GRBL motor controller
# Open arduino/grbl/ in Arduino IDE and upload to Arduino Uno
```

### Run the Application
```bash
# 1. Clone the repository
git clone https://github.com/ELE495-2526Spring/capstoneproject-grup-4.git
cd capstoneproject-grup-4

# 2. Start the camera server on the host (separate terminal)
python3 app/camera_server.py

# 3. Build and run the Docker container
docker compose up --build
```

Dashboard will be available at `http://<raspberry-pi-ip>:5000`.

### Remote Access via ZeroTier (Optional)
```bash
# Install ZeroTier
curl -s https://install.zerotier.com | sudo bash

# Join your network
sudo zerotier-cli join <network-id>

# Authorize the device at https://my.zerotier.com
# Access dashboard at http://<zerotier-ip>:5000
```

---

## Usage

1. Open `http://<ip>:5000` in a browser
2. Click **Connect** — establishes serial connections to both 
   Arduinos and opens the camera stream
3. Once all four indicators (GRBL, Test, Camera, YOLO) 
   turn green, click **Start**
4. The system runs the full pick-and-place cycle automatically:
```
vacuum pick → vision check → test station → measure
→ identify component → place on PCB or return to slot
→ placement verification
```

5. Monitor component status, test measurements, XYZ coordinates, 
   and placement accuracy in real time on the dashboard
6. Use the G-code input field for manual GRBL commands

---

## Screenshots

<!-- sistem mimarisi diyagramı buraya — Figure 6.6.1 -->
<!-- dashboard ekran görüntüsü buraya — WhatsApp image -->
<!-- makine fiziksel fotoğrafı buraya — Figure 6.3.8 -->

---

## Acknowledgements

| Contributor | Role | GitHub |
|---|---|---|
| Ayşenur Kurt | Image processing, dataset preparation |
| Eda İnan | Web interface, backend | [@inaneda](https://github.com/inaneda) |
| Mustafa Buğrahan Özgün | Mechanical design, GRBL integration |
| Mert Şenel | Test station design and firmware |

### Supervisor

Prof. Dr. Bülent Tavlı — TOBB University of Economics and Technology


| Resource | Link |
|---|---|
| Flask-SocketIO | [flask-socketio.readthedocs.io](https://flask-socketio.readthedocs.io) |
| FastAPI (2nd progress report) | [fastapi.tiangolo.com](https://fastapi.tiangolo.com) |
| ONNX Runtime | [onnxruntime.ai](https://onnxruntime.ai) |
| Docker | [docs.docker.com](https://docs.docker.com) |
| ZeroTier | [docs.zerotier.com](https://docs.zerotier.com) |
| Ultralytics YOLOv8 | [github.com/ultralytics/ultralytics](https://github.com/ultralytics/ultralytics) |
| Protoneer Raspberry-Pi-CNC | [github.com/Protoneer/Raspberry-Pi-CNC](https://github.com/Protoneer/Raspberry-Pi-CNC) |
| Mokey Laser (mechanical inspiration) | — |
