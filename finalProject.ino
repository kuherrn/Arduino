// Name: Kuherrn Nagayaindran
// Date: 16th August 2021
// README: 5-bit Binary Number Guesser Game

#include <LiquidCrystal.h> // Importing the LCD Library
#include <IRremote.h> // Importing the IR Remote Library

LiquidCrystal lcd( 12,11,5,4,3,2); // These pin numbers are hard coded in on the serial backpack board.
int RGBRedPin1 = 6; // RGBRedPin1 is connected to pin 6 of the Arduino
int RGBRedPin2 = 7; // RGBRedPin1 is connected to pin 7 of the Arduino
int RGBRedPin3 = 8; // RGBRedPin1 is connected to pin 8 of the Arduino
int RGBRedPin4 = 9; // RGBRedPin1 is connected to pin 9 of the Arduino
int RGBRedPin5 = 13; // RGBRedPin1 is connected to pin 13 of the Arduino

// Array of LEDs corresponding to their position on the board from left to right
int LEDs[5] = {RGBRedPin1, RGBRedPin2, RGBRedPin3, RGBRedPin4, RGBRedPin5};

int i; // First for loop counter variable
int j; // Second for loop counter variable
int funcCounter = 0; // 'funcCounter' variable to keep track of the number of the inputs 
int randNumber = 0; // 'randNumber' variable to store the random number generated
int col = 0; // 'col' variable to denote the col position of the LCD cursor
int row = 0; // 'row' variable to denote the row position of the LCD cursor 
int userNumber = 0; // 'userNumber' variable to store the user inputted number

int value = 0; // Stores the value from the IR Remote
int RECV_PIN = 10; // The pin where you connect the output pin of IR sensor
IRrecv irrecv(RECV_PIN); // Instantiate an instance of the IRRecv class
decode_results results; // Initialize a 'results' variable

// Array of integers whereby the index value corresponds to the digits on the remote & the element value correponds to the value stored in the 'value' variable
// button[10] == Play/Pause button
int button[11] = {12495, 2295, -30601, 18615, 10455, -22441, 26775, 6375, -26521, 22695, -24481};
// Array of int 'num'. Stores the value from the IR Remote
int numArray[2]; 


void setup()
{
  lcd.begin (16,2); // Initialize the LCD.
  irrecv.enableIRIn(); // Enable the IR receiver to be read
  pinMode(RGBRedPin1, OUTPUT); // Setup the RGBRedPin1 pin as an output pin
  pinMode(RGBRedPin2, OUTPUT); // Setup the RGBRedPin2 pin as an output pin
  pinMode(RGBRedPin3, OUTPUT); // Setup the RGBRedPin3 pin as an output pin
  pinMode(RGBRedPin4, OUTPUT); // Setup the RGBRedPin4 pin as an output pin
  pinMode(RGBRedPin5, OUTPUT); // Setup the RGBRedPin5 pin as an output pin
}

// Function to take the 'randNumber', convert it into its corresponding binary number array, and using said array, turn the LED on/off
// Accepts the 'randNumber' and 'LEDs' array as parameters
void lightsOn(int num, int array[]) {
  int A[5];
  int B[5];
  A[4] = num;
 
  for (i = 3; i >= 0; i--) {
		A[i] = A[i+1] / 2;      	
    }
  
  for (i = 0; i < 5; i++) {
    	B[i] = A[i] % 2;
    }
  
  for (i = 0; i < 5; i++) {
    if (B[i] == 1) {
      digitalWrite(array[i], HIGH);
    }else {
      digitalWrite(array[i], LOW);
    }
  }
}


// Function to display the user inputted number on the LCD screen. Maximum allowed inputs are 2
// Accepts the 'numArray' as a parameter
void display(int array[]) {
  int counter2 = 0;
  for (i = 1; i >= 0; i--) {
    if (array[i] <= 9) {
      lcd.setCursor(col + 15 - counter2, row + 1);
      lcd.print(array[i]);
      counter2++;
    }
  }
  
  while(true) {
	if (irrecv.decode(&results)) {
      value = (results.value);
      irrecv.resume();
      if (value == button[10]) {
        return; 
      }
    }
  }
}

// Function that converts the 'numArray' into a 'userNumber' integer
// Acceprts the 'numArray' as a parameter
void convert(int Array[]) {
  for (i = 0; i < 2; i++) {
    if (numArray[i] <= 9 && i == 0) {
	  userNumber += Array[i] * 10;
    }else if (numArray[i] <= 9) {
      userNumber += Array[i]; 
    }else {
      continue; 
    }
  }
}

