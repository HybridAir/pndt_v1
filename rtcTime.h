#ifndef RTCTIME_H
#define	RTCTIME_H

#include <Arduino.h>
#include <Time.h>
#include <MCP79412RTC.h>
#include <Wire.h>

class rtcTime {
    public:
        rtcTime();
        void begin();
        String getTime();
        String getDate();
};

#endif

