#ifndef _SERIAL_COMMUNICATIONS_H_
#define _SERIAL_COMMUNICATIONS_H_

#include <stdio.h> 
#include <wiringPi.h> 
#include <wiringSerial.h> 
#include <string.h> 
#include <errno.h> 
#include <termios.h> 
#include <unistd.h> 
#include <SDL2/SDL.h>

int openSerial();

int serialRead(void *data);

int InputThread(void *data);


#endif
