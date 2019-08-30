#include <WiFi.h>                  // Use this for WiFi instead of Ethernet.h
#include <HTTPClient.h>
WiFiClient client;

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

const char* ssid = "Pi3-AP";//"DESKTOP-8O9UF13 1533";//"Pi3-AP";   //WIFI_MaBox_5811C8 //Pi3-AP   // your SSID
const char* pass = "raspberry";  //"123456789";//"raspberry";//E8B8E1BB8A        //Pi3-AP    // your SSID Password

#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

/*......................................................................
 Fonction qui permette de definir un équipement dans la base de données
........................................................................*/
void definir_equipement( String nom_equipement, IPAddress ip_equipement){ // /api/creer_equipement.php 
        Serial.println("Voila ip : "); 
        Serial.println(ip_equipement);
        HTTPClient http;
        http.begin("http://10.10.0.1/api/creer_equipement.php?nom=" + nom_equipement + "&ip="+ip_equipement[0]+"."+ip_equipement[1]+"."+ip_equipement[2]+"."+ip_equipement[3]);
         //http.begin("http://10.10.0.1/api/clim_temp_capteur.php?nom=" + nom_equipement + "&temp=" + valeur_temp);// clim_temp_capteur.php
        http.GET();
        http.end();
}
/*..........................................................
        Fonction qui ajoute la valeur de la puissance
............................................................*/
void ajouter_une_puissance(String nom_equipement, float  valeur_puissance){
        HTTPClient http;
        http.begin("http://10.10.0.1/api/puissance.php?nom=" + nom_equipement + "&puissance=" + String(valeur_puissance));
        http.GET();
        http.end();
        
}

/*..........................................................
                  Fonction gestion de le WiFi
............................................................*/
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
/*..........................................................
          Fonction récupere la valeur de la puissance
............................................................*/
float puissanceInt( char reception_valeur_puissance ){
 float dataInt = 0;
   if (reception_valeur_puissance >0){
    dataInt =  Serial.parseFloat();
    Serial.flush();
    Serial.println( "La valeur de la Puissance est de " + String(dataInt/1000) +" W");
    affichage("La puissence", "est de " + String(dataInt) + "W");
    return dataInt;
}
}
/*..........................................................
          Fonction gestion de l'affichage
............................................................*/
void affichage(String nom_ou_ip_ou_temperature, String message_ligne_2)
{
  int placement_ligne_1 = (16-nom_ou_ip_ou_temperature.length())*0.5;
  int placement_ligne_2 = (16-message_ligne_2.length())*0.5;
     lcd.clear();
     lcd.setCursor(placement_ligne_1 , 0);
     lcd.print(nom_ou_ip_ou_temperature);
  
  if(nom_ou_ip_ou_temperature == "IP"){
    lcd.setCursor(placement_ligne_2, 1);
    lcd.print(WiFi.localIP());
  }else{
    lcd.setCursor(placement_ligne_2, 1);
    lcd.print(message_ligne_2);
  }
 
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
lcd.begin(16, 2);
affichage("Bienvenue chez","vous !!");
delay(10000);
affichage("Je vais me","lier au reseau");
delay(8000);
connexion_wifi(true);
definir_equipement( "ACS712",WiFi.localIP());
}

void loop() {
  connexion_wifi(false);
  float i = random(100);
  ajouter_une_puissance("ACS712", i);
  //ajouter_une_puissance("ACS712", puissanceInt(Serial.read()));
  delay(1000);
}
  //ajouter_une_puissance("ACS712", i);
  // put your main code here, to run repeatedly:
  
  //int i = random(100);
  //puissanceInt( char reception_valeur_puissance ); 
  //recuperation_des_infos(int httpPort, String fichierPhp, String nom);
