/*

RTC command library for the ST Micro's M41T93 Real Time Clock
Authored by Ted Ullrich, http://tomorrow-lab.com
Co-Authored by Justin Downs, http://groundlab.cc
January 2012
Built to work with the RTClib created by JeeLabs http://news.jeelabs.org/code/
Released to the public domain.

*/


#include <avr/pgmspace.h>
#include <WProgram.h>
#include <SPI.h>
#include "RTClib.h"
#include "RTC_M41T93.h"

#define makewriteAddress(address) (address | 0x80);
uint8_t myAddress;

/*
IMPORTANT - Set SPI to Mode0 - low clock IN, high clock OUT.
 SPI.setDataMode(SPI_MODE0); is required for this part! If you change the datamode (for example, to communicate with other components over the SPI lines, that's ok, just set it back to Mode0 when communicating with this chip.
 */

void RTC_M41T93::cs(int _value)
{
  SPI.setDataMode(SPI_MODE0);
  digitalWrite(cs_pin,_value);
}

uint8_t RTC_M41T93::begin(void)
{
  //POWER UP:

  //1. Clear Halt Bit (HT)
  myAddress = makewriteAddress(0x0C); //Set halt register as write
  cs(LOW);
  SPI.transfer(myAddress); //ADDRESS OF HALT WRITE MODE
  SPI.transfer(0x00); //clear bit 6
  cs(HIGH);


  // 2. Reset Stop Bit (ST) D7 bit to 1 then 0 (stop start clock, inits)
  myAddress = makewriteAddress(0x01); //Seconds register as write
  cs(LOW);
  SPI.transfer(myAddress); //ADRESS OF HALT WRITE MODE
  SPI.transfer(0x80); //ST bit high, setting it to 1
  SPI.transfer(myAddress); //ADRESS OF HALT WRITE MODE
  SPI.transfer(0x00); //ST bit low, setting it to 0
  cs(HIGH);

}







void RTC_M41T93::adjust(const DateTime& dt)
{
  cs(LOW);
  myAddress = makewriteAddress(0x00); //Set 00 reg as write
  SPI.transfer(myAddress); //ADDRESS OF milliseconds
  SPI.transfer(bin2bcd(0)); //Set to 0. Don't need this level of accuracy.
  cs(HIGH);

  cs(LOW);
  myAddress = makewriteAddress(0x01); //Set 01 reg as write
  SPI.transfer(myAddress); //ADDRESS OF seconds
  SPI.transfer(bin2bcd(dt.second())); //set value
  cs(HIGH);


  cs(LOW);
  myAddress = makewriteAddress(0x02); //Set 02 reg as write
  SPI.transfer(myAddress); //ADDRESS OF minutes
  SPI.transfer(bin2bcd(dt.minute())); //set value
  cs(HIGH);

  cs(LOW);
  myAddress = makewriteAddress(0x03); //Set 03 reg as write
  SPI.transfer(myAddress); //ADDRESS OF hours
  SPI.transfer(bin2bcd(dt.hour())); //set value
  cs(HIGH);


  cs(LOW);
  myAddress = makewriteAddress(0x04); //Set 04 reg as write
  SPI.transfer(myAddress); //ADDRESS OF day of week
  SPI.transfer(bin2bcd(dt.dayOfWeek())); //set value
  cs(HIGH);



  cs(LOW);
  myAddress = makewriteAddress(0x05); //Set 05 reg as write
  SPI.transfer(myAddress); //ADDRESS OF day (date)
  SPI.transfer(bin2bcd(dt.day())); //set value
  cs(HIGH);


  cs(LOW);
  myAddress = makewriteAddress(0x06); //Set 06 reg as write
  SPI.transfer(myAddress); //ADDRESS OF month
  SPI.transfer(bin2bcd(dt.month())); //set value
  cs(HIGH);


  cs(LOW);
  myAddress = makewriteAddress(0x07); //Set 07 reg as write
  SPI.transfer(myAddress); //ADDRESS OF year
  SPI.transfer(bin2bcd(dt.year() - 2000)); //set value
  cs(HIGH);




}

DateTime RTC_M41T93::now()
{

  /*
Reading the Time. 
   Time is held in Rgisters 0x00 through 0x07:
   00 - milliseconds (00-99)
   01 - seconds (00-59)
   02 - minutes (00-59)
   03 - hours, 24 hour format (00-23)
   04 - day of week (01-07)
   05 - date (01-31)
   06 - month (01-12)
   07 - year (00-99)
   
   Register reads are auto-advanced to the next line as you read each line,
   so you can send 0x00 transfers one after another.
   */

  cs(LOW);

  SPI.transfer(0x00); //Start address for reading time

  //Register 00
  uint8_t mss = SPI.transfer(0x00);  //Get the milliseconds
  mss = bcd2bin(mss);

  //Register 01
  uint8_t ss = SPI.transfer(0x00);        // Get the seconds
  ss = bcd2bin(ss);

  //Register 02
  uint8_t mm =  SPI.transfer(0x00);  // Get the minutes
  mm = bcd2bin(mm);

  //Register 03
  uint8_t hh =  SPI.transfer(0x00);        // Get the hours
  hh = bcd2bin(hh);

  //Register 04
  uint8_t wday =  SPI.transfer(0x00);        // Get the day of the week
  wday = bcd2bin(wday);

  //Register 05
  uint8_t d =  SPI.transfer(0x00);        // Get the month day (date)
  d = bcd2bin(d);


  //Register 06
  uint8_t m =  SPI.transfer(0x00);        // Get the month
  m = bcd2bin(m);

  //Register 07
  uint16_t y =  SPI.transfer(0x00);        // Get the year
  y = bcd2bin(y) + 2000;			// add 2000 to the value returned.

  cs(HIGH);

  return DateTime (y, m, d, hh, mm, ss);
}

