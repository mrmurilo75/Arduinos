const byte display[]={2,12,4,5,6,7,8};
const byte RPin=9, GPin=10, BPin=11, LPin=3;
const byte buttonPin=13;

boolean buttonPressed(){
	static boolean p=false;
	static byte buttonState=LOW, buttonPrevState=buttonState, ledState=LOW;

	if((buttonState=digitalRead(buttonPin))==buttonPrevState)
		return false;
	if(buttonState==LOW){
		buttonPrevState=LOW;
		return true;
	}
	buttonPrevState=buttonState;
	return false;
}

void start7Segment(){
	for(byte s=0;s<=5;s++)
		digitalWrite(display[s], 1);
	digitalWrite(display[6], 0);
}
	

byte addTo7Segment(){
	static byte i=0;

	switch((++i)%=13){
		case 0: digitalWrite(display[0], 1); digitalWrite(display[5], 1);
			digitalWrite(display[6], 0);
			break;
		case 1: digitalWrite(display[0], 0); digitalWrite(display[3], 0); digitalWrite(display[4], 0); digitalWrite(display[5], 0);
			break;
		case 2: digitalWrite(display[0], 1); digitalWrite(display[3], 1); digitalWrite(display[4], 1); digitalWrite(display[6], 1);
			digitalWrite(display[2], 0);
			break;
		case 3:	digitalWrite(display[2], 1);
			digitalWrite(display[4], 0);
			break;
		case 4: digitalWrite(display[5], 1);
			digitalWrite(display[0], 0); digitalWrite(display[3], 0);
			break;
		case 5:	digitalWrite(display[0], 1); digitalWrite(display[3], 1);
			digitalWrite(display[1], 0);
			break;
		case 6:	digitalWrite(display[4], 1);
			break;
		case 7: digitalWrite(display[1], 1);
			digitalWrite(display[3], 0); digitalWrite(display[4], 0); digitalWrite(display[6], 0);
			break;
		case 8: digitalWrite(display[3], 1); digitalWrite(display[4], 1); digitalWrite(display[6], 1);
			break;
		case 9: digitalWrite(display[4], 0);
			break;
		case 10: digitalWrite(display[4], 1);
			 digitalWrite(display[1], 0); digitalWrite(display[5], 0);
			 break;
		case 11: digitalWrite(display[0], 0);
			 break;
		case 12: digitalWrite(display[1], 1);
			 break;
	}
	return i;
}

void startAlarm(byte alarm){
	if(alarm==0)
		return;

	for(int s=0; s<7; s++)
		digitalWrite(display[s], 0);
	digitalWrite(RPin, 1);
	digitalWrite(GPin, 1);
	digitalWrite(BPin, 1);
	
	delay((alarm-1)*3600000+1800000);

	long time=millis(), s=0;
	for(int res=1, s=1; (res)!=0; res=s%256){
		if(millis()-time>(long)14000){
			s++;
			time=millis();
		}
		analogWrite(RPin, 255-res);
		analogWrite(GPin, 255-res/2);
		if(res>127)
			analogWrite(BPin, 382-res);
	}
	delay(3600000);
}


int stateHVS(int st, const int res){
	if(st<3){
		switch(st%3){
			case 0: return 255;
			case 1: return 255-res;
			case 2: return res;
		}
	}
	switch(st%3){
		case 0: return 255;
		case 1: return 255-(res)/2;
		case 2: return res/2+128;
	}
		
}

void powerHVS(){
	static long anal=millis();

	static long s=0;
	static int res=0, is=0;
	if(millis()-anal >(long)30){
		s++;
		if((res=s%256)==0)
			is++;
		analogWrite(RPin, stateHVS((is)%3, res));
		analogWrite(GPin, stateHVS((is+1)%3+3, res));
		analogWrite(BPin, stateHVS((is+2)%3+3, res));
		anal=millis();
	}
}

void seg7InfinitCircle(){
	const int seq[]={0,1,6,4,3,2,6,5};
	
	static long i=0, time=millis();
	digitalWrite(display[seq[i]], HIGH);
	if(millis()-time>(long)200){
		digitalWrite(display[seq[i++]], LOW);
		time=millis();
	}
	if(i==8)
		i=0;

}


void setup(){
	for(int i=2; i<13; i++)
		pinMode(i, OUTPUT);
	pinMode(13, INPUT);

	digitalWrite(LPin, 1);

	start7Segment();
}

void loop(){

	//delay(8*60*60*1000);
	
	static byte alarm=0, p=-1;
	static long time=millis();
	if(p && buttonPressed()){
		alarm=addTo7Segment();
		time=millis();
	}

	if(p && millis()-time>(long)5000){
		p=0;
		startAlarm(alarm);
	}

	powerHVS();
	
	if(p==0) seg7InfinitCircle();

}
