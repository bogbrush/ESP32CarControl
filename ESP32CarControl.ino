
#include <elapsedMillis.h>
#include <WiFiClientSecure.h>
#include "base64.h"
const char* ssid2     = "AAAA";
const char* password2 = "BBBB";

const char* ssid     = "CCCC";
const char* password = "DDDD";


const char* host = "?????.net";  //you will need a domain registered

const String morse[] = { ".----", "..---", "...--", "....-", ".....", 
  "-....", "--...", "---..", "----.", "-----", ".-", "-...", "-.-.", "-..", 
  ".","..-.","--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", 
  ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", 
  "--.."};
const int dotLen = 100; //delay in milliseconds
const int dashLen = 3*dotLen; // 3X longer than the dot
const int wordLen = 7*dotLen; // space between words.

struct PinCommand {
  int fireTime;
  int pinNumber;
  int state;
};



//you will need an SSL certificate for your domain
const char* test_root_ca= \
     "-----BEGIN CERTIFICATE-----\n" \
"MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB\n" \
"iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n" \
"cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n" \
"BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw\n" \
"MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV\n" \
"BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU\n" \
"aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy\n" \
"dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK\n" \
"AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B\n" \
"3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY\n" \
"tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/\n" \
"Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2\n" \
"VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT\n" \
"79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6\n" \
"c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT\n" \
"Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l\n" \
"c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee\n" \
"UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE\n" \
"Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd\n" \
"BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G\n" \
"A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF\n" \
"Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO\n" \
"VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3\n" \
"ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs\n" \
"8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR\n" \
"iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze\n" \
"Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ\n" \
"XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/\n" \
"qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB\n" \
"VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB\n" \
"L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG\n" \
"jjxDah2nGN59PRbxYvnKkKj9\n" \
"-----END CERTIFICATE-----\n";

     WiFiClientSecure client;
void setup()
{
    Serial.begin(115200);
    delay(10);

    pinMode(14, OUTPUT);
    digitalWrite(14, HIGH);
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);
    pinMode(26, OUTPUT);
    digitalWrite(26, HIGH);
    pinMode(25, OUTPUT);
    digitalWrite(25, HIGH);

    //gpio34,35,36,39  are input only on ESP32 

    pinMode(33, OUTPUT);
    digitalWrite(33, HIGH);
    pinMode(32, OUTPUT);
    digitalWrite(32, HIGH);
    pinMode(22, OUTPUT);
    digitalWrite(22, HIGH);
    pinMode(23, OUTPUT);
    digitalWrite(23, HIGH);

    pinMode(16, OUTPUT);
    digitalWrite(16, HIGH);
    pinMode(17, OUTPUT);
    digitalWrite(17, HIGH);
    pinMode(18, OUTPUT);
    digitalWrite(18, HIGH);
    pinMode(19, OUTPUT);
    digitalWrite(19, HIGH);


    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid2);

    WiFi.begin(ssid2, password2);
    
    int i=0;
    while (i<10 && WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        i++;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Could not connect");
      Serial.print("Connecting to ");
      Serial.println(ssid);
      WiFi.begin(ssid, password);
  
      while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
      }
    }
  
    Serial.println("");
    Serial.print("WiFi connected ");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

      client.setCACert(test_root_ca);
}

int restartVal = 0;

