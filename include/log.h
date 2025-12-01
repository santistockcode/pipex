/* Simple logging macros controlled by -DDEBUG build flag */
#ifndef PIPEX_LOG_H
#define PIPEX_LOG_H

#include <stdio.h>

#ifdef DEBUG
  #define PIPEX_LOG(fmt, ...) \
    do { fprintf(stderr, "[pipex][%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while (0)
#else
  #define PIPEX_LOG(fmt, ...) do { } while (0)
#endif

#endif /* PIPEX_LOG_H */