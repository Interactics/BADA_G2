#include "EveryTimerB.h".

const byte  RIGHT_ENC_CHA = 12;
const byte  RIGHT_ENC_CHB = 11;

void EncoderA_CB();
void printTest();

int encoder_a = 0;

int i = 0;
int j = 10;
bool p = false;
void setup() {
  Serial.begin(115200);

  TimerB2.initialize();
  TimerB2.attachInterrupt(printTest);
  TimerB2.setPeriod(10000);

  pinMode(RIGHT_ENC_CHA, INPUT_PULLUP);
  pinMode(RIGHT_ENC_CHB, INPUT_PULLUP);

  attachInterrupt(0, EncoderA_CB, CHANGE);

}
void loop() {
  //Serial.println(encoder_a);     // "78"
  int switchValue = digitalRead(RIGHT_ENC_CHA);
  if (switchValue == LOW)
    digitalWrite(13, HIGH);
  if ( p) {
    Serial.println(i);
    p = false;
  };

}
void printTest() {
  i++;
  if (i % 10 == 0)  p = true;

}
void EncoderA_CB() {
  if (digitalRead(RIGHT_ENC_CHA) == digitalRead(RIGHT_ENC_CHB))
    encoder_a++;
  else
    encoder_a--;
}
