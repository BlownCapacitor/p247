#include <WiFi.h>

const char* ssid     = "Arrtel_Swati";
const char* password = "9650337477@123";

WiFiServer server(80);

String redString = "0";
String greenString = "0";
String blueString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;
String header;
const int redPin = 13;     
const int greenPin = 12;
const int bluePin = 14;   

const int freq = 8000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;
const int resolution = 8;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 5000;
int buttonState = 0;
void setup() {
  Serial.begin(115200);
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);
  
  ledcAttachPin(redPin, redChannel);
  ledcAttachPin(greenPin, greenChannel);
  ledcAttachPin(bluePin, blueChannel);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){

  WiFiClient client = server.available();   
  if (client) {                           
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");        
    String currentLine = "";              
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      if (client.available()) {           
        char c = client.read();            
        Serial.write(c);                          header += c;
        if (c == '\n') {                   
            if (currentLine.length() == 0) {
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">");
client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");
client.println("</head><body><div class=\"container text-center\">");
client.println("<div class=\"row\"><div class=\"col\"><h1>RGB LED Controller</h1></div></div>");
client.println("<div class=\"row\"><div class=\"col\"><a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Send</a></div></div>");
client.println("<div class=\"row mt-3\"><div class=\"col\"><input class=\"jscolor {onFineChange:'update(this)'}\" id=\"rgb\"></div></div>");
client.println("<div class=\"row\"><div class=\"col\"></div></div>"); 
client.println("<script>function update(picker) {document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");
client.println("document.getElementById(\"change_color\").href=\"?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}");
client.println("window.addEventListener('load', function() { jscolor.trigger('rgb', 'show'); });"); 
client.println("</script></div></body></html>");
client.println();
 
            if(header.indexOf("GET /?r") >= 0) {
              pos1 = header.indexOf('r');
              pos2 = header.indexOf('g');
              pos3 = header.indexOf('b');
              pos4 = header.indexOf('&');
              redString = header.substring(pos1+1, pos2);
              greenString = header.substring(pos2+1, pos3);
              blueString = header.substring(pos3+1, pos4);
              ledcWrite(redChannel, 255 - redString.toInt());
              ledcWrite(greenChannel, 255 - greenString.toInt());
              ledcWrite(blueChannel, 255- blueString.toInt());
            
            }
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;   
        }
      }
    }
    header = "";
  
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}






