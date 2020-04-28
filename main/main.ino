#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Servo.h>

//const char *ssid = "ucak";
//const char *password = "password";

const char *ssid = "AndrroidAP";
const char *password = "password";
int stateLED = LOW;

int value = 0; // set values you need to zero

Servo firstESC, sol_serv, sag_serv ; 

ESP8266WebServer server(80);

void handleRoot() {
    response();
}

void handleLedOn() {
  stateLED = LOW;
  value = 700;
  delay(1000);
  value = 1300;
  delay(1000);
  value = 2000;
  response();
}

void handleLedOff() {
  stateLED = HIGH;
  value = 0;
  response();
}

void motor_middle(){
  value = 1300;
  response();
}

void servo_up(){
   atach_servos();
   sag_serv.write(90);
   sol_serv.write(110);
   detach_servos();
   response();
}
void servo_down(){
   atach_servos();
   sag_serv.write(110);
   sol_serv.write(90);
   detach_servos();
   response();
}
void servo_left(){
   atach_servos();
   sag_serv.write(100);
   sol_serv.write(110);
   detach_servos();
   response();
}
void servo_right(){
   atach_servos();
   sag_serv.write(110);
   sol_serv.write(100);
   detach_servos();
   response();
}
void servo_move(){
   atach_servos();
   sag_serv.write(100);
   sol_serv.write(100);
   detach_servos();
   response();
}


void atach_servos()
{
   sag_serv.attach(0);//d3
   sol_serv.attach(2);//d4
}
void detach_servos()
{
   delay(500);
   sag_serv.detach();//d3
   sol_serv.detach();//d4
}


const String HtmlHtml = "<html><head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /></head>";
const String HtmlHtmlClose = "</html>";

const String HtmlLedStateLow = "<big>MOTOR is now <b>ON</b></big><br/>\n";
const String HtmlLedStateHigh = "<big>MOTOR is now <b>OFF</b></big><br/>\n";
const String HtmlButtons = 
    "<a href=\"mOn\"><button style=\"display: block; width: 100%;\">ON</button></a><br/>"
    "<a href=\"mOff\"><button style=\"display: block; width: 100%;\">OFF</button></a><br/>"
    "<a href=\"up\"><button style=\"display: block; width: 100%;\">up</button></a><br/>"
    "<a href=\"down\"><button style=\"display: block; width: 100%;\">down</button></a><br/>"
    "<a href=\"left\"><button style=\"display: block; width: 100%;\">left</button></a><br/>"
    "<a href=\"right\"><button style=\"display: block; width: 100%;\">right</button></a><br/>"
    "<a href=\"move\"><button style=\"display: block; width: 100%;\">move</button></a><br/>"
    "<a href=\"middle\"><button style=\"display: block; width: 100%;\">middle</button></a><br/>";
void response(){
  String htmlRes = HtmlHtml;
  if(stateLED == LOW){
    htmlRes += HtmlLedStateLow;
  }else{
    htmlRes += HtmlLedStateHigh;
  }

  htmlRes += HtmlButtons;
  htmlRes += HtmlHtmlClose;

  server.send(200, "text/html", htmlRes);
}

void setup() {
    delay(1000);
    Serial.begin(9600);
    Serial.println();

    firstESC.attach(16);// d0
    //sag_serv.attach(0);//d3
    //sol_serv.attach(2);//d4
    
    //WiFi.softAP(ssid, password);
    WiFi.begin(ssid, password); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
          
    IPAddress apip = WiFi.softAPIP();
    Serial.print("visit: \n");
    //Serial.println(apip);
    Serial.println(WiFi.localIP());
    server.on("/", handleRoot);
    server.on("/mOn", handleLedOn);
    server.on("/mOff", handleLedOff);
    server.on("/up", servo_up);
    server.on("/down", servo_down);
    server.on("/left", servo_left);
    server.on("/right", servo_right);
    server.on("/move", servo_move);
    server.on("/middle", motor_middle);
    
    server.begin();
    Serial.println("HTTP server beginned");
    pinMode(LED_BUILTIN, OUTPUT);
    //sag_serv.write(100);
    //sol_serv.write(100);
}

void loop() {
    server.handleClient();
    firstESC.writeMicroseconds(value);
}
