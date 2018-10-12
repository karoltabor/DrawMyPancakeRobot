/********************************************************
*   Set Global Constants and Variables
********************************************************/
task monitorPlotter();

bool penPosition = false;
int xPosition, xDestination;
int yPosition, yDestination;
int gridWidth = 1000;
int gridHeight = 700;
int factor = 5;
int unprecision = 5;
int margin = 20;
int letterBoxWidth;
int letterBoxHeight;
int letterWidth; // Extra for double letters


/*******************************************************
*   Monitor Plotters Variables & Display on LCD
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

/**************************************************
* Calibrate axes at same time
***************************************************/

void calibrate() {
	while(!(getTouchValue(xHome) && getTouchValue(xHome2) && getTouchValue(yHome))) {
		if(getTouchValue(xHome) == false) {
			setMotor(xMotor,-20);
			} else {
			setMotor(xMotor,0);
		}

		if(getTouchValue(xHome2) == false) {
			setMotor(xMotor2,-20);
			} else {
			setMotor(xMotor2,0);
		}

		if(getTouchValue(yHome) == false) {
			setMotor(yMotor,-20);
			} else {
			setMotor(yMotor,0);
		}
	}
	stopAllMotors();
	resetMotorEncoder(xMotor);
	resetMotorEncoder(xMotor2);
	resetMotorEncoder(yMotor);
	xPosition = 0; xDestination = 0;
	yPosition = 0; yDestination = 0;
}

/********************************************************
*  Press Bottle
********************************************************/
task Pump()
{
	while(true) {
		setMotor(pumpMotor, 50);
	}
	setMotor(pumpMotor, 0);
}

void pressBottle(bool press) {
	if(press){
		startTask(Pump);
	} else {
		stopTask(Pump);
		playSoundFile("Air release");
	}
}

/********************************************************
*   Movement functions
********************************************************/
void moveLinear(int amountToMoveX, int amountToMoveY, int speed){
	float ratio;
	if(amountToMoveX == 0 || amountToMoveY == 0){
		ratio = 1;
		} else {
		ratio = (float) amountToMoveX / amountToMoveY;
	}
	int xSpeed;
	int ySpeed;
	if(ratio>1){
		xSpeed = speed;
		ySpeed = abs(speed / ratio);
		} else if (ratio<1){
		xSpeed = abs(speed * ratio);
		ySpeed = speed;
		} else {
		xSpeed = speed;
		ySpeed = speed;
	}

	xPosition = xPosition+amountToMoveX;
	yPosition = yPosition+amountToMoveY;

	if(xPosition <= gridWidth && yPosition <= gridHeight){
		xDestination = factor*amountToMoveX; // xDestination = 5x Current xPosition;
		yDestination = -factor*amountToMoveY; // yDestination = 5x Current yPosition;

		int yDesinationEncoder = (getMotorEncoder(yMotor) + yDestination);
		int xDesinationEncoder = (getMotorEncoder(xMotor) + xDestination);

		while(!((getMotorEncoder(yMotor) > yDesinationEncoder-unprecision) && (getMotorEncoder(yMotor) < yDesinationEncoder+unprecision)) ||
			!((getMotorEncoder(xMotor) > xDesinationEncoder-unprecision) && (getMotorEncoder(xMotor) < xDesinationEncoder+unprecision))){

			//playSoundFile("Laser");

			if(yDesinationEncoder > (getMotorEncoder(yMotor)-unprecision)){
				setMotor(yMotor,-ySpeed);
			}
			else if(yDesinationEncoder < (getMotorEncoder(yMotor)+unprecision)){
				setMotor(yMotor,ySpeed);
			}
			else {
				yDestination = 1;
			}

			if(xDesinationEncoder > (getMotorEncoder(xMotor)-unprecision)){
				setMotor(xMotor,xSpeed);
				setMotor(xMotor2,xSpeed);
			}
			else if(xDesinationEncoder < (getMotorEncoder(xMotor)+unprecision)){
				setMotor(xMotor,-xSpeed);
				setMotor(xMotor2,-xSpeed);
			}
			else{
				setMotor(xMotor,0);
				setMotor(xMotor2,0);
				xDestination = 1;
			}
		}
		stopAllMotors();
	}
}


/********************************************************
*   Fill circle
********************************************************/
void fillCircle(int diameter, int margin, int steps, int speed) {
	bool left = false;

	pressBottle(false);
	moveLinear(diameter / 2 + margin, margin, speed);

	pressBottle(true);
	for (int i = 0; i < diameter; i += 5) {
		if (left) {
			moveLinear(i * 2, 0, speed);
			moveLinear(steps, steps, speed);
			left = false;
			} else {
			moveLinear(-i * 2, 0, speed);
			moveLinear(-steps, steps, speed);
			left = true;
		}
	}
	pressBottle(false);
}

