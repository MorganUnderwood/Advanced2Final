#include "user_input.h"

int serialRead(void *data) {
  int serial_port = *(int *)data;

  char buf[100];
  int bufIndex = 0;
  int tokenCount = 0;

  while (running) {
    if (!serialDataAvail(serial_port)) {
      SDL_Delay(1);
      continue;
    }

    bufIndex = 0;
    tokenCount = 0;
    while (serialDataAvail(serial_port) > 0) {
      char c = serialGetchar(serial_port);
      if (c == '\n')
        break;
      buf[bufIndex++] = c;
    }
    buf[bufIndex] = '\0';
    char *tokens[20];

    char *token;

    token = strtok(buf, "!");

    while (token != NULL) {
      tokens[tokenCount] = token;
      tokenCount++;
      token = strtok(NULL, "!");
    }
    if (tokenCount >= 2) {
      if (tokenCount == 3) {
        button = true;
      }

      int pastDays = atoi(tokens[0]);
      int futureDays = atoi(tokens[1]);

      SDL_LockMutex(inputsMTX);
      sharedInputs.minusDays = pastDays;
      sharedInputs.plusDays = futureDays;
      SDL_UnlockMutex(inputsMTX);
    }

    tcflush(serial_port, TCIOFLUSH);
    tokenCount = 0;
  }
  return 0;
}
int openSerial() {
  int serial_port;

  if ((serial_port = serialOpen("/dev/ttyACM0", 115200)) < 0) {
    return 1;
  }
  if (wiringPiSetup() == -1) {
    return 1;
  }

  return serial_port;
}
int InputThread(void *data) {
  SDL_Event event;
  volatile bool *running = (volatile bool *)data;
  while (*running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        *running = false;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_q ||
            event.key.keysym.sym == SDLK_ESCAPE) {
          *running = false;
        }
      }
    }

  }

  return 0;
}
int InputThread(void *data) {
  SDL_Event event;
  volatile bool *running = (volatile bool *)data;
  while (*running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        *running = false;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_q ||
            event.key.keysym.sym == SDLK_ESCAPE) {
          *running = false;
        }
      }
    }

  }

  return 0;
}