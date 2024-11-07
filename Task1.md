# Task 1
// Define pins for the ultrasonic sensor
const int trigPin = 11;
const int echoPin = 10;

// Define pins for LEDs
const int greenLED = 7;
const int yellowLED = 6;
const int redLED = 5;

// Define pin for potentiometer (now using analog pin A4)
const int potentiometerPin = A4;

// Initial distance thresholds
int upperThreshold = 20;
int lowerThreshold = 10;

void setup() {
  // Set up serial communication
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {
  // Get distance from ultrasonic sensor
  long distance = getDistance();

  // Adjust thresholds based on potentiometer position
  adjustThresholds();

  // Display distance and thresholds for debugging
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Lower Threshold: ");
  Serial.print(lowerThreshold);
  Serial.print(" cm, Upper Threshold: ");
  Serial.println(upperThreshold);

  // Control LEDs based on distance
  if (distance > upperThreshold) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
  } else if (distance > lowerThreshold && distance < upperThreshold) {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
  } else if(distance < lowerThreshold){
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);
  }

  // Small delay to stabilize readings
  delay(100);
}

// Function to get distance from the ultrasonic sensor
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

// Function to adjust thresholds based on potentiometer input
void adjustThresholds() {
  int potValue = analogRead(potentiometerPin); // Read potentiometer value (0 to 1023)

  // Map the potentiometer value to a threshold range
  lowerThreshold = map(potValue, 0, 1023, 5, 15);   // Lower threshold between 5 and 15 cm
  upperThreshold = map(potValue, 0, 1023, 20, 50);  // Upper threshold between 20 and 50 cm

  // Ensure upperThreshold is always greater than lowerThreshold by at least 5 cm
  if (upperThreshold <= lowerThreshold + 5) {
    upperThreshold = lowerThreshold + 5;
  }
}
