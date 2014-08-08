
int rightMotor = 6;         //solded
int leftMotor = 5;          //solded
int rightA = 2;             //orange
int leftA = 3;              //yellow
int rightB = 10;            //brown
int leftB = 11;             //grey
int rightReverse = 4;       //green
int leftReverse = 8;        //green

int currentRight = A0;      //white
int currentLeft = A1;       //blue

int currLeftValue = 0;
int currRightValue = 0;
int r_val = 0;
int l_val = 0;

volatile long r_count = 0;
volatile long l_count = 0;
volatile int count_flag = 0;

long r_count_old = 0;
long l_count_old = 0;
long newtime = 0;
long oldtime = 0;
long r_vel;
long l_vel;

long difftime=0;
long diffcount=0;

byte byteRead;
int r_dir = 0;
int l_dir = 0;
int r_pwm = 0;
int l_pwm = 0;

void setup() {
  pinMode(rightA, INPUT);
  pinMode(rightB, INPUT);
  pinMode(leftA, INPUT);
  pinMode(leftB, INPUT);
  pinMode(rightMotor, OUTPUT);  
  pinMode(leftMotor, OUTPUT);  
  pinMode(rightReverse, OUTPUT);
  pinMode(leftReverse, OUTPUT);
  attachInterrupt(0, r_isr, CHANGE);
  attachInterrupt(1, l_isr, CHANGE);
  Serial.begin(57600); 
}

void loop() {

  if (Serial.available()) {
    
    byteRead = Serial.read();
    if (byteRead == 127)
    {
      l_dir = Serial.read();
      l_pwm = Serial.read();  //reference input speed
      r_dir = Serial.read();
      r_pwm = Serial.read();  //reference input speed
    }
    
    digitalWrite(leftReverse, l_dir);
    digitalWrite(rightReverse, r_dir);
    delay(125);
    
    
    
    analogWrite(leftMotor, l_pwm);
    analogWrite(rightMotor, r_pwm);
    /*currRightValue = analogRead(currentRight);
    currRightValue = map(currRightValue, 0, 1023, 0, 255);
    delay(1);
    currLeftValue = analogRead(currentLeft);
    currLeftValue = map(currLeftValue, 0, 1023, 0, 255);*/
    //Serial.write(byteRead);
  }
  
  newtime = micros();
  l_vel = (l_count-l_count_old) * 1000000 /(newtime-oldtime);
  r_vel = (r_count-r_count_old) * 1000000 /(newtime-oldtime);
  diffcount = r_count-r_count_old;
  difftime = newtime-oldtime;
  oldtime = newtime;
  r_count_old = r_count;
  l_count_old = l_count;
  
  // PROTOCOL
  // 127-255-RCNT_MSB-RCNT_LSB-LCNT_MSB-LCNT_LSB-RVEL_MSB-RVEL_LSB-LVEL_MSB-LVEL_LSB
  
  Serial.write(127); // start byte
  Serial.write(255);
  //Serial.write(outputPitch); 
  //Serial.write(outputRoll);
  outputBytes(r_count); 
  outputBytes(l_count);  
  outputBytes(r_vel); 
  outputBytes(l_vel);  
  Serial.write(currRightValue);
  Serial.write(currLeftValue);
  //Serial.println(difftime);
  //Serial.println(diffcount);
 
  delay(250);                     
}

void r_isr()
{
  r_val = digitalRead(rightA);
  if (digitalRead(rightB) == r_val) 
    r_count = r_count + 1;
  else
    r_count = r_count - 1;
}

void l_isr()
{
  l_val = digitalRead(leftA);
  if (digitalRead(leftB) == l_val) 
    l_count = l_count - 1;
  else
    l_count = l_count + 1;
}

void outputBytes(int value){
  int MSB;
  int LSB;

  MSB = value >> 8;
  LSB = value & 0b0000000011111111;

  Serial.write(MSB);
  Serial.write(LSB);
}
