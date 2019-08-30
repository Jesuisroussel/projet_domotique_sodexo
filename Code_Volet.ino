#include <WiFi.h>                  // Use this for WiFi instead of Ethernet.h
#include <HTTPClient.h>
WiFiClient client;            // Use this for WiFi instead of EthernetClient
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

const char* ssid = "Pi3-AP";//"DESKTOP-8O9UF13 1533";//"Pi3-AP";   //WIFI_MaBox_5811C8 //Pi3-AP   // your SSID
const char* pass = "raspberry";  //"123456789";//"raspberry";//E8B8E1BB8A        //Pi3-AP    // your SSID Password

int relais_1 = 17;
int relais_2 = 18;
int b_monter = 35;
int b_descendre = 25;
int b_fin_de_course_haut = 16;
int b_fin_de_course_bas = 5;
void definir_equipement( String nom_equipement, IPAddress ip_equipement){ // /api/creer_equipement.php 
        Serial.println("Voila ip : "); 
        Serial.println(ip_equipement);
        HTTPClient http;
        http.begin("http://10.10.0.1/api/creer_equipement.php?nom=" + nom_equipement + "&ip="+ip_equipement[0]+"."+ip_equipement[1]+"."+ip_equipement[2]+"."+ip_equipement[3]);
         //http.begin("http://10.10.0.1/api/clim_temp_capteur.php?nom=" + nom_equipement + "&temp=" + valeur_temp);// clim_temp_capteur.php
        http.GET();
        http.end();
}
String recuperation_des_infos( String fichierPhp, String nom){
  String line;
  
   Serial.print("connecter a: ");
    Serial.println("10.10.0.1");
   const char* host = "10.10.0.1";
   const int httpPort = 80;
    // Use WiFiClient class to create TCP connections
    //WiFiClient client;
  
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
    }
    // This will send the request to the server
    client.print(String("GET ") + "/api/volet.php" + "?nom=" + nom + " HTTP/1.1\r\n" +
                 "Host: " + "10.10.0.1" + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
        }
    }
    
    while(client.available()) { line = String(client.readStringUntil('\r')); }
    
    line[0] = '\0';
    line[line.length()-1] = '\0';
    line = String(line[1]);
    Serial.print(line);

    Serial.println();
    Serial.println("closing connection");
    return line;
}
void connexion_wifi(boolean initialisation_ou_probleme){
  int compteur = 0;
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, pass);
    while ( WiFi.status() != WL_CONNECTED){
      if(initialisation_ou_probleme){
      Serial.println("Connection en cours...");
      //affichage("Connexion...","");
      }
      else{
        if(compteur == 0){
        Serial.println("La box" + String(ssid) + " est deconnecter, reconnection en cours...");
        //affichage("La box " + String(ssid), "est deconnecté");
        }
        delay(1000);
        compteur++;
      }
    }
//    affichage("On est connecte",";)");
    Serial.println("");
    Serial.println("La box a pris " + String(compteur) +" secondes pour ce reconnecter"); // il prend 80 secondes pour redemarer
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //connexion_wifi(true);
  //definir_equipement("volet1", WiFi.localIP());
  pinMode(relais_1, OUTPUT);
  pinMode(relais_2, OUTPUT);
  pinMode(b_monter, INPUT);
  pinMode(b_descendre, INPUT);
  pinMode(b_fin_de_course_haut, INPUT);
  pinMode(b_fin_de_course_bas, INPUT);
}

void loop() {
 // connexion_wifi(false);
  //recuperation_des_infos("volet.php", "volet1") == "1" || 
  if (digitalRead(b_descendre) == HIGH){
   while(digitalRead(b_fin_de_course_bas) == LOW){
    digitalWrite(relais_1,LOW);
   }
   digitalWrite(relais_1,HIGH);
   // Envoyé au serveur un chiffre different de 0 1 2
  }else{
    //recuperation_des_infos("volet.php", "volet1") == "0" ||
    if( digitalRead(b_monter) == HIGH){
      while(digitalRead(b_fin_de_course_haut) == LOW){
      digitalWrite(relais_2,LOW);
      }
      digitalWrite(relais_2,HIGH);
      // Envoyé au serveur un chiffre different de 0 1 2
    } else{
      digitalWrite(relais_2,HIGH);
      digitalWrite(relais_1,HIGH); // eteindre 
    }
  }
}
