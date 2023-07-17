#include "rotary.h"
#include "mqtt.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_log.h"

#define ROTARY_PIN_A 2
#define ROTARY_PIN_B 4

int counter = 0;

int rotary_r = 0;
int rotary_g = 0;
int rotary_b = 0;

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
            rotary_r = (int) valorR;
            rotary_g = (int) valorG;
            rotary_b = (int) valorB;
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

    esp_err_t res = nvs_set_i32(particao_padrao_handle, "led_vermelho", rotary_r);
    nvs_commit(particao_padrao_handle);

    res = nvs_set_i32(particao_padrao_handle, "led_verde", rotary_g);
    nvs_commit(particao_padrao_handle);

    res = nvs_set_i32(particao_padrao_handle, "led_azul", rotary_b);
    nvs_commit(particao_padrao_handle);

    if(res != ESP_OK) {
        ESP_LOGE("NVS", "Não foi possível escrever no NVS (%s)", esp_err_to_name(res));
    }
    
    nvs_close(particao_padrao_handle);  
}

void control_rotary_decoder() {
  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pin_bit_mask = (1ULL<<ROTARY_PIN_A) | (1ULL<<ROTARY_PIN_B);
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  gpio_config(&io_conf);

  int lastA = 0;

  while (true) {
    int levelA = gpio_get_level(ROTARY_PIN_A);
    int levelB = gpio_get_level(ROTARY_PIN_B);

    if(activeMax == 't'){
      rotary_r = rotary_g = rotary_b = 255;
    }
    else if (activeMin == 't'){
      rotary_r = rotary_g = rotary_b = 0;
    }
    else {
      if(levelA != lastA){
        if(levelA == levelB){
          if(activeR == 't' && rotary_r < 255){
            rotary_r++;
          }
          if(activeG == 't' && rotary_g < 255){
            rotary_g++;
          }
          if(activeB == 't' && rotary_b < 255){
            rotary_b++;
          }
        }
        else {
          if(activeR == 't' && rotary_r  > 0){
            rotary_r--;
          }
          if(activeG == 't' && rotary_g  > 0){
            rotary_g--;
          }
          if(activeB == 't' && rotary_b  > 0){
            rotary_b--;
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