#pragma once
#include <stdio.h>
#include <string.h>
#define swap(a, b)                                                             \
  do {                                                                         \
    char temp[sizeof(a)];                                                      \
    memcpy(temp, &(a), sizeof(a));                                             \
    memcpy(&(a), &(b), sizeof(a));                                             \
    memcpy(&(b), temp, sizeof(b));                                             \
  } while (0)
#define print_array(arr, n, fmt)                                               \
  do {                                                                         \
    printf("{");                                                               \
    for (int i = 0; i < (n); i++) {                                            \
      printf(fmt, (arr)[i]);                                                   \
      if (i != (n) - 1)                                                        \
        printf(", ");                                                          \
    }                                                                          \
    printf("}\n");                                                             \
  } while (0)

static inline void cursor_move_up(int n) { printf("\033[%dA", n); }

static inline void cursor_move_down(int n) { printf("\033[%dB", n); }

static inline void cursor_move_right(int n) { printf("\033[%dC", n); }

static inline void cursor_move_left(int n) { printf("\033[%dD", n); }

static inline void cursor_move_home(void) { printf("\r"); }

static inline void cursor_save_position(void) { printf("\033[s"); }

static inline void cursor_restore_position(void) { printf("\033[u"); }

static inline void cursor_hide(void) { printf("\033[?25l"); }

static inline void cursor_show(void) { printf("\033[?25h"); }

static inline void line_clear(void) { printf("\033[2K"); }

static inline void screen_clear(void) { printf("\033[2J\033[H"); }

static inline void line_clear_and_home(void) {
  cursor_move_home();
  line_clear();
}

static inline void previous_line_clear(void) {
  cursor_move_up(1);
  cursor_move_home();
  line_clear();
}
