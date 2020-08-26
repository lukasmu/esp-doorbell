#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <driver/rtc_io.h>
#include "esp-doorbell-config.h"
#include "Sip.h"

// The SSID of your WLAN
const char* wifi_ssid = WIFI_SSID;
// The password of your WLANs
const char* wifi_password = WIFI_PASSWORD;
// The network name of your ESP32
const char* wifi_host = "doorbell";

// The SIP user
const char* sip_user = SIP_USER;
//  The password associated with the SIP user above
const char* sip_password = SIP_PASSWORD;
// The SIP  port
const int sip_port = 5060;

// Number that the doorbell calls
const char* ring_number = "**1";
// Time that the doorbell rings (in s)
const int ring_time = 10;
// The name that is displayed on the phones
const char* ring_name = "Doorbell";

// The WLAN configuration buffer
RTC_DATA_ATTR byte wifi_mac[6] = {0, 0, 0, 0, 0, 0};
RTC_DATA_ATTR byte wifi_channel = 0;
RTC_DATA_ATTR char wifi_ip[16]; 
RTC_DATA_ATTR char wifi_gateway[16]; 
RTC_DATA_ATTR char wifi_mask[16]; 
RTC_DATA_ATTR char wifi_dns[16]; 
RTC_DATA_ATTR int wifi_buffered = 0; 

// Function to put ESP32 into deep sleep
void sleep(void) {
  rtc_gpio_pullup_en(GPIO_NUM_33);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0);
  esp_deep_sleep_start();
}

// Main process
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 wakes up");
  
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);

  if( wifi_buffered != 0 ) {
    Serial.println("Found buffered WiFi config");
    Serial.println("Connecting to WiFi using this config");
    IPAddress temp_ip, temp_gateway, temp_mask, temp_dns;
    temp_ip.fromString(wifi_ip);
    temp_gateway.fromString(wifi_gateway);
    temp_mask.fromString(wifi_mask);
    temp_dns.fromString(wifi_dns);
    WiFi.config(temp_ip, temp_gateway, temp_mask, temp_dns);
    WiFi.begin(wifi_ssid, wifi_password, wifi_channel, wifi_mac);
  }
  else {
    Serial.println("Did not find buffered WiFi config");
    Serial.println("Conneting to WiFi using SSID and password only");
    WiFi.begin(wifi_ssid, wifi_password);
  }

  WiFi.setHostname(wifi_host);
  
  int retries = 0;
  int wifi_status = WiFi.status();
  while( wifi_status != WL_CONNECTED ) {
    retries++;
    if( retries == 100 ) {
      // This is called after 5s
      Serial.println("Could not connect to WiFi");
      Serial.println("Retrying using SSID and password only");
      WiFi.disconnect();
      delay(50);
      wifi_buffered = 0;
      WiFi.begin(wifi_ssid, wifi_password);
    }
    if( retries == 600 ) {
      // This is called after 30s 
      Serial.println("Could not connect to WiFi");
      Serial.println("Aborting");
      WiFi.disconnect(true);
      delay(50);
      Serial.println("ESP32 goes back to deep sleep");
      Serial.println("");
      sleep();
    }
    delay(50);
    wifi_status = WiFi.status();
  }

  if ( wifi_buffered == 0 ) {
    Serial.println("Buffering WiFi config (to speedup next WiFi connection)");
    uint8_t* bssid;
    bssid = WiFi.BSSID();
    for (uint32_t i = 0; i < sizeof(wifi_mac); i++) wifi_mac[i] = bssid[i];
    WiFi.localIP().toString().toCharArray(wifi_ip, 16);
    WiFi.gatewayIP().toString().toCharArray(wifi_gateway, 16);
    WiFi.subnetMask().toString().toCharArray(wifi_mask, 16);
    WiFi.dnsIP().toString().toCharArray(wifi_dns, 16);;
    wifi_buffered = 1;
  }

  Serial.println("Initiating the SIP call");
  Sip sip_client;
  sip_client.Init(wifi_gateway, sip_port, wifi_ip, sip_port, sip_user, sip_password, ring_time);
  sip_client.Dial(ring_number, ring_name);
  sip_client.Wait();

  Serial.println("ESP32 goes back to deep sleep");
  Serial.println("");
  sleep();
}

// We will never enter the loop as we are always going to deep sleep in the setup method
void loop(){
}
