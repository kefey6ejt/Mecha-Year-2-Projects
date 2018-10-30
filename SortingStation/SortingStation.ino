// 2, 3, 18, 19, 20, 21  = Interrupt Pin
//Inputs
int metalSensor = 2;
int sortSensor = 3; //IR sensor
int queueCounter = 0;

//OUTPUTS
int belt_1 = 5;
int belt_2 = 6;
int sortSolenoid = 4;

//boolean control and timing interval
bool flag_pass = false;
bool flag_counter = true;
int fp_interval = 2500;
int extendPeriod = 1000;

unsigned long currentTime;
unsigned long fp_latchedTime = 0;
unsigned long componentDetect_time = 0;

void setup() {
  Serial.begin(9600);
  pinMode(metalSensor, INPUT_PULLUP);
  pinMode(sortSensor, INPUT_PULLUP);
  pinMode(sortSolenoid, OUTPUT);
  pinMode(belt_1 , OUTPUT);
  pinMode(belt_2 , OUTPUT);
  attachInterrupt(digitalPinToInterrupt(metalSensor), metalDetected, RISING); //Read when it goes from LOW to HIGH
  attachInterrupt(digitalPinToInterrupt(sortSensor), componentDetected_1st, HIGH);

  digitalWrite(belt_1, HIGH);
  digitalWrite(belt_2, HIGH);
}

void loop() {
  Serial.println(digitalRead(sortSensor));
  currentTime = millis();
  if ( (currentTime - fp_latchedTime) > fp_interval) {
    flag_pass = false;
  }

  if ((currentTime - componentDetect_time) > extendPeriod) {
    digitalWrite( sortSolenoid, LOW);
    flag_counter = true;
  }
}

void metalDetected() {
  flag_pass = true;
  fp_latchedTime = currentTime;
  //  Serial.println("Metal");
}

void componentDetected_1st () {
  if (!flag_pass && queueCounter <=5) {
    //    Serial.println("not flagged and sensed component");
    digitalWrite(sortSolenoid, HIGH);
    componentDetect_time = currentTime;
    if (flag_counter) {
      queueCounter++;
      flag_counter = false;
    }
  }
}
