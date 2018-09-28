
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
/********************************************************
*   Set Global Constants and Variables
********************************************************/
task monitorPlotter();
void calibrateX();
void calibrateY();
void pressBottle(bool enabled);
void moveLinear(int amountToMoveX, int amountToMoveY, int speed);
void moveEllipse(int xCenter, int yCenter, int xradius, int yradius, int finalangle, int speed);


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
int letterWidth; // formula: (letterBoxWidth-(2*margin))/3;


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


// TODO: calibrate simultaneously
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
void moveLinear(int amountToMoveX, int amountToMoveY, int speed){
	//100, 0, 20 => 1,20,20
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
				setMotorSync(xMotor,xMotor2,0,xSpeed);
			}
			else if(xDesinationEncoder < (getMotorEncoder(xMotor)+unprecision)){
				setMotorSync(xMotor,xMotor2,0,-xSpeed);
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
*   Move circular
********************************************************/
/*void moveCircle(int xCenter, int yCenter, int radius, int finalAngle, int speed){
	moveLinear(xCenter, yCenter-radius,speed);
	int angle = 0;
	int step = 4;
	while(angle <=finalAngle){
		int xPoint = xCenter + radius * sinDegrees(angle);
		int yPoint = yCenter + radius * -cosDegrees(angle);
		moveLinear(xPoint - xPosition, yPoint - yPosition,speed);
		angle += step;
	}

}*/


/********************************************************
*   Move Elipse
********************************************************/
void moveEllipse (int xCenter, int yCenter, int xradius, int yradius, int finalangle, int speed){
    int preSin = 1;
    int preCos = 1;
    int radius = 1;
    int angle = 0;
    int step = 4;

    if (xradius > yradius){
        preSin = xradius/yradius;
        radius = yradius;
    }
    else if (xradius < yradius){
        preCos = yradius/xradius;
        radius = xradius;

    }
    moveLinear(xCenter, yCenter-yradius, speed);
    while (angle <= finalangle){
        int xPoint = xCenter + radius * (preSin * sinDegrees(angle);
        int yPoint = yCenter + radius * (preCos * -cosDegrees(angle);

				moveLinear(xPoint - xPosition, yPoint - yPosition,speed);
        angle += step;

    }

}

/********************************************************
*   Draw Square
********************************************************/
void drawSquare(int speed){
	moveLinear(20,20,speed);
	moveLinear(150,0,speed);
	moveLinear(0,120,speed);
	moveLinear(-150,0,speed);
	moveLinear(0,-120,speed);
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
*   Write the word "LEGO!"
********************************************************/
void WriteLEGO(){
	moveLinear(10,10,20);
	// Letter 'L'
	moveLinear(10,0,20);
	moveLinear(0,40,20);
	moveLinear(30,0,20);
	moveLinear(0,10,20);
	moveLinear(-40,0,20);
	moveLinear(0,-55,20);

	moveLinear(60,0,20);

	// Letter 'E'
	moveLinear(40,0,20);
	moveLinear(0,15,20);
	moveLinear(-30,0,20);
	moveLinear(0,10,20);
	moveLinear(20,0,20);
	moveLinear(0,10,20);
	moveLinear(-20,0,20);
	moveLinear(0,5,20);
	moveLinear(30,0,20);
	moveLinear(0,10,20);
	moveLinear(-40,0,20);
	moveLinear(0,-50,20);

	moveLinear(60,0,20);

	// Letter 'G'
	moveLinear(40,0,20);
	moveLinear(0,15,20);
	moveLinear(-30,0,20);
	moveLinear(0,30,20);
	moveLinear(20,0,20);
	moveLinear(0,-15,20);
	moveLinear(-10,0,20);
	moveLinear(0,-10,20);
	moveLinear(20,0,20);
	moveLinear(0,30,20);
	moveLinear(-40,0,20);
	moveLinear(0,-50,20);

	moveLinear(60,0,20);

	// Letter '0'
	moveLinear(40,0,20);
	moveLinear(0,50,20);
	moveLinear(-40,0,20);
	moveLinear(0,-55,20);
	moveLinear(10,15,20);
	moveLinear(20,0,20);
	moveLinear(0,30,20);
	moveLinear(-20,0,20);
	moveLinear(0,-35,20);

	moveLinear(60,0,20);

	// Letter '!'
	moveLinear(10,0,20);
	moveLinear(0,30,20);
	moveLinear(-10,0,20);
	moveLinear(-30,0,20);
	moveLinear(0,40,20);
	moveLinear(10,0,20);
	moveLinear(0,10,20);
	moveLinear(-10,0,20);
	moveLinear(0,10,20);

	moveLinear(-60,100,20);

	wait1Msec(10000);
}

/********************************************************
*		Draw Smiley
********************************************************/
void drawSmiley(int xCenter, int yCenter, int radius, int speed){
    pressBottle(false);
    moveLinear(xCenter, yCenter-radius, speed)

    pressBottle(true);
    moveEllipse(xCenter, yCenter, radius, radius, 360, speed);
    pressBottle(false);
    float mouthstartx = xCenter - (0.075 * radius);
    float mouthstarty = yCenter - (0.5 * radius);
    moveLinear(mouthstartx - xPosition, mouthstarty - yPosition, speed);

    pressBottle(true);
    moveEllipse(xCenter, yCenter- (0.3 * radius), 0.5 * radius, 0.35 * radius, 180, speed);
    moveLinear(-radius, 0, speed)

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
/******************************************************
*		Letter L
******************************************************/
void drawL(int speed){
	pressBottle(false);
	moveLinear(margin,margin,speed);
	pressBottle(true);
	moveLinear(0, letterBoxHeight-(2*margin), speed);
	moveLinear(letterWidth, 0 , speed);
	moveLinear(-margin, -margin, speed);
}

/********************************************************
*   Vakken letters
********************************************************/

void makeFields(){
	letterBoxHeight = gridHeight - margin*2;
	letterBoxWidth = gridWidth/6 - margin*2;
	letterWidth = (letterBoxWidth-(2*margin))/3;
}

/*void writeText(string text, int speed) {
	char *chars = text;
	//add spaces to center text

	makeFields();
	moveLinear(margin, margin, 20);

	for(int i = 0; i < strlen(text);i++){
		switch(chars[i]){
		case "a": drawA(speed);break;
		case "b": drawB(speed);break;
		case "c": drawC(speed);break;
		case "d": drawD(speed);break;
		case "e": drawE(speed);break;
		case "f": drawF(speed);break;
		case "g": drawG(speed);break;
		case "h": drawH(speed);break;
		case "i": drawI(speed);break;
		case "j": drawJ(speed);break;
		case "k": drawK(speed);break;
		case "l": drawL(speed);break;
		case "m": drawM(speed);break;
		case "n": drawN(speed);break;
		case "o": drawO(speed);break;
		case "p": drawP(speed);break;
		case "q": drawQ(speed);break;
		case "r": drawR(speed);break;
		case "s": drawS(speed);break;
		case "t": drawT(speed);break;
		case "u": drawU(speed);break;
		case "v": drawV(speed);break;
		case "w": drawW(speed);break;
		case "x": drawX(speed);break;
		case "y": drawY(speed);break;
		case "z": drawZ(speed);break;

		case "0": draw0(speed);break;
		case "1": draw1(speed);break;
		case "2": draw2(speed);break;
		case "3": draw3(speed);break;
		case "4": draw4(speed);break;
		case "5": draw5(speed);break;
		case "6": draw6(speed);break;
		case "7": draw7(speed);break;
		case "8": draw8(speed);break;
		case "9": draw9(speed);break;

		default: break;

		}
		moveLinear(letterBoxWidth, 0 , speed);
	}
}*/
