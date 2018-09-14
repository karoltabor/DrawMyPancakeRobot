
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
/********************************************************
*   Set Global Constants and Variables
********************************************************/
task monitorPlotter();
void calibrateX();
void calibrateY();
void pressBottle(bool enabled);
void moveLinear(int amountToMoveX, int amountToMoveY);


bool penPosition = false;
int xPosition, xDestination;
int yPosition, yDestination;


/*******************************************************
*   Monitor Plotters Variables	&amp; Display on LCD
********************************************************/
task monitorPlotter()
{
	while (true)
	{
		wait1Msec(100);
		displayBigTextLine(3, "X pos: <%d>;", getMotorEncoder(xMotor));
		displayBigTextLine(4, "Y pos: <%d>;", getMotorEncoder(yMotor));
		/*		displayBigTextLine(5, "Press pos: &lt; %d&gt;", nMotorEncoder[pressMotor]);
		if(SensorValue(pressHome) == 1){
		displayBigTextLine(2, "Press is on, &lt; %d&gt;", SensorValue(pressHome));}
		else {
		displayBigTextLine(2, "Press is off, &lt; %d&gt;", SensorValue(pressHome));
		}*/
	}

}



/********************************************************
*   Calibration X - Axis
********************************************************/
void calibrateX(){
	while(getTouchValue(xHome) == false){
		setMotor(xMotor,-20);
		setMotor(xMotor2,-20);
		if(getTouchValue(xHome2) == true) {
			setMotor(xMotor2,0);
		}
	}
	while(getTouchValue(xHome2) == false){
		setMotor(xMotor2,-20);
	}
	setMotorSync(xMotor, xMotor2, 0, 0);
	resetMotorEncoder(xMotor);
	xPosition = 0; xDestination = 0;
}



/********************************************************
*   Calibration Y - Axis
********************************************************/
void calibrateY(){
	while(getTouchValue(yHome) == false){
		setMotor(yMotor, -20);
	}
	setMotor(yMotor, 0);
	resetMotorEncoder(yMotor);
	yPosition = 0; yDestination = 0;
}



/********************************************************
*  Press Bottle
********************************************************/
void pressBottle(bool enabled)   //True = pen down, write; False = pen up
{
	/*	if (enabled == false) {
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
	penPosition = true; }*/
}



/********************************************************
*   Movement functions
********************************************************/
void moveLinear(int amountToMoveX, int amountToMoveY){
	xPosition = xPosition+amountToMoveX;
	yPosition = yPosition+amountToMoveY;
	if(xPosition <= 1000 && yPosition <= 600){
		xDestination = 5*xPosition; // xDestination = 5x Current xPosition;
		yDestination = 5*yPosition; // yDestination = 5x Current yPosition;

		delay(1000);

		int yDesinationEncoder = (getMotorEncoder(yMotor) + yDestination;
		int xDesinationEncoder = (getMotorEncoder(xMotor) + xDestination;
		// TODO: Insert writeToDataStream
		while((getMotorEncoder(yMotor) != yDesinationEncoder) && (getMotorEncoder(xMotor) != xDesinationEncoder)){

			if(yDesinationEncoder > getMotorEncoder(yMotor)){
				setMotor(yMotor,20);
			}
			else if(yDesinationEncoder < getMotorEncoder(yMotor)){
				setMotor(yMotor,-20);
			}
			else {
				setMotor(yMotor, 0);
				yDestination = 1;
			}

			if(xDesinationEncoder > getMotorEncoder(xMotor)){
				setMotorSync(xMotor,xMotor2,0,20);
			}
			else if(xDesinationEncoder < getMotorEncoder(xMotor)){
				setMotorSync(xMotor,xMotor2,0,20);
			}
			else{
				setMotorSync(xMotor,xMotor2,0,0);
				xDestination = 1;
			}

		}
		stopAllMotors();
	}
}



/********************************************************
*   Draw Squares
********************************************************/
void drawSquares(){
	moveLinear(20,20);

	moveLinear(50,0);
	moveLinear(0,50);
	moveLinear(-50,0);
	moveLinear(0,-50);

	/*moveLinear(20,20);
	moveLinear(70,0);
	moveLinear(0,70);
	moveLinear(-70,0);
	moveLinear(0,-77);


	moveLinear(20,20);
	moveLinear(100,0);
	moveLinear(0,100);
	moveLinear(-100,0);
	moveLinear(0,-107);

	moveLinear(-50,150);*/

	wait1Msec(10000);
}


/********************************************************
*   Write the word "LEGO!"
********************************************************/
void WriteLEGO(){
	moveLinear(10,10);
	// Letter 'L'
	moveLinear(10,0);
	moveLinear(0,40);
	moveLinear(30,0);
	moveLinear(0,10);
	moveLinear(-40,0);
	moveLinear(0,-55);

	moveLinear(60,0);

	// Letter 'E'
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

	moveLinear(60,0);

	// Letter 'G'
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

	moveLinear(60,0);

	// Letter '0'
	moveLinear(40,0);
	moveLinear(0,50);
	moveLinear(-40,0);
	moveLinear(0,-55);
	moveLinear(10,15);
	moveLinear(20,0);
	moveLinear(0,30);
	moveLinear(-20,0);
	moveLinear(0,-35);

	moveLinear(60,0);

	// Letter '!'
	moveLinear(10,0);
	moveLinear(0,30);
	moveLinear(-10,0);
	moveLinear(-30,0);
	moveLinear(0,40);
	moveLinear(10,0);
	moveLinear(0,10);
	moveLinear(-10,0);
	moveLinear(0,10);

	moveLinear(-60,100);

	wait1Msec(10000);
}



/********************************************************
*   LEGO Mindstorms NXT X-Y Plotter
********************************************************/
/*task main()
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

wait1Msec(20000); // Wait 20 Seconds to Change Paper

// Begin Drawing Squares
drawSquares();

// Move Back to show Drawing
calibrateX();
moveLinear(0,150);

wait1Msec(20000); // Wait 20 Seconds to Change Paper

pressBottle(false);
calibrateY();
calibrateX();
}*/
