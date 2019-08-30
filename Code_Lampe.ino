#include <WiFi.h>                  // Use this for WiFi instead of Ethernet.h
#include <HTTPClient.h>
WiFiClient client;            // Use this for WiFi instead of EthernetClient
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

const char* ssid = "Pi3-AP";//"DESKTOP-8O9UF13 1533";//"Pi3-AP";   //WIFI_MaBox_5811C8 //Pi3-AP   // your SSID
const char* pass = "raspberry";  //"123456789";//"raspberry";//E8B8E1BB8A        //Pi3-AP    // your SSID Password
const char* host = "10.10.0.1";

int relais = 18;
int interrupteur = 16;
boolean etat = false;
char oppose = '1'; 
void definir_equipement( String nom_equipement, IPAddress ip_equipement){ // /api/creer_equipement.php 
        Serial.println("Voila ip : "); 
        Serial.println(ip_equipement);
        HTTPClient http;
        http.begin("http://10.10.0.1/api/creer_equipement.php?nom=" + nom_equipement + "&ip="+ip_equipement[0]+"."+ip_equipement[1]+"."+ip_equipement[2]+"."+ip_equipement[3]);
         //http.begin("http://10.10.0.1/api/clim_temp_capteur.php?nom=" + nom_equipement + "&temp=" + valeur_temp);// clim_temp_capteur.php
        http.GET();
        http.end();
}
char recuperation_des_infos(){
  String line;
  const char* host = "10.10.0.1";
   const int httpPort = 80;
   Serial.print("connecter a: ");
    Serial.println("10.10.0.1");

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
  
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
    }
    // This will send the request to the server
    client.print(String("GET ") + "/api/lampe.php" + "?nom=" + "lampe1" + " HTTP/1.1\r\n" +
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
    Serial.print(line);

    Serial.println();
    Serial.println("closing connection");
    return line[1];
}
void connexion_wifi(boolean initialisation_ou_probleme){
  int compteur = 0;
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, pass);
    while ( WiFi.status() != WL_CONNECTED){
      if(initialisation_ou_probleme)
      Serial.println("Connection en cours...");
      else{
        if(compteur == 0){
        Serial.println("La box" + String(ssid) + " est deconnecter, reconnection en cours...");}
        delay(1000);
        compteur++;
      }
    }
    Serial.println("");
    Serial.println("La box a pris " + String(compteur) +" secondes pour ce reconnecter"); // il prend 80 secondes pour redemarer
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}
void modifier_etat_lampe(String nom_equipement, int  valeur_lampe){
        HTTPClient http;
        http.begin("http://10.10.0.1/api/puissance.php?nom=" + nom_equipement + "&puissance=" + String(valeur_lampe));
        http.GET();
        http.end();
        
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(relais, OUTPUT); // RELAY 
pinMode(interrupteur, INPUT); // INTERRUPTEUR ALLUMER
connexion_wifi(true);
oppose = recuperation_des_infos();
}

void loop() {
  connexion_wifi(false);
  char etat_lampe = recuperation_des_infos();
  Serial.println(etat_lampe);
  Serial.println(oppose);
  if(etat_lampe != oppose || digitalRead(interrupteur) == HIGH){oppose = etat_lampe;  etat = !etat;}
  else {
    if(etat ==  true) digitalWrite(relais, LOW);
    else {
      if(etat ==  false) digitalWrite(relais, HIGH);
    }
  }
  
 //connexion_wifi(false);
 //ecuperation_des_infos("lampe.php", "lampe1");
/*recuperation_des_infos(int httpPort, String fichierPhp, String nom);
definir_equipement( String nom_equipement, IPAddress ip_equipement);
modifier_etat_lampe(String nom_equipement, int  valeur_lampe);*/
}
