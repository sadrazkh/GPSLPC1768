#ifndef GPS_H
#define	GPS_H

#include <string.h>
#define false 0
#define true 1


struct _GPS {
    char latitude[10];
    char longitude[11];
    char lat_cardinal;
    char lon_cardinal;
};
typedef struct _GPS GPS;
extern int _GPS_location_avail;

void (*_UartInit)();
unsigned char (*_UartReceiveByte)();
void (*_UartSendByte)(unsigned char);
void _GPSInit();
void _GPSRoutines();
GPS _GPSValues();
#endif	/* GPS_H */
