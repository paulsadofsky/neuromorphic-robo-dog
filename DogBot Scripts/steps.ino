const int steps = 5; // Number of steps
const int limbs = 4; // Number of limbs

// Walking Steps
int states[steps][limbs] = 
{
  {0, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 1, 0, 0},
  {0, 1, 0, 0},
};

// Setting Counting Variables
int countBR = 0;
int countBL = 0; 
int countFR = 0; 
int countFL = 0;
int stepIndex = 0; // To track the current step

void setup() {
  // Initialization code goes here
}

void loop() {
  // Calculate the current step index
  int currentStep = stepIndex % steps;

  // Updating counts based on the current step
  if (states[currentStep][0] == 1) countBR++;
  if (states[currentStep][1] == 1) countBL++;
  if (states[currentStep][2] == 1) countFR++;
  if (states[currentStep][3] == 1) countFL++;

  Serial.print("Step Index: ");
  Serial.print(stepIndex);
  Serial.print(" - BR: ");
  Serial.print(countBR);
  Serial.print(", BL: ");
  Serial.print(countBL);
  Serial.print(", FR: ");
  Serial.print(countFR);
  Serial.print(", FL: ");
  Serial.println(countFL);

  // Resetting total step counter after reaching 27
  if (countBR == 27 || countBL == 27 || countFR == 27 || countFL == 27) {
    countBR = 0;
    countBL = 0;
    countFR = 0;
    countFL = 0; 
  }

  // Move to the next step
  stepIndex++;
  
  // Add a delay if necessary, for example:
  // delay(1000); // Delay 1 second
}
