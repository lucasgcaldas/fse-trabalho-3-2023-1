#ifndef MQTT_H
#define MQTT_H

extern char activeR;
extern char activeG;
extern char activeB;
extern char activeMax;
extern char activeMin;

void mqtt_start();

void mqtt_envia_mensagem(char * topico, char * mensagem);
void handle_data(char *data);

#endif