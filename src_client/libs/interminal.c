#include "interminal.h"
#include <unistd.h>
#include <sys/select.h>

void set_raw_mode(struct termios *orig){
	struct termios raw;
	tcgetattr(STDIN_FILENO, orig);
	raw = *orig;
	raw.c_lflag &= ~(ICANON);  // | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void restore_mode(struct termios *orig){
	tcsetattr(STDIN_FILENO, TCSANOW, orig);
}

int read_char_timeout(int timeout_sec) {
    char c;
    fd_set fds;
    struct timeval timeout;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    timeout.tv_sec  = timeout_sec;
    timeout.tv_usec = 0;

    int ret = select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout);

    if (ret < 0)  return -1;
    if (ret == 0) return 0;

    read(STDIN_FILENO, &c, 1);
    return (unsigned char)c;
}
