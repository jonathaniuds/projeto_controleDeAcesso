//==========================DISPLAY===========================
#include <Wire.h>     //biblioteca do display
#include <U8g2lib.h>  //biblioteca do display

//COMANDO PARA VARIAVEL TELA
U8X8_SH1106_128X64_NONAME_HW_I2C tela(U8X8_PIN_NONE);
//============================RFID============================
#include <SPI.h>      //Biblioteca para utilização do protocolo SPI;
#include <MFRC522.h>  //Biblioteca para utilização do circuito RFID MFRC522;

#define C_SELECT D4  //Pino SDA do módulo;
#define RESET D3     //Pino RESET do módulo MFRC522;

MFRC522 rfid(C_SELECT, RESET);  //Declaração do módulo com o nome "rfid" no sistema com os pinos do define;
//=============================================================
#include <RTClib.h>

RTC_DS1307 rtc;

//====================== FUNCAO PARA UTILIZAR O MODULO RTC ==================================
String data_hora() {
  DateTime data_hora_atual = rtc.now();

  String anoStr = String(data_hora_atual.year(), DEC);
  String mesStr = (data_hora_atual.month() < 10 ? "0" : "") + String(data_hora_atual.month(), DEC);
  String diaStr = (data_hora_atual.day() < 10 ? "0" : "") + String(data_hora_atual.day(), DEC);
  String horaStr = (data_hora_atual.hour() < 10 ? "0" : "") + String(data_hora_atual.hour(), DEC);
  String minutoStr = (data_hora_atual.minute() < 10 ? "0" : "") + String(data_hora_atual.minute(), DEC);
  String segundoStr = (data_hora_atual.second() < 10 ? "0" : "") + String(data_hora_atual.second(), DEC);

  String tempoFormatado = anoStr + "-" + mesStr + "-" + diaStr + " " + horaStr + ":" + minutoStr + ":" + segundoStr;
  Serial.println("Data e Hora: " + tempoFormatado);
  return tempoFormatado;
}
//===========================================================================================

//============================ESP NODE MCU================================
#include <ESP8266WiFi.h>       //Biblioteca que contém as funcionalidades de Wi-Fi
#include <ESP8266WebServer.h>  //Biblioteca que permite a criação de um servidor HTTP via página web;

ESP8266WebServer server(80);  //Instanciação do servidor web na porta padrão HTTP (porta 80);

const char* ssid = "NodeMCU - Teste AP";  // Nome da rede Access Point (Semelhante a um roteador)
const char* password = "Teste123";        //Senha da rede Access Point
//==========================================================================

//==================== STRUCT =================
struct Usuario {
  String nome;
  String endereco;
  bool status;
};

struct JSON{
    String tempoJSON;
    String enderecoJSON;
    String nomeJSON;
    String statusJSON;
}
//===============================================

//========================= VARIAVEIS =============================
short contadorUsuario = 0;          // variavel para contar de usuarios existentes no sistemas 
const short capacida_MAX = 50; // Constante para armazenar a capacidade maxima de usuario no sistema
Usuario usuario[capacida_MAX];
JSON objetoJsn;

bool usuarioEncontrado = false;
String estado;

unsigned long tempo = 0;
unsigned long espera = 3000;

String dados = "";  //String vazia para armazenar o endereço da tag/cartão RFID;

//=================================================
String hexa(byte* ptr_buf, byte tam_buf) {
  String endereco = "";

  for (byte j = 0; j < tam_buf; j++) {
    if (ptr_buf[j] < 0x10) {
      endereco.concat(String(" 0"));
    } else {
      endereco.concat(String(" "));
    }
    endereco.concat(String(ptr_buf[j], HEX));
  }
  endereco.toUpperCase();
  return endereco;
  endereco = "";
}
//====================================================

