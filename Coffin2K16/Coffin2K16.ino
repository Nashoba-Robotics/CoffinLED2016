
#define PIN_HOOD_BOTTOM 22
#define PIN_HOOD_TOP 23
#define PIN_HOOD_AT_THRESHOLD 24

#define PIN_INTAKE_TOP_STOP 25
#define PIN_INTAKE_TOP 26
#define PIN_INTAKE_TOP_INTAKE 27
#define PIN_INTAKE_INTAKE_HOME 28
#define PIN_INTAKE_HOME 29
#define PIN_INTAKE_HOME_BOTTOM 30
#define PIN_INTAKE_BOTTOM 31
#define PIN_INTAKE_BOTTOM_STOP 32

#define PIN_PHOTO_1
#define PIN_PHOTO_2
#define PIN_PHOTO_3

#define PIN_ELEVATOR_BOTTOM_HEIGHT
#define PIN_ELEVATOR_TOP_HEIGHT
#define PIN_MOTOR_RUNNING


void setup() 
{
    Serial.begin(9600);
    Serial.println("Waiting for Commands");
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    String inputString = Serial.readStringUntil('\n');
    
  }
}
