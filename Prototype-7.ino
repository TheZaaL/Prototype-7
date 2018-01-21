const int TARGET_COUNT = 2;

const int trigPins[TARGET_COUNT] = {2, 3};
const int ledPins[TARGET_COUNT] = {4, 5};
const int echoPins[TARGET_COUNT] = {6, 7};

const int distance = 20;
const int valcompteurmax = 3;

int currentTarget = -1;
int compteur = 0;

unsigned long nextTargetTime = 0;
const unsigned long nextTargetCooldown = 3000;

void setup() {
  Serial.begin(9600);

  // Initialize pins
  for(int i = 0; i < TARGET_COUNT; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);

    digitalWrite(trigPins[i], LOW);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop()
{
  // If no target is turned on, check if we need to turn one on
  if(currentTarget < 0) {
    trySetNextTarget();
  }

  else if(isCurrentTargetReached())
  {
    // Turn led off and wait for user to return to center position
    digitalWrite(ledPins[currentTarget], LOW);
    currentTarget = -1;
    nextTargetTime = millis() + nextTargetCooldown;
    
    Serial.println("Target reached");
  }

  delay(100);
}

void trySetNextTarget(){
  if (millis() > nextTargetTime) {
    // Turn on next target
    currentTarget = random(0, TARGET_COUNT);
    digitalWrite(ledPins[currentTarget], HIGH);
  }
}

bool isCurrentTargetReached(){
  
  long duration, cm;

  // Measure hand distance
  digitalWrite(trigPins[currentTarget], LOW);
  delayMicroseconds(2);
  digitalWrite(trigPins[currentTarget], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPins[currentTarget], LOW);
  
  duration = pulseIn(echoPins[currentTarget], HIGH);
  cm = microsecondsToCentimeters(duration);
  
  Serial.print(cm);
  Serial.println("cm ");
  
  // If hand not in range, reset counter
  if (cm > distance) {
    compteur = 0;
    Serial.println(compteur);

    return false;
  }
  
  // If hand in range, increase counter if it has not attained the max value
  else if (compteur < valcompteurmax) {
    compteur++;
    Serial.println(compteur);

    return false;
  }

  // At this point, we have the max counter value: user has reached
  // the target and we wait for user to get back to center
  else {
    return true;
  }
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
