#ifndef MQTT_H
#define MQTT_H

extern int activeR;
extern int activeG;
extern int activeB;
extern int activeMax;
extern int activeMin;

void mqtt_start();

void mqtt_envia_mensagem(char *topico, char *mensagem);
void handle_data(char *data);

#endif