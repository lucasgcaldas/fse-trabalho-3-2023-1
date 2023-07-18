#include <stdio.h>
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#include "wifi.h"
#include "mqtt.h"
#include "led.h"
#include "encoder.h"

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

void trataComunicacaoComServidor(void * params)
{
  char mensagem[50];
  if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY)) {
    while(true) {
      sprintf(mensagem, "{\"potencia_vermelho\": %d}", encoder_r);
      mqtt_envia_mensagem("v1/devices/me/attributes", mensagem);

      sprintf(mensagem, "{\"potencia_verde\": %d}", encoder_g);
      mqtt_envia_mensagem("v1/devices/me/attributes", mensagem);

      sprintf(mensagem, "{\"potencia_azul\": %d}", encoder_b);
      mqtt_envia_mensagem("v1/devices/me/attributes", mensagem);
      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
}

void app_main(void)
{
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
}
