#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sdl\SDL.h>
#include <sdl\SDL_mixer.h>
#include <sdl\SDL_main.h>
#include <windows.h>
#include <conio.h>

/* We use a dynamic 2d array instead of doubly linked list because of
* randomisations and selective playback 
*/

char **playlist; //playlist 2d array
int total_songs=0; // stores the total number of songs
int nowplaying=0; // stores the index of the currently playing song
char folder[100]; // path of diretory containing the songs
Mix_Music *music; // SDL_Mix pointer
int prev=0; // flag to move to previous song
int next=0; // flag to move to next song
int running=1; // flag to keep the loop for checking commands running
int control=1; // flag to keep playing songs
void next_play();

void check_number_of_songs(){
    DIR* FD;
    struct dirent* in_file;
    total_songs=0;

    /* Scanning the in directory */
    if (NULL == (FD = opendir (folder))) 
    {
        printf("Error : Failed to open input directory\n");
       

        total_songs=-1;
    }
    while ((in_file = readdir(FD))) 
    {
        /*we don't want current and parent directories*/
        /*d_name stores the name of the files with extensions*/
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;
        //we only consider files with .mp3 as songs
        if (strstr(in_file->d_name,".mp3"))
            total_songs++;    
}
}
void shuffle_playlist(){
    /* Shuffling the playlist using the Fischer Yates Shuffling Algorithm
    *  which swaps the last element of the array with a random index including the 
    *  last one and repeats this process until it reaches the first element  */
    char *temp;
    int i=0;
    srand(time(NULL)); //so that we get random sequence everytime the program is run
    for(i=total_songs-1;i>0;i--){
        int j=rand() % (i+1);
        temp=playlist[i];
        playlist[i]=playlist[j];
        playlist[j]=temp;

    } 
}
void create_playlist(){
    printf("Searching for all songs....\n");
    check_number_of_songs();
    if(total_songs >0){
        int count=0;
        printf("Total songs found : %d\n",total_songs);
        playlist=(char**)malloc(total_songs * sizeof(char[100]));
        for (int i = 0; i < total_songs; i++) {
            playlist[i] = (char*)malloc(100 * sizeof(char));
            }
        
        DIR* FD=opendir (folder);
        struct dirent* in_file;
        while ((in_file = readdir(FD))){
    
        /*  we don't want current and parent directories */
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;
        if (strstr(in_file->d_name,".mp3")){
            strcpy(playlist[count],in_file->d_name);
            playlist[count][strlen(in_file->d_name)]='\0';
            count++;
            }   
        }
        shuffle_playlist();
    }
    else if(total_songs==0)
        printf("No songs in the directory.\nAdd some songs and try again!");
    
}
void display_playlist(){
    int i;
    for(i=0;i<total_songs;i++){
        printf("%d ", (i+1));
        puts(playlist[i]);
    }
    printf("Press Any letter to return to player: ");
    getchar();
    getchar();
    return;
}

void display_controls(int n)
{
    system("cls");
    printf("Now playing: %s\n", playlist[n]);
    printf("Controls:\n");
    printf("  p: Pause\n");
    printf("  r: Resume\n");
    printf("  b: Previous\n");
    printf("  n: Next\n");
    printf("  c: Rewind\n");
    printf("  d: Display Playlist\n");
    printf("  q: Quit\n");
    printf("Enter command: \n");
}

void play(int n) {
    char path[100];
    strcpy(path, folder);
    strcat(path, playlist[n]);
    music = Mix_LoadMUS(path); //load the music file

    if (!music) {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        control=0;
        return;
    }

    if (Mix_PlayMusic(music, 0) == -1) { //play the music
        printf("Mix_PlayMusic Error: %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        control = 0;
        return;
    }

    Mix_HookMusicFinished(next_play); // Ensures next song plays automatically using callback function
    display_controls(n);
    while (running) {
        char command='\0';
         if (_kbhit()) {  // Check if a key is pressed
            command = _getch();
            display_controls(n);
        } 

        switch (command) {
            case 'p': Mix_PauseMusic(); break;
            case 'r': Mix_ResumeMusic(); break;
            case 'b': 
                if (nowplaying > 0) {
                    Mix_HookMusicFinished(NULL); // temporarily unhook the callback function
                    Mix_HaltMusic();   // Stop the current song
                    prev = 1;          // Set previous flag
                    next = 0;          // Reset next flag
                    running = 0;       // Exit current loop
                } else {
                    printf("No previous songs available.\n");
                    Sleep(1000);
                }
                break;
            case 'n':
                if (nowplaying < total_songs - 1) {
                    Mix_HookMusicFinished(NULL); // temporarily unhook the callback function
                    Mix_HaltMusic();   // Stop the current song
                    next = 1;          // Set next flag
                    prev = 0;          // Reset previous flag
                    running = 0;       // Exit current loop
                } else {
                    printf("End of playlist...\n");
                    Sleep(1000);
                }
                break;
            case 'c': Mix_RewindMusic(); break;
            case 'd': display_playlist(); display_controls(n); break;
            case 'q':
                control=0;  
                return;
        }
    }
}

void playlist_play() {
    while (control) {  // Keep looping until user quits
        if (prev == 1 && next == 0) {  // User pressed 'b'
            prev = 0;                  // Reset flags
            next = 0;
            nowplaying--;              // Move to previous song
        } 
        else if (next == 1 && prev == 0) {  // User pressed 'n'
            prev = 0;                  // Reset flags
            next = 0;
            nowplaying++;              // Move to next song
        }

        running = 1;                   // Reset running for the new song
        play(nowplaying);              // Play the current song
    }
    printf("Quitting...\n");
}

void next_play() {
    if (nowplaying < total_songs - 1) {
        next = 1;        // Automatically set next flag
    } else {
        printf("No more songs to play...\n");
        control = 0;     // Exit the playlist loop if at the end
    }
    running = 0;         // Ensure `play()` exits
}

 int main(int argc, char const *argv[])
 {
     // Initialize SDL and SDL_mixer
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        printf("An Error Occured setting up the playback: %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_Init(MIX_INIT_MP3) == 0) {
    printf("Mix_Init Error: %s\n", Mix_GetError());
    SDL_Quit();
    return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 16384) == -1) {
    printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
    SDL_Quit();
    return 1;
    }   
    printf("Enter the path of folder containing the songs\n");
    gets(folder);
    printf("Loading...\n");
    Sleep(1000);
    printf("Creating Playlist... \n");
    create_playlist();
    playlist_play();
    int i=0;
    for(i=0;i<total_songs;i++){
        free(playlist[i]);
    }
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    free(playlist);
    return 0;
 }