/********************************************************
*   Free draw functions
* 	Removing null inputs
********************************************************/
char* removeLeadingZeros(char *coord){
	string coordString = coord;
	while(coord[0] == '0'){
		if(strlen(coordString)>1){
			coordString = coord;
			stringDelete(coordString, 0, 1);
			coord = coordString;
		}
		else {
			break;
		}
	}
	coordString = coord;
	return coordString;
}

/*****************************************
*	Read inputstring variables
******************************************/

void freeDraw(char *input, int speed) {
	int count = 0;

	string xCoor = "0";
	string yCoor = "0";
	//for each instuction of 9 characters
	while(count < (strlen(input)/9)){
		string instruction, bottlePressed;
		strcpy(instruction, input);
		stringDelete(instruction, 0, count*9);
		stringDelete(instruction, 9, strlen(instruction)-9);

		//split instruction into x-coordinate, y-coordinate and a boolean for the bottle press function
		strcpy(xCoor, instruction);
		stringDelete(xCoor, 4 , 5);
		strcpy(yCoor, instruction);
		stringDelete(yCoor, 0 , 4);
		stringDelete(yCoor, 4 , 1);
		strcpy(bottlePressed, instruction);
		stringDelete(bottlePressed, 0 , 8);

		char *xCoorArr = xCoor;
		char *yCoorArr = yCoor;

		int previousXCoor, previousYCoor;
		sscanf(xCoor, "%d", &previousXCoor);
		sscanf(yCoor, "%d", &previousYCoor);

		int xCoorInt, yCoorInt;
		xCoor = removeLeadingZeros(xCoorArr);
		yCoor = removeLeadingZeros(yCoorArr);
		sscanf(xCoor, "%d", &xCoorInt);
		sscanf(yCoor, "%d", &yCoorInt);

		int xDistance = xCoorInt-previousXCoor;
		int yDistance = yCoorInt-previousYCoor;

		bool bottle;
		if(bottlePressed == "T"){
			pressBottle(true);
			} else {
			pressBottle(false);
		}

		//execute instruction
		moveLinear(xDistance, yDistance, speed);
		writeDebugStreamLine("xCoor: %s", previousXCoor);
		writeDebugStreamLine("yCoor: %s", previousYCoor);
		writeDebugStreamLine("prev xCoor: %s", xCoor);
		writeDebugStreamLine("prev yCoor: %s", yCoor);
		writeDebugStreamLine("xDistance: %d", xDistance);
		writeDebugStreamLine("yDistance: %d", yDistance);
		writeDebugStreamLine("bottlePressed: %d", bottle);

		count++;
	}
}


/********************************************************
*   Move Elipse
********************************************************/
void moveEllipse (int xCenter, int yCenter, int xradius, int yradius, int finalangle, int speed){
	int preSin = 1;
	int preCos = 1;
	int radius;
	int angle = 0;
	int step = 4;

	if (xradius > yradius){
		preSin = xradius/yradius;
		radius = yradius;
	}
	else if (xradius < yradius){
		preCos = yradius/xradius;
		radius = xradius;
		} else {
		radius = xradius;
	}
	writeDebugStreamLine("radius: %d", radius);
	displayBigTextLine(3, "Test1");
	while (angle <= finalangle){
		int xPoint = xCenter + radius * (preSin * sinDegrees(angle));
		int yPoint = yCenter + radius * (preCos * -cosDegrees(angle));

		moveLinear(xPoint - xPosition, yPoint - yPosition,speed);
		angle += step;

	}

}

/********************************************************
*   Draw Square
********************************************************/
void drawSquare(int speed){
	pressBottle(true);
	moveLinear(20,20,speed);
	moveLinear(150,0,speed);
	moveLinear(0,120,speed);
	moveLinear(-150,0,speed);
	moveLinear(0,-120,speed);
	pressBottle(false);
}

/********************************************************
*   Draw Triangle
********************************************************/
void drawTriangle(int sideLength, int speed){
	moveLinear(20,20,speed);
	moveLinear(sideLength,0,speed);
	int height = (sqrt(3)/2) * sideLength;
	int xTop = (sideLength/2) + 20;
	int yTop = height + 20;
	moveLinear(xTop-xPosition,yTop-20,speed);
	moveLinear(20-xTop,20-yTop,speed);
}

