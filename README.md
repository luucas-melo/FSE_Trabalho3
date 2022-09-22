# Trabalho 3 - Fundamentos de Sistemas Embarcados

## Sobre

Este trabalho visa a criação de sensores e atuadores distribuídos baseados nos microcontroladores ESP32 conectados via Wifi através do protocolo MQTT.
Os microcontroladores ESP32 controlam a aquisição de dados dos sensores, botões e chaves além de acionar saídas com leds e sensor de toque.

Mais especificações em: [Trabalho Final 2022/1.](https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-3-2022-1)

## Funcionalidades do sistema

Os componentes do sistema permitem:

- Leitura e envio dos valores de temperatura e umidade;
- Acionamento do LED (Saída) à partir dos comandos RPC enviados pelo Dashboard de maneira dimerizável, com sua intensidade controlada à partir da técnica de PWM;
- Acionamento de um sensor de toque que alterna a cor do LED RG quando acionado.

## Vídeo de demonstração 

<div align="center">
   <iframe width="560" height="315" src="https://youtu.be/JCy7crpBh_U" frameborder="0" allowfullscreen>
   </iframe>
</div>

## Instruções de uso

1. Clonar o repositório:

```
$ git clone https://github.com/luucas-melo/FSE_Trabalho3.git
```

2. Para atualizar os dados de conexão wifi acesse o menu de configuração:
```
$ pio run -t menucofig
```

3. . O menu de configuração do projeto é: 
```
$ ** Project Configuration --->
```

4. Para rodar os servidores distribuídos instale extensão [Platform.IO](https://platformio.org/) no VsCode execute o comando: 

```
$ platformio run --target upload --target monitor --environment esp32doit-devkit-v1
```

5. O dashboard com a leitura dos dispositivos pode ser visualizado em [Ailamar-Lucas-Dashboard.](http://164.41.98.25:443/dashboards/6cd45ee0-360f-11ed-be92-e3a443145aec)

## Dashboard

### Temperatura e humidade
![image](https://user-images.githubusercontent.com/48891167/191668391-da5a0af6-d44b-4020-add6-699f7623cb84.png)

### Sensor de toque e LED RG
![image](/assets/sensores-toque-pwm.JPG)


