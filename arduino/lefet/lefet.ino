#include "FastLED.h"

#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    30
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

// variables pour les leds
CRGB leds[NUM_LEDS];
uint32_t period = 5 * 60000L;       // 5 minutes
uint8_t gHue = 0; // rotating "base color" used by many of the 

// On définit le bouton poussoir
const int buttonPin = 12;
int buttonState = 0;
int lastButtonState = 0;

unsigned long temps = millis();
unsigned long temps_start = millis();

// On définit le buzzer
const int buzzerPin = 11;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  noTone(buzzerPin);

  // Initialisation du chrono
  temps = 0;
  doSequenceInit();
}

void loop() {

// on fixe un timer par défaut à period = 5 minutes
// après 5 minutes (ou après appui sur le bouton) on recommence
for( uint32_t tStart = millis();  (millis()-tStart) < period;  ){
   // Si bouton appuyé on sort et on va à la séquence de fin
   if (checkButton() > 0) {
    break;
   }
   // mise à jour du chrono
   temps = millis()-temps_start;
   Serial.print("Time: ");
   Serial.println(temps);
   // mise à jour des leds 
   updateLeds(temps);
   delay(500);
  }
// les 5 minutes sont passées ou on a appuyé sur le bouton 
doSequenceEnd();
delay(2000);
// on réinitialise tout (affichage des leds, timers, ..) et on recommence...
doSequenceInit();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------
void doSequenceInit(){
  Serial.println("Initialisation");
  // Un serpent de leds passe. On utilise toutes les leds
  
  for (int i=0; i < NUM_LEDS ; i++){
    leds[i].setRGB(12, 255, 0);
    FastLED.show();
    delay(100);
  }
  switchLedsOff();
  temps_start = millis();
}

void doSequenceEnd(){
  Serial.println("Ending Sequence...");
  // basé sur un exemple fourni par fastled
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  FastLED.show();
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
    // insert a delay to keep the framerate modest
  FastLED.delay(60000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  delay(5000);
  switchLedsOff();
}

// on vérifie juste l'état du bouton appuyé/pas appuyé
int checkButton(){
  buttonState = digitalRead(buttonPin);
  Serial.println("Check button state");
  if (buttonState == HIGH) { // bouton appuyé 
    Serial.println("Check button state");
    return 1;
  } else {
    return 0;
  }
  
}

// mise à jour des leds
// ça dépend de l'avancement du chrono
// on fait un avancement régulier etr on répartit les 30 leds sur les 5 minutes
void updateLeds(unsigned long temps){

  switchLedsOff();
  Serial.println("Update leds");
  Serial.print("Time: ");
  Serial.println(temps);
  
  int pos = int(NUM_LEDS*temps/period);
  Serial.print("pos:");
  Serial.println(pos);

  int delta = int(500/(pos+1));
  Serial.print("delta:");
  Serial.println(delta);
  
  for(int i=0; i< pos;i++){
      Serial.print("Led:");
      Serial.println(i);
      Serial.print("Couleur:");

         if (i < 5) {
           leds[i].setRGB(0, 0, 255);
           Serial.println("Bleu");
          }else if (i> 4 and i <10){
           leds[i].setRGB(0, 200, 255);
           Serial.println("Bleu CLair");
          }else if(i > 9 and i <15){
           leds[i].setRGB(12, 255, 0);
           Serial.println("Vert");
         }else if(i > 14 and i <20){
           leds[i].setRGB(255, 102, 255);
           Serial.println("Rose");
         }else if(i > 19 and i <25){
           leds[i].setRGB(255, 128, 0);
           Serial.println("Orange");
          } else {
           leds[i].setRGB(255, 0, 0);
           Serial.println("Rouge");
          }
    FastLED.show();
    delay(delta);
  }
  switchLedsOff();
}

// on éteint toutes les leds
void switchLedsOff(){
    for (int i=0; i < NUM_LEDS; i++){
    leds[i].setRGB(0, 0, 0);
    FastLED.show();
    delay(33);
    }
}
