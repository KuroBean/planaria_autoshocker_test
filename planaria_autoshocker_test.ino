#include <Servo.h>
int count = 0;
#define uv 5
#define led 7
#define shock 12
#define buzzer 2
#define servo 4

#define uvStrength 255
#define uvTest false

//DATA RECORDING
#define isRecordingUV false
#define isRecordingShock true
#define shockDuration 300
#define uvDuration 3000//for tail cond test:3000
#define recordDuration 5000 //duration of each recording in msec, for tail cond test: 14000
#define recordNum 20 //how many recordings 
#define waitMinute 2 //wait time between trials (in minutes)

#define servoDefaultAng 120
#define servoEndAng 165 //servo: vertically up is 90 deg otu of 180, + is clockwise from servo pov

#define shockOnly false
#define autoMode false
#define shockTimes 1//30 //per series
#define seriesTimes 1//2
#define bigSeriesTimes 1//12
#define cooldown 60000 //cool down time after each trial, msec
#define minutes 10 //cool down after series of trials in minutes
#define bigSeriesTimeGapHours 6//hours of time gap between each big series, should add with 
//bigSereisTimeGapMinutes to be 6hr 38min for each day to have 3 big series, assuming (32min+10min)*2 per big series
#define bigSeriesTimeGapMinutes 38//the thing said above

#define initialRun false
Servo servy;
void setup() {
  // put your setup code here, to run once:

  pinMode(uv, OUTPUT);
  pinMode(shock, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  servy.attach(servo);
  servy.write(servoDefaultAng);
  Serial.begin(9600);
  delay(1000);


  if (!uvTest && initialRun) {

    for (int b = 0; b < bigSeriesTimes; b++) {//for each big series, should be 1 morning, afternoon, and night, 3 per day

      for (int i = 0; i < seriesTimes; i++) {//counts series
        digitalWrite(led, HIGH);
        Serial.print("num of series of trials: ");
        Serial.println(i + 1);
        for (int m = 0; m < shockTimes; m++) {
          digitalWrite(uv, HIGH);
          delay(2000);
          digitalWrite(shock, HIGH);
          digitalWrite(led, HIGH);
          delay(300);
          digitalWrite(shock, LOW);
          digitalWrite(led, LOW);
          digitalWrite(uv, LOW);
          Serial.print("\t");
          Serial.println(m + 1);
          delay (cooldown);
        }
        digitalWrite(led, LOW);

        for (int i = 0; i < 1000; i++) {
          delay(minutes * 60);
        }
      }//big series ends
      
      for (int hours = 0; hours < bigSeriesTimeGapHours; hours++) {
        //belwo delays 1 hour
        for (int mi = 0; mi < 60; mi++) {
          for (int sec = 0; sec < 60; sec++) {
            delay(1000);
          }
        }

      }
      for (int i = 0; i < 1000; i++) {
        delay(bigSeriesTimeGapMinutes * 60);
      }
    }


  }
  else if (uvTest && !shockOnly) {

    analogWrite(uv, uvStrength);//meanwhile turn on slightly early for camera to get used to brightness
    delay(5000);
    digitalWrite(uv, LOW);
  }
  else if (shockOnly) {

    digitalWrite(led, HIGH);

    digitalWrite(shock, HIGH);
    delay(1000);//buzz(1000,2);
    digitalWrite(buzzer, LOW);
    digitalWrite(shock, LOW);

  }
  else if (isRecordingUV) {
    digitalWrite(led, HIGH);
    for (int i = 0; i < recordNum; i++) {
      //servo: vertical/upward pointing= 90 deg out of 180, + is away from you
      //first click
      for (int d = servoDefaultAng; d <= servoEndAng; d++) {
        servy.write(d);
        delay(7);
        Serial.println(d);
      }
      servy.write(servoDefaultAng);
      delay(80);//part of wait for recording button to load and actually begin, was 140-->100-->80
      analogWrite(uv, uvStrength);//meanwhile turn on slightly early for camera to get used to brightness
      delay(uvDuration + 200);

      
      digitalWrite(uv, LOW);//CHANGED CODE TO ADD UV DURATION VS RECORD TIME DIFFERENCE BUT BEWARE WHEN RECORDTIME=UVTIME

      //recording wait
      if (recordDuration - 140 - uvDuration >= 0) {
        delay(recordDuration - 140 - uvDuration); //-200 constant for lag after
      }
      
      
      //tap to stop
      for (int d = servoDefaultAng; d <= servoEndAng; d++) {
        servy.write(d);
        delay(7);
      }
      servy.write(servoDefaultAng);
      delay(690);
      
      //delay between recordings
      for (int i = 0; i < 60; i++) {
        delay(waitMinute * 1000); //first num is minutes
      }
    }
  } else if (isRecordingShock) {
    digitalWrite(led, HIGH);
    for (int i = 0; i < recordNum; i++) {
      //servo: vertical/upward pointing= 90 deg out of 180, + is away from you
      //first click
      for (int d = servoDefaultAng; d <= servoEndAng; d++) {
        servy.write(d);
        delay(7);
      }
      servy.write(servoDefaultAng);
      delay(460);//WAS 250, part of wait for recording button to load and actually begin
      digitalWrite(shock, HIGH);//meanwhile turn on slightly early for camera to get used to brightness
      //recording wait
      delay(shockDuration);
      digitalWrite(shock, LOW);
      delay(recordDuration - 200 - shockDuration); //-200 constant for lag after
      for (int d = servoDefaultAng; d <= servoEndAng; d++) {
        servy.write(d);
        delay(7);
      }
      servy.write(servoDefaultAng);
      delay(690);
      digitalWrite(uv, LOW);

      //delay between recordings
      for (int i = 0; i < 60; i++) {
        delay(waitMinute * 1000); //first num is minutes
      }
    }
  }
  digitalWrite(led, LOW);//OR HIGH WHEN CONDITIIONING
}


void loop() {
  // put your main code here, to run repeatedly:
  if (autoMode) {
    digitalWrite(led, HIGH);
    count++;
    Serial.print("num of series of trials: ");
    Serial.println(count);
    for (int i = 0; i < shockTimes; i++) {
      digitalWrite(uv, HIGH);
      delay(2000);
      digitalWrite(shock, HIGH);
      digitalWrite(led, HIGH);
      delay(300);
      digitalWrite(shock, LOW);
      digitalWrite(led, LOW);
      digitalWrite(uv, LOW);
      Serial.print("\t");
      Serial.println(i + 1);
      delay (cooldown);
    }
    digitalWrite(led, LOW);
  }
  for (int i = 0; i < 1000; i++) {
    delay(minutes * 60);
  }
}
