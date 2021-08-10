//Libraries for WiFi Functions and WebServer Functions
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

//Most of the variables are declared global as it makes their usage simple while using larger functions repeatedly

int Device0=D2;                                                 //Assigning Device pins for all 4 device(+1 to know the current status this is Device0 and lets say the device as root device)
int Device1=D5;
int Device2=D6;
int Device3=D7;
int Device4=D8;
char *Stat1="",*Stat2="",*Stat3="",*Stat4="";                   //Declaring strings to store the status of our device as active or inactive
bool D0status=0,D1status=0,D2status=0,D3status=0,D4status=0;    //Declaring the boolean variables that store data from digital pins if it is high or low
bool c1=0,c2=0,c3=0,c4=0;                                      //Declaring boolean variables that help us know if a button is already pressed (represents on and off)

const char *ssid="xxxxxx your wifi name";                     //Delaring constant strings to store wifi name and password to connect
const char *pswd="xxxxxx your wifi password";

ESP8266WebServer server(8080);                                //Helps us create a webserver on nodemcu(esp8266 module) with an IPaddress followed by 8080(generally it is 80 but we can change)    

void setup() {
  // put your setup code here, to run once:
  WiFi.disconnect();                          //disconnect from the current wifi and start the process again when powered up
  delay(2000);
  Serial.begin(115200);                       //begin serial communication
  delay(100);
  pinMode(Device0,OUTPUT);                    //Assigning the mode of the devices we are using as outputs 
  pinMode(Device1,OUTPUT);
  pinMode(Device2,OUTPUT);
  pinMode(Device3,OUTPUT);
  pinMode(Device4,OUTPUT);
  digitalWrite(Device1,LOW);                  //As the Device 1,2 are LEDs first they are LOW(off)
  digitalWrite(Device2,LOW);
  digitalWrite(Device3,HIGH);                //However Device 3,4 are connected to Relay module connected and we used NO(normally open)terminal, so a HIGH signal mean it is open circuited at present
  digitalWrite(Device4,HIGH);
  delay(200);
  //Serial.println("Starting");             //To be used while configuring
  WiFi.begin(ssid,pswd);                    //Connect to the WiFi with 'that' ssid and pswd
  while((WiFi.status() !=WL_CONNECTED)){    //While the wifi is not connected keep printing '...' until you connect
    delay(200);
    //Serial.print(".");
  }
  //Serial.println("Connected");            //After the wifi is connected print connected (to be used while configuring)
  //Serial.println((WiFi.localIP().toString()));//Prints the IPaddress to access the website
  for(int i=0;i<10;i++){                    //Blink the root device telling that it connected to wifi and ipaddress is obtained
    digitalWrite(Device0,HIGH);
    delay(500);
    digitalWrite(Device0,LOW);
    delay(500);
  }
  server.on("/",handleRoot);              //create a server and "'ipaddress/" is the home site and if the server is in this page then it should execute the function beside it i.e. handleRoot
  server.on("/dev1=0",handledev1);        //create a server and "'ipaddress/dev1=0" is the home site and if the server is in this page then it should execute the function beside it i.e. handledev1
  server.on("/dev2=0",handledev2);        //create a server and "'ipaddress/dev2=0" is the home site and if the server is in this page then it should execute the function beside it i.e. handledev2
  server.on("/dev3=0",handledev3);        //create a server and "'ipaddress/dev3=0" is the home site and if the server is in this page then it should execute the function beside it i.e. handledev3
  server.on("/dev4=0",handledev4);        //create a server and "'ipaddress/dev4=0" is the home site and if the server is in this page then it should execute the function beside it i.e. handledev4
  server.onNotFound(handleNotFound);      //if the server is not found then execute the function handleNotFound
  server.begin();                         //begin the server
  //Serial.println("HTTP server started");//to be used while configuring
  for(int i=0;i<255;){                    //Glow the LED with increasing brightness telling that the server has been started and you can access it now
    analogWrite(Device0,i);
    i+=64;
    delay(200);
  }
  digitalWrite(Device0,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  server.handleClient();                  //implememt this fuunction continuosly it handles the server client
  delay(1000);
}

//Function that has the contents of the page i.e. the html and css(style) content that will be forwarded to server
//As the stat1,stat2.. strings are global they can be used without passing as arguements
String getPage(){
  String page="<html><head><meta name='viewport' content=\"width=device-width, initial-scale=1\" http-equiv='refresh' content='300'/>";
  page+="<title>Smart Hub</title></head>";
  page+="<style>body {font-family: Helvetica Neue; font-size:24; Color: #ffffff;text-align: center;}";
  page+=".imgcontent { background-image: url(\"http://gdurl.com/7AvX\"); background-size: cover; height: 100%; overflow: hidden;z-index=1;}";
  page+="button {font-family: Helvetica Neue; font-size:20; Color: #000000;margin: 10px; background-color:#ffffff; border-radius: 5px;opacity: 1;}";
  page+=".div1  {background-color: #000000; position:fixed; padding-top: 5px; margin: 10px; margin-top: 20px; margin-left: 20%; margin-right: 20%; height: 80%; width: 60%; opacity: 0.45;z-index=2;}";
  page+=".div2  {position:fixed; padding-top: 5px; margin: 10px; margin-top: 50px; height: 500px; width: 95%; text-align: center;z-index=10;opacity: 1}</style>";
  page+="<body class=\"imgcontent\"><center><div class=\"div1\"></div><div class=\"div2\">";
  page+="<h1><center>Smart Hub</center><span><center><button onclick=\"window.location.href='/';\">Refresh &orarr;</button></center></span></h1>";
  page+="<h3><center>&ensp;Device Id<span>&ensp;status</span></center></h3>";
  page+="<center><button onclick=\"window.location.href='/dev1=0';\">Device No 1</button><span>";
  page+=Stat1;
  page+="</span><br><button onclick=\"window.location.href='/dev2=0';\">Device No 2</button><span>";
  page+=Stat2;
  page+="</span><br><button onclick=\"window.location.href='/dev3=0';\">Device No 3</button><span>";
  page+=Stat3;
  page+="</span><br><button onclick=\"window.location.href='/dev4=0';\">Device No 4</button><span>";
  page+=Stat4;
  page+="</span></center></center></div></body>";
  page+="</html>";
  return page;
}
//To handle the root(home) page we use this function(this page is reached initially and also when refresh button is pressed)
void handleRoot(){
  digitalWrite(Device0,HIGH);                 //First we make the root device as high(ON) telling that we are in the root page
  statusread();                               //Read the status of all the pins using the function statusread whether they are on/off
  if(D0status==1){                            //If the root device is on that means we are in home page so execute the decision function that tells if the devices are active/inactive by using data from statusread
    decision();
    delay(100);
  }
  server.send(200,"text/html",getPage());     //Now send this to the server, 200 means the http status for success/ok, the incoming text is html, the text is the data returned from the function getpage  
}
//To handle the device 1 page(this page is reached when the button device no 1 is pressed)
//The reason the if,else is used here is to handle the case when the same button is pressed more times. When pressed for first time it should ON,
//when pressed again it should OFF, when pressed again it should ON and so on, it should understand the difference

void handledev1(){
  digitalWrite(Device0,LOW);                  //As we pressed a button and changed page we are not in root page so make the root device LOW(off)
  if(c1>0){                                   //if c1 which is usually 0 (button represents OFF position) is 1 then OFF the device i.e the device is already ON so OFF it
    digitalWrite(Device1,LOW);
    delay(100);
    statusread();                             //Now read the status and give the decision based on it
    decision();
    c1=0;                                     //Make c1 as 0 so that the code knows that the device is already OFF 
  }
  else{                                       //if c1 is 0 i.e the button represents OFF position and it was pressed, so make the device HIGH(ON)
    digitalWrite(Device1,HIGH);
    delay(100);
    statusread();                           //Now read the status and give the decision based on it
    decision();
    c1=1;                                   //Make c1 as 1 so that the code knows that the device is already ON 
  }
  server.send(200,"text/html",getPage());
}
//To handle the device 2 page(this page is reached when the button device no 2 is pressed)
void handledev2(){
  digitalWrite(Device0,LOW);
  if(c2>0){
    digitalWrite(Device2,LOW);
    delay(100);
    statusread();
    decision();
    c2=0;
  }
  else{
    digitalWrite(Device2,HIGH);
    delay(100);
    statusread();
    decision();
    c2=1;
  }
  server.send(200,"text/html",getPage());
}
//similar to device 1,2
void handledev3(){
  digitalWrite(Device0,LOW);
  if(c3>0){
    digitalWrite(Device3,HIGH);
    delay(100);
    statusread();
    decision();
    c3=0;
  }
  else{
    digitalWrite(Device3,LOW);
    delay(100);
    statusread();
    decision();
    c3=1;
  }
  server.send(200,"text/html",getPage());
}
//similar to device 1,2,3
void handledev4(){
  digitalWrite(Device0,LOW);
  if(c4>0){
    digitalWrite(Device4,HIGH);
    delay(100);
    statusread();
    decision();
    c4=0;
  }
  else{
    digitalWrite(Device4,LOW);
    delay(100);
    statusread();
    decision();
    c4=1;
  }
  server.send(200,"text/html",getPage());
}
//Function to handle if the server is not found or some error
void handleNotFound(){
  String message = "File Not Found\n\n";                                //Get the data from http status and print the cause of error on the server 
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for ( uint8_t i = 0; i < server.args(); i++ ) {
  message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.send ( 404, "text/plain", message );
}
//Function to read the status of the devices
void statusread(){
  D0status=digitalRead(Device0);
  D1status=digitalRead(Device1);
  D2status=digitalRead(Device2);
  D3status=digitalRead(Device3);
  D4status=digitalRead(Device4);
}
//Function to make a decision based on data from statusread, as all the variables are global they can be accessed here and can be modified easily and the changes are effected everywhere
void decision(){
    if(D1status==1){                //As device 1,2 are LEDs if theu are HIGH(0N) that means they are Active so set their status as active, if not then set as inactive accordingly
      Stat1="Active";
    }
    else if(D1status==0){
      Stat1="Inactive";
    }
    if(D2status==1){
      Stat2="Active";
    }
    else if(D2status==0){
      Stat2="Inactive";
    }
    if(D3status==0){              //As device 3,4 are connected to Realy Modules connected with NO terminal, a LOW means they are on, so set their status as Active, if not then set as inactive accordingly
      Stat3="Active";
    }
    else if(D3status==1){
      Stat3="Inactive";
    }
    if(D4status==0){
      Stat4="Active";
    }
    else if(D4status==1){
      Stat4="Inactive";
    }
}
