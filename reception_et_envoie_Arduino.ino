float zero_senseur; 
int PIN_ACS712 = A0;

float valeurACS712( int pin ){
  int valeur;
  float moyenne = 0;
  
  int nbr_lectures = 50;
  for( int i = 0; i < nbr_lectures; i++ ){
      valeur = analogRead( pin );
      moyenne += float(valeur)*4.8828125;// transformation en V x1000
  }
  moyenne = moyenne / float(nbr_lectures); // J'ai la valeur de la tension x1000
  return moyenne;
}

void puissance(){
  float courant;    
  float tension_efficace = 230; // tension efficace du réseau electrique
  float puissance_efficace; 
  
  float valeur_senseur = valeurACS712( PIN_ACS712 );
  puissance_efficace = (float)(valeur_senseur - zero_senseur)*13.633265167007498295841854124063 * 230;// 230/66 //(float)(valeur_senseur - zero_senseur)/66; 

  Serial.print(puissance_efficace,5);
  Serial.println( "," );
  delay(1000); // attendre une seconde 
}
void setup(){
  // calibration du senseur  (SANS COURANT)
  pinMode(16,OUTPUT);
  digitalWrite(16,HIGH);
  delay(500);
  zero_senseur = valeurACS712( PIN_ACS712 );
  delay(500);
  digitalWrite(16,LOW);
  Serial.begin(115200);
}



void loop(){
  puissance();
}
