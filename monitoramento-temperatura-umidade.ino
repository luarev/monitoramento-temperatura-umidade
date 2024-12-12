#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <WiFi.h>
#include <DHT.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

const char* ssid = "SUA_REDE_WIFI";  // Nome da rede Wi-Fi
const char* password = "SUA_SENHA";   // Senha da rede Wi-Fi

hd44780_I2Cexp lcd;

#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

float temperatura = 0.0;
float umidade = 0.0;

void notifyClients() {
  String json = "{\"temperatura\":" + String(temperatura) + ",\"umidade\":" + String(umidade) + "}";
  ws.textAll(json);
}

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Inicializa o DHT11
  dht.begin();

  // Inicializa o LCD
  int status = lcd.begin(16, 2);
  if (status) {
    Serial.print("Erro ao inicializar LCD: ");
    Serial.println(status);
    while (1);
  }

  lcd.setCursor(0, 0);
  lcd.print("Inicializando...");
  delay(2000);
  lcd.clear();

  // Conecta ao Wi-Fi
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Configura o WebSocket
  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
      Serial.printf("Cliente conectado: %u\n", client->id());
    }
  });
  server.addHandler(&ws);

  // Configura o servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Monitoramento de Temperatura e Umidade</title>
        <style>
          body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background: linear-gradient(135deg, #56ccf2, #2f80ed);
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            color: white;
          }
          .card {
            background: white;
            border-radius: 15px;
            box-shadow: 0 8px 15px rgba(0, 0, 0, 0.2);
            max-width: 350px;
            width: 100%;
            text-align: center;
            padding: 20px;
            margin: 20px;
            color: #333;
          }
          .card h1 {
            font-size: 1.8em;
            margin-bottom: 10px;
            color: #2f80ed;
          }
          .data p {
            font-size: 1.2em;
            margin: 5px 0;
          }
          .value {
            font-size: 2.5em;
            font-weight: bold;
            margin: 10px 0;
          }
          .footer {
            font-size: 0.9em;
            margin-top: 15px;
            color: #555;
          }
        </style>
        <script>
          let socket = new WebSocket("ws://" + window.location.hostname + "/ws");
          socket.onmessage = function(event) {
            let data = JSON.parse(event.data);
            let tempColor = data.temperatura < 18 || data.temperatura > 30 ? "red" : "green";
            let umidColor = data.umidade < 40 || data.umidade > 80 ? "red" : "green";
            document.getElementById("umidade").innerHTML = data.umidade + "%";
            document.getElementById("temperatura").innerHTML = data.temperatura + "°C";
            document.getElementById("temperatura").style.color = tempColor;
            document.getElementById("umidade").style.color = umidColor;
          };
        </script>
      </head>
      <body>
        <div class="card">
          <h1>Monitoramento de Temperatura e Umidade</h1>
          <div class="data">
            <p>Umidade:</p>
            <p id="umidade" class="value" style="color: green;">--%</p>
            <p>Temperatura:</p>
            <p id="temperatura" class="value" style="color: green;">--°C</p>
          </div>
          <div class="footer">
            Atualizado em tempo real via ESP32
          </div>
        </div>
      </body>
      </html>
    )rawliteral";
    request->send(200, "text/html; charset=UTF-8", html);
  });

  server.begin();
}

void loop() {
  // Lê os valores do sensor DHT11
  umidade = dht.readHumidity();
  temperatura = dht.readTemperature();

  // Verifica se os valores são válidos
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Erro ao ler o sensor!");
    lcd.setCursor(0, 0);
    lcd.print("Erro no sensor!");
    delay(2000);
    return;
  }

  // Mostra os valores no LCD
  lcd.setCursor(0, 0);
  lcd.print("Umid: ");
  lcd.print(umidade);
  lcd.print("%   ");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperatura);
  lcd.write(223); // Adiciona o caractere "º"
  lcd.print("C   ");

  // Envia os dados aos clientes WebSocket
  notifyClients();

  // Aguarda 2 segundos
  delay(2000);
}
