# ESP32-FreeRTOS

## Description
This program uses an **ESP32** equipped with **gas sensors** and **fire sensors** to detect hazardous conditions such as gas leaks and fires. The main feature of this program is the integration of **FreeRTOS**, a real-time operating system that enables the ESP32 to handle multiple tasks simultaneously with quick and efficient responses. Additionally, the gas and fire sensor statuses can be viewed directly through a **web interface** by accessing the ESP32's IP address.

### Key Features
- **Gas Sensor**: Detects the presence of dangerous gases like carbon monoxide, methane, or LPG. If gas concentration exceeds the set threshold, the system will alert the user.
- **Fire Sensor**: Detects signs of fire through temperature or smoke detection. When fire conditions are detected, the alarm is activated.
- **FreeRTOS**: Allows for efficient management of multiple tasks, such as monitoring gas and fire sensors, triggering alarms, and sending data to the web in a responsive and efficient manner.
- **Web Access**: The status of the gas and fire sensors can be monitored via a web interface by entering the ESP32's IP address in a browser. Information about the presence of gas or fire will be displayed in real-time.

### Role of FreeRTOS
**FreeRTOS** is a real-time operating system that enables multitasking management, providing several important benefits in this program:

- **Multitasking**: FreeRTOS allows the ESP32 to perform multiple tasks concurrently, such as reading the gas sensor, monitoring the fire sensor, activating the alarm, and sending data to the web simultaneously.
- **Task Scheduling**: With FreeRTOS, higher-priority tasks, like detecting gas leaks or fire, can be executed immediately, while lower-priority tasks can run in the background.
- **Real-Time Response**: FreeRTOS ensures that when gas or fire is detected, the system responds quickly and processes the event with minimal delay by triggering the alarm and updating the web status.

### Gas and Fire Status on the Web
- **Real-Time Monitoring**: Through the web interface, users can view the sensor status directly. If gas or fire is detected, the status will be updated and displayed on the web page.
- **Accessing the ESP32 IP**: To monitor the status, users simply need to access the **ESP32 IP** connected to the network. On the web page, users will see whether the gas or fire sensor detects a threat, and the status will show "Gas Detected" or "Fire Detected" if danger is present.

### Benefits of FreeRTOS in This Program
- **Real-Time Performance**: Ensures quick and efficient response to hazardous conditions.
- **Efficient Resource Management**: Ensures the ESP32 can handle multiple tasks concurrently without affecting performance.
- **Reliability**: FreeRTOS improves the program's reliability by allowing real-time monitoring and quick responses to gas or fire threats.
  
## Applications
- **Home Safety**: Detects gas leaks and fire risks, providing real-time status updates via the web for homeowners.
- **Industrial Safety**: Monitors industrial environments for gas leaks or fire hazards and provides real-time status updates.
- **Smart Building Systems**: Integrates into smart building management systems for continuous safety monitoring.

## Technologies Used
- **ESP32**: Handles data processing, sensor readings, and sending data to the web.
- **Gas and Fire Sensors**: Provide real-time data for hazard detection.
- **FreeRTOS**: Manages multiple tasks to ensure real-time, efficient operation.
- **Web Interface**: Allows users to monitor the sensor status via a browser by accessing the ESP32 IP.

## Installation and Usage
1. Connect the ESP32 to the gas and fire sensors.
2. Upload the FreeRTOS-based program to the ESP32.
3. Power up the ESP32 and connect it to a network for communication.
4. Access the web interface by entering the **ESP32 IP** in a browser to monitor the gas and fire sensor status in real-time.

## Future Enhancements
- **Cloud Integration**: Send data to the cloud for logging, analysis, or remote monitoring.
- **Additional Sensor Integration**: Add other environmental sensors, such as smoke detectors, temperature sensors, or humidity sensors, to enhance the detection system.

---
With the **FreeRTOS** feature, this program allows the **ESP32** to handle multiple tasks concurrently, including monitoring gas and fire sensors, triggering alarms, and providing real-time status via the web interface accessible through the ESP32 IP address.
