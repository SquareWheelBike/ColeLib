#include <DS3231.h>

// function call memory bits available
	boolean oneShotBits[16];			// oneshot bits available for use for oneshot or toggle calls
										// BITS 32-63 ARE FOR FAULTS ONLY!!	
	
	uint8_t ONSTracker;
	// uint8_t debounceTracker;
	
	boolean timerInSession[16];		// for speed, so we only update timer timers when needed
	boolean timerMemory[sizeof(timerInSession)];			// make function calls smaller by remembering previous output state
	unsigned long timerTimers[sizeof(timerInSession)];	// debounce timers available for use
	uint8_t timerTracker;

    // clockRoutine Variables
	RTClib RTC;

void loop()		
{
	ONSTracker = 0;
	// debounceTracker = 0;
	timerTracker = 0;
	
}


// FUNCTION CALLS
boolean FlasherBit(float hz)
{
	int T = round(1000.0 / hz);
	if ( millis() % T >= T/2 ) return 1;
	else return 0;
}

boolean oneShot(boolean precond, uint8_t OSR)
{
	// use global memory to keep track of oneshot bits
	if (precond == 1 && oneShotBits[OSR] == 0)
	{
		oneShotBits[OSR] = 1;
		return 1;
	}
	else if (precond == 0 && oneShotBits[OSR] == 1)
	{
		oneShotBits[OSR] = 0;
		return 0;
	}
	else return 0;
}

boolean toggleState(boolean precond, boolean toggled, uint8_t OSR)
{
	if (oneShot(precond, OSR)) toggled = !toggled;
	return toggled;
}

float voltsFromAnalogIn (int input)
{
	float output = ((float)input * 5.0) / 1024.0;
	return output;
}

int voltsToAnalogIn (float input)
{
	int output = (int)((input * 1024.0) / 5.0);
	return output;
}

int voltsToAnalogOut(float input)
{
	int output = (input * 255.0 / 5.0);
	return output;
}

boolean TON(boolean input, int preset, int timerNumber)
{
	if (input && !timerInSession[timerNumber]) timerTimers[timerNumber] = millis();
	else if (input && timerMemory[timerNumber]) return 1;
	else if (input && millis() - timerTimers[timerNumber] >= preset) 
	{
		timerMemory[timerNumber] = 1;
		return 1;
	}
	else;
	timerMemory[timerNumber] = 0;
	timerInSession[timerNumber] = input;
	return 0;
}

boolean limit(float input, float lower, float upper) 
{
	if (input < lower) return 0;
	else if (input > upper) return 0;
	else return 1;
}

String currentTime()
{
	char clockBuffer[14] = "--:--:--";
    DateTime now = RTC.now();
	sprintf(clockBuffer,"%02u:%02u:%02u",now.hour(),now.minute(),now.second());
	return String(clockBuffer);
}

String currentDate()
{	
	// if (!(auxPower || clockDebugActive)) return "----/--/--";
	char clockBuffer[14] = "----/--/--";
    DateTime now = RTC.now();
	sprintf(clockBuffer,"%04u/%02u/%02u",now.year(),now.month(),now.day());
	return String(clockBuffer);
}

String secondsToClock(int input)
{
	int seconds, minutes, hours;
	
	seconds = input % 60;
	minutes = (input / 60) % 60;
	hours = input / 3600;
	
	char outputBuffer[14];
	sprintf(outputBuffer,"%02u:%02u:%02u",hours,minutes,seconds);
	
	return String(outputBuffer);
}