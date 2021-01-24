const char display[]={2,12,4,5,6,7,8};
const char RPin=9, GPin=10, BPin=11, LPin=3;


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

	digitalWrite(LPin, 1);

}

void loop(){

	//delay(8*60*60*1000);

	powerHVS();

	seg7InfinitCircle();

}
