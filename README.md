# C_Music_Player
A simple command-line music player built using SDL2 and SDL_mixer for playing MP3 files. The player can shuffle, navigate through the playlist, and control playback via keyboard commands.

## Features
- **Playlist Support**: Create and shuffle a playlist of MP3 files from a specified directory.
- **Playback Controls**: Pause, resume, skip to the next song, and go to the previous song.
- **Music Controls**: Rewind current song and show the playlist.
- **Auto Next Song**: The player automatically moves to the next song when the current song finishes.

## Requirements
- [SDL2](https://www.libsdl.org/) and [SDL_mixer](https://www.libsdl.org/projects/SDL_mixer/) for audio playback.
- C compiler (e.g., GCC, MinGW) for compiling the code.

### Windows-specific
- This project uses Windows-specific headers like `windows.h` and `conio.h` for handling user input.

## Installation

1. **Clone the repository**:
    ```bash
    git clone https://github.com/your-username/music-player.git
    cd music-player
    ```

2. **Install SDL2 and SDL_mixer**:
    - Download and install [SDL2](https://www.libsdl.org/download-2.0.php).
    - Download and install [SDL_mixer](https://www.libsdl.org/projects/SDL_mixer/).
    
    Make sure to link the required SDL2 and SDL_mixer libraries during compilation.

3. **Compile the code**:
    ```bash
    gcc -o music_player music_player.c -lSDL2 -lSDL2_mixer
    ```

## Usage

1. Run the program by executing:
    ```bash
    ./music_player
    ```

2. The program will ask you to input the path to the folder containing the MP3 files.

3. **Controls**:
    - `p` : Pause the song.
    - `r` : Resume the song.
    - `b` : Play the previous song.
    - `n` : Play the next song.
    - `c` : Rewind the current song.
    - `d` : Display the playlist.
    - `q` : Quit the player.

4. The player will continue to loop through the playlist until you quit.

## How It Works

The program consists of several key functions that work together to handle music playback, control flow, and playlist management:

### Functions:

1. **`check_number_of_songs()`**:
   - **Purpose**: Scans the given directory for MP3 files and counts how many songs are available.
   - **Working Principle**: 
     - It opens the specified directory using `opendir()`.
     - Iterates through each entry using `readdir()`.
     - For each MP3 file, it increments the `total_songs` counter.

2. **`create_playlist()`**:
   - **Purpose**: Creates a playlist by reading all MP3 files in the specified directory and stores them in a dynamic 2D array.
   - **Working Principle**: 
     - Calls `check_number_of_songs()` to count the number of songs.
     - Allocates memory for the playlist.
     - Reads the MP3 filenames from the directory and stores them in the playlist.
     - Calls `shuffle_playlist()` to randomize the order of the playlist.

3. **`shuffle_playlist()`**:
   - **Purpose**: Shuffles the playlist to randomize the song order.
   - **Working Principle**: 
     - Implements the Fisher-Yates algorithm to swap each song with a random one in the playlist.

4. **`display_playlist()`**:
   - **Purpose**: Displays the current playlist.
   - **Working Principle**: 
     - Iterates through the playlist array and prints each song's filename.
     - Waits for user input using `getchar()`.

5. **`display_controls(int n)`**:
   - **Purpose**: Displays the current song's information and the available playback controls.
   - **Working Principle**: 
     - Prints the name of the song currently playing (`playlist[n]`).
     - Lists the available controls: pause, resume, previous song, next song, rewind, display playlist, quit.

6. **`play(int n)`**:
   - **Purpose**: Loads and plays the song at index `n`.
   - **Working Principle**: 
     - Constructs the full file path using the `folder` path and the song name from the playlist.
     - Uses `Mix_LoadMUS()` to load the MP3 file.
     - Plays the song using `Mix_PlayMusic()` and sets up the music to automatically move to the next song when it finishes.
     - Waits for user input to control playback, and processes the commands (`p`, `r`, `b`, `n`, `c`, `d`, `q`).

7. **`next_play()`**:
   - **Purpose**: Callback function that gets triggered when a song finishes playing.
   - **Working Principle**: 
     - If there is a next song, it sets the `next` flag to 1, indicating that the player should proceed to the next song.
     - If there are no more songs to play, it sets `control` to 0, indicating the player should quit.

8. **`playlist_play()`**:
   - **Purpose**: Loops through the playlist and controls the playback flow.
   - **Working Principle**: 
     - Continuously plays songs from the playlist.
     - Checks the `prev` and `next` flags to determine whether the user wants to move to the previous or next song.
     - Calls `play(nowplaying)` to play the current song and waits for the song to finish or the user to input commands.

9. **`main()`**:
   - **Purpose**: The entry point of the program.
   - **Working Principle**: 
     - Initializes SDL2 and SDL_mixer.
     - Prompts the user for the path to the song directory and creates the playlist.
     - Calls `playlist_play()` to start the music player and handle playback.
     - Frees memory and cleans up SDL resources at the end.

## Known Issues
- The player currently uses Windows-specific libraries for input handling (`conio.h`), so it may not be portable to other operating systems without modification.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements
- [SDL2](https://www.libsdl.org/) for providing the multimedia framework.
- [SDL_mixer](https://www.libsdl.org/projects/SDL_mixer/) for audio playback support.

---

Feel free to contribute or report issues. Enjoy the music!