/********************************************************
*   Draw Heart
********************************************************/
void drawHeart(int width){
	moveLinear(20+(width/2),20,20);
	//Right straight part
	writeDebugStreamLine("x-point: %d, y-point: %d", xPosition, yPosition);
	moveLinear(width/2,width/2,20);
	//Right half circle of the heart
	int rightHeartAngle = 90;
	int xRightHeartCenter = (width*0.75)+20;
	int yRightHeartCenter = (width/2)+20;
	int heartRadius = width/4;
	int heartStep = 5;
	while(rightHeartAngle <= 270){
		int xRightHeartPoint = xRightHeartCenter + heartRadius * sinDegrees(rightHeartAngle);
		int yRightHeartPoint = yRightHeartCenter + heartRadius * -cosDegrees(rightHeartAngle);
		moveLinear(xRightHeartPoint - xPosition, yRightHeartPoint - yPosition,20);
		writeDebugStreamLine("y-point: %d, move-y: %d", yRightHeartPoint, yRightHeartPoint - yPosition);
		rightHeartAngle += heartStep;
	}
	//Left half circle of the heart
	int leftHeartAngle = 90;
	int xLeftHeartCenter = (width*0.25)+20;
	int yLeftHeartCenter = (width/2)+20;
	int leftRadius = width/4;
	int leftStep = 5;
	while(leftHeartAngle <= 270){
		int xLeftHeartPoint = xLeftHeartCenter + leftRadius * sinDegrees(leftHeartAngle);
		int yLeftHeartPoint = yLeftHeartCenter + leftRadius * -cosDegrees(leftHeartAngle);
		moveLinear(xLeftHeartPoint - xPosition, yLeftHeartPoint - yPosition,20);
		writeDebugStreamLine("y-point: %d, move-y: %d", yLeftHeartPoint, yLeftHeartPoint - yPosition);
		leftHeartAngle += leftStep;
	}
	writeDebugStreamLine("x-point: %d, y-point: %d", xPosition, yPosition);
	//Left straight part
	moveLinear(width/2, -(width/2),20);
	writeDebugStreamLine("x-point: %d, y-point: %d", xPosition, yPosition);
}


/********************************************************
*   Draw Star
********************************************************/
void drawStar(int outerRadius, int innerRadius, int speed){
	moveLinear(outerRadius+20,outerRadius+20,speed);
	int centerX = outerRadius + 20;
	int centerY = outerRadius + 20;
	int starPoints = 0;
	while(starPoints < 5){
		int innerX = centerX + innerRadius * cosDegrees((starPoints*72)+36);
		int innerY = centerY + innerRadius * sinDegrees((starPoints*72)+36);
		moveLinear(innerX - xPosition, innerY - yPosition,speed);
		int outerX = centerX + outerRadius * cosDegrees(starPoints*72);
		int outerY = centerY + outerRadius * sinDegrees(starPoints*72);
		moveLinear(outerX - xPosition, outerY - yPosition,speed);
		starPoints += 1;
	}
}

/********************************************************
*		Draw Spiral
********************************************************/
void drawSpiral(int xCenter, int yCenter, float radius, int speed){
	moveLinear(xCenter, yCenter-radius,speed);
	int angle = 0;
	int step = 5;
	while(radius > 5){
		int xPoint = xCenter + radius * sinDegrees(angle);
		int yPoint = yCenter + radius * -cosDegrees(angle);
		moveLinear(xPoint - xPosition, yPoint - yPosition,speed);
		writeDebugStreamLine("y-point: %d, move-y: %d", yPoint, yPoint - yPosition);
		angle += step;
		radius = radius - 0.2 ;
		if(angle >= 360){
			angle = 0;
		}
	}
}

/********************************************************
*		Draw Eiffel Tower
********************************************************/

