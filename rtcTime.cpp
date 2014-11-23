//manages all rtc time related things

#include "rtcTime.h"

rtcTime::rtcTime() {                                                                      //default constructor
    //RTC.set(1416113806);
     // the function to get the time from the RTC
        
      //setTime(1416113806);
    //RTC.set(1416113806);
    
    //setTime(1416113806);
    //RTC.set(now());
      
      

}

String rtcTime::getTime() {
    String out;
    out.concat(hour());
    out.concat(":");
    out.concat(minute());
    out.concat(":");
    out.concat(second());
    return out;
}

String rtcTime::getDate() {
    String out;
    out.concat(month());
    out.concat(" ");
    out.concat(day());
    //out.concat(" ");
    //out.concat(year());
    return out;
}