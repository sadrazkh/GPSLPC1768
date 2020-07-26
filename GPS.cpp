#include "GPS.h"

int _GPS_location_avail = false;

GPS gps;
int rec_count = 0;

#define UARTBUFFERSIZE 45
char UartBuffer[UARTBUFFERSIZE];

void (*_UartInit)();
unsigned char (*_UartReceiveByte)();
void (*_UartSendByte)(unsigned char);

void _GPSRoutines(){
    int i = 0;
    char x = _UartReceiveByte();
    if(x=='$')
        rec_count =0;
    UartBuffer[rec_count++] = x;
    UartBuffer[rec_count] = '\0';
    if(UartBuffer[0] == '$' && rec_count <= 42){
         if(rec_count == 42 && UartBuffer[3] == 'G' && UartBuffer[4] == 'G' && UartBuffer[5] == 'A' ){
            gps.lat_cardinal = UartBuffer[28];
            gps.lon_cardinal = UartBuffer[41];
            for(i=0;i<9;i++)
                gps.latitude[i] = UartBuffer[i+18];
            gps.latitude[i] = '\0';
            for(i=0;i<10;i++)
                gps.longitude[i] = UartBuffer[i+30];
            gps.longitude[i] = '\0';
            _GPS_location_avail = true;
        }
    }else{
        rec_count = 0;
        UartBuffer[rec_count] = '\0';
        _GPS_location_avail = false;
    }

}

void _GPSInit(){
    _UartInit();
    rec_count =0;
}

GPS _GPSValues(){
    _GPS_location_avail = false;
    rec_count = 0;
    UartBuffer[rec_count] = '\0';
    return gps;
}