void drawEiffel(int heigth, int speed){
	int boxDrawWidth = heigth/9;
	pressBottle(false);
	moveLinear(20, 20, speed);
	pressBottle(true);
	moveLinear(boxDrawWidth, 2*boxDrawWidth, speed);
	moveLinear(-(0.2*boxDrawWidth), 0, speed);
	moveLinear(0, 0.6*boxDrawWidth, speed);
	moveLinear(0.4*boxDrawWidth, 0, speed);
	moveLinear(0.4*boxDrawWidth, 1.4*boxDrawWidth, speed);
	moveLinear(-(0.2*boxDrawWidth), 0, speed);
	moveLinear(0, 0.4*boxDrawWidth, speed);
	moveLinear(0.4*boxDrawWidth, 0, speed);
	moveLinear(1.2*boxDrawWidth, 4*boxDrawWidth, speed);
	moveLinear(0, 0.6*boxDrawWidth, speed);
	pressBottle(false);
	moveLinear(0, -(0.6*boxDrawWidth), speed);
	pressBottle(true);
	moveLinear(1.2*boxDrawWidth, -4*boxDrawWidth, speed);
	moveLinear(0.4*boxDrawWidth, 0, speed);
	moveLinear(0, -0.4*boxDrawWidth, speed);
	moveLinear(-(0.2*boxDrawWidth), 0, speed);
	moveLinear(0.4*boxDrawWidth, -1.4*boxDrawWidth, speed);
	moveLinear(0.4*boxDrawWidth, 0, speed);
	moveLinear(0, -0.6*boxDrawWidth, speed);
	moveLinear(-(0.2*boxDrawWidth), 0, speed);
	moveLinear(boxDrawWidth, -2*boxDrawWidth, speed);
	moveLinear(-1.6*boxDrawWidth, 0, speed);
	moveEllipse(20+(3*boxDrawWidth), 20, 1.6*boxDrawWidth, 1.4*boxDrawWidth, 180, speed);
	moveLinear(-1.6*boxDrawWidth, 0, speed);
	pressBottle(false);
	moveLinear(boxDrawWidth, 2*boxDrawWidth, speed);
	pressBottle(true);
	moveLinear(4*boxDrawWidth, 0, speed);
	pressBottle(false);
	moveLinear(-0.2*boxDrawWidth, 0.6*boxDrawWidth, speed);
	pressBottle(true);
	moveLinear(-3.6*boxDrawWidth, 0, speed);
	pressBottle(false);
	moveLinear(boxDrawWidth, 0.2*boxDrawWidth, speed);
	pressBottle(true);
	moveLinear(1.6*boxDrawWidth, 0, speed);
	moveLinear(-0.2*boxDrawWidth, boxDrawWidth, speed);
	moveLinear(-1.2*boxDrawWidth, 0, speed);
	moveLinear(-0.2*boxDrawWidth, -boxDrawWidth, speed);
	pressBottle(false);
	moveLinear(-0.6*boxDrawWidth, 1.2*boxDrawWidth, speed);
	pressBottle(true);
	moveLinear(2.8*boxDrawWidth, 0, speed);
	pressBottle(false);
	moveLinear(-0.2*boxDrawWidth, 0.4*boxDrawWidth, speed);
	pressBottle(true);
	moveLinear(-2.4*boxDrawWidth, 0,speed);
	pressBottle(false);
	moveLinear(0.8*boxDrawWidth, 0.2*boxDrawWidth, speed);
	pressBottle(true);
	moveLinear(0.8*boxDrawWidth, 0, speed);
	moveLinear(-0.4*boxDrawWidth, 2*boxDrawWidth, speed);
	moveLinear(-0.4*boxDrawWidth, -2*boxDrawWidth, speed);
	pressBottle(false);
}

/********************************************************
*		Draw Smiley
********************************************************/
void drawSmiley(int xCenter, int yCenter, int radius, int speed){
	pressBottle(false);
	moveLinear(xCenter, yCenter-radius, speed);

	pressBottle(true);
	moveEllipse(xCenter, yCenter, radius, radius, 360, speed);
	pressBottle(false);
	float mouthstartx = xCenter - (0.075 * radius);
	float mouthstarty = yCenter - (0.5 * radius);
	moveLinear(mouthstartx - xPosition, mouthstarty - yPosition, speed);

	pressBottle(true);
	moveEllipse(xCenter, yCenter- (0.3 * radius), 0.5 * radius, 0.35 * radius, 180, speed);
	moveLinear(-radius, 0, speed);

	pressBottle(false);
	moveLinear(0.1875 * radius, 0.3 * radius, speed);

	pressBottle(true);
	moveEllipse(xCenter - (0.3125 * radius), yCenter + (0.25 * radius), 0.1875 * radius, 0.25 * radius, 360, speed);

	pressBottle(false);
	moveLinear(0.625 * radius, 0, speed);

	pressBottle(true);
	moveEllipse(xCenter - (0.3125 * radius), yCenter + (0.25 * radius), 0.1875 * radius, 0.25 * radius, 360, speed);

	pressBottle(false);
}

/******************************************************
*		Alfabet
******************************************************/

