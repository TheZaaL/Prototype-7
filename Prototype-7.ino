const int TARGET_COUNT = 2;
const int CALIBRATION_COOLDOWN = 300;
const int AMOUNT_TO_REACH = 20;
const int RECEIVER_REACHED_THRESHOLD = 200;
const int ORIGIN_REACHED_THRESHOLD = 200;

const int irReceiverPins[TARGET_COUNT] = {A1, A2};
const int ledPins[TARGET_COUNT] = {4, 7};
const int led2Pins[TARGET_COUNT] = {5, 8};

const int originIrLed = 2;
const int originIrReceiver = A0;

// Calibration of the receivers, default value of 0
int baseReceiverValues[TARGET_COUNT] = {};
int baseOriginReceiverValue = 0;

int targetReachedCount = 0;
int currentTarget = 0;
bool isTargetOn = false;

unsigned long atOriginTimes[AMOUNT_TO_REACH] = {};
unsigned long atTargetTimes[AMOUNT_TO_REACH] = {};

unsigned long nextTargetTime = 0;
const unsigned long nextTargetCooldown = 3000;

void setup() {
  Serial.begin(9600);

  // Initialize pins
  initPins();

  // Getting ready
  setGameParameters();
  
  calibrateSensors();

  playReadyAnimation();

  // Start game
  setNextTarget();
}

void loop()
{
  if(isTargetOn && isTargetReached()) {
    // Turn origin pins off and turn a random target on
    setTargetReached();
  }

  // else if(!isTargetOn && isOriginReached()){
  else if(!isTargetOn && isOriginReachedSTUB()){
    // Turn target off and turn origin sensors on
    setNextTarget();
  }

  if(targetReachedCount >= AMOUNT_TO_REACH){
    // End the game and display results
    triggerEndGame();
  }
}

void initPins(){
  // Initialize target pins
  for(int i = 0; i < TARGET_COUNT; i++) {
    pinMode(irReceiverPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
    pinMode(led2Pins[i], OUTPUT);

    digitalWrite(ledPins[i], LOW);
    digitalWrite(led2Pins[i], LOW);
  }

  // Initialize origin pins
  pinMode(originIrReceiver, INPUT);
  pinMode(originIrLed, OUTPUT);

  digitalWrite(originIrLed, LOW);
}

void setGameParameters(){
  // Set number of targets to reach, or time limit, etc.
  // TODO
}

void calibrateSensors(){
  // Calibrate origin receiver
  // Turn on LEDS
  digitalWrite(originIrLed, HIGH);
  delay(CALIBRATION_COOLDOWN);

  // Read base value and turn off LEDS
  baseOriginReceiverValue = analogRead(originIrReceiver);
  digitalWrite(originIrLed, LOW);

  // Calibrate targets
  for(int i = 0; i < TARGET_COUNT; i++) {
    // Turn on LEDS
    digitalWrite(ledPins[i], HIGH);
    digitalWrite(led2Pins[i], HIGH);
    delay(CALIBRATION_COOLDOWN);

    // Read base value and turn off LEDS
    baseReceiverValues[i] = analogRead(irReceiverPins[i]);
    digitalWrite(ledPins[i], LOW);
    digitalWrite(led2Pins[i], LOW);
  }
}

void playReadyAnimation(){
  // Have fun! (something like 3-2-1-GO)
  // TODO
}

bool isOriginReachedSTUB(){
  delay(500);
  return true;
}

bool isOriginReached(){
  // Read the receiver value
  int receiverValue = analogRead(originIrReceiver);

  // Ajust value with calibration
  receiverValue = receiverValue - baseOriginReceiverValue;

  // If the calibrated value is lower than the treshold, player has reached the origin
  return (receiverValue < ORIGIN_REACHED_THRESHOLD);
}

bool isTargetReached(){
  // Read the receiver value
  int receiverValue = analogRead(irReceiverPins[currentTarget]);

  // Ajust value with calibration
  receiverValue = receiverValue - baseReceiverValues[currentTarget];

  // If the calibrated value is higher than the treshold, player has reached the target
  return (receiverValue > RECEIVER_REACHED_THRESHOLD);
}

void setNextTarget(){
  // Save current time
  atOriginTimes[targetReachedCount] = millis();

  // Turn off origin LED
  digitalWrite(originIrLed, LOW);

  // Choose next target
  currentTarget = random(0, TARGET_COUNT);

  // Turn on target LEDS
  digitalWrite(ledPins[currentTarget], HIGH);
  digitalWrite(led2Pins[currentTarget], HIGH);

  // Target is on!
  isTargetOn = true;
}

void setTargetReached(){
  // Save current time
  atTargetTimes[targetReachedCount] = millis();

  // Turn off target LEDS
  digitalWrite(ledPins[currentTarget], LOW);
  digitalWrite(led2Pins[currentTarget], LOW);

  // Turn on origin LED
  digitalWrite(originIrLed, HIGH);

  // Increment target reached count
  targetReachedCount++;

  // Target is off!
  isTargetOn = false;
}

void triggerEndGame(){
  // Stop everything, show stats, etc.
  // TODO
}

