# Fake Keyboard Sound

Turn any keyboard into a mechanical keyboard.

This Linux utility simulates mechanical keyboard sounds by playing audio samples in response to keyboard input events.

The program reads keyboard events directly from the Linux input subsystem and plays corresponding WAV files using SDL2 and SDL2_mixer, allowing any keyboard to mimic the sound profile of a mechanical keyboard.

## Why?

Mechanical keyboards sound great.

Mine wasn't mechanical.

So I wrote a program that listens for key presses and plays mechanical keyboard sound effects in real time, making a cheap keyboard sound far more expensive than it actually is.

## Features

* Monitors physical keyboard input events
* Interactive keyboard selection
* Custom sound pack support
* Automatic fallback for unmapped keys
* Low-latency audio playback using SDL2_mixer
* Drops elevated privileges after opening the selected input device
* Lightweight native implementation in C

## How It Works

```text
Keyboard
    │
    ▼
Linux Input Events
    │
    ▼
fake_keyboard
    │
    ▼
SDL2_mixer
    │
    ▼
Mechanical Keyboard Sounds
```

Accessing devices under `/dev/input` requires elevated privileges. The program starts with root permissions in order to enumerate and open keyboard devices.

After a keyboard is selected:

1. The device is opened.
2. Root privileges are dropped using the original user information provided by `sudo`.
3. SDL2 and SDL2_mixer are initialized.
4. Sound samples are loaded into memory.
5. Keyboard events are continuously monitored.
6. Matching audio samples are played whenever keys are pressed.

The privilege drop ensures that only the device discovery phase requires elevated access.

## Security

The application only uses elevated privileges to discover and open input devices.

Once the selected keyboard has been opened, root privileges are immediately dropped and the remainder of the application runs as the invoking user.

## Dependencies

### Runtime

* SDL2
* SDL2_mixer

### Build

* Clang
* Make

## Building

```bash
git clone https://github.com/mokshitarora2006/fake-keyboard-sound.git
cd fake-keyboard-sound
make
```

## Usage

```bash
sudo ./fake_keyboard <soundpack_directory>
```

Example:

```bash
sudo ./fake_keyboard ./nk-cream/
```

The program will list all detected keyboard devices and prompt you to select one.

## Sound Packs

Sound packs are directories containing WAV files named after keyboard keys.

Example:

```text
a.wav
b.wav
c.wav
enter.wav
space.wav
tab.wav
backspace.wav
shift.wav
```

When a dedicated sample for a key is unavailable, the program automatically selects a fallback sound from the loaded sound pack.

## Project Structure

```text
fake_keyboard.c    Main application
helper.h           Utility functions
vectors.h          Dynamic array implementation
Makefile           Build configuration
```

## Credits

The sound packs used during development and testing were obtained from Mechvibes.

Credit for the audio assets belongs to their respective creators.

## License

MIT License
