#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
#define LED 2
// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif


void btStatus(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("Client Connected");
    digitalWrite(LED, HIGH);
  } else if (event == ESP_SPP_CLOSE_EVT) {
    Serial.println("Client Disconnected");
    digitalWrite(LED, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); // Bluetooth device name
  SerialBT.register_callback(btStatus);
  pinMode(LED, OUTPUT);
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (SerialBT.available()) {
    String message = SerialBT.readString();
    message.trim();
    SerialBT.print("Received: ");
    SerialBT.println(message);
    Serial.println("Sent response: " + message);
  }
  delay(20);
}
