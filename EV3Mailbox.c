#define MAX_MAILBOXNAME_LENGTH 20
#define MAX_MSG_LENGTH 32


// Function: Open input mailbox.
// Parameters: name of the mailbox to open.
// Return value: void.
void openMailboxIn(char *name)
{
	// To keep it simple, we only use mailbox 0.
	// Because of a ROBOTC limitation, the name must be equal to the index as a string.
	int index;
	char internal_name[MAX_MAILBOXNAME_LENGTH];
	sscanf(name, "EV3_INBOX%d", &index);
	sscanf(name, "EV3_INBOX%s", internal_name);
	if (index >=0 && index < 10)
	{
		openMailbox(index, internal_name);
		writeDebugStreamLine("Opened mailbox %d", index);
	}
}


// Function: Close input mailbox.
// Parameters: name of the mailbox to close.
// Return value: void.
void closeMailboxIn(char *name)
{
	int index;
	char internal_name[MAX_MAILBOXNAME_LENGTH];
	sscanf(name, "EV3_INBOX%d", &index);
	sscanf(name, "EV3_INBOX%s", internal_name);
	if (index >=0 && index < 10)
	{
		closeMailbox(index);
	}
}


// Function: Read input mailbox. Non-blocking function.
// Parameters:
//   name: the name of the mailbox to read
// Return value: message read as a string. If no message available, an empty string is returned.
void readMailboxIn(char *name, char *msg)
{
	int index = -1;
	int size;
	strcpy(msg, "");
	sscanf(name, "EV3_INBOX%d", &index);
	if (index >=0 && index < 10)
	{
		size = getMailboxMessageSize(index);
		if (size > 0)
		{
			readMailbox(index, msg, MAX_MSG_LENGTH);
			writeDebugStreamLine("Read mailbox %d: %s", index, msg);
		}
	}
}
