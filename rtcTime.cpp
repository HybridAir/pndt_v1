//manages all rtc time related things

#include "rtcTime.h"

rtcTime::rtcTime() {               

}

void rtcTime::begin() {
    setSyncProvider(RTC.get);
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