/********************************************************
*        Letter A
*********************************************************/
void drawA(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	pressBottle(true);
	moveLinear(letterWidth, letterBoxHeight-(2*margin), speed);
	moveLinear(letterWidth, 0, speed);
	moveLinear(letterWidth, -(letterBoxHeight-(2*margin)), speed);
	pressBottle(false);
	moveLinear(-(0.5 * letterWidth), 0.5 * (letterBoxHeight-(2*margin)), speed);
	pressBottle(true);
	moveLinear(-2 * letterWidth, 0, speed);
	pressBottle(false);
}
/********************************************************
*        Letter B
*********************************************************/
void drawB(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(0.6 * letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(-(0.6 * letterWidth), 0.6 * letterWidth, speed);
	moveLinear(0 ,letterWidth, speed);
	moveLinear(0.4 * letterWidth, 0.4 * letterWidth, speed);
	moveLinear(2.6 * letterWidth, 0, speed);
	pressBottle(false);
	moveLinear(-(2.6 * letterWidth), 0, speed);
	pressBottle(true);
	moveLinear(-(0.4* letterWidth), 0.4 * letterWidth, speed);
	moveLinear(0, letterWidth, speed);
	moveLinear(0.6 * letterWidth, 0.6 * letterWidth, speed);
	moveLinear(2.4 * letterWidth, 0, speed);
	moveLinear(0, -(4 * letterWidth), speed);
	moveLinear(-(2.4 * letterWidth), 0, speed);
	pressBottle(false);
}

/********************************************************
*        Letter C
*********************************************************/
void drawC(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(0, letterWidth, speed);
	pressBottle(true);
	moveLinear(letterWidth, -letterWidth, speed);
	moveLinear(letterWidth, 0, speed);
	moveLinear(letterWidth, letterWidth, speed);
	moveLinear(0, 2 * letterWidth, speed);
	moveLinear(-letterWidth, letterWidth, speed);
	moveLinear(-letterWidth, 0, speed);
	moveLinear(-letterWidth, -letterWidth, speed);
	pressBottle(false);
}

/********************************************************
*        Letter D
*********************************************************/
void drawD(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(-letterWidth, letterWidth, speed);
	moveLinear(0, 2* letterWidth, speed);
	moveLinear(letterWidth, letterWidth, speed);
	moveLinear(2 * letterWidth, 0, speed);
	moveLinear(0, 4 * letterWidth, speed);
	moveLinear(-(2 * letterWidth), 0, speed);
	pressBottle(false);
}

/********************************************************
*				Letter E
********************************************************/
void drawE(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	pressBottle(true);
	moveLinear(3* letterWidth, 0, speed);
	moveLinear(0, 2 * letterWidth, speed);
	moveLinear(-(2.5 * letterWidth), 0, speed);
	pressBottle(false);
	moveLinear(2.5 * letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(0, 2 * letterWidth, speed);
	moveLinear(-(3 * letterWidth), 0, speed);
	pressBottle(false);
}

/********************************************************
*				Letter F
********************************************************/
void drawF(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(3 * letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(0, 2 * letterWidth, speed);
	moveLinear(-(2.5 * letterWidth), 0, speed);
	pressBottle(false);
	moveLinear(2.5 * letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(0, 2 * letterWidth, speed);
	moveLinear(-(3 * letterWidth), 0, speed);
	pressBottle(false);
}

/********************************************************
*				Letter G
********************************************************/
void drawG(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(0, 3 * letterWidth, speed);
	pressBottle(true);
	moveLinear(letterWidth, letterWidth, speed);
	moveLinear(letterWidth, 0, speed);
	moveLinear(letterWidth, -letterWidth, speed);
	moveLinear(0, -(2 * letterWidth), speed);
	moveLinear(-letterWidth, -letterWidth, speed);
	moveLinear(-letterWidth, 0, speed);
	moveLinear(-letterWidth, letterWidth, speed);
	moveLinear(0, letterWidth, speed);
	moveLinear(letterWidth, 0, speed);
	pressBottle(false);
}

/********************************************************
*				Letter H
********************************************************/
void drawH(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	pressBottle(true);
	moveLinear(0, 4 * letterWidth, speed);
	pressBottle(false);
	moveLinear(0, -(2 * letterWidth), speed);
	pressBottle(true);
	moveLinear(3 * letterWidth, 0, speed);
	moveLinear(0, 2*letterWidth, speed);
	pressBottle(false);
	moveLinear(0, -(2* letterWidth), speed);
	pressBottle(true);
	moveLinear(0, -(2* letterWidth), speed);
	pressBottle(false);
}

/********************************************************
*				Letter I
********************************************************/
void drawI(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(1.5 * letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(0, 2.75 * letterWidth, speed);
	pressBottle(false);
	moveLinear(0, 0.5 * letterWidth, speed);
	pressBottle(true);
	moveLinear(0, 0.75 * letterWidth, speed);
	pressBottle(false);
}

/********************************************************
*				Letter J
********************************************************/
void drawJ(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(0, 4* letterWidth, speed);
	pressBottle(true);
	moveLinear(0, -(3 * letterWidth), speed);
	moveLinear(letterWidth, -letterWidth, speed);
	moveLinear(letterWidth, 0, speed);
	moveLinear(letterWidth, letterWidth, speed);
	pressBottle(false);
}

/********************************************************
*				Letter K
********************************************************/
void drawK(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	pressBottle(true);
	moveLinear(2*letterWidth, 2*letterWidth, speed);
	moveLinear(-(2*letterWidth), 2*letterWidth, speed);
	pressBottle(false);
	moveLinear(2*letterWidth, -(2*letterWidth), speed);
	pressBottle(true);
	moveLinear(letterWidth, 0, speed);
	moveLinear(0, 2 * letterWidth, speed);
	pressBottle(false);
	moveLinear(0, -(2*letterWidth), speed);
	pressBottle(true);
	moveLinear(0, -(2*letterWidth), speed);
	pressBottle(false);
}

/********************************************************
*        Letter L
*********************************************************/
void drawL(int speed){
	pressBottle(false);
	moveLinear(margin,margin,speed);
	pressBottle(true);
	moveLinear(3*letterWidth, 0, speed);
	moveLinear(0, 4*letterWidth, speed);
	pressBottle(false);
}

/********************************************************
*				Letter M
********************************************************/
void drawM(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	pressBottle(true);
	moveLinear(0, 4*letterWidth, speed);
	moveLinear(1.5*letterWidth, -(2*letterWidth), speed);
	moveLinear(1.5*letterWidth, 2*letterWidth, speed);
	moveLinear(0, -(4*letterWidth), speed);
	pressBottle(false);
}

/********************************************************
*				Letter N
********************************************************/
void drawN(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(3*letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(0, 4*letterWidth, speed);
	moveLinear(-(3* letterWidth), -(4*letterWidth), speed);
	moveLinear(0, 4*letterWidth, speed);
	pressBottle(false);
}

/********************************************************
*				Letter O
********************************************************/
void drawO(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(letterWidth, 0, speed);
	moveLinear(letterWidth, letterWidth, speed);
	moveLinear(0, 2*letterWidth, speed);
	moveLinear(-letterWidth, letterWidth, speed);
	moveLinear(-letterWidth, 0, speed);
	moveLinear(-letterWidth, -letterWidth, speed);
	moveLinear(0, -(2*letterWidth), speed);
	moveLinear(letterWidth, -letterWidth, speed);
	pressBottle(false);
}

/********************************************************
*				Letter P
********************************************************/
void drawP(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(3*letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(0, 4*letterWidth, speed);
	moveLinear(-(2.5*letterWidth), 0, speed);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	moveLinear(0, -letterWidth, speed);
	moveLinear(0.5 * letterWidth, -(0.5*letterWidth), speed);
	moveLinear(2.5*letterWidth, 0, speed);
	pressBottle(false);
}

/********************************************************
*				Letter Q
********************************************************/
void drawQ(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	pressBottle(true);
	moveLinear(letterWidth, letterWidth, speed);
	pressBottle(false);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	pressBottle(true);
	moveLinear(0.5*letterWidth, -(0.5*letterWidth), speed);
	moveLinear(letterWidth, 0, speed);
	moveLinear(letterWidth, letterWidth, speed);
	moveLinear(0, 2*letterWidth, speed);
	moveLinear(-letterWidth, letterWidth, speed);
	moveLinear(-letterWidth, 0, speed);
	moveLinear(-letterWidth, -letterWidth, speed);
	moveLinear(0, -(2*letterWidth), speed);
	moveLinear(0.5*letterWidth, -(0.5*letterWidth), speed);
	pressBottle(false);
}

/********************************************************
*				Letter R
********************************************************/
void drawR(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	pressBottle(true);
	moveLinear(2*letterWidth, 2*letterWidth, speed);
	moveLinear(-(1.5*letterWidth), 0, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(0, letterWidth, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(2.5*letterWidth, 0, speed);
	moveLinear(0, -(4*letterWidth), speed);
	pressBottle(false);
	moveLinear(0, 2*letterWidth, speed);
	pressBottle(true);
	moveLinear(-letterWidth, 0, speed);
	pressBottle(false);
}

/********************************************************
*				Letter S
********************************************************/
void drawS(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(3*letterWidth, 0.5*letterWidth, speed);
	pressBottle(true);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	moveLinear(-(2*letterWidth), 0, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(0, letterWidth, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(2*letterWidth, 0, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(0, letterWidth, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(-(2*letterWidth), 0, speed);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	pressBottle(false);
}

/********************************************************
*				Letter T
********************************************************/
void drawT(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(1.5*letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(0, 4*letterWidth, speed);
	moveLinear(1.5*letterWidth, 0, speed);
	pressBottle(false);
	moveLinear(-(1.5*letterWidth), 0, speed);
	pressBottle(true);
	moveLinear(-(1.5*letterWidth), 0, speed);
	pressBottle(false);
}

/********************************************************
*				Letter U
********************************************************/
void drawU(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(0, 4*letterWidth, speed);
	pressBottle(true);
	moveLinear(0, -(3*letterWidth), speed);
	moveLinear(letterWidth, -letterWidth, speed);
	moveLinear(letterWidth, 0, speed);
	moveLinear(letterWidth, letterWidth, speed);
	moveLinear(0, 3*letterWidth, speed);
	pressBottle(false);
}

/********************************************************
*				Letter V
********************************************************/
void drawV(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(0, 4*letterWidth, speed);
	pressBottle(true);
	moveLinear(1.5*letterWidth, -(4*letterWidth), speed);
	moveLinear(1.5*letterWidth, 4*letterWidth, speed);
	pressBottle(false);
}

/********************************************************
*				Letter W
********************************************************/
void drawW(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(0, 4*letterWidth, speed);
	pressBottle(true);
	moveLinear(0.75*letterWidth, -(4*letterWidth), speed);
	moveLinear(0.75*letterWidth, 4*letterWidth, speed);
	moveLinear(0.75*letterWidth, -(4*letterWidth), speed);
	moveLinear(0.75*letterWidth, 4*letterWidth, speed);
	pressBottle(false);
}

/********************************************************
*				Letter X
********************************************************/
void drawX(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	pressBottle(true);
	moveLinear(3*letterWidth, 4*letterWidth, speed);
	pressBottle(false);
	moveLinear(-(3*letterWidth), 0, speed);
	pressBottle(true);
	moveLinear(3*letterWidth, -(4*letterWidth), speed);
	pressBottle(false);
}

/********************************************************
*				Letter Y
********************************************************/
void drawY(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(1.5*letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(0, 1.5*letterWidth, speed);
	moveLinear(1.5*letterWidth, 2.5*letterWidth, speed);
	pressBottle(false);
	moveLinear(-(3*letterWidth), 0, speed);
	pressBottle(true);
	moveLinear(1.5*letterWidth, -(2.5*letterWidth), speed);
	pressBottle(false);
}

/********************************************************
*				Letter Z
********************************************************/
void drawZ(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	pressBottle(true);
	moveLinear(3*letterWidth, 0, speed);
	moveLinear(-(3*letterWidth), 4*letterWidth, speed);
	moveLinear(3*letterWidth, 0, speed);
	pressBottle(false);
}

/********************************************************
*				Cijfer 1
********************************************************/
void draw1(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(0.7*letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(1.6*letterWidth, 0, speed);
	pressBottle(false);
	moveLinear(-(0.8*letterWidth), 0, speed);
	pressBottle(true);
	moveLinear(0, 4*letterWidth, speed);
	moveLinear(0.8*letterWidth, -(0.8*letterWidth), speed);
	pressBottle(false);
}

/********************************************************
*				Cijfer 2
********************************************************/
void draw2(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	pressBottle(true);
	moveLinear(3*letterWidth, 0, speed);
	moveLinear(0, 1.5*letterWidth, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(-(2*letterWidth), 0, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(0, letterWidth, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(2*letterWidth, 0, speed);
	moveLinear(0.5*letterWidth, -(0.5*letterWidth), speed);
}


/********************************************************
*				Cijfer 3
********************************************************/
void draw3(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(3*letterWidth, 0.5*letterWidth, speed);
	pressBottle(true);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	moveLinear(-(2*letterWidth), 0, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(0, 1.25*letterWidth, speed);
	moveLinear(0.25*letterWidth, 0.25*letterWidth, speed);
	moveLinear(1.5*letterWidth, 0, speed);
	pressBottle(false);
	moveLinear(-(1.5*letterWidth), 0, speed);
	pressBottle(true);
	moveLinear(-(0.25*letterWidth), 0.25*letterWidth, speed);
	moveLinear(0, 1.25*letterWidth, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(2*letterWidth, 0, speed);
	moveLinear(0.5*letterWidth, -(0.5*letterWidth), speed);
	pressBottle(false);
}

/********************************************************
*				Cijfer 4
********************************************************/
void draw4(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(2*letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(0, 4*letterWidth, speed);
	moveLinear(-(2*letterWidth), -(2*letterWidth), speed);
	moveLinear(3*letterWidth, 0, speed);
	pressBottle(false);
}

/********************************************************
*				Cijfer 5
********************************************************/
void draw5(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(3*letterWidth, 0.5*letterWidth, speed);
	pressBottle(true);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	moveLinear(-(2*letterWidth), 0, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(0, letterWidth, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(2*letterWidth, 0, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(0, 1.5*letterWidth, speed);
	moveLinear(-(3*letterWidth), 0, speed);
	pressBottle(false);
}

/********************************************************
*				Cijfer 6
********************************************************/
void draw6(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(0, 3.5*letterWidth, speed);
	pressBottle(true);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(2*letterWidth, 0, speed);
	moveLinear(0.5*letterWidth, -(0.5*letterWidth), speed);
	moveLinear(0, -(3*letterWidth), speed);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	moveLinear(-(2*letterWidth), 0, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(0, letterWidth, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(2.5*letterWidth, 0, speed);
	pressBottle(false);
}

/********************************************************
*				Cijfer 7
********************************************************/
void draw7(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(1.5*letterWidth, 0, speed);
	pressBottle(true);
	moveLinear(-(1.5*letterWidth), 4*letterWidth, speed);
	moveLinear(3*letterWidth, 0, speed);
	pressBottle(false);
}

/********************************************************
*				Cijfer 8
********************************************************/
void draw8(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(0.5*letterWidth, 2*letterWidth, speed);
	pressBottle(true);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	moveLinear(0, -letterWidth, speed);
	moveLinear(0.5*letterWidth, -(0.5*letterWidth), speed);
	moveLinear(2*letterWidth, 0, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(0, letterWidth, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(-(2*letterWidth), 0, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(0, letterWidth, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(2*letterWidth, 0, speed);
	moveLinear(0.5*letterWidth, -(0.5*letterWidth), speed);
	moveLinear(0, -letterWidth, speed);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	pressBottle(false);
}

/********************************************************
*				Cijfer 9
********************************************************/
void draw9(int speed){
	pressBottle(false);
	moveLinear(margin, margin, speed);
	moveLinear(3*letterWidth, 0.5*letterWidth, speed);
	pressBottle(true);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	moveLinear(-(2*letterWidth), 0, speed);
	moveLinear(-(0.5*letterWidth), 0.5*letterWidth, speed);
	moveLinear(0, 3*letterWidth, speed);
	moveLinear(0.5*letterWidth, 0.5*letterWidth, speed);
	moveLinear(2*letterWidth, 0, speed);
	moveLinear(0.5*letterWidth, -(0.5*letterWidth), speed);
	moveLinear(0, -letterWidth, speed);
	moveLinear(-(0.5*letterWidth), -(0.5*letterWidth), speed);
	moveLinear(-(2.5*letterWidth), 0, speed);
	pressBottle(false);
}
/********************************************************
*   Vakken letters
********************************************************/

void makeFields(){
	letterBoxHeight = gridHeight - margin*2;
	letterBoxWidth = gridWidth/6 - margin*2;
	letterWidth = (letterBoxWidth-(2*margin))/3;
}

void writeText(char *text, int speed) {
	//add spaces to center text

	makeFields();
	moveLinear(margin, margin, 20);

	for(int i = 0; i < strlen(text);i++){
		switch(text[i]){
			case 'a': drawA(speed);break;
			case 'b': drawB(speed);break;
			case 'c': drawC(speed);break;
			case 'd': drawD(speed);break;
			case 'e': drawE(speed);break;
			case 'f': drawF(speed);break;
			case 'g': drawG(speed);break;
			case 'h': drawH(speed);break;
			case 'i': drawI(speed);break;
			case 'j': drawJ(speed);break;
			case 'k': drawK(speed);break;
			case 'l': drawL(speed);break;
			case 'm': drawM(speed);break;
			case 'n': drawN(speed);break;
			case 'o': drawO(speed);break;
			case 'p': drawP(speed);break;
			case 'q': drawQ(speed);break;
			case 'r': drawR(speed);break;
			case 's': drawS(speed);break;
			case 't': drawT(speed);break;
			case 'u': drawU(speed);break;
			case 'v': drawV(speed);break;
			case 'w': drawW(speed);break;
			case 'x': drawX(speed);break;
			case 'y': drawY(speed);break;
			case 'z': drawZ(speed);break;

			case '0': drawO(speed);break;
			case '1': draw1(speed);break;
			case '2': draw2(speed);break;
			case '3': draw3(speed);break;
			case '4': draw4(speed);break;
			case '5': draw5(speed);break;
			case '6': draw6(speed);break;
			case '7': draw7(speed);break;
			case '8': draw8(speed);break;
			case '9': draw9(speed);break;

			default: break;

		}
		moveLinear(letterBoxWidth, 0 , speed);
	}
}
