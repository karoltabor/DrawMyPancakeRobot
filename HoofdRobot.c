#pragma config(Sensor, S1,     xHome,          sensorEV3_Touch)
#pragma config(Sensor, S2,     xHome2,         sensorEV3_Touch)
#pragma config(Sensor, S3,     yHome,          sensorEV3_Touch)
#pragma config(Motor,  motorA,          xMotor,        tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          yMotor,        tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorC,          xMotor2,       tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorD,          pumpMotor,     tmotorEV3_Large, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "EV3Mailbox.c"
#include "xy-plot.c"

task main()
{
	displayBigTextLine(0, "Started!");

	char action[MAX_MSG_LENGTH];
	char payload[MAX_MSG_LENGTH];

	openMailboxIn("EV3_INBOX0");
	openMailboxIn("EV3_INBOX1");

	while (true)
	{
		readMailboxIn("EV3_INBOX0", action);
		readMailboxIn("EV3_INBOX1", payload);

		//wait untill payload is received
		while( (strcmp(action, "Text") == 0 || strcmp(action, "FreeDraw") == 0) && !(strlen(payload) > 0)){
			readMailboxIn("EV3_INBOX1", payload);
		}

		if (strcmp(action, "") != 0)
		{
			playSoundFile("Download");
			eraseDisplay();
			startTask(monitorPlotter);
			wait1Msec(2000);
			calibrate();


			/*******************************************
			* 						Warm up the Pump						 *
			********************************************/
			delay(2000);
			pressBottle(true);
			delay(15000);
			pressBottle(false);

			/*******************************************
			* read input to determine action					 *
			* strings in freeDraw are split in xy-plot *
			********************************************/
			if (strcmp(action, "Circle") == 0) {
				moveEllipse((gridWidth/10-margin), (gridHeight/10-margin), 50, 50, 360, 20);
			} else if (strcmp(action, "Heart") == 0) {
				drawHeart(100);
			} else if (strcmp(action, "Square") == 0) {
				drawSquare(75,75,20);
			} else if (strcmp(action, "Triangle") == 0) {
				drawTriangle(30, 40);
			} else if (strcmp(action, "Star") == 0) {
				drawStar(50, 40, 20);
			} else if (strcmp(action, "Spiral") == 0) {
				drawSpiral(50, 50, 50, 20);
			} else if (strcmp(action, "Eiffel") == 0) {
				drawEiffel(99, 20);
			} else if (strcmp(action, "Smiley") == 0) {
				drawSmiley(50,50,50,20);
			} else if (strcmp(action, "Text") == 0) {
				writeText(payload, 150, 75, 20);
			} else if (strcmp(action, "FreeDraw") == 0) {
				//iterate through instructions
				while (strcmp(payload, "") != 0) {
					freeDraw(payload, 20);
				}
			}

			calibrate();

			//fill the pancake
			if(strcmp(action, "Text") == 0){
				drawSquare(150, 75, 15);
				calibrate();
				//wait a few seconds to give the drawing time to get some color
				wait1Msec(30000); //TODO: change the amount of seconds to wait
				fillRectangle(145, 70, 1, 20);
			} else {
				drawSquare(100, 100, 15);
				calibrate();
				//wait a few seconds to give the drawing time to get some color
				wait1Msec(30000); //TODO: change the amount of seconds to wait
				fillRectangle(95, 95, 3, 20);
			}

			//wait a few seconds to bake the pancake
			wait1Msec(1000); //TODO: change the amount of seconds to wait

			playSoundFile("Thank you");
			calibrate();
		}
		else
		{
			displayBigTextLine(8, "empty message!");
		}
	}
	closeMailboxIn("EV3_INBOX0");
	closeMailboxIn("EV3_INBOX1");
	return;
}
