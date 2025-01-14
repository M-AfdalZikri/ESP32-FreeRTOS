#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Change these to your network credentials
const char *ssid = "Your_SSID";
const char *password = "Your_Password";

#define api_threshold 50
#define gas_threshold 1300

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create Event Source at /events
AsyncEventSource events("/events");

// Sensor pins
const int gasSensorPin = 34;
const int fireSensorPin = 35;

// ESP32 client IP address
const char *clientIP = "192.168.5.22"; // Replace with the ESP32 client's IP address

// Task handles
TaskHandle_t FireSensorTaskHandle = NULL;
TaskHandle_t GasSensorTaskHandle = NULL;

// Initialize WiFi
void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println('.');
    delay(1000);
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Sensor Notifications</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p { font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #50B8B4; color: white; font-size: 1rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); max-width: 600px; margin: 0 auto; }
    .status { font-size: 1.4rem; }
    .status.red { color: red; }
  </style>
</head>
<body>
  <div class="topnav">
    <h1>ESP32 Sensor Notifications</h1>
  </div>
  <div class="content">
    <div class="card">
      <p class="status" id="fire-status">Fire Status: Loading...</p>
      <p class="status" id="gas-status">Gas Status: Loading...</p>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');

 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);

 source.addEventListener('fire-status', function(e) {
  console.log("fire-status", e.data);
  var fireStatus = document.getElementById("fire-status");
  fireStatus.innerHTML = "Fire Status: " + e.data;
  if (e.data == "Fire detected") {
    fireStatus.classList.add("red");
  } else {
    fireStatus.classList.remove("red");
  }
 }, false);

 source.addEventListener('gas-status', function(e) {
  console.log("gas-status", e.data);
  var gasStatus = document.getElementById("gas-status");
  gasStatus.innerHTML = "Gas Status: " + e.data;
  if (e.data == "Gas detected") {
    gasStatus.classList.add("red");
  } else {
    gasStatus.classList.remove("red");
  }
 }, false);
}
</script>
</body>
</html>)rawliteral";

// Task for fire sensor
void FireSensorTask(void *pvParameters)
{
  (void)pvParameters;
  int fireSensorStatus;
  String fireStatus;
  String fireSensorValue;

  for (;;)
  {
    fireSensorStatus = analogRead(fireSensorPin);
    fireSensorValue = String(fireSensorStatus);

    // Process fire sensor status
    if (fireSensorStatus > api_threshold)
    {
      fireStatus = "No fire";
      Serial.print("No fire | Value: ");
      Serial.println(fireSensorStatus);
    }
    else
    {
      fireStatus = "Fire detected";
      Serial.print("Fire detected | Value: ");
      Serial.println(fireSensorStatus);
    }

    // Send Event to Web Client with Sensor Status and Sensor Value
    events.send(fireStatus.c_str(), "fire-status", millis());
    events.send(fireSensorValue.c_str(), "fire-value", millis());

    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
  }
}

// Task for gas sensor
void GasSensorTask(void *pvParameters)
{
  (void)pvParameters;
  int gasSensorStatus;
  String gasStatus;
  String gasSensorValue;

  for (;;)
  {
    gasSensorStatus = analogRead(gasSensorPin);
    gasSensorValue = String(gasSensorStatus);

    // Process gas sensor status
    if (gasSensorStatus > gas_threshold)
    {
      gasStatus = "Gas detected";
      Serial.print("Gas detected | Value: ");
      Serial.println(gasSensorStatus);
    }
    else
    {
      gasStatus = "No gas";
      Serial.print("No gas | Value: ");
      Serial.println(gasSensorStatus);
    }

    // Send Event to Web Client with Sensor Status and Sensor Value
    events.send(gasStatus.c_str(), "gas-status", millis());
    events.send(gasSensorValue.c_str(), "gas-value", millis());

    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
  }
}

void setup()
{
  // Disable brownout detector
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  pinMode(fireSensorPin, INPUT);
  pinMode(gasSensorPin, INPUT);

  initWiFi();

  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });

  server.addHandler(&events);
  server.begin();

  // Create task for fire sensor
  xTaskCreatePinnedToCore(
      FireSensorTask,        // Function to execute as a task
      "FireSensorTask",      // Task name
      8192,                  // Task stack size (can be adjusted)
      NULL,                  // Parameter passed to the task
      1,                     // Task priority (higher value means higher priority)
      &FireSensorTaskHandle, // Handle for the task
      0                      // Core where the task will run (0 or 1)
  );

  // Create task for gas sensor
  xTaskCreatePinnedToCore(
      GasSensorTask,        // Function to execute as a task
      "GasSensorTask",      // Task name
      8192,                 // Task stack size (can be adjusted)
      NULL,                 // Parameter passed to the task
      1,                    // Task priority (higher value means higher priority)
      &GasSensorTaskHandle, // Handle for the task
      0                     // Core where the task will run (0 or 1)
  );
}

void loop()
{
  // The main loop is empty as all logic has been moved to tasks.
  delay(1000);
}
