/*
* Open serial monitor, press one of the keys below and click 'Send':
* 'S' - to start all timers
* 'P' - to pause all timers
* 'R' - to restart all timers
* 'T' - to stop all timers
*/
#include "MultiTimer.h"

//MultiTimer //tUp;
MultiTimer tDown;

void setup()
{
	Serial.begin(9600);
	pinMode(13, OUTPUT);
        
    // Count-up timer with 10s
	//tUp.config(0, 0, 0, 10, tUp.COUNT_UP, print_time1, tUpComplete);
    // Call print_time1() method every 1s.

    // Count-down timer with 21s
	tDown.config(0, 0, 0, 21, tDown.COUNT_DOWN, print_time2, tDownComplete);
    // Call print_time2() method every 1s.

	Serial.println("Press one of the keys below and click 'Send':");
	Serial.println("'S' - to start all timers");
	Serial.println("'P' - to pause all timers");
	Serial.println("'R' - to restart all timers");
	Serial.println("'T' - to stop all timers\n");
	Serial.print("tDown: "); Serial.println(tDown.getCurrentTime(AUTO_DISPLAY)); Serial.println("");
}

void loop()
{
	
	tDown.run(USE_MILLIS, 0);

	if (Serial.available() > 0)
	{
		char c = toupper(Serial.read());

		switch (c)
		{
			case 'T':
				//tUp.stop();
				tDown.stop();
				digitalWrite(13, LOW);
				break;
			case 'R':
				//tUp.restart();
				tDown.restart();
				break;
			case 'S':
				//tUp.start();
				tDown.start();
				break;
			case 'P':
				//tUp.pause();
				tDown.pause();
				digitalWrite(13, LOW);
				break;
			default:
				break;
		}
	}
}

void print_time2()
{
	Serial.print("tDown: ");
	Serial.println(tDown.getCurrentTime(AUTO_DISPLAY));
	digitalWrite(13, heartbeat);
}

void tDownComplete()
{
	Serial.print("tDown Complete");
	digitalWrite(13, HIGH);
}
