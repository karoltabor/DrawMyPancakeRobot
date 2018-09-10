#pragma config(Sensor, S1,     pressHome,      sensorEV3_Touch)
#pragma config(Sensor, S2,     xHome,          sensorEV3_Touch)
#pragma config(Sensor, S3,     yHome,          sensorEV3_Touch)
#pragma config(Motor,  motorA,          xMotor,        tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          yMotor,        tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          pressMotor,    tmotorEV3_Large, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "EV3Mailbox.c"

/********************************************************
*   Set Global Constants and Variables
********************************************************/
task monitorPlotter();
void drawLego();
void calibrateX();
void calibrateY();
void pressBottle(bool enabled);
void moveLinear(int amountToMoveX, int amountToMoveY);


bool penPosition = false;
int xPosition, xDegrees;
int yPosition, yDegrees;


/*******************************************************
*   Monitor Plotters Variables	&amp; Display on LCD
********************************************************/
task monitorPlotter()
{
	while (true)
	{
		wait1Msec(100);
		displayBigTextLine(3, "X pos: <%d>", nMotorEncoder[xMotor]);
		displayBigTextLine(4, "Y pos: <%d>", nMotorEncoder[yMotor]);
		displayBigTextLine(5, "Press pos: <%d>;", nMotorEncoder[pressMotor]);
		if(SensorValue(pressHome) == 1){
			displayBigTextLine(2, "Press is on, <%d>", SensorValue(pressHome));}
		else {
			displayBigTextLine(2, "Press is off, <%d>", SensorValue(pressHome));
		}
	}

}



/********************************************************
*   Calibration X - Axis
********************************************************/
void calibrateX(){
	while(SensorValue(xHome) == 0){
		motor[xMotor] = -50;
	}
	motor[xMotor] = 0;
	nMotorEncoder[xMotor] = 0;        // Reste the X-Axis Motor to 0�
	xPosition = 0; xDegrees = 0;
}



/********************************************************
*   Calibration Y - Axis
********************************************************/
void calibrateY(){
	while(SensorValue(yHome) == 0){
		motor[yMotor] = -50;
	}
	motor[yMotor] = 0;
	nMotorEncoder[yMotor] = 0;        // Reset the Y-Axis Motor to 0�
	yPosition = 0; yDegrees = 0;
}



/********************************************************
*  Press Bottle
********************************************************/
void pressBottle(bool enabled)   //True = pen down, write; False = pen up
{
	/*if (enabled == false) {
		displayBigTextLine(1, "Press on:  &lt; %d&gt;", nMotorEncoder[pressMotor]);
		while(SensorValue(pressHome) == 0){motor[pressHome] = -10;}      // Raise the Pen
		motor[pressMotor] = 0;
		nMotorEncoder[pressMotor] = 0;
		//    penPosition = false;
	}

	if (enabled == true) {
		displayBigTextLine(1, "Press off: &lt; %d&gt;", nMotorEncoder[pressMotor]);
		while(nMotorEncoder[pressMotor] < 250){motor[pressMotor] = 10;}   // Lower the Pen by 250� to Draw
		motor[pressMotor] = 0;
		penPosition = true;
	}*/
}



/********************************************************
*   Movement functions
********************************************************/
void moveLinear(int amountToMoveX, int amountToMoveY){
	xPosition = xPosition+amountToMoveX;
	yPosition = yPosition+amountToMoveY;
	if(xPosition <= 1400 && yPosition <= 1400){
		xDegrees = 5*xPosition; // xDegrees = 5x Current xPosition;
		yDegrees = 5*yPosition; // yDegrees = 5x Current yPosition;
		if(xDegrees > nMotorEncoder[yMotor]){
			motor[yMotor] = 20/abs(amountToMoveY/amountToMoveX)+1;
		}
		else{
			motor[yMotor] = -20/abs(amountToMoveY/amountToMoveX)+1;
		}
		if(yDegrees > nMotorEncoder[motorA]){
			motor[xMotor] = 20/abs(amountToMoveX/amountToMoveY)+1;
		}
		else{
			motor[xMotor] = -20/abs(amountToMoveX/amountToMoveY)+1;
		}
		while(xDegrees != 1 || yDegrees != 1){
			if(nMotorEncoder[yMotor] > xDegrees-3 && nMotorEncoder[yMotor] < xDegrees+3){
				motor[yMotor] = 0;
				xDegrees = 1;
			}
			if(nMotorEncoder[xMotor] > yDegrees-2 && nMotorEncoder[xMotor] < yDegrees+2){
				motor[xMotor] = 0;
				yDegrees = 1;
			}
		}
	}
}



/********************************************************
*   Draw Squares
********************************************************/
void drawSquares(){
	moveLinear(20,20);
	pressBottle(true);
	moveLinear(50,0);
	moveLinear(0,50);
	moveLinear(-50,0);
	moveLinear(0,-57);
	moveLinear(50,50);
	pressBottle(false);

	moveLinear(20,20);
	pressBottle(true);
	moveLinear(70,0);
	moveLinear(0,70);
	moveLinear(-70,0);
	moveLinear(0,-77);
	pressBottle(false);

	pressBottle(false);
	moveLinear(20,20);
	pressBottle(true);
	moveLinear(100,0);
	moveLinear(0,100);
	moveLinear(-100,0);
	moveLinear(0,-107);
	pressBottle(false);

	pressBottle(false);
	moveLinear(-50,150);

	wait1Msec(10000);
}


