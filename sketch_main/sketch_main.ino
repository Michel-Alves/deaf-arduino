#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0, 100);
EthernetServer server(80);
int LDR;
String hexaValue;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        LDR = analogRead(A0);
        hexaValue = toHexa(LDR * 7);
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
         
          client.println("<html>");
          client.println("<head>");
          client.println("<meta charset=\"utf-8\">");
          client.println("<script type=\"text/javascript\"><!--");
          client.println("window.addEventListener('load',");
          client.println("function () {");
          client.println("var elem = document.getElementById('medida');");
          client.println("var context = elem.getContext('2d');");
          client.println("context.fillStyle   = '#" + hexaValue + "';");
          client.println("context.strokeStyle = '#000000';");
          client.println("context.lineWidth   = 4;");
          client.println("context.beginPath();");
          client.println("context.moveTo(10, 10);");
          client.println("context.lineTo(100, 100);");
          client.println("context.lineTo(200, 10);");
          client.println("context.lineTo(9, 10);");
          client.println("context.fill();");
          client.println("context.stroke();");
          client.println("},");
          client.println("false);");
          client.println("</script>");
          client.println("<body>");
          client.println("<h1> Leitura de valor em LDR - Testando a Ethernet Shield </h1>");
          client.println("<canvas id=\"medida\" width=\"300\" height=\"300\" align=\"center\" />");
          client.println("</body>");
          client.println("</html>");
          
          
          
          
          
          //client.println("<meta http-equiv=\"refresh\" content=\"5\">");
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

}

public String toHexa(int in){
  String hexaValue = "000000";
  if(in < 10){ 
    hexaValue.setCharAt(5, char(in));
    return hexaValue;
  }else if (in < 16) {
    char hexaSimbol = '';  
  
    switch (in) {
      case 10: hexaSimbol = 'A'; break;
      case 11: hexaSimbol = 'B'; break;
      case 12: hexaSimbol = 'C'; break;
      case 13: hexaSimbol = 'D'; break;
      case 14: hexaSimbol = 'E'; break;
      case 15: hexaSimbol = 'F'; break;
    }
    hexaValue.setCharAt(5, hexaSimbol);
    return hexaValue;   
  
  }else {
    int homeHexa = 0;
    char hexaSimbol = '';
    int quociente = in, resto = 1;
    while (resto >= 1) {
      quociente = quociente / 16;
      resto = quociente % 16;
      if(resto < 10) {
        hexaValue.setCharAt(homeHexa, char(resto));
      else {
        switch (in) {
          case 10: hexaSimbol = 'A'; break;
          case 11: hexaSimbol = 'B'; break;
          case 12: hexaSimbol = 'C'; break;
          case 13: hexaSimbol = 'D'; break;
          case 14: hexaSimbol = 'E'; break;
          case 15: hexaSimbol = 'F'; break;
          hexaValue.setCharAt(homeHexa, char(resto));
       }
      }
    } 
    homeHexa++;
   }
   return hexaValue;
  }
} 
