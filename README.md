
ESP32 IoT Fire and Gas Detection System with Blynk

An internet-of-things (IoT) smart home safety project that leverages an ESP32 microcontroller to detect fires and hazardous gas leaks. The system utilizes a hardware debounce algorithm to eliminate false alarms, triggers local audio-visual indicators, and pushes real-time emergency notifications directly to a smartphone via the Blynk IoT Cloud.
The project is fully compatible with physical hardware deployment and includes pre-configured network profiles for virtual testing within the Wokwi Simulation Environment.

Core Features

• Dual Threat Monitoring: Continuously tracks data from a digital Flame Sensor (Infrared) and an MQ-2 Gas/Smoke Sensor.
• False Alarm Prevention: Features a built-in 2-second software debounce mechanism to ignore temporary ambient light reflections and noise spikes.
• Smart App Notifications: Sends high-priority push notifications (fire_alert and gas_alert) to the user's phone lock screen.
• Remote Mobile Mute: Allows the user to silence the local buzzer and LED directly from the Blynk mobile dashboard using virtual pins.
• 1-Minute Alarm Timeout: Automatically silences the physical alarms after 60 seconds of continuous beeping to save power and protect hardware components.

Hardware Components & Pin Configuration


Component	ESP32 GPIO Pin	I/O Direction	Wiring Details
MQ-2 Gas Sensor	GPIO 32	Input	Digital Output (DOUT) line
Flame Sensor	GPIO 35	Input	Data (DAT) / Digital Output line
Active Buzzer	GPIO 25	Output	Positive terminal
Red LED	GPIO 26	Output	Anode (long leg) via 220Ω resistor


Simulation Configuration

To run the code within Wokwi, the network configuration uses the virtual access point credentials:
• SSID: Wokwi-GUEST
• Password: "" (Empty string)

Software Dependencies

The following libraries must be installed in your development environment to compile the sketch:
• WiFi.h (Built-in ESP32 core library)
• BlynkSimpleEsp32.h (Available via the Arduino Library Manager)

Blynk Cloud Dashboard Configuration

1. Create a new Device Template named Flame and gas detection.
2. Configure Virtual Pin V1 as an Integer Datastream (Min: 0, Max: 1) linked to a Button Widget for the mute switch.
3. Add two custom Events in the Developer Settings: fire_alert and gas_alert.
4. Enable Push Notifications under the notification rules and set the target to Device Owner.
