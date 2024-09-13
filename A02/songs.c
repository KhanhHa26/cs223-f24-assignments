/**
* The songs program for Assignment 2.
*
* This program allows the user to edit attribute of a playlist
*
* @author: Khanh Ha Nguyen
* @version: Sep, 12, 2024
*/

#include <stdio.h>
#include <string.h>

//Song Struct
struct song {
  int durationSec;
  int durationMin;
  float dance;
  char artist[1024];
  char title[1024];
};

/** Check what attribute the user wanna edit
* then change the attribute's value to user's input
* @param attribute The attribute that the user input
* @param id The id of the song
* @param songs[] The songs array that contains the playlist
*/
void edit(char attribute[], int id, struct song songs[]) {
  //If the user wants to edit duration
  if (strcmp(attribute, "duration") == 0) {
    int dMin;
    int dSec;
    printf("Enter a duration (minutes): ");
    scanf("%d", &dMin);
    printf("Enter a duration (seconds): ");
    scanf("%d", &dSec);
    songs[id].durationMin = dMin;
    songs[id].durationSec = dSec;

  //If the user wants to edit artist  
  } else if (strcmp(attribute, "artist") == 0) {
    char newArtist[1024];
    printf("Enter an artist: ");
    scanf(" %[^\n]%*c", newArtist);
    strcpy(songs[id].artist, newArtist);
  
  //If the user wants to edit title  
  } else if (strcmp(attribute, "title") == 0) {
    char newTitle[1024];
    printf("Enter a title: ");
    scanf(" %[^\n]%*c", newTitle);
    strcpy(songs[id].title, newTitle);
  
  //If the user wants to edit danceability  
  } else if (strcmp(attribute, "danceability") == 0) {
    float newDance;
    printf("Enter danceability: ");
    scanf("%f", &newDance);
    songs[id].dance = newDance;
  
  //If the user inputs an invalid attribute  
  } else {
    printf("Please choose one of the attributes");
  }
}

/** Print the song array in a nice format
* @param songs[] The song array that has the playlist
*/
void printSongs(struct song songs[]) {
  printf("\n\n---------------------------------------------------------------------------------------------------------\n");
  for (int i = 0; i < 3; i++) {
    struct song curSong = songs[i];
    printf("\n|   %d) %-20s artist: %-20s duration: %d:%-15d danceability: %g   |", i, curSong.title, curSong.artist, curSong.durationMin, curSong.durationSec, curSong.dance);
  }
  printf("\n\n----------------------------------------------------------------------------------------------------------\n");
}

//The main function
int main() {
  //Songs initialization
  //I will just go with the examples for easy debugging if have :)
  struct song song1, song2, song3;
  song1.durationSec = 11;
  song1.durationMin = 4;
  song1.dance = 0.50;
  strcpy(song1.artist, "Tears for Fears");
  strcpy(song1.title, "Shout");

  song2.durationSec = 47;
  song2.durationMin = 2;
  song2.dance = 0.70;
  strcpy(song2.artist, "Harry Styles");
  strcpy(song2.title, "As It Was");

  song3.durationSec = 34;
  song3.durationMin = 5;
  song3.dance = 0.30;
  strcpy(song3.artist, "Pink Floyd");
  strcpy(song3.title, "Wish You Were Here");

  struct song songs[3] = {song1, song2, song3};
  printSongs(songs);

  //Prompting the user for song id
  int id;
  char attribute[1024];
  printf("Welcome to Khanh Ha's song list!\nEnter a song id to edit: ");
  scanf("%d", &id);

  //If the id is valid, then ask the user what they wanna edit
  if (id > 2 || id < 0) {
    printf("Invalid choice\n");
  } else {
    printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
    scanf("%s", attribute);
    edit(attribute, id, songs);
    printSongs(songs);
  }
  return 0;
}