void loop()
{   
    client.stop();
    
    delay(1);
    ++restartVal;
    
    if (restartVal>15)
    {
      ESP.restart();
    }

    Serial.print("connecting to ");
    Serial.println(host);




   
    const int httpPort = 443;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/api/Commands/PollNextCommand";
    

    Serial.print("Requesting URL: ");
    Serial.println(url);

    //basic authentication - add you username and password here
    String auth = base64::encode("username:password");
    
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Authorization: Basic " + auth + "\r\n" +
                 "Connection: close\r\n\r\n");


                 
    unsigned long timeout = millis();

    Serial.println(client.available());
    Serial.println("starting timeout loop ONE");  
    while (client.available() == 0) {
        
        if (millis() - timeout > 110000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
        delay(10);
         //Serial.println("client not available in timeout loop ONE");

    }
    
  if (!client.available())
  {
    Serial.println("client not available");
  }
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String payload = client.readStringUntil('\r\n');


       Serial.print(payload);
    
      if (payload.startsWith("\"") && payload != "\"\"")
      {
        //if we have a non empty command, do not restart anytime soon
        restartVal = 0;
        payload.replace("\"", "");
  
        String command = getValue( payload, '|', 0);
        Serial.println("Command: " + command);
  
        if (command == "Custom")
        {
          PinCommand pinCommands [100];
          unsigned int p = 1;
          String command = getValue( payload, '|', p);
          while (command != "" and p <= 100)
          {
           
            unsigned int fireTime = getValue( command, ';', 0).toInt();
            unsigned int pinNumber = getValue( command, ';', 1).toInt();
             unsigned int state = getValue( command, ';', 2).toInt();
            PinCommand pinCommand = { fireTime , pinNumber , state };
            pinCommands [p-1] = pinCommand;
   
            p++;
            command = getValue( payload, '|', p);
          }
  
          int numCommands = p -1;
  
          //process commands
          elapsedMillis timeElapsed = 0;
          p=1;
          while (p <= numCommands)
          {
            if (timeElapsed > pinCommands [p-1].fireTime)
            {
              if (pinCommands [p-1].state == 1)
              {
                TurnOn(pinCommands [p-1].pinNumber);
              }
              else
              {
                TurnOff(pinCommands [p-1].pinNumber);
              }
  
              p++;
              timeElapsed = 0;
            }
            
            delay(1);
          }
   
        }
        else if (command == "Read")
        {
          unsigned int p = 1;
          String gpio = getValue(payload, '|', p);
          while (gpio != "" and p <= 100)
          {
            int sensorValue = 0;
            sensorValue = analogRead(gpio.toInt());
            Serial.println("GPIO " + gpio + ": " + String(sensorValue));
            
            if (!client.connect(host, httpPort)) 
            {
                    Serial.println("connection failed");
                    return;
            }
            url= "/api/TodoItems/Log?Command=GPIO" + gpio + "=" + String(sensorValue);
            client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: close\r\n\r\n");
            

            
            p++;
            gpio = getValue(payload, '|', p);
          }
           
        }
        else
        {

          String pinstring = getValue( payload, '|', 1);
          Serial.println("Pin: " + pinstring);
          
          String param1 = getValue( payload, '|', 2);
          Serial.println("Param1: " + param1);
          
          String param2 = getValue( payload, '|', 3);
          Serial.println("Param2: " + param2);
    
          int pin = pinstring.toInt();
     
          
          
          
          if (command == "On")
          {
             if (param1 == "")
             {
                TurnOn(pin);
             }
             else
             {
                FlashOnce(pin,param1.toInt());
             }
          }
    
          if (command == "OnOff")
          {
             unsigned int repeat = 1;
              
             if (param2 != "")
             {
                repeat = param2.toInt();
             }
    
             for (unsigned int i=1; i<= repeat; i++)
             {
               if (param1 != "")
               {
                  FlashOnce(pin,param1.toInt());
               }
               else
               {
                  FlashOnce(pin,1000);
               }
               delay(500);
             }
          }
    
          if (command == "Off")
          {
             TurnOff(pin);
          }
    
          if (command ==  "SOS")
               SOS(pin);
               
          if (command ==  "Morse")
          {
              Morse(pin, param1);
          }
        }
      }
    }

}



void SOS(int pin)
{
    FlashOnce(pin,200);delay(200);  FlashOnce(pin,200);delay(200); FlashOnce(pin,200);delay(200); // S
    delay(300); 
    FlashOnce(pin,500);delay(500);FlashOnce(pin,500);delay(500);FlashOnce(pin,500);delay(500);// O

    FlashOnce(pin,200);delay(200); FlashOnce(pin,200);delay(200); FlashOnce(pin,200);delay(500); // S

}

void TurnOn(int pin) 
{
  Serial.println("TurnOn ");
  digitalWrite(pin, LOW);
}

void TurnOff(int pin) 
{
  Serial.println("TurnOff ");
  digitalWrite(pin, HIGH);
}

void FlashOnce(int pin, int onTime) 
{
  Serial.println("FlashOnce");
  digitalWrite(pin, LOW);
  delay(onTime);
  digitalWrite(pin, HIGH);
}


void Morse(int pin, String message)  
{
  unsigned int i = 0; //counter variable to move through the array
  char C; //holds each letter as it gets converted to blinks
  while (message[i]) { // fails when reaches the end of the string ("0")
    C = toupper(message[i]);
        
    if (C == ' ') {
      delay(wordLen); // space between words  
    } 
    else if (C>47 && C<58) {  // 0-9. ASCII 48-57
      signalMorse(pin, morse[C-48]);
    } 
    else if (C>64 && C<91) {  // A-Z, ASCII 65-90, 10-34 in morse[]
      signalMorse(pin, morse[C-55]);
    }
    else {
      delay(wordLen);
    }
    i++; // move to the next character in your message
  }
  delay(2 * wordLen);
}

void signalMorse(int pin, String c) 
{
  unsigned int i = 0; 
  while (c[i]) { // for each dot or dash in the letter...
    
    digitalWrite(pin, LOW);
    if (c[i] == '.') 
    { 
       delay(dotLen); // if dot, short delay
       Serial.println("dot");
    } else if (c[i] == '-') {
       delay(dashLen); // if dash, longer delay
       Serial.println("dash");
    }
  
    digitalWrite(pin,HIGH); 
    delay(dotLen); 
    i++;    //bring in the next dot or dash from the morse letter!]'
  };
  // We already delayed one dot length in the loop. A Word break = dashLen..
  delay(dashLen-dotLen); 
}

String getValue(String data, char separator, int index)
{
    unsigned int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length();

    for (unsigned int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
