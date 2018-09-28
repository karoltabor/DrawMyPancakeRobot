#pragma config(Sensor, S1,     xHome,          sensorEV3_Touch)
#pragma config(Sensor, S3,     yHome,          sensorEV3_Touch)
#pragma config(Sensor, S4,     xHome2,         sensorEV3_Touch)
#pragma config(Motor,  motorA,          xMotor,        tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          yMotor,        tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,          xMotor2,       tmotorEV3_Large, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "EV3Mailbox.c"
#include "xy-plot.c"

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
			eraseDisplay();
			startTask(monitorPlotter);
			wait1Msec(2000);
			calibrate();

			if (strcmp(msgBufIn, "Forward ") == 0) {
				setMotorSyncEncoder(motorA, motorB, 0, 50, 50);
				} else if (strcmp(msgBufIn, "Backward") == 0) {
				setMotorSyncEncoder(motorA, motorB, 0, -50, -50);
				} else if (strcmp(msgBufIn, "Left") == 0) {
				setMotorSyncEncoder(motorA, motorB, 50, 50, 50);
				} else if (strcmp(msgBufIn, "Right") == 0) {
				setMotorSyncEncoder(motorA, motorB, -50, 50, 50);
				} else if (strcmp(msgBufIn, "Circle") == 0) {
				moveEllipse(60, 100, 50, 50, 360, 20);
				//drawSmiley(50, 50, 50, 20);
				//fillCircle(100, 20, 5, 50);
				} else if (strcmp(msgBufIn, "Heart") == 0) {
				drawHeart(100);
				} else if (strcmp(msgBufIn, "Square") == 0) {
				//drawSquare(30);
				fillCircle(70, 20, 5, 20);
				} else if (strcmp(msgBufIn, "Triangle") == 0) {
				drawTriangle(30, 40);
			} else if (strcmp(msgBufIn, "Text") == 0) {}
			wait1Msec(1000);
			calibrate();
		}
		else
		{
			displayBigTextLine(8, "empty message!");
		}


		// Here fill your outgoing message handling.
		// This means sending the distance in cm measured by the ultrasonic sensor
		// and the angle in degrees measured by the gyro sensor.
		// Put both values in msgBufOut.
		// Use the format ?<distance> <angle>" (space between distance and angle) so that it can be read by the host program.

		writeMailboxOut("EV3_OUTBOX0", msgBufOut);

		delay(100);  // Wait 100 ms to give host computer time to react.
	}
	closeMailboxIn("EV3_INBOX0");
	closeMailboxOut("EV3_OUTBOX0");
	return;
}
