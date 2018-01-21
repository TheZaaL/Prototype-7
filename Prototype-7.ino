const int trigPin = 2;
const int ledPin = 3;
const int echoPin = 4;
const int distance = 40;
const int valcompteurmax = 3;

int saut_appel = 0;
int compteur = 0;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  long duration, cm;

  // Determine hand distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  
  cm = microsecondsToCentimeters(duration);
  
  Serial.print(cm);
  Serial.println("cm ");
  
  // If hand not in range, reset counter
  if (cm > distance) {
    digitalWrite(ledPin, HIGH);
    compteur = 0;
    Serial.println(compteur);
    delay(100);
  }
  
  // If hand in range, increase counter if it has not attained the max value
  else if (compteur < valcompteurmax) {
    compteur++;
    Serial.println(compteur);
    delay(100);
  }

  // At this point, we have the max counter value: user has reached
  // the target and we wait for user to get back to center
  else {
    digitalWrite(ledPin, LOW);
    saut_appel = 1;
    Serial.println(saut_appel);
    delay(5000);
    saut_appel = 0;
  }
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
