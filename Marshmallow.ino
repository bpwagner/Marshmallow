/*
 * Copyright (c) 2015 RF Digital Corp. All Rights Reserved.
 *
 * The source code contained in this file and all intellectual property embodied in
 * or covering the source code is the property of RF Digital Corp. or its licensors.
 * Your right to use this source code and intellectual property is non-transferable,
 * non-sub licensable, revocable, and subject to terms and conditions of the
 * SIMBLEE SOFTWARE LICENSE AGREEMENT.
 * http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt
 *
 * THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
 *
 * This heading must NOT be removed from this file.
 */

 /* Marsmallow toaster
  *  LVL1
  *  Jose Cabrera
  *  Brian Wagner
  *  Tammy 
  *  9/24/16
  */

#include <Servo.h>
#include <SimbleeForMobile.h>
#include "image.h"
#include "pitches.h"

#define UP 180
#define MID 90
#define DOWN 0

int RelayA = 0;
int RelayB = 1;

Servo s1;  //pin2
Servo s2;  //pin3
Servo s3;  //pin4
Servo s4;  //pin5

int photocell = 6;
int laser = 7;
int firesensor = 8;

int buzzer = 10;

// debounce time (in ms)
int debounce_time = 10;

// maximum debounce timeout (in ms)
int debounce_timeout = 100;

int currentState, previousState = 0;
uint8_t ui_button;
uint8_t ui_status;
int count = 0;
int state = 0;
int started = 0;
int update_status = 0;
int status_updated = 0;
String  my_status ="";
char charBuf[50];
int angle;
int i;
int test;


/*
 * Traditional Arduino setup routine
 * 
 * Initialize the SimbleeForMobile environment.
 */
void setup() {
  //Serial.begin(9600);
  //Serial.println("Started");

  // button press will be shown on the iPhone app)
  //pinMode(button, INPUT);
  pinMode(photocell, INPUT);
  pinMode(RelayA, OUTPUT);
  pinMode(RelayB, OUTPUT);
  pinMode(laser, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(firesensor, INPUT);

  //servos
  s1.attach(2);
  s2.attach(3);
  s3.attach(4);
  s4.attach(5);
  
  digitalWrite(RelayA,LOW);
  digitalWrite(RelayB,LOW);


 //playSong();
  
  // put your setup code here, to run once:
  SimbleeForMobile.deviceName = "Marshmallow";
  SimbleeForMobile.advertisementData = "tstr";
  SimbleeForMobile.domain = "template.simblee.com";
  // Begin Simblee UI
  SimbleeForMobile.begin();
  //playSong();
}


void playSong(){
// notes in the melody:

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzer, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzer);
  }




  
}


/*
 * The traditional Arduino loop method
 * 
 * Enable SimbleeForMobile functionality by calling the process method
 * each time through the loop. This method must be called regularly for
 * functionality to work.
 */
