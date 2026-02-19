#ifndef INTERMINAL_H
#define INTERMINAL_H

#include <termios.h>

void set_raw_mode(struct termios *orig);
void restore_mode(struct termios *orig);
int read_char_timeout(int timeout_sec);

#endif
