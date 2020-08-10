# PC WiFi Switch
A hardware switch for controlling a computer power switch. This project will also work for any device that requires a switch or simple power control system.

This project simply hosts a REST API on the ESP8266 to toggle a PC switch.

# API

`GET /`
- Returns current state of the device (0=off, 1=on)

`POST /`
- Set the state of the device
- **Params:**
  - "0" - off
  - "1" - on

`GET /name`
- Returns the name of the device
- Set with `deviceName` in the arduino script

`POST /flip`
- Flips the switch on, waits for a set delay, and then turns off
- **Params:**
  - {int} - ms to delay for
- Most computers will be fine with a 500ms delay so the following request will be best: `curl -X POST -d "500" <device_ip>/flip`