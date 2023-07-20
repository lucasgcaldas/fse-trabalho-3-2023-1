#include "encoder.h"
#include "mqtt.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"

#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 4

int counter = 0;

int encoder_r = 255;
int encoder_g = 255;
int encoder_b = 255;

void inicia_valores() {
    ESP_ERROR_CHECK(nvs_flash_init());

    int32_t valorR = 0;
    int32_t valorG = 0;
    int32_t valorB = 0;

    nvs_handle particao_padrao_handle;

    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READONLY, &particao_padrao_handle);

    if(res_nvs == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
    }
    else {
        esp_err_t res = nvs_get_i32(particao_padrao_handle, "led_vermelho",  &valorR);
        res = nvs_get_i32(particao_padrao_handle, "led_verde",     &valorG);
        res = nvs_get_i32(particao_padrao_handle, "led_azul",      &valorB);

        switch (res)
        {
        case ESP_OK:
            //printf("Valores armazenados: %d %d %d\n", (int) valorR, (int) valorG, (int) valorB);
            encoder_r = (int) valorR;
            encoder_g = (int) valorG;
            encoder_b = (int) valorB;

            // encoder_r = 255;
            // encoder_g = 255;
            // encoder_b = 255;
            break;
        case ESP_ERR_NOT_FOUND:
            ESP_LOGE("NVS", "Valor não encontrado");
        default:
            ESP_LOGE("NVS", "Erro ao acessar o NVS (%s)", esp_err_to_name(res));
            break;
        }

    }
    
    nvs_close(particao_padrao_handle);
}

void grava_valores() {
    ESP_ERROR_CHECK(nvs_flash_init());

    nvs_handle particao_padrao_handle;

    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READWRITE, &particao_padrao_handle);

    if(res_nvs == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
    }

    esp_err_t res = nvs_set_i32(particao_padrao_handle, "led_vermelho", encoder_r);
    nvs_commit(particao_padrao_handle);

    res = nvs_set_i32(particao_padrao_handle, "led_verde", encoder_g);
    nvs_commit(particao_padrao_handle);

    res = nvs_set_i32(particao_padrao_handle, "led_azul", encoder_b);
    nvs_commit(particao_padrao_handle);

    if(res != ESP_OK) {
        ESP_LOGE("NVS", "Não foi possível escrever no NVS (%s)", esp_err_to_name(res));
    }
    
    nvs_close(particao_padrao_handle);  
}

void control_encoder_decoder() {
  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pin_bit_mask = (1ULL<<ENCODER_PIN_A) | (1ULL<<ENCODER_PIN_B);
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  gpio_config(&io_conf);

  int lastA = 0;

  while (true) {
    int levelA = gpio_get_level(ENCODER_PIN_A);
    int levelB = gpio_get_level(ENCODER_PIN_B);
    // printf("activeMax %d\n", activeMax);
    // printf("activeMin %d\n", activeMin);
    // printf("activeR %d\n", activeR);
    // printf("activeG %d\n", activeG);
    // printf("activeB %d\n", activeB);
    // printf("level A %d\n", levelA);
    // printf("last A %d\n", lastA);
    // printf("level B %d\n", levelB);
    // printf("encoder_r %d\n", encoder_r);
    // printf("encoder_g %d\n", encoder_g);
    // printf("encoder_b %d\n", encoder_b);

    if(activeMax == 1){
      encoder_r = encoder_g = encoder_b = 0;
    }
    else if (activeMin == 1){
      encoder_r = encoder_g = encoder_b = 255;
    }
    else {
      if(levelA != lastA){
        if(levelA == levelB){
          if(activeR == 1 && encoder_r  > 5){
            encoder_r -= 25;
          }
          if(activeG == 1 && encoder_g  > 5){
            encoder_g -= 25;
          }
          if(activeB == 1 && encoder_b  > 5){
            encoder_b -= 25;
          }
        }
        else {
          if(activeR == 1 && encoder_r < 255){
            encoder_r += 25;
          }
          if(activeG == 1 && encoder_g < 255){
            encoder_g += 25;
          }
          if(activeB == 1 && encoder_b < 255){
            encoder_b += 25;
          }
        }
      }
    }

    counter++;

    if(counter % 400 == 0) {
      grava_valores();
    }

    lastA = levelA;
    vTaskDelay(40 / portTICK_PERIOD_MS);
  }
    
}