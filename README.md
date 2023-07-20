## Trabalho 3 de Fundamentos de Sistemas embarcados

Alunos:
Lucas Gomes Caldas - 212005426
Victor Souza Dantas Martins Lima - 190044403

### Requisitos

- Placa ESP32
- Sensores
- Cabo USB para conectar a placa ao computador
- Ambiente de desenvolvimento configurado com a ESP-IDF e as ferramentas necessárias (toolchain)
- Visual Studio Code instalado com a extensão ESP-IDF Explorer

### Compilar e Flash

Obs: $ sudo chmod 777 /dev/ttyUSB0

1. Clone o repositório do projeto para o seu computador.

2. Abra o Visual Studio Code e navegue até a pasta do projeto clonado.

3. Verifique se a extensão ESP-IDF Explorer está instalada e habilitada no VSCode.

4. Vá em SDK config e altere em configuração do wifi CONFIG_ESP_WIFI_SSID para o nome da sua rede e CONFIG_ESP_WIFI_PASSWORD para a senha da sua rede 

5. No VSCode, clique no ícone do ESP-IDF Explorer na barra lateral (logotipo da Espressif).

6. No ESP-IDF Explorer, clique na tarefa "Build" (ícone de martelo) para compilar o projeto.

7. Aguarde o processo de compilação ser concluído e verifique se não houve erros.

8. Conecte a placa ESP32 ou ESP8266 ao computador usando o cabo USB.

9. No ESP-IDF Explorer, clique na tarefa "Flash" (ícone de seta para a direita) para fazer o flash do firmware na placa.

10. Aguarde o processo de flash ser concluído e verifique se não houve erros.
 

### Executar o Projeto

Agora que o firmware foi compilado e gravado na placa, você pode executar o projeto na placa ESP32.

### Sensores Utilizados

1. Sensor de medição de temperatura
2. Display de 8 segmentos
3. Sensor de proximidade
4. Buzzer
5. Led RGB
6. Encoder

### Link para o DashBoard

http://164.41.98.25:443/dashboards/b3558200-25c3-11ee-95da-bd0125f1f921

Acesse esse link para testar o projeto com o dashboard criado

