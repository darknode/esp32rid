#ifndef SQUID_NETWORK_H
#define SQUID_NETWORK_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include <nvs_flash.h>
#include "BLEDevice.h"
#include "BLEUtils.h"

esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);
esp_err_t event_handler(void *, system_event_t *);

static const char *password = "password";

#define SD_NETWORK_QUEUE_SIZE 100
#define SD_NETWORK_PULSE 60

// void construct2(void);
// void init2(char *, int, uint8_t *, uint8_t);
// int transmit_wifi2(uint8_t *, int);
// int transmit_ble2(uint8_t *, int);

typedef enum Squid_Network_Mode
{
    SD_NETWORK_MODE_HYBRID = 0,
    SD_NETWORK_MODE_BT = 1,
    SD_NETWORK_MODE_WIFI = 2,
} Squid_Network_Mode_t;

struct Squid_Network_Message
{
    uint8_t mac[6];
    char ssid[32];
    int ssid_length;
    uint8_t protocol;
    uint8_t *buffer;
    int length;
    uint8_t placed;
};

class Squid_Network
{

public:
    Squid_Network();
    void begin(Squid_Network_Mode_t);
    void begin();
    void setWifiDriver(int);
    void loop();
    bool addMessage(uint8_t mac[6], char ssid[32], int ssid_length, uint8_t *buffer, int length);

private:
    void transmit_bt(Squid_Network_Message *message);
    void transmit_wifi(Squid_Network_Message *message);
    bool dequeue(Squid_Network_Message *message);
    bool enqueue(Squid_Network_Message message);

    esp_ble_adv_data_t advData;
    esp_ble_adv_params_t advParams;
    BLEUUID service_uuid;
    Squid_Network_Mode_t mode;
    Squid_Network_Message queue[SD_NETWORK_QUEUE_SIZE];

    uint16_t
        queue_index = 0;
    uint8_t
        bt_ok = 0,
        bt_running  = 0,
        wifi_driver = 0,
        bt_msg_counter[16];
    uint32_t
        msg_last = 0,
        bt_last = 0,
        wifi_last = 0;
};

#endif