/********************************************************
*   Write the word "LEGO!"
********************************************************/
void WriteLEGO(){
	pressBottle(false);
	moveLinear(10,10);
	// Letter 'L'
	pressBottle(true);
	moveLinear(10,0);
	moveLinear(0,40);
	moveLinear(30,0);
	moveLinear(0,10);
	moveLinear(-40,0);
	moveLinear(0,-55);
	pressBottle(false);

	moveLinear(60,0);

	// Letter 'E'
	pressBottle(true);
	moveLinear(40,0);
	moveLinear(0,15);
	moveLinear(-30,0);
	moveLinear(0,10);
	moveLinear(20,0);
	moveLinear(0,10);
	moveLinear(-20,0);
	moveLinear(0,5);
	moveLinear(30,0);
	moveLinear(0,10);
	moveLinear(-40,0);
	moveLinear(0,-50);
	pressBottle(false);

	moveLinear(60,0);

	// Letter 'G'
	pressBottle(true);
	moveLinear(40,0);
	moveLinear(0,15);
	moveLinear(-30,0);
	moveLinear(0,30);
	moveLinear(20,0);
	moveLinear(0,-15);
	moveLinear(-10,0);
	moveLinear(0,-10);
	moveLinear(20,0);
	moveLinear(0,30);
	moveLinear(-40,0);
	moveLinear(0,-50);
	pressBottle(false);

	moveLinear(60,0);

	// Letter '0'
	pressBottle(true);
	moveLinear(40,0);
	moveLinear(0,50);
	moveLinear(-40,0);
	moveLinear(0,-55);
	pressBottle(false);
	moveLinear(10,15);
	pressBottle(true);
	moveLinear(20,0);
	moveLinear(0,30);
	moveLinear(-20,0);
	moveLinear(0,-35);
	pressBottle(false);

	moveLinear(60,0);

	// Letter '!'
	pressBottle(true);
	moveLinear(10,0);
	moveLinear(0,30);
	moveLinear(-10,0);
	moveLinear(-30,0);
	pressBottle(false);
	moveLinear(0,40);
	pressBottle(true);
	moveLinear(10,0);
	moveLinear(0,10);
	moveLinear(-10,0);
	moveLinear(0,10);
	pressBottle(false);

	moveLinear(-60,100);

	wait1Msec(10000);
}

void drawLego()
{
	eraseDisplay();
	startTask(monitorPlotter);
	wait1Msec(2000);
	pressBottle(false);
	calibrateY();
	calibrateX();
	wait1Msec(2000);

	// Write the Word "LEGO!"
	WriteLEGO();

	// Move Back to show Drawing
	calibrateX();
	moveLinear(0,150);

}


task main()
{
	displayBigTextLine(0, "Started!");

	char msgBufIn[MAX_MSG_LENGTH];  // To contain the incoming message.
	char msgBufOut[MAX_MSG_LENGTH];  // To contain the outgoing message

	openMailboxIn("EV3_INBOX0");
	openMailboxOut("EV3_OUTBOX0");

	while (true)
	{
		// Read input message.
		// readMailboxIn() is non-blocking and returns "" if there is no message.
		readMailboxIn("EV3_INBOX0", msgBufIn);
		if (strcmp(msgBufIn, "") != 0)
		{
			writeDebugStreamLine(msgBufIn);
			// Here fill your incoming message handling.
			// For example when message "Forward" is received, the EV3 should move forward.
			if(strcmp(msgBufIn,"Forward") == 0)
			{
				setMotorSyncEncoder(motorA, motorB, 0, 50, 50);
			}
			if(strcmp(msgBufIn,"Backward") == 0)
			{
				setMotorSyncEncoder(motorA, motorB, 0, -50, -50);
			}
			if(strcmp(msgBufIn,"Left") == 0)
			{
				setMotorSyncEncoder(motorA, motorB, 50, 50, 50);
			}
			if(strcmp(msgBufIn,"Right") == 0)
			{
				setMotorSyncEncoder(motorA, motorB, -50, 50, 50);
			}
			if(strcmp(msgBufIn,"Rond") == 0)
			{
				drawLego();
			}
		}
		else
		{

			displayBigTextLine(8, "empty message!");
		}

		// Here fill your outgoing message handling.
		// This means sending the distance in cm measured by the ultrasonic sensor
		// and the angle in degrees measured by the gyro sensor.
		// Put both values in msgBufOut.
		// Use the format �<distance> <angle>" (space between distance and angle) so that it can be read by the host program.

		writeMailboxOut("EV3_OUTBOX0", msgBufOut);

		delay(100);  // Wait 100 ms to give host computer time to react.
	}
	closeMailboxIn("EV3_INBOX0");
	closeMailboxOut("EV3_OUTBOX0");
	return;
}
