#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

void setup() {
    Serial.begin(9600);
    Serial.println(F("AMG88xx pixels"));

    bool status;
    
    // default settings
    status = amg.begin();
    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }
    
    Serial.println("-- Pixels Test --");

    Serial.println();

    pinMode(13, OUTPUT);

    delay(100); // let sensor boot up

}


void loop() { 
    //read all the pixels
    amg.readPixels(pixels);

    Serial.print("[");
    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
      Serial.print(pixels[i-1]);
      Serial.print(", ");
      if( i%8 == 0 ) Serial.println();
    }
    Serial.println("]");
    Serial.println();

    int WIDTH = 3;
    int numHotCells = 24;
    int numColdCells = 40;

    float hotSum = 0;
    float coldSum = 0;
    boolean flag = false;
    int location = 0;
    float limit = 0.25;
    int check = 0;
    
 
    for(int k = 1; k <= 6; k++) {
      for(int j = 1; j <= AMG88xx_PIXEL_ARRAY_SIZE; j++) {
        if(k != 6) {
          check = k+2;
        } else { check = 0; }
        if((j % 8 == k) || (j % 8 == k+1) || (j % 8 == check)) {
          hotSum += pixels[j-1];
        } else {
          coldSum += pixels[j-1];
        }  
      }

      if(hotSum / numHotCells > coldSum / numColdCells + limit) {
        flag = true;
        location = k;
        break;
      }
    //delay a second
    delay(10);
    }
    Serial.println(hotSum / numHotCells);
    Serial.println(coldSum / numColdCells);
    if(flag == true) {
      Serial.println("Human");
      digitalWrite(13, HIGH);
      //delay(1000);
    } else {
      Serial.println("No human");
      digitalWrite(13, LOW);
    }
}
