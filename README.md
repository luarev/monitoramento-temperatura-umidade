# Monitoramento de Temperatura e Umidade

Este projeto utiliza um **ESP32**, um **sensor DHT11** e um **display LCD I2C** para monitorar em tempo real a **temperatura** e a **umidade** de um ambiente. Os dados são exibidos no **display LCD** e também em uma interface **web responsiva**, acessível via Wi-Fi, que atualiza os valores em tempo real.


---

## Funcionalidades
- **Monitoramento em tempo real**:
  - Exibe os dados no **display LCD**.
  - Atualiza os dados dinamicamente em um servidor web.
- **Interface Web Responsiva**:
  - Interface agradável com cores indicativas (verde e vermelho) para os valores de temperatura e umidade.
  - Atualização em tempo real usando **WebSocket**.
- **Conectividade Wi-Fi**:
  - Permite o acesso remoto ao servidor via Wi-Fi.
- **Indicadores Visuais**:
  - Valores fora dos limites normais (configuráveis) são exibidos em vermelho.

---

## Componentes Utilizados
- **ESP32**
- **Sensor DHT11**
- **Display LCD I2C 16x2**
- **Protoboard**
- **Resistor 10kΩ**
- **Fios Jumpers**

---

## Circuito
Aqui está a configuração dos pinos e conexões:
1. **Sensor DHT11**:
   - **VCC** → VIN do ESP32
   - **GND** → GND do ESP32
   - **DATA** → GPIO15 do ESP32
2. **Display LCD I2C**:
   - **VCC** → 3.3V do ESP32
   - **GND** → GND do ESP32
   - **SDA** → GPIO21 do ESP32
   - **SCL** → GPIO22 do ESP32

---

## Como Usar

### 1. Pré-requisitos
- **Arduino IDE** instalada.
- **Bibliotecas necessárias**:
  - `hd44780`
  - `DHT`
  - `ESPAsyncWebServer`
  - `AsyncTCP`

Para instalar as bibliotecas, use a **Library Manager** no Arduino IDE:
1. Vá em **Sketch > Incluir Biblioteca > Gerenciar Bibliotecas**.
2. Procure as bibliotecas mencionadas acima e instale.

---

### 2. Configuração do Código
No arquivo `monitoramento-temperatura-umidade.ino`:
1. Substitua as seguintes linhas pelas credenciais da sua rede Wi-Fi:
   ```cpp
   const char* ssid = "SEU_WIFI";
   const char* password = "SUA_SENHA";

## 3. Acessando o Servidor Web
1. Abra o Monitor Serial no Arduino IDE para obter o endereço IP gerado pelo ESP32.
2. Copie o endereço IP e cole no navegador (ex.: `http://192.168.1.116`).
3. Você verá uma interface interativa exibindo os valores de temperatura e umidade em tempo real.

--- 

## Autores

- **Luana Revoredo** - Desenvolvimento do sistema e documentação.


