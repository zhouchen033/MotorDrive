// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       motor.ino
    Created:	2019/5/13 8:24:15
    Author:     Raymond-PC\Raymond
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts
const int IN1 = 3;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 9;

byte MSG_STATE = 0;
byte MSG_ID = -1;

byte payload[5];
byte payload_size = 0;

void setup()
{
	Serial.begin(9600);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
}

// Add the main program code into the continuous loop() function
void parseMsg(char c) {
	byte b = (byte)c;
	switch (MSG_STATE) {
	case 0x00:
		//Serial.println("STATE 00");
		if (b == 0xAA) {
			MSG_STATE = 1;
			MSG_ID = -1;
			payload_size = 0;
		}
		break;
	case 0x01:
		if ( b == 0xF1 ) {
			MSG_STATE = 2;
			MSG_ID = b;
		}
		else {
			MSG_STATE = 0;
		}
		break;
	case 0x02:
		if ( b == 0xAB ) {
			MSG_STATE = 0;
		}
		else {
			payload[payload_size++] = b;
		}
		break;
	}
}

void driveMotor() {
	if (MSG_STATE == 0 && MSG_ID != -1) {
		if ( MSG_ID == 0xF1 ) {
			byte delta = 255 - 100;
			byte vx = payload[0];
			byte vy = payload[1];
			if ( vx >= 50 ) {
				byte delta_x = vx - 50;
				if (delta_x > 15) {
					analogWrite(IN1, 0);
					analogWrite(IN2, delta*((vx - 50) / 50) + 80 + (vy-50)/3);
					analogWrite(IN3, 0);
					analogWrite(IN4, delta*((vx - 50) / 50) + 80 + (50 - vy) / 3);
				}
				else {
					analogWrite(IN1, 0);
					analogWrite(IN2, 0);
					analogWrite(IN3, 0);
					analogWrite(IN4, 0);
				}
			}
			else {
				byte delta_x = 50 - vx;
				if (delta_x > 15) {
					analogWrite(IN2, 0);
					analogWrite(IN1, delta*((vx - 50) / 50) + 80 +(vy - 50) / 3);
					analogWrite(IN4, 0);
					analogWrite(IN3, delta*((vx - 50) / 50) + 80 + (50 - vy) / 3);
				}
				else {
					analogWrite(IN1, 0);
					analogWrite(IN2, 0);
					analogWrite(IN3, 0);
					analogWrite(IN4, 0);
				}
			}
		}
	}
}

void loop()
{
	char c = 0;
	while ( Serial.available() > 0 ) {
		parseMsg(Serial.read());
	}
	driveMotor();
	delay(100);

	// min - 130
	/*analogWrite(IN1, 0);
	analogWrite(IN2, 255);
	analogWrite(IN3, 0);
	analogWrite(IN4, 130);*/

}