void paginaPrincipal() {
  String pagina = R"rawliteral(
   <!DOCTYPE html>
<html lang="pt-BR">

<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Sistema RFID</title>

    <style>
        :root {
            --primaria: #2c3e50;
            --secundaria: #2980b9;
            --fundo: #f0f2f5;
            --texto-claro: #ffffff;
            --texto-escuro: #34495e;
            --verde: #2ecc71;
            --vermelho: #e74c3c;
            --cinza: #bdc3c7;
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        html, body {
            height: 100%;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: var(--fundo);
            color: var(--texto-escuro);
        }

        header {
            background-color: var(--primaria);
            color: var(--texto-claro);
            padding: 20px;
            text-align: center;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }

        .main {
            display: flex;
            height: calc(100% - 80px);
        }

        .coluna-esquerda {
            width: 30%;
            padding: 20px;
        }

        .coluna-direita {
            width: 70%;
            display: flex;
            flex-direction: column;
            gap: 20px;
            padding: 20px;
        }

        .card {
            background-color: #fff;
            border-radius: 10px;
            box-shadow: 0 3px 8px rgba(0, 0, 0, 0.05);
            padding: 20px;
            display: flex;
            flex-direction: column;
        }

        .card-header {
            font-size: 20px;
            font-weight: 600;
            color: var(--secundaria);
            border-bottom: 2px solid var(--cinza);
            margin-bottom: 15px;
            padding-bottom: 5px;
        }

        .ultimo-acesso {
            display: flex;
            flex-direction: column;
            gap: 20px;
        }

        .info-item {
            display: flex;
            gap: 8px;
            font-size: 16px;
        }

        .label {
            font-weight: bold;
            color: #555;
        }

        .valor {
            color: var(--texto-escuro);
        }

        table {
            width: 100%;
            border-collapse: collapse;
            font-size: 14px;
        }

        th, td {
            padding: 10px;
            border-bottom: 1px solid #e0e0e0;
            text-align: left;
        }

        th {
            background-color: #f8f9fa;
            color: var(--texto-escuro);
        }

        tr:hover {
            background-color: #f1f3f5;
        }

        .status-tag {
            padding: 5px 10px;
            border-radius: 20px;
            font-size: 13px;
            font-weight: bold;
            display: inline-block;
        }

        .entrada {
            background-color: #e9f7ef;
            color: var(--verde);
        }

        .saida {
            background-color: #fdecea;
            color: var(--vermelho);
        }

        .atualizacao {
            text-align: center;
            font-size: 13px;
            color: #888;
            padding: 10px;
        }

        .tabela-card {
            flex: 1;
            overflow-y: auto;
            max-height: 45%;
            border: 1px solid #ddd;
            border-radius: 8px;
        }

        .tabela-card::-webkit-scrollbar {
            width: 6px;
        }

        .tabela-card::-webkit-scrollbar-thumb {
            background-color: #ccc;
            border-radius: 10px;
        }

    /* Rodape */
    .footer {
      background-color: var(--white);
      border-radius: var(--border-radius);
      box-shadow: var(--shadow);
      text-align: center;
      color: var(--text-muted);
      font-size: 0.85rem;
      padding: 15px;
      margin-top: 20px;
    }
    
    .footer strong {
      color: var(--primary);
    }

        @media (max-width: 768px) {
            .main {
                flex-direction: column;
                height: auto;
            }

            .coluna-esquerda,
            .coluna-direita {
                width: 100%;
                height: auto;
            }

            .tabela-card {
                max-height: none;
            }
        }
    </style>
</head>

<body>
    <header>
        <h1>Sistema de Monitoramento RFID</h1>
    </header>

    <div class="main">
        <div class="coluna-esquerda">
            <div class="card" style="height: 100%;">
                <div class="card-header">Ultimo Acesso</div>
                <div class="ultimo-acesso">
                    <div class="info-item"><span class="label">ID:</span><span class="valor" id="ID">Carregando...</span></div>
                    <div class="info-item"><span class="label">Nome:</span><span class="valor" id="nome">Carregando...</span></div>
                    <div class="info-item"><span class="label">Data/Hora:</span><span class="valor" id="tempo">Carregando...</span></div>
                    <div class="info-item"><span class="label">Status:</span><span class="valor" id="status">Carregando...</span></div>
                </div>
            </div>
        </div>

        <div class="coluna-direita">
            <div class="card tabela-card">
                <div class="card-header">Historico de Entradas</div>
                <table id="tabela-entradas">
                    <thead>
                        <tr>
                            <th>Data/Hora</th>
                            <th>ID</th>
                            <th>Nome</th>
                            <th>Status</th>
                        </tr>
                    </thead>
                    <tbody></tbody>
                </table>
            </div>

            <div class="card tabela-card">
                <div class="card-header">Historico de Saidas</div>
                <table id="tabela-saidas">
                    <thead>
                        <tr>
                            <th>Data/Hora</th>
                            <th>ID</th>
                            <th>Nome</th>
                            <th>Status</th>
                        </tr>
                    </thead>
                    <tbody></tbody>
                </table>
            </div>
        </div>
    </div>

    <div class="atualizacao">
        Ultima atualizacao: <span id="ultima-atualizacao">-</span>
    </div>

    <!-- Rodape -->
    <div class="footer">
      <strong>Sistema de Controle de Acesso RFID</strong> | Powered by NodeMCU<br>
      Ultima atualizacao: <span id="ultima-atualizacao">-</span>
    </div>
  </div>

      <script>
        let historicoEntradas = "";
        let historicoSaidas = "";

        // Variável para armazenar último registro inserido
        let ultimoRegistro = {
            id: null,
            nome: null,
            tempo: null,
            status: null
        };

        function atualizarDados() {
            fetch('/dados')
                .then(response => response.json())
                .then(data => {
                    // Verifica se os dados mudaram em relação ao último registro
                    if (
                        data.id !== ultimoRegistro.id ||
                        data.nome !== ultimoRegistro.nome ||
                        data.tempo !== ultimoRegistro.tempo ||
                        data.status !== ultimoRegistro.status
                    ) {
                        // Atualiza os dados do último acesso
                        document.getElementById('ID').textContent = data.id;
                        document.getElementById('nome').textContent = data.nome;
                        document.getElementById('tempo').textContent = data.tempo;
                        document.getElementById('status').textContent = data.status;

                        // Atualiza a hora da última atualização
                        document.getElementById('ultima-atualizacao').textContent = new Date().toLocaleTimeString();

                        // Nova linha
                        let novaLinha = `
                            <tr>
                                <td>${data.tempo}</td>
                                <td>${data.id}</td>
                                <td>${data.nome}</td>
                                <td><span class="status-tag ${data.status.toLowerCase()}">${data.status}</span></td>
                            </tr>
                        `;

                        if (data.status.toLowerCase() === "entrada") {
                            historicoEntradas = novaLinha + historicoEntradas;
                            document.getElementById('tabela-entradas').querySelector('tbody').innerHTML = historicoEntradas;
                        } else if (data.status.toLowerCase() === "saida") {
                            historicoSaidas = novaLinha + historicoSaidas;
                            document.getElementById('tabela-saidas').querySelector('tbody').innerHTML = historicoSaidas;
                        }

                        // Atualiza o último registro conhecido
                        ultimoRegistro = {
                            id: data.id,
                            nome: data.nome,
                            tempo: data.tempo,
                            status: data.status
                        };
                    }
                });
        }

        setInterval(atualizarDados, 1000);
        window.onload = atualizarDados;
    </script>
</body>

</html>

  )rawliteral";
  server.send(200, "text/html", pagina);
}