// Function that prints the error message to the LCD screen if 'userNumber' > 31
void error() {
  lcd.clear();
  lcd.setCursor(col + 0, row + 0);
  lcd.print("Number");
  lcd.setCursor(col + 0, row + 1);
  lcd.print("too high!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(col, row);
  lcd.print("Guess the number");
}

// Function that converts the 'value' read from the IR Remote into its numeric keypad value
// Accepts the 'numArray' and 'convert' func as a parameter
void mainFunc(int arrayFunc[], void (*convertingFunc)(int*)) {
  
  if (irrecv.decode(&results)) {
    value = (results.value);
    irrecv.resume();
    for (j = 0; j < 11; j++) {
      if (value == button[10] && funcCounter == 1) {
        arrayFunc[1] = arrayFunc[0];
        arrayFunc[0] = 888;        
        convertingFunc(arrayFunc);
        value = 0;
      } else if (value == button[j]) {
        numArray[i] = j;
        lcd.setCursor(col + 15 - funcCounter, row + 1);
        lcd.print(j);
        funcCounter++;
      }
    }
  }
}

// Main function that accepts the rest of the functions as parameters
// Iterates once or twice (depending on the buttons pressed on the IR Remote), converting the user input into an integer value stores in 'userNumber'
// Performs error handling based on the 'userNumber'. Prints an error message to the LCD screen if appropriate
void userInput (void (*receiveFunc)(int*, void(*)(int*)), void (*convertFunc)(int*), 
                void (*errorMessage)(), int randArray[], void(*displayFunc)(int*)) {
  
  while (userNumber < 31) {
    funcCounter = 0;
    lcd.setCursor(col + 15, row + 1);
    lcd.print(" ");
    lcd.setCursor(col + 14, row + 1);
    lcd.print(" ");

    for (i = 0; i < 2; i++) {
      while (funcCounter == i) {
        receiveFunc(randArray, convertFunc);
        if (numArray[0] == 888) {
          numArray[0] = 0;
          return;
        } 
      }
    }
    displayFunc(randArray);
    convertFunc(randArray);
    
    if (userNumber > 31) {
      errorMessage();
      userNumber = 0;
    } else {
      return; 
    }
  }  
}

// Main loop function                          
void loop(){
  int correct = 0; // 'correct' variable to store the number of times the user's input matches the randomly generated num variable
  
  // Prints the starting message
  while (true) {
    lcd.setCursor(col, row); // Set cursor at (col[0], row[0]) 
    lcd.print("Press Play"); // Print at cursor location
    lcd.setCursor(col, row + 1); // Set cursor at (col[0], row[1])
    lcd.print("To get started"); // Print at cursor location
    if (irrecv.decode(&results)) { // If IR remote is pressed
      value = (results.value); // Save the results value into type the 'value' int
      irrecv.resume(); // Clear the 'results' address buffer
      if (value == button[10]) { // If Play/Pause button is pressed
        lcd.clear(); // Clear the LCD screen
        lcd.setCursor(col, row); // Set cursor at (col[0], row[0]) 
        lcd.print("Guess the number"); // Print at cursor location
        break; // Break from the while loop
      }
    }
  }
  
  // Loop through 5 times, which is the number of times the user is going to play for each round
  for (int n = 0; n < 5; n++) {
    // Invoke the random function and save the value to the 'randNumber' variable
    randNumber = random(1, 32);
    // Invoke the 'lightsOn' function, passsing the 'randNumber' int & 'LEDs' array as params
    lightsOn(randNumber, LEDs);
    // Invoke the 'userInput' function passing the 'mainFunc', 'convert', 'display' & 'error' functions and 'numArray' array as params
    userInput(mainFunc, convert, error, numArray, display);
    // If the 'randNumber' == 'userNumber' increment the 'correct' counter by 1
    if (randNumber == userNumber) {
      correct++;
    } 
    userNumber = 0;
  }
  
  // Prints the score to the LCD screen
  lcd.clear();
  lcd.setCursor(col + 0, row + 0);
  lcd.print("Your Score:");
  lcd.setCursor(col + 0, row + 1);
  lcd.print(correct);
  lcd.setCursor(col + 7, row + 1);
  lcd.print("/");
  lcd.setCursor(col + 15, row + 1);
  lcd.print("5");
  delay(5000);
  lcd.clear();
}



  

  
  
  
  
  

  