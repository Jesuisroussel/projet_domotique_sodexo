/* 
 Nom des fichier pour les equipements
clim.php
clim_temp.php // la 
clim_temp_capteur.php // envoi la temperature 
creer_equipement.php
lampe.php // etat de la lampe
volet.php
puissance.php// 
*/
//#include <MsTimer2.h>
#include "DHT.h"
#define DHTPIN 21
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <WiFi.h>                  // Use this for WiFi instead of Ethernet.h
#include <HTTPClient.h>
WiFiClient client;            // Use this for WiFi instead of EthernetClient
#include <LiquidCrystal.h>

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int i = 0;

const char* ssid = "Pi3-AP";//"DESKTOP-8O9UF13 1533";//"Pi3-AP";   //WIFI_MaBox_5811C8 //Pi3-AP   // your SSID
const char* pass = "raspberry";  //"123456789";//"raspberry";//E8B8E1BB8A        //Pi3-AP    // your SSID Password


LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

int led_bleu = 32, led_vert = 35, led_rouge = 34;

void definir_equipement( String nom_equipement, IPAddress ip_equipement){ // /api/creer_equipement.php 
        Serial.println("Voila ip : "); 
        Serial.println(ip_equipement);
        HTTPClient http;
        http.begin("http://10.10.0.1/api/creer_equipement.php?nom=" + nom_equipement + "&ip="+ip_equipement[0]+"."+ip_equipement[1]+"."+ip_equipement[2]+"."+ip_equipement[3]);
        http.GET();
        http.end();
        //http.begin("http://10.10.0.1/api/clim_temp_capteur.php?nom=" + nom_equipement + "&temp=" + valeur_temp);// clim_temp_capteur.php
}
void ajouter_une_temperature(String nom_equipement, float  valeur_temp){
        HTTPClient http;
        //http.begin("http://10.10.0.1/api/clim_temp_capteur.php?nom=" + nom_equipement + "&temp=" + String(valeur_temp));// clim_temp_capteur.php
         http.begin("http://10.10.0.1/api/temperature.php?nom=" + nom_equipement + "&temp=" + String(valeur_temp));
        http.GET();
        http.end();
}

String recuperation_des_infos(String fichierPhp, String nom){ // temperature voulu
  String reponse_serveur;
  const char* host = "10.10.0.1";
   const int httpPort = 80;
    // Use WiFiClient class to create TCP connections
    
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        affichage("Pas de connexion", "Verifier la Box");
    }
    // client.print(String("GET ") + "/api/clim.php"+ "?nom=" + nom + " HTTP/1.1\r\n" +  // Allumer la clim
    // This will send the request to the server
    client.print(String("GET ") + "/api/"+ fichierPhp + "?nom=" + nom + " HTTP/1.1\r\n" +
                 "Host: " + "10.10.0.1" + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            affichage("Pas de connexion", "Verifier la Box");
            client.stop();
        }
    }
    
    while(client.available()) { reponse_serveur = String(client.readStringUntil('\r')); }
    
    reponse_serveur[0] = '\0';
    reponse_serveur[reponse_serveur.length()-1] = '\0';
    //Serial.print(reponse_serveur);
    
    return reponse_serveur;
}

void affichage(String nom_ou_ip_ou_temperature, String message_ligne_2)
{
  int placement_ligne_1 = (16-nom_ou_ip_ou_temperature.length())*0.5;
  int placement_ligne_2 = (16-message_ligne_2.length())*0.5;
     lcd.clear();
     lcd.setCursor(placement_ligne_1 , 0);
     lcd.print(nom_ou_ip_ou_temperature);
  
  if(nom_ou_ip_ou_temperature == "IP"){
    lcd.setCursor(3, 1);
    lcd.print(WiFi.localIP());
  }else{
    lcd.setCursor(placement_ligne_2, 1);
    lcd.print(message_ligne_2);
  }
 
}
float capture_dht11(){
  float t = dht.readTemperature();
  return t;
}
void connexion_wifi(boolean initialisation_ou_probleme){
  int compteur = 0;
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, pass);
    while ( WiFi.status() != WL_CONNECTED){
      if(initialisation_ou_probleme){
      Serial.println("Connection en cours...");
      affichage("Connexion...","");}
      else{
        if(compteur == 0){
        Serial.println("La box" + String(ssid) + " est deconnecter, reconnection en cours...");
        affichage("La box " + String(ssid), "est deconnecté");
        }
        delay(1000);
        compteur++;
      }
    }
    affichage("On est connecte",";)");
    Serial.println("");
    Serial.println("La box a pris " + String(compteur) +" secondes pour ce reconnecter"); // il prend 80 secondes pour redemarer
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}
void setup() {
  // put your setup code here, to run once:
dht.begin();
Serial.begin(115200);
lcd.begin(16, 2);
pinMode(led_bleu, OUTPUT);
pinMode(led_vert, OUTPUT);
pinMode(led_rouge, OUTPUT);
lcd.setCursor(0, 0);
lcd.print("Sodexo Guyane");
lcd.setCursor(0, 1);
lcd.print("Projet Domotique");
delay(5000);
lcd.clear();
//WiFi.begin(ssid, pass);
    connexion_wifi(true);
    affichage("IP","");
    definir_equipement("clim1", WiFi.localIP());
}
void led_activation(String nom_led, String etat){
  if( nom_led == "BLEU" && etat ==  "HIGH") digitalWrite(led_bleu, HIGH);
  else{
  if( nom_led == "BLEU" && etat ==  "LOW")digitalWrite(led_bleu, LOW);
  if( nom_led == "VERT" && etat ==  "HIGH")digitalWrite(led_vert, HIGH);
  if( nom_led == "VERT" && etat ==  "LOW")digitalWrite(led_vert, LOW);
  if( nom_led == "ROUGE" && etat ==  "HIGH")digitalWrite(led_rouge, HIGH);
  if( nom_led == "ROUGE" && etat ==  "LOW")digitalWrite(led_rouge, LOW);
  }
}

void loop() {
  
  //affichage(String nom_ou_ip_ou_temperature, String message_ligne_2);
  connexion_wifi(false);
  ajouter_une_temperature("clim14",capture_dht11());
  String etat = recuperation_des_infos("clim.php", "clim1");
  String temp_voulu = recuperation_des_infos("clim_temp.php", "clim1");
  /*Serial.println(etat);
    delay(1000);
    Serial.println(temp_voulu);
    delay(1000);*/
              if(etat[1] == '1'){// Etat de la clim
                  Serial.println(temp_voulu);  
                  affichage("Clim on", "Temp :"+String(temp_voulu[1])+String(temp_voulu[2])+" C"); 
              }else{
                Serial.println("La Clim est éteint");
                affichage("La Clim", "est eteinte");
                delay(10);
                }
  }

  /*for(int temp = 0; temp < 100; temp++){
  ajouter_une_temperature("clim14", temp);
  delay(500);
  }
Serial.println(WiFi.localIP());
delay(500);*/