void setup() {

  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running. Setting time.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //======================== AREA PARA CADASTRO DE USUARIO =====================
  usuario[0] = {"Miranda Santos", " 70 BA 1B A4", false}; contadorUsuario ++;
  usuario[1] = {"Jhefferson Cardoso" , " 30 63 13 A4", false}; contadorUsuario ++;
  usuario[2] = {"Adriano Correa", " A3 7B FB 1A", false}; contadorUsuario ++;
  //=============================================================================

  //=================================== TELA ====================================
  tela.begin();                               // Inicializa o display
  tela.setPowerSave(0);                       // Liga o display (desliga o modo de economia de energia)
  tela.setFont(u8x8_font_chroma48medium8_r);  // Escolhe uma fonte
  //=============================================================================

  Serial.begin(9600);
  SPI.begin();  //Inicialização do protocolo SPI;

  //======================RFID================
  rfid.PCD_Init();  //Inicialização do módulo RFID;
  Serial.println("RFID: Operacional");
  //=======================================

  //Caso as configurações das linhas 159, 160 e 161 não sejam utilizadas, o NodeMCU vai utilizar o endereço IP 192.168.4.1 por padrão
  IPAddress local_ip(192, 168, 17, 1);  // Endereço IP local do Access Point
  IPAddress gateway(192, 168, 17, 1);   // Mesmo IP se o ESP for o "roteador"
  IPAddress subnet(255, 255, 255, 0);   // Máscara padrão

  WiFi.softAPConfig(local_ip, gateway, subnet);               // Configura o endereçamento da rede Wi-Fi do Access Point com as informações citadas anteriormente
  WiFi.softAP(ssid, password);                                // Cria a rede Wi-Fi do Access Point com os dados "ssid" (nome da rede) e "password" (senha da rede)
  Serial.println("IP do AP: " + WiFi.softAPIP().toString());  // Imprime na Serial o endereço IP configurado

  //------------------------ DECLARAÇÃO DO OBJETO JSON ------------------------
  // Objetos JSON (JavaScript Object Notation) são estruturas de dados que são organizadas em pares chave-valor
  // Pense neles como "blocos lógicos" usados para encapsular valores de diversos tipos (String, int, float e etc)

  server.on("/dados", []() {                     // Define uma rota HTTP para responder à requisição GET da função de JavaScript "atualizarDados()"
    String json = "{";                           // Definição da String que armazena o conteúdo do objeto JSON, e inicialização da mesma com "{"
    json += "\"id\":\"" + objetoJsn.enderecoJSON + "\",";      // Forma o par de chave "\"id\":\"" com o valor da variável "objetoJsn.enderecoJSON"
    json += "\"nome\":\"" + objetoJsn.nomeJSON + "\",";        // Forma o par de chave "\"nome\":\"" com o valor da variável "objetoJsn.nomeJSON"
    json += "\"tempo\":\"" + objetoJsn.tempoJSON + "\",";      // Forma o par de chave "\"tempo\":\"" com o valor da variável "objetoJsn.tempoJSON"
    json += "\"status\":\"" + objetoJsn.statusJSON + "\"";     // Forma o par de chave "\"status\":\"" com o valor da variável "objetoJsn.statusJSON"
    json += "}";                                  // Termina o objeto JSON com "}"
    server.send(200, "application/json", json);   // Essa função realiza 3 procedimentos:
                                                  // 1) O servidor envia o código HTTP 200 OK, indicando que a requisição foi processada corretamente;
                                                  // 2) Indica que o tipo de dado enviado é um objeto JSON, através de "application/json";
                                                  // 3) Envia o conteúdo do objeto JSON contido na String "json";
  });
  //-------------------- FINAL DA DECLARAÇÃO DO OBJETO JSON -------------------

  server.on("/", paginaPrincipal);  // Inicializa, na rota padrão da página (ou seja, http://192.168.10.1/), todo o conteúdo encontrado na string "pagina" que está declarada na função "paginaPrincipal"
  server.begin();                   // Inicialização do servidor "server"
  Serial.println("Servidor iniciado");
}

