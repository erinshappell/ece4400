#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMVEHICLES 4
#define WARNDIST 3

int main()
{
	
	int i;
	int dist;
	char command[50];
	
	struct Vehicle{
		char name[10];		// "Car 1", "Bike 2", etc.
		char file[10];		// name of file holding position
		char port[10];		// ip address (e.g., "10.10.1.2")
		FILE *ptr;			// pointer to file
		int pos;			// read from file
	};

	struct Vehicle vehicles[5];		// array of vehicles
	
	strcpy(vehicles[0].name, "Car 1");
	strcpy(vehicles[0].file, "c1pos");
	strcpy(vehicles[0].port, "10.10.1.2");
/*	
	strcpy(vehicles[0].name, "Car 2");
	strcpy(vehicles[0].file, "c2pos");
	strcpy(vehicles[0].port, "10.10.2.1");
*/	
	strcpy(vehicles[1].name, "Bike 1");
	strcpy(vehicles[1].file, "b1pos");
	strcpy(vehicles[1].port, "10.10.3.1");
	
	strcpy(vehicles[2].name, "Bike 2");
	strcpy(vehicles[2].file, "b2pos");
	strcpy(vehicles[2].port, "10.10.4.1");
	
	strcpy(vehicles[3].name, "Bike 3");
	strcpy(vehicles[3].file, "b3pos");
	strcpy(vehicles[3].port, "10.10.7.1");
	
	while(1)
	{
		for(i=0;i<NUMVEHICLES;i++)	// make switch listen for position updates from each vehicles
		{							// and funnel the position to the corresponding file
			sprintf(command, "nc -l %s > %s", vehicles[i].port, vehicles[i].file);
			system(command);
		}
		
		for(i=0;i<NUMVEHICLES;i++)
		{
			vehicles[i].ptr = fopen(vehicles[i].file,"r");	// open vehicle's file
			if(vehicles[i].ptr == NULL)					// exit if can't open
			{
				printf("couldn't open file %s\n",vehicles[i].file);
				exit(1);
			}
			
			fscanf(vehicles[i].ptr, "%d", &vehicles[i].pos);	// update position
			fclose(vehicles[i].ptr);	// close file (necessary for file to get updated?)
		}
		

		for(i=1;i<NUMVEHICLES;i++)	// check proximities to all three bikes.
		{
			dist = abs(vehicles[0].pos - vehicles[i].pos);
			if(dist < WARNDIST) // vehicles within warning threshold? (difference less than 3)
			{
				if(!dist)	//vehicles in hazard threshold? (positions are equal)
				{
					printf("%s and %s within hazard threshold\n",vehicles[0].name,vehicles[i].name);
					
					// send alert to car and bike
					sprintf(command,"echo \"HAZARD ALERT: %s\" | nc -q 0 %s",vehicles[0].name,vehicles[1].port);
					system(command);
					sprintf(command,"echo \"HAZARD ALERT: %s\" | nc -q 0 %s",vehicles[1].name,vehicles[0].port);
					system(command);
				}
				else
				{
					printf("%s and %s within warning threshold\n",vehicles[0].name,vehicles[i].name);
					
					// send warning to car and bike
					sprintf(command,"echo \"warning: near %s\" | nc -q 0 %s",vehicles[0].name,vehicles[1].port);
					system(command);
					sprintf(command,"echo \"warning: near %s\" | nc -q 0 %s",vehicles[1].name,vehicles[0].port);
					system(command);
				}
			}
		}	
	}	
	return 0;
}
