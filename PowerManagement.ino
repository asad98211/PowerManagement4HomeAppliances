

/*
 DNS and DHCP-based Web client

Circuit:
* Ethernet shield attached to pins 10, 11, 12, 13
*/

#include <SPI.h>
#include <Ethernet.h>
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int led1 = 22, led2 = 24, led3 = 26, led4 = 28, led5 = 30, switchIn = 32, switchOut=34;
// Enter a MAC address for your controller below.
byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
char serverName[] = "http://iotinstorage.000webhostapp.com";

int timestep=1,flagindex=0,index=0,power=0,var=0,turn=0;
String location = "getcontrol.php?sensor=light HTTP/1.0";
char buff[1000];
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
// the "L" is needed to use long type numbers
// Initialize the Ethernet client library
EthernetClient client;

void setup() {
 // start the serial library:
 Serial.begin(9600);
 pinMode(led1,OUTPUT);
 pinMode(led2,OUTPUT);
 pinMode(led3,OUTPUT);
 pinMode(led4,OUTPUT);
 pinMode(led5,OUTPUT);
 pinMode(switchIn,INPUT);
 pinMode(switchOut,OUTPUT);
 // start the Ethernet connection:
 if (Ethernet.begin(mac) == 0) {
   Serial.println("Failed to configure Ethernet using DHCP");
   // no point in carrying on, so do nothing forevermore:
   while(true);
 }
 // give the Ethernet shield a second to initialize:
 delay(1000);
 
}
void powerAC1(){
    switch(round(map(power,0,1550,0,5))){
        case 0:
            digitalWrite(led1,LOW);
            digitalWrite(led2,LOW);
            digitalWrite(led3,LOW);
            digitalWrite(led4,LOW);
            digitalWrite(led5,LOW);
            break;
        case 1:
            digitalWrite(led1,HIGH);
            digitalWrite(led2,LOW);
            digitalWrite(led3,LOW);
            digitalWrite(led4,LOW);
            digitalWrite(led5,LOW);
            break;
        case 2:
            digitalWrite(led1,HIGH);
            digitalWrite(led2,HIGH);
            digitalWrite(led3,LOW);
            digitalWrite(led4,LOW);
            digitalWrite(led5,LOW);
            break;
        case 3:
            digitalWrite(led1,HIGH);
            digitalWrite(led2,HIGH);
            digitalWrite(led3,HIGH);
            digitalWrite(led4,LOW);
            digitalWrite(led5,LOW);
            break;
        case 4:
            digitalWrite(led1,HIGH);
            digitalWrite(led2,HIGH);
            digitalWrite(led3,HIGH);
            digitalWrite(led4,HIGH);
            digitalWrite(led5,LOW);
            break;
        case 5:
            digitalWrite(led1,HIGH);
            digitalWrite(led2,HIGH);
            digitalWrite(led3,HIGH);
            digitalWrite(led4,HIGH);
            digitalWrite(led5,HIGH);
            break;
    }
}
void calcpower(){
  int i;
  power=0;
  for(i=flagindex+1;i<index;i++){
    if(buff[i]=='.'){
      break;
    }
    power=power*10 + buff[i]-'0';
  }
  index=0;
}
void loop()
{
 // digitalWrite(switchOut,digitalRead(switchIn));
//  if(digitalRead(switchIn)){
     // if there are incoming bytes available
     // from the server, read them and print them:
//     if(turn==1){
        while (client.available()){
           buff[index]= client.read();
           Serial.print(buff[index]);
           if(buff[index]==':'){
              flagindex=index;
           }
           if(buff[index]=='}'){
              calcpower();
              Serial.println();
              Serial.print("Power:");
              Serial.println(power);
              powerAC1();
              break;
          }
         index++;
        }
//     turn =1;
    // if ten seconds have passed since your last connection,
    // then connect again and send data:
    if (millis() - lastConnectionTime > postingInterval) {
        httpRequest();
    }
 //  }
 // else{
 /*    turn = 0;
     timestep=1;
     var=0;
     power=0;
     powerAC1(); 
     client.stop();
 */
}

void httpRequest(){
    // close any connection before send a new request.
    // This will free the socket on the WiFi shield
    client.stop();

    // if there's a successful connection:
    Serial.println("new connection...");

   // if you get a connection, report back via serial:
 
    if(timestep==1){
        if (client.connect(serverName, 80)) {
             Serial.println("connected");
             // Make a HTTP request:
             client.print("GET http://iotinstorage.000webhostapp.com/getcontrol.php?sensor=light");
             client.print("HTTP/1.0");
             client.println();
             // note the time that the connection was made:
             lastConnectionTime = millis();
       }
      else {
            // if you didn't get a connection to the server:
            Serial.println("connection failed");
        }
        timestep++;
     }  
     else{
        if (client.connect(serverName, 80)) {
              Serial.println("connected");
               // Make a HTTP request:
               client.print("GET http://iotinstorage.000webhostapp.com/getcontrol.php?sensor=light");
               //client.print(var);
               client.print("HTTP/1.0");
               client.println();
               // note the time that the connection was made:
               lastConnectionTime = millis();
          }
      else {
              // if you didn't get a connection to the server:
              Serial.println("connection failed");
      }
     }
     var = var+3;
}

