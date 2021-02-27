// Telegram bot library by Brian: https://github.com/sponsors/witnessmenow/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
 #include <UniversalTelegramBot.h> 

char ssid[] = "SUA REDE";     // your network SSID (name)
char password[] = "senha"; // your network key

// Initialize Telegram BOT
#define BOTtoken "tOkEn:ToKeN" // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 500;
unsigned long lastTimeBotRan;

int D1Status = 0;
int D2Status = 0;
int D3Status = 0;
int D4Status = 0;
int D5Status = 0;
int D6Status = 0;
int D7Status = 0;
int D8Status = 0;
int A0val = 0;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if(text == "/e"){
      bot.sendMessage(chat_id, "Esquerda", "");
      GPIO_REG_WRITE(GPIO_OUT_ADDRESS, 0x6000); //aciona motor direito
      delay(1000);
      ESP.wdtFeed();
      GPIO_REG_WRITE(GPIO_OUT_ADDRESS, 0x0000);
      
      //D4Status = 1;
      
    }
    if(text == "/d"){
      bot.sendMessage(chat_id, "Direita", "");
      GPIO_REG_WRITE(GPIO_OUT_ADDRESS, 0x9000);
      delay(1000);
      ESP.wdtFeed();
      GPIO_REG_WRITE(GPIO_OUT_ADDRESS, 0x0000);
      //D4Status = 0;
      
    }
    if(text == "/f"){
      bot.sendMessage(chat_id, "Frente", "");
      //GPIO_REG_WRITE(GPIO_OUT_ADDRESS, 0x5000);
      /*
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(14, HIGH);
      digitalWrite(15, LOW);
      */
      delay(1000);
      ESP.wdtFeed();
      GPIO_REG_WRITE(GPIO_OUT_ADDRESS, 0x0000);
      //D4Status = 0;
    }
    if(text == "/p"){
      bot.sendMessage(chat_id, "Parei", "");
      GPIO_REG_WRITE(GPIO_OUT_ADDRESS, 0x0000);
      D4Status = 0;
    }

    if (text == "/D4ON") {
      digitalWrite(2,HIGH);
      D4Status = 1;
      bot.sendMessage(chat_id, "D4 is HIGH", "");
    }

    if (text == "/D4OFF") {
      D4Status = 0;
      digitalWrite(2, LOW);
      bot.sendMessage(chat_id, "D4 is LOW", "");
    }
    
    if (text == "/status") {
      if(D8Status){
        bot.sendMessage(chat_id, "D8 is HIGH", "");
      } else {
        bot.sendMessage(chat_id, "D8 is LOW", "");
      }
    }

    if (text == "/start") {
      String welcome = "ESP8266 controller, " + from_name + ".\n";
      welcome += "/f : frente";
      welcome += "/p : parar";
      welcome += "/e : esquerda";
      welcome += "/d : direita";
      welcome += "/D4ON : to switch the D4 pin HIGH\n";
      welcome += "/D4OFF : to switch the D4 pin LOW\n";
      
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


void setup() {
  //Serial.begin(115200);

  client.setInsecure();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  //Serial.print("Connecting Wifi: ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(500);
  }

  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());

  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);
  delay(10);
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      //Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}
