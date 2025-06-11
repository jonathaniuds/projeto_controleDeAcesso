
🔐 Controle de Acesso RFID com NodeMCU

<p align="center">
  Sistema inteligente de controle de entrada e saída com RFID, NodeMCU e interface web em tempo real.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Status-Concluído-green" />
  <img src="https://img.shields.io/badge/Hardware-NodeMCU-blue" />
  <img src="https://img.shields.io/badge/Leitor-RFID_RC522-orange" />
  <img src="https://img.shields.io/badge/Web-HTML/CSS/JS-yellow" />
</p>

---

📌 Descrição

Este projeto foi desenvolvido com o objetivo de criar um **sistema de controle de acesso** utilizando **RFID (Identificação por Radiofrequência)**, exibindo os dados do usuário em um **display OLED** e também em uma **interface web**, com comunicação via **Wi-Fi** usando o **NodeMCU ESP8266**.

---

⚙️ Como Funciona

1. O usuário aproxima uma **tag/cartão RFID** do leitor.
2. O sistema verifica a **autenticidade** da tag.
3. Se válida:
   - 🖥️ Exibe no **Display OLED**:
     - 🆔 ID do cartão
     - 👤 Nome do usuário
     - 🔄 Status: Entrada ou Saída
   - 🌐 Envia os dados via **JSON** para uma página web.
4. A interface web mostra o **último acesso** e **histórico completo**.

---

🧰 Componentes Utilizados

🔧 Hardware
- 📡 **NodeMCU ESP8266**
- 🔒 **Módulo RFID RC522**
- 🖥️ **Display OLED I2C**

💻 Software
- Arduino IDE
- HTML, CSS e JavaScript (Interface Web)
- JSON (armazenamento temporário)
- Wi-Fi (comunicação HTTP)

---

## 🌐 Interface Web

A interface web foi construída com foco em **simplicidade e responsividade**, utilizando:

- 🧱 **HTML/CSS** para estrutura e estilo
- ⚡ **JavaScript** para interação e exibição dos dados
- 🔁 Comunicação com o NodeMCU via **HTTP GET/POST**
- 🧩 Dados transmitidos em **formato JSON**

---

## 💡 Melhorias Futuras

- 🔐 Login de administrador para gerenciamento de acessos
- 📁 Armazenamento dos dados em **banco de dados (MySQL/MongoDB)**
- 📲 Integração com notificações via e-mail ou app
- 🧑‍💼 Interface web com painel de cadastro de usuários
- 💾 Salvamento local em cartão SD
- 📈 Gráficos e relatórios de acessos

---

## 🚀 Como Usar

1. Clone este repositório:
   ```bash
   git clone https://github.com/seu-usuario/nome-do-projeto.git
   ```

2. Abra o arquivo `trabalhofinalizado.ino` no **Arduino IDE**.

3. Instale as bibliotecas:
   - `SPI.h`
   - `MFRC522.h`
   - `Wire.h`
   - `Adafruit_GFX.h`
   - `Adafruit_SSD1306.h`
   - `ESP8266WiFi.h`
   - `ESP8266WebServer.h`

4. Configure o **SSID e senha do Wi-Fi** no código.

5. Faça o upload para o **NodeMCU** e acesse o IP local fornecido.

---

## 🤝 Contribuições

Contribuições são bem-vindas!  
Sinta-se livre para abrir uma **issue**, sugerir uma **nova funcionalidade** ou enviar um **pull request**.

---

## 📜 Licença

Este projeto está sob a licença **MIT**.  
Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

---

## 👨‍💻 Autor

**Jonathan Rafael Iuds**  
📧 jonathaniuds@hotmail.com  
🌐 [LinkedIn](https://www.linkedin.com/in/jonathan-nogarolli)

---

> 💬 *“Tecnologia é melhor quando conecta pessoas.”*
