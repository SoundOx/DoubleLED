int diodePin = A0;
int sensorValue = 0;
int sensorVoltage = 0;
int SV1 = 0;
int SV2 = 0;
int avgR[10] = {0,0,0,0,0,0,0,0,0,0};
int avgTotalR = 0;
int div10R = 0;
int avgIR[10] = {0,0,0,0,0,0,0,0,0,0};
int avgTotalIR = 0;
int div10IR = 0;
int i = 0;
int j = 0;
int IRRead = 0;
int RRead = 0;
boolean IRON;
boolean prev;
volatile int count = 0;

void setup() 
{
  Serial.begin(57600);
  //9 & 10 AS OUTPUTS
  DDRB |= (1<<PB1)|(1<<PB2);

  TCCR1B = 0x00;
  TCCR1A = 0X00;
  digitalWrite(10,HIGH);
  digitalWrite(9,LOW);
  IRON = false;
  prev = false;
  TCCR1B |= (1 << WGM12);
  TIMSK1 |= (1 << OCIE1A);

  //.008S * 16MHz / 64
  OCR1A = 1000;
  //OCR1A = 200;
  //OCR1A = 4000;
  
  // .33s * 16MHz / 256
  //OCR1A = 20625;
  //OCR1A = 62500;

  //USE PRESCALAR 64
  TCCR1B |= (1 << CS11)|(1 << CS10);
  //USE PRESCALAR 256
  //TCCR1B |= (1 << CS12);
  //TCCR1B &= ~((1<<CS11)|(1<<CS10));
  //SET BOTH OC1A AND OC1B (9, 10) TO TOGGLE ON COMPARE MATCH
  //TCCR1A = 0b01010000;
  sei();


}

void loop() 
{
  //if(prev^IRON){
  //  delay(300);
  //}
  prev = IRON;
  if(!IRON){
    sensorValue = analogRead(A0);
    sensorVoltage = sensorValue*(5000/1024);
    avgTotalR += sensorVoltage;
    avgTotalR -= avgR[i];
    div10R = avgTotalR/10;
    avgR[i] = sensorVoltage;
    i++;
    if(i==10){
      i=0;
    }
    RRead = sensorVoltage;
    //Serial.println(sensorVoltage);
    //Serial.print(" ");
    //Serial.println(0);
  }
  else{
    SV1 = analogRead(A1);
    SV2 = SV1*(5000/1024);
    avgTotalIR += SV2;
    avgTotalIR -= avgIR[j];
    div10IR = avgTotalIR/10;
    avgIR[j] = SV2;
    j++;
    if(j==10){
      j=0;
    }
    IRRead = SV2;

    /*
    Serial.print(0);
    Serial.print(" ");
    Serial.println(div10IR);
    */
  }
  
  //Serial.println(div10R);
  //Serial.print(" ");
  Serial.println(div10IR);
  
  /*
  Serial.print(IRRead);
  Serial.print(" ");
  Serial.println(RRead);
  */
  delay(10);
}


ISR(TIMER1_COMPA_vect){
  PINB |= (1<<PB1)|(1<<PB2);
  IRON ^= true;
}
