 #include <Arduino.h>
#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#define BOTtoken "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
const char* ssid     = "";
const char* password = "";
int eeprom_first = 12;
int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
int first_run;
bool new_chat_id_reg = false;


int wifiStatus;
String authorized_chat_id = "";

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void setup() {

    Serial.begin(115200);
    /*
    EEPROM.begin(512);
    for (int i = 0; i < 100; i++) {
      EEPROM.write(i,'0');
    }
    EEPROM.commit();
    *//*
    Serial.println(EEPROM.read(eeprom_first));
    if (EEPROM.read(eeprom_first) == 0) {
        new_chat_id_reg = true;
        Serial.println("new_reg");
    }

    else {
      String read_data = "000000000";
      for (int i = 0; i < 9; i++) {
        read_data[i]=EEPROM.read(i);

      }
      Serial.println("load_auth");
      authorized_chat_id = read_data;
    }*/
////////////////////////////////////////////////////////////////////////////
    //Wifi connecting
    delay(200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Wifi connected");
    //Wifi connecting
    bot.sendMessage(authorized_chat_id,"Device connected to internet!", "Markdown");
    delay(3000);
    bot.sendMessage(authorized_chat_id,"Rufus Security System is running....", "Markdown");
}
///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for(int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
  /*
    //Registration
    if(new_chat_id_reg == true){
      Serial.println("ChatId registration is running!");
      EEPROM.begin(512);
      for (int i = 0; i < chat_id.length(); i++) {
        EEPROM.write(i,chat_id[i]);
      }
      EEPROM.commit();
      EEPROM.begin(512);
       EEPROM.write(eeprom_first,1);
      EEPROM.commit();
      new_chat_id_reg= false;
      authorized_chat_id=chat_id;
      Serial.print("Registration successful!");
    }
    //Registration
*/

    if (chat_id == authorized_chat_id){
      String text = bot.messages[i].text;
      //-------------------------------------------
      if (text == "/help") {
        String welcome = "Welcome to Rufus Security System!\n";
        welcome = welcome + "/menu \n";
        welcome = welcome + "/status : \n";
        welcome = welcome + "/options : \n";
        bot.sendMessage(chat_id, welcome, "Markdown");
      }
      //--------------------------------------------
    //-----------------------------------------------
      if (text == "/asd") {
        String asd = "Welcome";
        bot.sendMessage(chat_id,asd , "Markdown");
      }

    //------------------------------------------------
    if (text == "/menu") {
      String menu ="Menu";
      bot.sendMessage(chat_id,menu, "Markdown");
    }

  //-------------------------------------------------
  if (text == "/status") {
    String status ="Status\n";
    status=status +"Authorized ChatId is \n" + authorized_chat_id;
    bot.sendMessage(chat_id,status, "Markdown");
  }

//----------------------------------------------------
if (text == "/options") {
  String options="Options";
  bot.sendMessage(chat_id,options, "Markdown");
  }
//-----------------------------------------------------
}






    else {
      bot.sendMessage(chat_id,"Unauthorized access!!!!", "Markdown");
      bot.sendMessage(authorized_chat_id,"Unauthorized access attempt!!! ChatId: "+chat_id,"Markdown");
    }

  }
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void loop() {
    if (millis() > Bot_lasttime + Bot_mtbs)  {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while(numNewMessages) {
          Serial.println("got response");

          handleNewMessages(numNewMessages);
          numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        Bot_lasttime = millis();
      }
}
