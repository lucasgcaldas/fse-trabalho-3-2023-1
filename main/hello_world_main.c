#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "wifi.h"
#include "mqtt.h"
#include "led.h"
#include "encoder.h"
#include "gpio.h"

#include "driver/gpio.h"
#include "driver/ledc.h"

#include "adc_module.h"

#define TEMP_SENSOR ADC_CHANNEL_6

#define TOUCH_SENSOR 18
#define SOUND_SENSOR 19

// Definição dos pinos GPIO para os segmentos
#define SEGMENT_A_PIN 14
#define SEGMENT_B_PIN 25  
#define SEGMENT_C_PIN 33  
#define SEGMENT_D_PIN 12
#define SEGMENT_E_PIN 13
#define SEGMENT_F_PIN 27
#define SEGMENT_G_PIN 26

const int segments[] = {
    SEGMENT_A_PIN,
    SEGMENT_B_PIN,
    SEGMENT_C_PIN,
    SEGMENT_D_PIN,
    SEGMENT_E_PIN,
    SEGMENT_F_PIN,
    SEGMENT_G_PIN
};

SemaphoreHandle_t conexaoWifiSemaphore;
SemaphoreHandle_t conexaoMQTTSemaphore;

void conectadoWifi(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      mqtt_start();
    }
  }
}

// Função para exibir um dígito no display de 7 segmentos
void displayDigit(uint8_t digit) {
    // Mapeamento dos dígitos para o código dos segmentos
    const uint8_t digits[] = {
        0b1000000,  // 0
        0b1111001,  // 1
        0b0100100,  // 2
        0b0110000,  // 3
        0b0011001,  // 4
        0b0010010,  // 5
        0b0000010,  // 6
        0b1111000,  // 7
        0b0000000,  // 8
        0b0010000   // 9
    };

    // Verifica se o dígito é válido (0-9)
    if (digit >= 0 && digit <= 9) {
        // Ativa os segmentos correspondentes para exibir o dígito
        for (int i = 0; i < 7; i++) {
            gpio_set_level(segments[i], (digits[digit] >> i) & 1);
        }
    } else {
        // Desativa todos os segmentos se o dígito for inválido
        for (int i = 0; i < 7; i++) {
            gpio_set_level(segments[i], 0);
        }
    }
}

int getTemperature() {
    return analogRead(TEMP_SENSOR);
}

void trataComunicacaoComServidor(void * params)
{
  char mensagem[50];
  if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY)) {
    int i = 0;
    while(true) {
      
      if (i == 10) 
        i = 0;

      sprintf(mensagem, "{\"potencia_vermelho\": %d}", encoder_r);
      mqtt_envia_mensagem("v1/devices/me/attributes", mensagem);

      sprintf(mensagem, "{\"potencia_verde\": %d}", encoder_g);
      mqtt_envia_mensagem("v1/devices/me/attributes", mensagem);

      sprintf(mensagem, "{\"potencia_azul\": %d}", encoder_b);
      mqtt_envia_mensagem("v1/devices/me/attributes", mensagem);

      float temperatura = getTemperature() / 100.0;
      sprintf(mensagem, "{\"temperatura\": %f}", temperatura);
      mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);

      displayDigit(i % 10);
      sprintf(mensagem, "{\"numero\": %d}", i);
      mqtt_envia_mensagem("v1/devices/me/attributes", mensagem);
      i++;

      sprintf(mensagem, "{\"proximidade\": %d}", proximidade);
      mqtt_envia_mensagem("v1/devices/me/attributes", mensagem);

      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
}

void app_main(void)
{ 
    // Configura os pinos dos segmentos como saída
    for (int i = 0; i < 7; i++) {
        esp_rom_gpio_pad_select_gpio(segments[i]);
        gpio_set_direction(segments[i], GPIO_MODE_OUTPUT);
    }

    // Sensor de proximidade
    esp_rom_gpio_pad_select_gpio(TOUCH_SENSOR);
    gpio_set_direction(TOUCH_SENSOR, GPIO_MODE_INPUT);

    // Buzzer
    esp_rom_gpio_pad_select_gpio(SOUND_SENSOR);
    gpio_set_direction(SOUND_SENSOR, GPIO_MODE_OUTPUT);

    // Temperatura
    adc_init(ADC_UNIT_1);
    adc_config_pin(TEMP_SENSOR);

    inicia_valores();

    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(control_encoder_decoder, "Controle do Encoder", 4096, NULL, 1, NULL);
    xTaskCreate(&conectadoWifi,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);
    xTaskCreate(pwm_r, "Controle LED Vermelho", 4096, NULL, 1, NULL);
    xTaskCreate(pwm_g, "Controle LED Verde", 4096, NULL, 1, NULL);
    xTaskCreate(pwm_b, "Controle LED Azul", 4096, NULL, 1, NULL);
    xTaskCreate(touchSensor, "Presenca e Som", 4096, NULL, 1, NULL);
}