void loop() {

  if (SimbleeForMobile.updatable){

    if (update_status == 1) {
      my_status.toCharArray(charBuf, 50) ;
      SimbleeForMobile.updateText(ui_status, charBuf);
      update_status = 0;
      status_updated = 1;
    }
    
  }

  switch (state) {
    case 0: {  //just started, wait for the app
      //Serial.println("state == 0");
      digitalWrite(RelayA,LOW);
      digitalWrite(RelayB,LOW); 
//
//    for (i=0;i<100;i++){
//      test = analogRead(photocell);
//      //Serial.print("test: ");
//      //Serial.println(test);
//      delay(500);
//    }

//    for (i=0;i<100;i++){
//      test = digitalRead(firesensor);
//      //Serial.print("test: ");
//      //Serial.println(test);
//      delay(500);
//    }
    
      if (status_updated == 0){
         my_status = "Waiting...";
         update_status = 1;
      }
      if (started == 1){
         state = 1;
         status_updated = 0;
      }
      s1.write(MID);  //fire is set to 
      s4.write(MID);  //s1 and s4 are fire
      s2.write(MID);
      s3.write(DOWN);

      //s2.detach();



    }
    break;
    case 1:{
      //Serial.println("state == 1");
      if (status_updated == 0){
         my_status = "Lighting the fire";
         update_status = 1;
      }
      else {

        //move the s1 servo to light the fire
        for(angle = MID; angle < UP-40; angle++) { 
            s1.write(angle);  
            delay(10); 
        } 
        delay(1000);

         //move the s1 servo to light the fire
        for(angle = MID; angle > DOWN+40; angle--) { 
            s4.write(angle); 
            delay(10); 
        } 
        delay(1000);


        //check for fire

        test = digitalRead(firesensor);
        if (test == LOW) {
            //nofire
            my_status = "Check to see you have fire";
            my_status.toCharArray(charBuf, 50) ;
            SimbleeForMobile.updateText(ui_status, charBuf);
        }
        
        
        state =2;
        status_updated = 0;
      }
      
      
    }
    break;
    case 2:{
      //Serial.println("state == 2");
      if (status_updated == 0){
         my_status = "Moving marshmallow into place";
         update_status = 1;
      }
      else {


        digitalWrite(laser,HIGH);

        //s2.attach(3);
        //move the s2 servo to toast position
        for(angle = MID; angle < UP; angle++) { 
            s2.write(angle); 
            delay(10); 
        } 
        //delay(1000);

        test = digitalRead(photocell);
        if (test == LOW) {
            //nofire
            my_status = "Is the marshmallow in place";
            my_status.toCharArray(charBuf, 50) ;
            SimbleeForMobile.updateText(ui_status, charBuf);
        }
        

        digitalWrite(laser,LOW);
        
        state =3;
        status_updated = 0;
        
      }

    }
    break;
    case 3:{
      //Serial.println("state == 3");
      if (status_updated == 0){
         my_status = "Roasting - burn, baby, burn!";
         update_status = 1;
      } else {

        //rotate marshmallow slowly
        for (i = 0; i < 5; i++) {
          for(angle = DOWN; angle < UP; angle++) { 
              s3.write(angle); 
              delay(10); 
          } 
          delay(50);
          for(angle = UP; angle > DOWN; angle--) { 
              s3.write(angle); 
              delay(10); 
          } 
          delay(50);
        }

        state = 4;
        status_updated = 0;




        
        
      }

      
    }
    break;
    case 4:{
      //Serial.println("state == 4");
      if (status_updated == 0){
         my_status = "Removing the marshhmallow";
         update_status = 1;
      } else {

        //move the s1 servo to light the fire
        for(angle = UP-40; angle > MID; angle--) { 
            s1.write(angle);  
            delay(10); 
        } 
        delay(1000);

         //move the s1 servo to light the fire
        for(angle = DOWN+40; angle < MID; angle++) { 
            s4.write(angle); 
            delay(10); 
        } 
        delay(1000);

       digitalWrite(RelayA,HIGH);
      delay(3000);
      digitalWrite(RelayA,LOW);
        

        //move the s2 servo to plate position
        for(angle = UP; angle > DOWN+20; angle--) { 
            s2.write(angle); 
            delay(20); 
        } 
        delay(1000);
        
        state = 5;
        status_updated = 0;


        
      }

    }
    break;
    case 5:{
      //Serial.println("state == 5");
      if (status_updated == 0){
         my_status = "Completeing the s'more";
         update_status = 1;
      } else {

        delay(2000);
        state = 6;
        status_updated = 0;

        //playSong();
        delay(5000);


        
      }

    }
    break;
    case 6:{
      //Serial.println("state == 6");
      if (status_updated == 0){
         my_status = "Back to start";
         update_status = 1;
      } else {

        delay(2000);
        state = 0;
        started = 0;
        status_updated = 0;
      }


    }
    break;
  }
  
  // put your main code here, to run repeatedly:
  // process must be called in the loop for Simblee UI
  SimbleeForMobile.process();  
}


void ui()
{  

  #define IMAGE1 1
  SimbleeForMobile.beginScreen();

  printf("UI screen size: %dx%d", SimbleeForMobile.screenWidth, SimbleeForMobile.screenHeight);
  //Serial.println(SimbleeForMobile.screenWidth);
  //Serial.println(SimbleeForMobile.screenHeight);
  
  color_t darkgray = rgb(85,85,85);
  color_t hotpink = rgb(255,20,147);
  
  SimbleeForMobile.beginScreen(hotpink);


  SimbleeForMobile.imageSource(IMAGE1, PNG, lvl1_butterscotch_sm_png, lvl1_butterscotch_sm_png_len);
  SimbleeForMobile.drawImage(IMAGE1, 35,400);


  SimbleeForMobile.drawText(30, 90, "Marshmallow Toaster\n"
                                    "LVL1 Hackerspace\n"
                                    "burn, baby, burn!", WHITE, 30);

  ui_status = SimbleeForMobile.drawText(30, 300, "  ",WHITE);

  ui_button = SimbleeForMobile.drawButton(120, 220, 100, "Start!",WHITE,0);
  SimbleeForMobile.setEvents(ui_button, EVENT_PRESS | EVENT_RELEASE);  

  SimbleeForMobile.endScreen();
}





/*
 * SimbleeForMobile event callback method
 */
void ui_event(event_t &event)
{
    if (event.id == ui_button) {
      if (event.type == EVENT_PRESS) {
        //Serial.println("button pressed");
        started = 1; //start the state machine
      }
      //else if (event.type == EVENT_RELEASE)
        //Serial.println("button released");
  }
}

void SimbleeForMobile_onDisconnect()
{ 
  //Serial.println("onDisconnect");
  state = 0;
  started = 0;
  s1.write(90);
  
  
}
