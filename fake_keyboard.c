#include "helper.h"
#include "vectors.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
bool is_paused = false;
typedef struct {
  char path[100];
  char name[256];
} metadata;
int *keys;
int small_keys[34] = {
    KEY_A,        KEY_B,         KEY_C,          KEY_D,         KEY_E,
    KEY_F,        KEY_G,         KEY_H,          KEY_I,         KEY_J,
    KEY_K,        KEY_L,         KEY_M,          KEY_N,         KEY_O,
    KEY_P,        KEY_Q,         KEY_R,          KEY_S,         KEY_T,
    KEY_U,        KEY_V,         KEY_W,          KEY_X,         KEY_Y,
    KEY_Z,        KEY_ENTER,     KEY_TAB,        KEY_LEFTSHIFT, KEY_BACKSPACE,
    KEY_CAPSLOCK, KEY_LEFTBRACE, KEY_RIGHTBRACE, KEY_BACKSLASH};
void signal_handler(int sig);
int key_name(const char *string);
int open_keyboard(void);
void control_keyboard(int fd, Mix_Chunk **sounds);
Mix_Chunk **load_audio_files(char *path);

int main(int argc, char **argv) {
  keys = vector_create(int);
  srand(time(NULL));
  if (argc != 2) {
    printf("Usage: %s <path>\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "--help") == 0) {
    printf("Usage: %s <path>\n", argv[0]);
    return 0;
  }
  if (geteuid() != 0) {
    printf("%s is meant to be run via root!\n", argv[0]);
    return 1;
  }
  signal(SIGINT, signal_handler);
  signal(SIGTSTP, signal_handler);
  printf("Opening keyboard:\n");
  fflush(stdout);
  int fd = open_keyboard();
  Mix_Chunk **sounds = load_audio_files(argv[1]);
  printf("You may begin typing now: \n");
  control_keyboard(fd, sounds);
  return 0;
}

int open_keyboard(void) {
  metadata keyboards[100];
  char bit_map[96];
  char *fixed_path = "/dev/input/";
  char variable_path[100];
  char name[256];
  DIR *dir = opendir("/dev/input");
  if (dir == NULL) {
    perror("dir:");
    exit(1);
  }
  struct dirent *content;
  int temp_fd;
  int i = 0;
  while ((content = readdir(dir)) != NULL) {
    strcpy(variable_path, fixed_path);
    strcat(variable_path, content->d_name);
    int temp_fd = open(variable_path, O_RDONLY);
    if (temp_fd == -1) {
      // perror("open: ");
    }
    if (ioctl(temp_fd, EVIOCGNAME(sizeof(name)), name) == -1) {
      // perror("ioctl: ");
      continue;
    };
    if (ioctl(temp_fd, EVIOCGBIT(EV_KEY, sizeof(bit_map)), bit_map) == -1) {
      // perror("ioctl");
    }
    int index = KEY_A / 8;
    int bit = KEY_A % 8;
    if (bit_map[index] & (1 << bit)) {
      strcpy(keyboards[i].name, name);
      strcpy(keyboards[i++].path, variable_path);
    }
  }
  printf("Which keyboard you want to select: \n");
  for (int j = 0; j < i; j++) {
    printf("%d\t%-50s %s\n", j + 1, keyboards[j].name, keyboards[j].path);
  }
  int choice;
  printf("Choice: ");
  scanf("%d", &choice);
  if (choice > i) {
    printf("Error: Out of bounds\n");
    exit(1);
  }
  choice--;
  temp_fd = open(keyboards[choice].path, O_RDONLY);
  if (temp_fd == -1) {
    printf("Error opening the keyboard!\n");
    exit(1);
  }
  setgid(atoi(getenv("SUDO_GID")));
  setuid(atoi(getenv("SUDO_UID")));
  struct passwd *pw = getpwuid(getuid());
  if (pw) {
    setenv("HOME", pw->pw_dir, 1);
  }
  char runtime[64];
  snprintf(runtime, sizeof(runtime), "/run/user/%d", getuid());
  setenv("XDG_RUNTIME_DIR", runtime, 1);
  return temp_fd;
}

Mix_Chunk **load_audio_files(char *path) {
  SDL_Init(SDL_INIT_AUDIO);
  Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 256);
  Mix_Chunk **sounds = calloc(768, sizeof(Mix_Chunk *));
  char temp_path[100];
  char *full_path = path;
  DIR *dir = opendir(full_path);
  if (dir == NULL) {
    perror("opendir");
  }
  struct dirent *sound;
  while ((sound = readdir(dir)) != NULL) {
    if (strstr(sound->d_name, ".wav")) {
      char string[256];
      strcpy(string, sound->d_name);
      string[strlen(string) - 4] = '\0';
      strcpy(temp_path, full_path);
      strcat(temp_path, sound->d_name);
      sounds[key_name(string)] = Mix_LoadWAV(temp_path);
      vector_push_back(keys, key_name(string));
    }
  }
  printf("All sounds have been loaded to the memory\n");
  return sounds;
}

