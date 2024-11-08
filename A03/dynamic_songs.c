/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date: 09/18/2024
 * Description: This program prints out the song list
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

//Song Struct
struct song {
  char title[1024];
  char artist[1024];
  int duration;
  float dance;
  float tempo;
  float energy;
  float valance;
};

int main() {
  printf("Welcome to Khanh Ha's Danceability Directory!\n");

  //Read the file 
  FILE *infile;
  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error: unable to open files");
    return 0;
  }

  //Read the total number of songs
  int totalSongs;
  fscanf(infile, "%d", &totalSongs);

  //Skip the ,,,, and the second line
  char buffer[100];
  fgets(buffer, sizeof(buffer), infile);
  fgets(buffer, sizeof(buffer), infile);

  //malloc songs array
  struct song *songs;
  songs = malloc(sizeof(struct song) * totalSongs);
  int count = 0;

  //for each line, create a struct song object and fill the songs array
  while (fgets(buffer, sizeof(buffer), infile) != NULL) {
    char* token;
    struct song currentSong;

    token = strtok(buffer, ","); //token will pointer to song title
    strcpy(currentSong.title, token);

    token = strtok(NULL, ","); //continue scanning the next phrase
    strcpy(currentSong.artist, token);

    token = strtok(NULL, ",");
    currentSong.duration = atoi(token);

    token = strtok(NULL, ",");
    currentSong.dance = atof(token);

    token = strtok(NULL, ",");
    currentSong.tempo = atof(token);

    token = strtok(NULL, ",");
    currentSong.valance = atof(token);
    songs[count] = currentSong;
    count += 1;
  }
  fclose(infile);

  //get and print out the song
  for (int i = 0; i < totalSongs; i++) {
    struct song curSong = songs[i];
    printf("\n|   %-2d) %-25s artist: %-20s duration: %-15d D: %-15g E: %-15g T: %-15g V: %-15g |", i, curSong.title, curSong.artist, curSong.duration, curSong.dance, curSong.energy, curSong.tempo, curSong.valance);
  }
  return 0;
}
