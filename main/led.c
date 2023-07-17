#include "led.h"
#include <driver/gpio.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "rotary.h"


#define LED_R 22
#define LED_G 23
#define LED_B 21

void pwm_r(){
    pwm('r');
}
void pwm_g(){
    pwm('g');
}
void pwm_b(){
    pwm('b');
}

void pwm(char led) {

int led_channel;
int other_channels;

if(led == 'r'){
    led_channel = LED_R;
    other_channels = 0;
}
else if(led == 'g'){
    led_channel = LED_G;
    other_channels = 2;
}
else if(led == 'b'){
    led_channel = LED_B;
    other_channels = 3;
}
else {
    return;
}

// Configuração do Timer
  ledc_timer_config_t timer_config = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .timer_num = other_channels,
    .freq_hz = 1000,
    .clk_cfg = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer_config);

  // Configuração do Canal
  ledc_channel_config_t channel_config_r = {
    .gpio_num = led_channel,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = other_channels,
    .timer_sel = other_channels,
    .duty = 0,
    .hpoint = 0
  };
  ledc_channel_config(&channel_config_r);

  ledc_fade_func_install(0);
  while(true) {
    if(led == 'r') {
      ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, other_channels, rotary_r, 1000, LEDC_FADE_WAIT_DONE);
    }
    else if(led == 'g') {
      ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, other_channels, rotary_g, 1000, LEDC_FADE_WAIT_DONE);
    }
    else if(led == 'b') {
      ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, other_channels, rotary_b, 1000, LEDC_FADE_WAIT_DONE);
    }
  }
}