# Trabalho 3 - Fundamentos de Sistemas Embarcados

## Sobre

Este trabalho visa a criação de sensores e atuadores distribuídos baseados nos microcontroladores ESP32 conectados via Wifi através do protocolo MQTT.
Os microcontroladores ESP32 irão controlar a aquisição de dados de sensores, botões e chaves além de acionar saídas com leds e sensor de toque.

Mais especificações em: [Trabalho Final 2022/1.](https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-3-2022-1)

## Vídeo de demonstração 
-

## Instruções

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

