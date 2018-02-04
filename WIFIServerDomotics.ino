/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>

const char* ssid = "EL_AZUL";
const char* password = "Gadirfjtp1169!";

// Background colors of buttons
String background2, background3, background4, background5;
String HeaderHTML, StyleHTML, JavascriptHTML, TableHTML, FooterHTML;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(8000);


/*****************/
/* INITIAL SETUP */
/*****************/
void setup() {
  Serial.begin(115200);
  delay(10);

  // Prepare output GPIOs
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  pinMode(3, OUTPUT);
  digitalWrite(3, 0);
  pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  pinMode(5, OUTPUT);
  digitalWrite(5, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the web server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}


/*************/
/* MAIN LOOP */
/*************/
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  //Serial.println(req);
  client.flush();

  manageGPIOS(req);

  client.flush();
  
  HTMLResponse();

  // Send the response to the client
  // Have to try to send more than one client.print...
  client.print(HeaderHTML);
  client.print(StyleHTML);
  client.print(JavascriptHTML);
  client.print(TableHTML);
  client.print(FooterHTML);
  delay(1);
  Serial.println("Client disconnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

/******************************/
/* Manage response of request */
/******************************/
void manageGPIOS(String req)
{
  // Match the request and setting the GPIOs ON/OFF
  if (req.indexOf("/gpio2/0") != -1)
    digitalWrite(2, 0);
  else if (req.indexOf("/gpio2/1") != -1)
    digitalWrite(2, 1);
  else if (req.indexOf("/gpio3/0") != -1)
    digitalWrite(3, 0);
  else if (req.indexOf("/gpio3/1") != -1)
    digitalWrite(3, 1);
  else if (req.indexOf("/gpio4/0") != -1)
    digitalWrite(4, 0);
  else if (req.indexOf("/gpio4/1") != -1)
    digitalWrite(4, 1);
  else if (req.indexOf("/gpio5/0") != -1)
    digitalWrite(5, 0);
  else if (req.indexOf("/gpio5/1") != -1)
    digitalWrite(5, 1);
}


/*****************************/
/* Generate de HTML response */
/*****************************/
void HTMLResponse()
{
  // Set the 'lights' background colors
  if (digitalRead(2)) {
    background2 = "red";
  }
  else {
    background2 = "green";
  }

  if (digitalRead(3)) {
    background3 = "red";
  }
  else {
    background3 = "green";
  }
  
  if (digitalRead(4)) {
    background4 = "red";
  }
  else {
    background4 = "green";
  }

  if (digitalRead(5)) {
    background5 = "red";
  }
  else {
    background5 = "green";
  }

  HeaderHTML = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  HeaderHTML += "<head>\r\n";
  HeaderHTML += "    <meta charset='utf-8'>\r\n";
  HeaderHTML += "    <meta name='description' content='Home control'>\r\n";
  HeaderHTML += "</head>\r\n";
  HeaderHTML += "<body>\r\n";
  StyleHTML = "    <style>\r\n";
  StyleHTML += "        .text";
  StyleHTML += "        {";
  StyleHTML += "            font-family: 'Georgia';";
  StyleHTML += "            font-weight: bold;";
  StyleHTML += "        }";
  StyleHTML += "        .button";
  StyleHTML += "        {";
  StyleHTML += "            padding: 10px 20px 11px !important;";
  StyleHTML += "            font-size: 21px !important;";
  StyleHTML += "            background-color: #b5b5b5;";
  StyleHTML += "            font-weight: bold;";
  StyleHTML += "            text-shadow: 1px 1px #717171;";
  StyleHTML += "            color: #ffffff;";
  StyleHTML += "            border-radius: 100px;";
  StyleHTML += "            -moz-border-radius: 100px;";
  StyleHTML += "            -webkit-border-radius: 100px;";
  StyleHTML += "            border: 2px solid #000000;";
  StyleHTML += "            cursor: pointer;";
  StyleHTML += "            box-shadow: 0 1px 0 rgba(255, 255, 255, 0.5) inset;";
  StyleHTML += "            -moz-box-shadow: 0 1px 0 rgba(255, 255, 255, 0.5) inset;";
  StyleHTML += "            -webkit-box-shadow: 0 1px 0 rgba(255, 255, 255, 0.5) inset;";
  StyleHTML += "        }";
  StyleHTML += "    </style>\r\n";
  JavascriptHTML = "    <script>\r\n";
  JavascriptHTML += "        function redirect(gpio, state)\r\n";
  JavascriptHTML += "        {\r\n";
  //html += "            window.location.replace = 'http://" + WiFi.localIP().toString() + ":8000/gpio' + gpio + '/' + state\r\n";
  JavascriptHTML += "            window.location.href = 'http://" + WiFi.localIP().toString() + ":8000/gpio' + gpio + '/' + state\r\n";
  JavascriptHTML += "        }\r\n";
  JavascriptHTML += "    </script>\r\n\r\n";
  TableHTML = "    <table style='margin:0 auto; border: 3px solid #b0b1b1; background-color: #dedede; border-radius: 10px; padding:20px;'>\r\n";
  TableHTML += "        <tr>\r\n";
  TableHTML += "            <td style='padding: 20px;'><h1><span class='text'>SECTOR 1</span></h1></td>\r\n";
  TableHTML += "            <td style='padding: 20px;'><h1><span class='text'>SECTOR 2</span></h1></td>\r\n";
  TableHTML += "            <td style='padding: 20px;'><h1><span class='text'>SECTOR 3</span></h1></td>\r\n";
  TableHTML += "            <td style='padding: 20px;'><h1><span class='text'>SECTOR 4</span></h1></td>\r\n";
  TableHTML += "        </tr>\r\n";
  TableHTML += "        <tr>\r\n";
  TableHTML += "            <td>\r\n";
  TableHTML += "                <table style='border: 1px solid black; margin:0 auto; border-radius: 50%; height: 50px; width: 50px; background-color: " + background2 + ";'>\r\n";
  TableHTML += "                    <tr>\r\n";
  TableHTML += "                        <td></td>\r\n";
  TableHTML += "                    </tr>\r\n";
  TableHTML += "                </table>\r\n";
  TableHTML += "            </td>\r\n";
  TableHTML += "            <td>\r\n";
  TableHTML += "                <table style='border: 1px solid black; margin:0 auto; border-radius: 50%; height: 50px; width: 50px; background-color: " + background3 + ";'>\r\n";
  TableHTML += "                    <tr>\r\n";
  TableHTML += "                        <td></td>\r\n";
  TableHTML += "                    </tr>\r\n";
  TableHTML += "                </table>\r\n";
  TableHTML += "            </td>\r\n";
  TableHTML += "            <td>\r\n";
  TableHTML += "                <table style='border: 1px solid black; margin:0 auto; border-radius: 50%; height: 50px; width: 50px; background-color: " + background4 + ";'>\r\n";
  TableHTML += "                    <tr>\r\n";
  TableHTML += "                        <td></td>\r\n";
  TableHTML += "                    </tr>\r\n";
  TableHTML += "                </table>\r\n";
  TableHTML += "            </td>\r\n";
  TableHTML += "            <td>\r\n";
  TableHTML += "                <table style='border: 1px solid black; margin:0 auto; border-radius: 50%; height: 50px; width: 50px; background-color: " + background5 + ";'>\r\n";
  TableHTML += "                    <tr>\r\n";
  TableHTML += "                        <td></td>\r\n";
  TableHTML += "                    </tr>\r\n";
  TableHTML += "                </table>\r\n";
  TableHTML += "            </td>\r\n";
  TableHTML += "        </tr>\r\n";
  TableHTML += "        <tr>\r\n";
  TableHTML += "            <td style='text-align: center; vertical-align: middle; padding: 20px;'>\r\n";
  TableHTML += "                <input type='button' class='button' onClick='redirect(2, " + (String)!digitalRead(2) + ")' value='' style='margin-top: 20px; height: 50px; width: 70px; border-radius: 5px;'>\r\n";
  TableHTML += "            </td>\r\n";
  TableHTML += "            <td style='text-align: center; vertical-align: middle; padding: 20px;'>\r\n";
  TableHTML += "                <input type='button' class='button' onclick='redirect(3, " + (String)!digitalRead(3) + ")' value='' style='margin-top: 20px; height: 50px; width: 70px; border-radius: 5px;'>\r\n";
  TableHTML += "            </td>\r\n";
  TableHTML += "            <td style='text-align: center; vertical-align: middle; padding: 20px;'>\r\n";
  TableHTML += "                <input type='button' class='button' onclick='redirect(3, " + (String)!digitalRead(4) + ")' value='' style='margin-top: 20px; height: 50px; width: 70px; border-radius: 5px;'>\r\n";
  TableHTML += "            </td>\r\n";
  TableHTML += "            <td style='text-align: center; vertical-align: middle; padding: 20px;'>\r\n";
  TableHTML += "                <input type='button' class='button' onclick='redirect(3, " + (String)!digitalRead(5) + ")' value='' style='margin-top: 20px; height: 50px; width: 70px; border-radius: 5px;'>\r\n";
  TableHTML += "            </td>\r\n";
  TableHTML += "        </tr>\r\n";
  TableHTML += "    </table>\r\n";
  FooterHTML = "</body>\r\n";
  FooterHTML += "</html>\n";
}
