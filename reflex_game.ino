/*
 * Creation Crate Reflex Game
 *
 * This reflex game pits two competitors against each other to see who has the fastest reaction time!
 * Players initiate a game by pushing the Start button. After a three flash countdown, the Start LED lights up 
 * after a random delay.
 *
 * Whoever presses their player button first when the start LED lights up, wins!
 *
 * Use the Arduino serial monitor for more statistics.
 */

// Step1: define inputs/outputs and key variables
int startButton = 8;				// pin assignment for start button
int startLED = 9;					// pin assignment for start LED
int startButtonState = 0;			// startButton sate indicator

int p1Button = 4;					// pin assignment for start button
int p1LED = 5;						// pin assignment for start LED
bool p1ButtonState = LOW;			// p1Button state indicator
bool p1Finished = false;			// p1 finished state indicator

int p2Button = 12;					// pin assignment for start button
int p2LED = 13;						// pin assignment for start LED
bool p2ButtonState = LOW;			// p2Button state indicator
bool p2Finished = false;			// p2 finished state indicator

// Step2: initialize variables so we can use later
int messageStart = 0;
int gameStart = 0;
int currentTime = 0;
int timeDelay = 0;
int startTime = 0;
int p1endTime = 0;
int p2endTime = 0;
bool p1win = false;
bool p2win = false;

// Step3: configure setup
void setup()
{
	// start serial interface (tools > serial monitor).
	Serial.begin(9600);

	// initialize pins
	pinMode(startButton, INPUT);
	pinMode(startLED, OUTPUT);
	pinMode(p1Button, INPUT);
	pinMode(p1LED, OUTPUT);
	pinMode(p2Button, INPUT);
	pinMode(p2LED, OUTPUT);
}

// Step4: main loop
void loop()
{
	if (messageStart == 0) {
		// plays the start message and blinks the start LED to indicate a game has begun.
		Serial.println(" ");
		Serial.println("--------------------------------");
		Serial.println("Press the Start button to begin!");

		// flash the start LED to indicate beginning of gameplay.
		flashLED(startLED, 5, 1);

		// set start message to 1; so that this block is not executed on future loops.
		messageStart = 1;
	}

	startButtonState = digitalRead(startButton);

	// wait until the start button is pushed to start the game!
	if (startButtonState == HIGH && gameStart == 0) {
		startGame(); 
	}
}

// Step5: LED flashing function implementation
void flashLED(int pin, int flashes, int duration) {
	unsigned long delayAmount = duration * 1000 / flashes / 2;
	for (int i = 0; i < flashes; i++) {
		digitalWrite(pin, HIGH);
		delay(delayAmount);

		digitalWrite(pin, LOW);
		delay(delayAmount);
	}
}

// Step6: game start function implementation
void startGame() {
	// note: all times are calculated in milliseconds!
	// delay(500) means 500 milliseconds, or half a second!

	// game has begun
	gameStart = 1; 

	// sets a random interval between t0 and t1 seconds for the reaction delay.
	timeDelay = random(5, 7) * 1000;

	// serial monitor countdown and LED flashing.
	flashLED(startLED, 1, 1);

	Serial.println(" ");
	Serial.println("Fingers ready...");

	flashLED(startLED, 1, 1);
	Serial.println("Set...");

	flashLED(startLED, 1, 1);
	Serial.println("REACT!");
	Serial.println(" ");

	// set the start time.
	startTime = millis();

	// loop that runs until both players have finished.
	while (p1Finished == false || p2Finished == false) {
		p1ButtonState = digitalRead(p1Button);
		p2ButtonState = digitalRead(p2Button);

		// calculate running timer
		currentTime = millis() - startTime;

		// save player 1's information when they push their button.
		if (p1ButtonState == HIGH && p1Finished == false) {
			p1endTime = millis();
			p1Finished = true;

			Serial.println("Player 1 has finished!");
			Serial.println(" ");
		}

		// save player 2's information when they push their button.
		if (p2ButtonState == HIGH && p2Finished == false) {
			p2endTime = millis();
			p2Finished = true;

			Serial.println("Player 2 has finished!");
			Serial.println(" ");
		}

		// turn on the start LED once timeDelay has elapsed!
		if (currentTime > timeDelay) {
			digitalWrite(startLED, HIGH);
		}
	}

	// game ended animation
	flashLED(startLED, 5, 1);

	// calculate reaction times for players
	p1endTime = p1endTime - startTime - timeDelay;
	p2endTime = p2endTime - startTime - timeDelay;

	// punish player1 if they pressed too soon
	if (p1endTime <= 0) {
		Serial.println("Player 1 pressed too soon!");
		Serial.println(" ");
	}

	// punish player2 if they pressed too soon
	if (p2endTime <= 0) {
		Serial.println("Player 2 pressed too soon!");
		Serial.println(" ");
	}

	// print out results to serial monitor
	Serial.println(" ");
	Serial.print("Player 1's reaction time was: ");
	Serial.print(p1endTime);
	Serial.print(" milliseconds.");
	Serial.println(" ");

	Serial.println(" ");
	Serial.print("Player 2's reaction time was: ");
	Serial.print(p2endTime);
	Serial.print(" milliseconds.");
	Serial.println(" ");

	// player 1 wins!
	if (p1endTime < p2endTime && p1endTime > 0) {
		Serial.println(" ");
		Serial.print("Player 1 wins by ");
		Serial.print(p2endTime - p1endTime);
		Serial.print(" milliseconds!");
		Serial.println(" ");
		Serial.println(" ");

		// flash p1LED to indicate that player1 won
		flashLED(p1LED, 5, 1);
	}

	// player 2 wins!
	if (p2endTime < p1endTime && p2endTime > 0) {
		Serial.println(" ");
		Serial.print("Player 2 wins by ");
		Serial.print(p1endTime - p2endTime);
		Serial.print(" milliseconds!");
		Serial.println(" ");
		Serial.println(" ");

		// flash p2LED to indicate that player2 won
		flashLED(p2LED, 5, 1);
	}

	// if its a tie (highly unlikely)
	if (p1endTime == p2endTime) {
		Serial.println("Both players won in a tie!!");
		Serial.println(" ");

		// flash both p1LED and p2LED to indicate a tie
		flashLED(p1LED, 5, 1);
		flashLED(p2LED, 5, 1);
	}

	// reset relevant variables for the next game!
	p1Finished = 0;
	p2Finished = 0;
	messageStart = 0;
	gameStart = 0;
	delay(1000);
}