int key_name(const char *name) {
  if (strcmp(name, "a") == 0)
    return KEY_A;
  if (strcmp(name, "b") == 0)
    return KEY_B;
  if (strcmp(name, "c") == 0)
    return KEY_C;
  if (strcmp(name, "d") == 0)
    return KEY_D;
  if (strcmp(name, "e") == 0)
    return KEY_E;
  if (strcmp(name, "f") == 0)
    return KEY_F;
  if (strcmp(name, "g") == 0)
    return KEY_G;
  if (strcmp(name, "h") == 0)
    return KEY_H;
  if (strcmp(name, "i") == 0)
    return KEY_I;
  if (strcmp(name, "j") == 0)
    return KEY_J;
  if (strcmp(name, "k") == 0)
    return KEY_K;
  if (strcmp(name, "l") == 0)
    return KEY_L;
  if (strcmp(name, "m") == 0)
    return KEY_M;
  if (strcmp(name, "n") == 0)
    return KEY_N;
  if (strcmp(name, "o") == 0)
    return KEY_O;
  if (strcmp(name, "p") == 0)
    return KEY_P;
  if (strcmp(name, "q") == 0)
    return KEY_Q;
  if (strcmp(name, "r") == 0)
    return KEY_R;
  if (strcmp(name, "s") == 0)
    return KEY_S;
  if (strcmp(name, "t") == 0)
    return KEY_T;
  if (strcmp(name, "u") == 0)
    return KEY_U;
  if (strcmp(name, "v") == 0)
    return KEY_V;
  if (strcmp(name, "w") == 0)
    return KEY_W;
  if (strcmp(name, "x") == 0)
    return KEY_X;
  if (strcmp(name, "y") == 0)
    return KEY_Y;
  if (strcmp(name, "z") == 0)
    return KEY_Z;

  if (strcmp(name, "space") == 0)
    return KEY_SPACE;
  if (strcmp(name, "enter") == 0)
    return KEY_ENTER;
  if (strcmp(name, "tab") == 0)
    return KEY_TAB;
  if (strcmp(name, "shift") == 0)
    return KEY_LEFTSHIFT;
  if (strcmp(name, "backspace") == 0)
    return KEY_BACKSPACE;
  if (strcmp(name, "caps lock") == 0 || strcmp(name, "esc") == 0)
    return KEY_CAPSLOCK;

  if (strcmp(name, "[") == 0)
    return KEY_LEFTBRACE;
  if (strcmp(name, "]") == 0)
    return KEY_RIGHTBRACE;
  if (strcmp(name, "\\") == 0)
    return KEY_BACKSLASH;

  return -1;
}

void control_keyboard(int fd, Mix_Chunk **sounds) {
  struct input_event key_press;
  int key;
  for (;;) {
    read(fd, &key_press, sizeof(key_press));
    if (key_press.type == EV_KEY && key_press.value == 1) {
      if (sounds[key_press.code]) {
        Mix_PlayChannel(-1, sounds[key_press.code], 0);
      } else {
        int n = vector_size(keys);
        key = keys[rand() % n];
        Mix_PlayChannel(-1, sounds[key], 0);
      }
    }
  }
}

void signal_handler(int sig) {
  if (sig == SIGINT) {
    line_clear_and_home();
    Mix_CloseAudio();
    SDL_Quit();
    printf("\nCleaning up......\n");
    exit(0);
  }
}
