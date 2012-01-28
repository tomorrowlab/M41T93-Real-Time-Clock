#include <SPI.h>
#include <RTClib.h>
#include <RTC_M41T93.h>

//Chip select for the RTC, which is wired to the SPI interface
//This is being used because I have multiple SPI devices, 
//and am using pin 19 as the chip select for the RTC.
//When using multiple SPI devices, you MUST set the Atmels' default SPI port to OUTPUT
//so that it acts as a master, not a slave
int RTC_cs = 19; 
RTC_M41T93 RTC(RTC_cs);

void setup () {
  Serial.begin(9600);

  //FOR SPI. To use pins other than pin20 (SS) for Chip Select, set it to ouput
  pinMode(20, OUTPUT); //pin 20 is the SPI CS/SS pin on the ATMEL AT90USB1286
  pinMode(RTC_cs, OUTPUT); //Chip select for the RTC. Other SPI chip select pins go here

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);

  RTC_open(); //Clears the Halt Bit, cycles the Stop Bit.
  RTC.begin(); //For Power up. Clears the Halt Bit and the Stop Bit.
  RTC.adjust(DateTime(__DATE__, __TIME__)); //sets the date and time at the moment of compiling this sketch.

  RTC_close();


}

void loop () {

  RTC_open();

  Serial.println(get_unix_time());
  Serial.println(get_formatted_time());

  RTC_close();

  delay(1000);
}



void RTC_open() {
  SPI.setDataMode(SPI_MODE0); //IMPORTANT - Set to Mode0 - low clock in high clock out
  digitalWrite(RTC_cs, LOW); //turn on RTC

}
void RTC_close() {
  digitalWrite(RTC_cs, HIGH); //turn off RTC
}


String get_formatted_time() {
  RTC_open();
  const int len = 32;
  static char buf[len];
  DateTime now = RTC.now();
  return (now.toString(buf,len));
  RTC_close();
}


String get_unix_time() {
  RTC_open();
  const int len = 32;
  static char buf[len];
  DateTime now = RTC.now();
  return (now.unixtime());
  RTC_close();
}



