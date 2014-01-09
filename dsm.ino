/*

  DigitalReadSerial

 Reads a digital input on pin 2, prints the result to the serial monitor 

 

 This example code is in the public domain.

 */



// digital pin 2 has a pushbutton attached to it. Give it a name:

int pushButton = 2;

unsigned long ch1;

const int time = 6;

unsigned long sum = 0;

int count = 0;



// the setup routine runs once when you press reset:

void setup() {

  // initialize serial communication at 9600 bits per second:

  Serial.begin(9600);

  //pinMode(3, INPUT); // make the pushbutton's pin an input:

  pinMode(pushButton, INPUT);

}



// the loop routine runs over and over again forever:

void loop() {

  unsigned long time1 = millis();

  // read the input pin:

  ch1 = pulseIn(pushButton, LOW);

  while((millis()-time1)<time*1000){

    ch1+=pulseIn(pushButton, LOW);

  }
  
  count++;

  //int buttonState = digitalRead(pushButton);

  // print out the state of the button:

  float lowRatio = (ch1/10000)/time;

  unsigned long pm25 = lowRatio*80;
  
  sum = pm25 + sum;

  
    Serial.print("PM 2.5: ");
    Serial.print(sum / count);
    Serial.println(" ug/m3.");
    Serial.print("                   Debug infor: Time: ");
    Serial.print(time * count);
    Serial.print("s,");

    Serial.print(" Low ratio - ");
    Serial.print(lowRatio);
    Serial.print("%");
    
    Serial.print(" Instantaneous data- ");
    Serial.println(pm25);
  

}






