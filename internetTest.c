////////////////////////////////////////////////////////////////////////////////
// ROBOTC EV3 remote control demonstration program.
// This program receives messages from the host computer: Forward, Backward, Left and Right.
// It sends back the distance in cm measured by the ultrasonic sensor and the angle in degrees
// measured by the gyro sensor.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Connection scheme:
// motorA: medium motor front lever
// motorB: large motor right wheel
// motorC: large motor left wheel
// motorD: -
// Sensor S1: Touch sensor
// Sensor S2: Gyro sensor
// Sensor S3: Color sensor
// Sensor S4: Ultrasonic sensor
////////////////////////////////////////////////////////////////////////////////


// EV3Mailbox.c is needed to enable sending and receiving mailbox messages.
// It is possible to have ten input mailboxes and one output mailbox.
// However in the below code only EV3_INBOX0 and EV3_OUTBOX0 are used.
// See the comments in EV3Mailbox.c for additional info.
// Unfortunately ROBOTC cannot handle multipe C files so we have to include it.
#include "EV3Mailbox.c"


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
		// Use the format ?<distance> <angle>" (space between distance and angle) so that it can be read by the host program.

		writeMailboxOut("EV3_OUTBOX0", msgBufOut);

		delay(100);  // Wait 100 ms to give host computer time to react.
	}
	closeMailboxIn("EV3_INBOX0");
	closeMailboxOut("EV3_OUTBOX0");
	return;
}
