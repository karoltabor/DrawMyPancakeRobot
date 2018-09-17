
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
		displayBigTextLine(6, "Y pos: <%d>;", getMotorEncoder(yMotor));
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
	setMotor(xMotor, 0);
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
void pressBottle(bool enabled)
{

}



/********************************************************
*   Movement functions
********************************************************/
void moveLinear(int amountToMoveX, int amountToMoveY){
	xPosition = xPosition+amountToMoveX;
	yPosition = yPosition+amountToMoveY;

	if(xPosition <= 1000 && yPosition <= 600){
		xDestination = 5*amountToMoveX; // xDestination = 5x Current xPosition;
		yDestination = -5*amountToMoveY; // yDestination = 5x Current yPosition;

		int yDesinationEncoder = (getMotorEncoder(yMotor) + yDestination);
		int xDesinationEncoder = (getMotorEncoder(xMotor) + xDestination);
		int deviation = 5;

		while(!((getMotorEncoder(yMotor) > yDesinationEncoder-deviation) && (getMotorEncoder(yMotor) < yDesinationEncoder+deviation)) ||
			!((getMotorEncoder(xMotor) > xDesinationEncoder-deviation) && (getMotorEncoder(xMotor) < xDesinationEncoder+deviation))){
			if(yDesinationEncoder > (getMotorEncoder(yMotor)-deviation)){
				setMotor(yMotor,-20);
			}
			else if(yDesinationEncoder < (getMotorEncoder(yMotor)+deviation)){
				setMotor(yMotor,20);
			}
			else {
				yDestination = 1;
			}

			if(xDesinationEncoder > (getMotorEncoder(xMotor)-deviation)){
				setMotorSync(xMotor,xMotor2,0,20);
			}
			else if(xDesinationEncoder < (getMotorEncoder(xMotor)+deviation)){
				setMotorSync(xMotor,xMotor2,0,-20);
			}
			else{
				setMotorSync(xMotor,xMotor2,0,0);
				xDestination = 1;
			}

		}
		stopAllMotors();
		delay(100);
	}
}


/********************************************************
*   Move circular
********************************************************/
void moveCircular(int xCenter, int yCenter, int radius, int finalAngle){
	moveLinear(xCenter, yCenter-radius);
	int angle = 0;
	int step = 5;
	while(angle <=finalAngle){
		int xPoint = xCenter + radius * sinDegrees(angle);
		int yPoint = yCenter + radius * -cosDegrees(angle);
		moveLinear(xPoint - xPosition, yPoint - yPosition);
		writeDebugStreamLine("y-point: %d, move-y: %d", yPoint, yPoint - yPosition);
		angle += step;
	}

}

/********************************************************
*   Draw Square
********************************************************/
void drawSquare(){
	moveLinear(20,20);
	moveLinear(150,0);
	moveLinear(0,120);
	moveLinear(-150,0);
	moveLinear(0,-120);
}


/********************************************************
*   Draw Triangle
********************************************************/
void drawTriangle(int sideLength){
	moveLinear(20,20);
	moveLinear(sideLength,0);
	int height = (sqrt(3)/2) * sideLength;
	int xTop = (sideLength/2) + 20;
	int yTop = height + 20;
	moveLinear(xTop-xPosition,yTop-20);
	moveLinear(20-xTop,20-yTop);
}

/********************************************************
*   Draw Heart
********************************************************/
void drawHeart(int width){
	moveLinear(20+(width/2),20);
	//Right straight part
	moveLinear(width/2,width/2);
	//Right half circle of the heart
	int rightHeartAngle = 0;
	int xRightHeartCenter = (width*0.75)+20;
	int yRightHeartCenter = (width/2)+20;
	int heartRadius = width/4;
	int heartStep = 5;
	while(rightHeartAngle <=180){
		int xRightHeartPoint = xRightHeartCenter + heartRadius * sinDegrees(rightHeartAngle);
		int yRightHeartPoint = yRightHeartCenter + heartRadius * cosDegrees(rightHeartAngle);
		moveLinear(xRightHeartPoint - xPosition, yRightHeartPoint - yPosition);
		writeDebugStreamLine("y-point: %d, move-y: %d", yRightHeartPoint, yRightHeartPoint - yPosition);
		rightHeartAngle += heartStep;
	}
	//Left half circle of the heart
	moveCircular((width/4)+20,(width/2)+20,width/4, 180);
	//Left straight part
	moveLinear(width/2, -(width/2));
}


/********************************************************
*   Draw Star
********************************************************/
void drawStar(int outerRadius, int innerRadius){
	moveLinear(outerRadius+20,20);
	int centerX = outerRadius + 20;
	int centerY = outerRadius + 20;
	int starPoints = 0;
	while(starPoints<= 5){
		int innerX = centerX + innerRadius * cosDegrees((starPoints*72)+36);
		int innerY = centerY + innerRadius * sinDegrees((starPoints*72)+36);
		moveLinear(innerX - xPosition, innerY - yPosition);
		int outerX = centerX + outerRadius * cosDegrees(starPoints*72);
		int outerY = centerY + outerRadius * sinDegrees(starPoints*72);
		moveLinear(outerX - xPosition, outerY - yPosition);
		starPoints += 1;
	}
}

/********************************************************
*		Draw Spiral
********************************************************/
void drawSpiral(int xCenter, int yCenter, double radius){
	moveLinear(xCenter, yCenter-radius);
	int angle = 0;
	int step = 5;
	while(!(xPosition = xCenter) && !(yPosition =yCenter)){
		int xPoint = xCenter + radius * sinDegrees(angle);
		int yPoint = yCenter + radius * -cosDegrees(angle);
		moveLinear(xPoint - xPosition, yPoint - yPosition);
		writeDebugStreamLine("y-point: %d, move-y: %d", yPoint, yPoint - yPosition);
		angle += step;
		radius -= 0.2 ;
		if(angle = 360){
			angle = 0;
		}
	}
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
