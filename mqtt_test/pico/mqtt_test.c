#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/mqtt.h"

// Wi-Fi credentials
#define WIFI_SSID "wifi_ssid"
#define WIFI_PASSWORD "wifi_password"

// MQTT broker settings - PC's IP address
#define MQTT_BROKER_IP "192.168.1.100"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "pico_client"

// MQTT topics
#define MQTT_TOPIC_PUBLISH "pico/data"
#define MQTT_TOPIC_SUBSCRIBE "pc/command"

static mqtt_client_t *mqtt_client;
static bool mqtt_connected = false;

// MQTT connection status callback
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("MQTT client connected\n");
        mqtt_connected = true;
        
        // Subscribe to topic to receive commands
        mqtt_sub_unsub(client, 
                      MQTT_TOPIC_SUBSCRIBE, 1,
                      NULL, NULL, 
                      1);
    } else {
        printf("MQTT client connection failed: %d\n", status);
        mqtt_connected = false;
    }
}

// Simple callbacks for receiving messages
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
    printf("Message received on topic: %s\n", topic);
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    if (flags & MQTT_DATA_FLAG_LAST) {
        char message[len + 1];
        memcpy(message, data, len);
        message[len] = 0;
        
        printf("Received: %s\n", message);
        
        // Control onboard LED based on command
        if (strcmp(message, "on") == 0) {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        } else if (strcmp(message, "off") == 0) {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        }
    }
}

int main() {
    stdio_init_all();
    printf("Pico W MQTT Client\n");
    sleep_ms(1000);

    // Initialize WiFi
    if (cyw43_arch_init()) {
        printf("WiFi init failed\n");
        return -1;
    }

    cyw43_arch_enable_sta_mode();
    printf("Connecting to WiFi...\n");
    
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("WiFi connection failed\n");
        return -1;
    }
    printf("WiFi connected\n");

    // Set up MQTT client
    ip_addr_t broker_addr;
    ipaddr_aton(MQTT_BROKER_IP, &broker_addr);
    
    mqtt_client = mqtt_client_new();
    
    // Configure MQTT client
    struct mqtt_connect_client_info_t client_info = {
        .client_id = MQTT_CLIENT_ID,
        .client_user = NULL,
        .client_pass = NULL,
        .keep_alive = 60,
        .will_topic = NULL,
        .will_msg = NULL,
        .will_retain = 0,
        .will_qos = 0
    };

    // Set callbacks
    mqtt_set_inpub_callback(mqtt_client, 
                          mqtt_incoming_publish_cb,
                          mqtt_incoming_data_cb,
                          NULL);
    
    // Connect to broker
    printf("Connecting to MQTT broker...\n");
    mqtt_client_connect(mqtt_client,
                      &broker_addr,
                      MQTT_BROKER_PORT,
                      mqtt_connection_cb,
                      NULL,
                      &client_info);

    // Main loop - send status every few seconds
    int counter = 0;
    char message[32];

    while (true) {
        // Publish message every 5 seconds if connected
        if (mqtt_connected && counter % 5 == 0) {
            sprintf(message, "pico_status:%d", counter);
            
            mqtt_publish(mqtt_client,
                       MQTT_TOPIC_PUBLISH,
                       message,
                       strlen(message),
                       0, // QoS 0 - fire and forget
                       0, // Not retained
                       NULL,
                       NULL);
            
            printf("Published: %s\n", message);
        }
        
        counter++;
        sleep_ms(1000);
    }

    return 0;
}