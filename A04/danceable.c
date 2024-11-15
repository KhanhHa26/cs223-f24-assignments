/*----------------------------------------------
 * Author: Khanh Ha Nguyen
 * Date:  09/26/2005
 * Description: This program asks the user to enter a char
 * and if the user enters "d", the program will delete the song
 * with the highest danceability
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

//Node Struct
struct node {
  struct song val;
  struct node* next;
};

//Printing out song with their details 
void printSongList(struct node* head) {
  struct node* curSong = head;
  int count = 0;
  while (curSong != NULL) {
    printf("\n %d) %-25s artist: %-20s duration: %-15d D: %-15g E: %-15g T: %-15g V: %-15g |", count, curSong->val.title, curSong->val.artist, curSong->val.duration, curSong->val.dance, curSong->val.energy, curSong->val.tempo, curSong->val.valance);
    count += 1;
    curSong = curSong->next;
  }
}

//Print out one given song
void printSong(struct song curSong) {
  printf("\n---------------------------------------- Most danceable ------------------------------------");
  printf("\n %-25s artist: %-20s duration: %-15d D: %-15g E: %-15g T: %-15g V: %-15g |", curSong.title, curSong.artist, curSong.duration, curSong.dance, curSong.energy, curSong.tempo, curSong.valance);
  printf("--------------------------------------------------------------------------------------------\n");

}

//Insert node at the front of the linked list 
struct node* insertAtFront(struct song val, struct node* head) { 
  struct node* n = (struct node*)malloc(sizeof(struct node));
  if (n == NULL) {
    printf("Error: out of space\n");
    exit(1);
  }
  n->val = val;
  n->next = head;
  return n;
}

//Remove song based on the given song 
struct node* removeSong(struct node* head, struct song* song) { 
  struct node* current = head;
  struct node* prev = NULL;

  while (current != NULL) {
    //If it's the song with the matching danceibility
    //We set the previous node to point to the next node
    //And return the current song
    // printf("%s", head->val.title);
    if (current->val.dance == song->dance) {
      // If removing the head node, set head to next of current
      if (current->val.dance == head->val.dance) {
        head = current->next;
      } else {
        prev->next = current->next;
      }      
      break;
    }
    prev = current;
    current = current->next;
  }
  return head;
}

struct song findMaxSong(struct node* head) {
  struct node* current = head;
  struct song tempMaxDance = head->val;  // Initialize a temporary song struct
  while (current != NULL) {
    if (current->val.dance > tempMaxDance.dance) {
      tempMaxDance = current->val;
    }
    current = current->next;  
  }
  return tempMaxDance;
}

//Main method
int main() {
  //Reading the file
  FILE *infile;
  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error: unable to open files");
    return 0;
  }

  //Skip the column name line
  char buffer[100];
  fgets(buffer, sizeof(buffer), infile);
  struct node* head = NULL;
  struct song maxDance;
  int totalSongs = 0;

  //Create struct song for every line of the file
  //And add it to the linked list
  while (fgets(buffer, sizeof(buffer), infile) != NULL) {
    char* token;
    struct song currentSong;

    token = strtok(buffer, ","); //token will pointer to song title
    strcpy(currentSong.title, token);

    token = strtok(NULL, ","); //continue scanning the next phrase
    strcpy(currentSong.artist, token);

    token = strtok(NULL, ",");
    currentSong.duration = atoi(token);

    //Keeping track of the song with max dance
    token = strtok(NULL, ",");
    currentSong.dance = atof(token);
    if (currentSong.dance > maxDance.dance) {
      maxDance = currentSong;
    }

    token = strtok(NULL, ",");
    currentSong.energy = atof(token);

    token = strtok(NULL, ",");
    currentSong.tempo = atof(token);

    token = strtok(NULL, ",");
    currentSong.valance = atof(token);

    head = insertAtFront(currentSong, head);
    totalSongs += 1;
  }
  fclose(infile);

  //Start of the program
  char userInput;

  //Repeatedly asking the user until running out of songs
  //Or the user inputs a non-d character
  while(1) {
    printSongList(head);
    printf("\nDataset contains %d songs\n\nPress 'd' to show the most danceable song (any other key to quit): ", totalSongs);
    scanf(" %c", &userInput); //have to put " " otherwise it will treat it as input in the next call
    if (userInput == 'd') {
      if (totalSongs > 0) {
        totalSongs -= 1;
        printSong(maxDance);
        head = removeSong(head, &maxDance);
        if (head != NULL)
          maxDance = findMaxSong(head);
      }
    } else {
      break;
    }
  }
  free(head);
  return 0;
}
