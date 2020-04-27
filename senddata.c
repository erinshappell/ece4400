#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  char command[50];
  char command0[50], command1[50];
//******change here for ip-address and port number******
  strcpy(command1, "\" | nc -q 0 10.10.3.1 55555");
//******change here for host name******
  strcpy(command0, "echo \"Bike-1: ");
  printf("Start Sending My Location Data...\n");
//******change here for the host's starting position******
  float startingPos = 10;
  float CurrPos = startingPos;
//******change here for moving direction******
  int Direction = 1;  //1 to the right; -1 to the left
//******change here for moving speed per second******
  float speed = 0.333;
//******change here for total time (in seconds) of sending location data******
  for (int i=0; i<10; i++) {
    if (i != 0) {
      CurrPos = CurrPos + Direction*speed;
    }
    //for testing purpose
    //printf("%.3f\n", CurrPos);
    char buffer[50];
    sprintf(buffer, "%.3f", CurrPos);
    strcpy(command, command0);
    strcat(command, buffer);
    strcat(command, command1);
    printf("%s\n", command);
    system(command);
    sleep(1);
  }
  printf("DONE!\n");
  return 1;
}
