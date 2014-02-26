/*

 Using DSM501 to retrieve the PM 2.5 data.
 
 */


int DSM_PM25_PIN = 3;
unsigned long sum = 0;
// initialize the count to -2 to pass the first 1 minute data
int count = 0;
int countl = 1;
// caculate the instantaneous pm 2.5 each 30s
const int interval = 30 * 1000;
int averageSixHours;
int averageQuarter;
boolean quarterFinish = false;

boolean DEBUG = true;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // make the pushbutton's pin an input:
  pinMode(DSM_PM25_PIN, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  count++;

  int instantData = caculatePm25(readSensorPm25(DSM_PM25_PIN));
  caculateAverageQuarter(instantData, count);
  if (quarterFinish) {
    countl++;
  }
    caculateAverage6Hours(averageQuarter);

  
    
  outputToSM(instantData);
}

unsigned long readSensorPm25(int pin) {
   unsigned long lowPulse = pulseIn(DSM_PM25_PIN, LOW);
   unsigned long startTime = millis();
   
   while(( millis() - startTime ) < interval){
    lowPulse += pulseIn(DSM_PM25_PIN, LOW);
   }
   return lowPulse;
}

unsigned long caculatePm25(unsigned long lowPulse) {
  float lowRatio =  (float)lowPulse / 10 / interval;

  int instant = lowRatio * 100;
  return instant;
}

void caculateAverageQuarter(int instant, int count) {  
  if (count <= 0){
    sum = 0;
    quarterFinish = false;
  } else if (count <= 30){
    sum += instant;
    quarterFinish = false;
    averageQuarter = sum / count;

  } else {
    count = 0;
    sum = 0;
    quarterFinish = true;
  }
}

int caculateAverage6Hours(int quarterVal) {
  int average[24];
  int j = countl % 24;
  average[j - 1] = quarterVal;
  int sumHours = 0;
  for (int i = 0; i < j; i++) {
    sumHours += average[i];
  }
  averageSixHours = (sumHours / j);
}

void outputToSM(int currentVal) {
  if (count <= 0){
      Serial.println("Initializing...");
      sum = 0;
  } else{
  
    Serial.print("PM 2.5 Quarter: ");
    Serial.print(averageQuarter);
    Serial.print(" ug/m3.");
    Serial.print("PM 2.5 six hours: ");
    Serial.print(averageSixHours);
    Serial.print(" ug/m3.");
    if (DEBUG) {
      Serial.print("                   Debug infor: Time - ");
      if (count == 1){
        Serial.print("30s,");
      } else if (count <= 120){
        Serial.print(count / 2);
        Serial.print("m");
        if (count % 2 == 0) {
        } else {
          Serial.print("30s");
        }
      } else {
        Serial.print(count / 120);
        Serial.print("h");
        Serial.print((count % 120) / 2);
        Serial.print("m");
      }
      
      Serial.print(" Instantaneous data - ");
      Serial.println(currentVal);
    } else {
      Serial.println("");

    }
  } 
}