void loop() {
  server.handleClient();

  unsigned long tempo_atual = millis();  //salvar o tempo do millis na variavel tempo

  if (tempo_atual - tempo >= espera) {
    tempo = tempo_atual;

    if (!rfid.PICC_IsNewCardPresent()) {  //If para testar caso o módulo NÃO tenha lido nenhum cartão/tag;
      return;
    }

    if (!rfid.PICC_ReadCardSerial()) {  //If para testar caso o módulo NÃO tenha conseguido ler o endereço do cartão/tag;
      return;
    }
  }

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {  //CASO ACHE UMA LEITURA NO RFID
    String retornoFunc = hexa(rfid.uid.uidByte, rfid.uid.size);
    tela.clear();
    Serial.println(retornoFunc);
    usuarioEncontrado = false;

    for (int i = 0; i < contadorUsuario; i++) {
      if (retornoFunc == usuario[i].endereco) {
        tela.clear();

        usuarioEncontrado = true;
        usuario[i].status = !usuario[i].status;
        
        objetoJsn.tempoJSON = data_hora();    // variaveis objetoJsn recebem os dados cadastrados do usuario
        objetoJsn.enderecoJSON = usuario[i].endereco;
        objetoJsn.nomeJSON = usuario[i].nome;
       
        switch (usuario[i].status) {
          case false:
            objetoJsn.statusJSON = "Saida";
            break;
          case true:
            objetoJsn.statusJSON = "Entrada";
            break;
        }

        //================ PRINTAR NO DISPLAY OLED =============================
        tela.setCursor(0, 2);   // PRINTAR O (ID) DO USUARIO
        tela.print(F("ID:"));
        tela.print(usuario[i].endereco);
        tela.setCursor(0, 4);  // PRINTAR O (NOME) DO USUARIO
        tela.print(F("Nome:"));
        tela.print(usuario[i].nome);
        tela.setCursor(0, 6); // PRINTAR O (STATUS) DO USUARIO
        tela.print(F("status: "));
        tela.print(objetoJsn.statusJSON);
      }

      if (!usuarioEncontrado) {
        objetoJsn.enderecoJSON = "NOT FOUND";
        objetoJsn.nomeJSON = "NOT FOUND";
        objetoJsn.statusJSON = "NOT FOUND";

        tela.clear();
        tela.setCursor(0, 2);
        tela.print(F(" id:"));
        tela.print(F("NOT FOUND"));
        tela.setCursor(0, 4);
        tela.print(F("Status: "));
        tela.print(F("NOT FOUND"));
        tela.setCursor(0, 6);
        tela.print(F("Nome: "));
        tela.print(F("NOT FOUND"));
      }
    }
  }  //FIM LEITURA CARTAO
  rfid.PICC_HaltA();
}
