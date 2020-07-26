#include "config.h"
#include "GPS.h"
#define BAUD 9600
#define SPBRG_VAL  (int)(_XTAL_FREQ/BAUD/16) -1
#define TX_PIN_DIR TRISC6
#define RX_PIN_DIR TRISC7
//#define debug_on

void my_serial_init(void)
{
	TX_PIN_DIR = 0;  // TX pin
	RX_PIN_DIR = 1;  // RX pin
	GIE=1;     // Global interrupt mask
	PEIE=1;     // peripheral int mask
	SPBRG= SPBRG_VAL;
	BRGH=1;
	SYNC=0;
	SPEN=1;     //Serial port enabled
	TXEN=1;     //Transmission enabled
	RCIE=0;     //Receive Interrupt enabled
	CREN=1;     //Receive disabled
}


void send_byte(unsigned char val)
{
	TXREG=val;
	while(!TXIF);
	TXIF=0;
	__delay_ms(1); //This delay is included due to a random error in transmission
}

unsigned char receive_byte(void)
{
    char x;
        RCIE = 0;
	while(!RCIF);
        x = RCREG;
        RCIE = 1;
        #ifdef debug_on
            send_byte(x);
        #endif
 	return x;
}

void send_str(unsigned char * str)
{
    while(*str)
        send_byte(*str++);
}
void main(){
    int i=0;
    GPS gps;
    OSCCONbits.IRCF = 7; 
    ANSEL = ANSELH = 0;
    _UartReceiveByte = &receive_byte;
    _UartInit = &my_serial_init;
    _UartSendByte = &send_byte;
    _GPSInit();
    send_str("Welcome\r\n");
    
    while(1){
        if(_GPS_location_avail){
            gps = _GPSValues();
            send_str("\r\n\nLatitude: ");
            send_str(gps.latitude);
            send_byte(' ');
            send_byte(gps.lat_cardinal);
            send_str("\r\nLongitude: ");
            send_str(gps.longitude);
            send_byte(' ');
            send_byte(gps.lon_cardinal);          
            
        }
        _GPSRoutines();
    }
}

void interrupt ISR(){
    if(RCIF)
        RCREG;
}