
/*------------------------------------------------------------------------------
# tatyana Vlaskin + references
# Filename: adventure.c
#reference:will be provided upon request

------------------------------------------------------------------------------*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <string.h>
#include <time.h>

/******************************************************************************
 * struct room_Structure{
 * will store room_Name, path and file name of the room,
 * int room_Type -- 3 possible types: 1,2 and 3 which corresponds to
 * start, middle romm and end
 * will also store connections of the room
 * will also store count of the connections
 ******************************************************************************/
struct room_Structure{
    char room_Name[20];
    char file_Room_Name[80];
    int room_Type;
    char connection[10][80];
    int room_Connection_Count;
};


//FUNCTION PROTOTYPES

/******************************************************************************
 *  Function:  void swap (int * value1, int *value2)
 *  Description: Function to swap 2 numbers, will be used as a helper
 *  function to randomize room
 *  Parameters: value1, value2 - these are 2 elements to be swapped
 ******************************************************************************/
void swap (int *value1, int *value2);

/******************************************************************************
 *  Function:  void randomize_Rooms(int array[], int n)
 *  Description: Function to generate a random permutations of array[] filled
 *  with room names
 *  Parameters: elements of the array. In or case, elements are rooms
 *  Helper function: void swap (int * value1, int *value2)
 ******************************************************************************/
void randomize_Rooms(int array[], int n);

/******************************************************************************
 *  Function:  int file_Permission()
 *  Description: Function to set permission of the file to
 *  0777 - all can read, write and execute and converting it to octal
 *  Parameters: none
 ******************************************************************************/
int file_Permission();

/******************************************************************************
 *  Function:  int get_Random_In_Range ( int min , int max )
 *  Description: Generates a random # in the range [min, max]
 *  Parameters: int min and int max
 ******************************************************************************/
int get_Random_In_Range(int min , int max);


/****************************************MAIN STARTS HERE*************************************************/
int main(){

        //per lecture notes we need a buffer that will hold the data that we will write
		char buffer[33];
        //variable for loop counters
        int i, x;
        //my username, will be used in the name of the directories of the room
		char username[33] = "vlaskint.rooms.";
		 //variable to store the name of the room directory
		char directory_Name[66];
		int steps = 0;
		char path[255][255];
		// variable to keep track of the room where we start a game
        int start = 0;
        //variable to count mid rooms
        int mid = 0;
        //variable to keep track of the end room
        int	end = 0;
        char currrent_Room_File[255]; //self explanatory
        char room_Buffer[255]; //everything will be written in the buffer first
        char connection_Display[255];  //to dipslay connections in a formated way
        char next_Room[255];  // will keep track of the rooms that the user will enter next
        char list_Of_Connections[255]; //list of possible connections that will be displayed to the user to enter
        int possible_Connections; //number of possible connections that will be gerated randomly
        int loop_Counter; //another variable for looping
        int dublicate_Check; // boolean will be used to check for dublicate connections
        char current_Room_Connections[10][255];
         //declare and initialize an array of numbers 1 though 10, for 10 rooms
        int array_Of_Ten_Rooms[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        //initialize an array of structure that will store 10 structures [10 rooms]
		//https://support.microsoft.com/en-us/kb/75602
		struct room_Structure room[10];
			for(i = 1; i <=10; i++){
				struct room_Structure room[i];
			}
        //call of a function to get the permission of the file
		int file_permission = file_Permission();
		//Write formatted data to string
        //Composes a string with the same text that would be printed if format was used on printf,
        //but instead of being printed, the content is stored as a C string in the buffer pointed by str.
        //we get the process ID of the calling process as an int then convert int to string and store it in the buffer
		snprintf(buffer, 33, "%d", getpid());
		//make a full directory name: username.room.process id
		snprintf(directory_Name, sizeof(username)+sizeof(buffer)+1,  "%s%s",username, buffer);
        //creating a directory
		mkdir(directory_Name, file_permission);
        //randomize the array of 10 rooms
        randomize_Rooms(array_Of_Ten_Rooms, 10);
        //initialize the name of the room in the array of structures
        //please note that with each run of the program, the names
        //will be different because of the calll of the randomize_Rooms(array_Of_Ten_Rooms, 10);
		/*room names, the array numbers are random each run of the program*/
		snprintf(room[array_Of_Ten_Rooms[0]].room_Name, 10, "Lithuania");
		snprintf(room[array_Of_Ten_Rooms[1]].room_Name, 10, "Macedonia");
		snprintf(room[array_Of_Ten_Rooms[2]].room_Name, 7,  "Russia");
		snprintf(room[array_Of_Ten_Rooms[3]].room_Name, 11, "San Marino");
		snprintf(room[array_Of_Ten_Rooms[4]].room_Name, 7,  "France");
		snprintf(room[array_Of_Ten_Rooms[5]].room_Name, 9,  "Portugal");
		snprintf(room[array_Of_Ten_Rooms[6]].room_Name, 8,  "Germany");
		snprintf(room[array_Of_Ten_Rooms[7]].room_Name, 9,  "Holy See");
		snprintf(room[array_Of_Ten_Rooms[8]].room_Name, 8,  "Austria");
		snprintf(room[array_Of_Ten_Rooms[9]].room_Name, 9,  "Slovakia");
		//set the connection count to 0
		for (i = 1; i <= 10; i++){
			room[i].room_Connection_Count = 0;
		}
        //we are choosing 7 rooms from the 10 rooms that are available to us
        //for each of those room we are assigning a connection
		for(i = 1; i <=7; i++){
            //declare and initialize an array with values 1-7, so the 7 rooms that will be selected
			int Assigned_Rooms[] = {1, 2, 3, 4, 5, 6, 7};
            //call of a function to randomize rooms in the selected_Rooms array
			randomize_Rooms(Assigned_Rooms, 7);
			int assigned_Room_Number;
			int k, l; //more loop counters
			int connections; //used for connection count
			int dublicate_Connection; //used with the strcmp function to check for dublicates
			int reverse_Connection; //loop over the reverse connections
			int iterarate_Connection_Count; //ued for reverse connection count
            int randomly_Assignged_Number_Of_Connections;
            // call a function to randomly assign # of connections for each room
			//min = 3 connections, max = 6 connections
			randomly_Assignged_Number_Of_Connections = get_Random_In_Range(3,6);
			for(k = 0; k <= randomly_Assignged_Number_Of_Connections; k++){
				 //reset the duplicate_Check to any number other than 0
				dublicate_Connection = 99;
				//once connection of chosen we need to make sure that its already is not on the list of connections
				assigned_Room_Number = Assigned_Rooms[k];
		        //look at the connections that already exist and make sure that
				//is not already on the list
				connections = room[i].room_Connection_Count;
				for(l = 1; l <= connections; l++){
                    //use of strcmp function to check for duplicate name
                    //if result of the comparison is 0, this is an indication that connection is already on the list and
                    //we need to chose a different connection
					dublicate_Connection = strcmp(room[i].connection[l], room[assigned_Room_Number].room_Name);
					if(dublicate_Connection == 0){
						break;
					}
				}
                //making sure that connections are not maxed out, if yes, connection count will be set to max value
				if(room[i].room_Connection_Count >=6){
                    k = randomly_Assignged_Number_Of_Connections;
				}
				//if connection is the dublicate connection , it will not be added to the list of connection
				else if(dublicate_Connection == 0){
				}
				//if room is trying to connect to itself, nothing will be done
				else if (strcmp(room[assigned_Room_Number].room_Name, room[i].room_Name) == 0){

					}
                //if connection passed all the tests it need to be added to the list of connections in the room file
				else{
					//increment to connection count
					room[i].room_Connection_Count++;
                    //http://www.qnx.com/developers/docs/6.5.0/index.jsp?topic=%2Fcom.qnx.doc.neutrino_lib_ref%2Fs%2Fsnprintf.html
                    //Write formatted output to a character array, up to a given maximum number of characters
					snprintf(room[i].connection[room[i].room_Connection_Count], sizeof(room[assigned_Room_Number].room_Name),room[assigned_Room_Number].room_Name);


                /*******************Code to go from this Connection Back to the Room from which we entered this connection************/
                /**************In other words we need to make sure that we can go back and forth between 2 connections [rooms]*********/
                    //now we need to make sure that there is a reverse connection - in other words we should be able to go back to the room from
                    //which we entered into this connection [room]
                    //first we'll check if connection already exists
                    //this is essentially copy and paste of the code from above/
					iterarate_Connection_Count = room[assigned_Room_Number].room_Connection_Count;

                    //check to see if there is already a connection
                    //copy and past of the code above
					reverse_Connection = room[assigned_Room_Number].room_Connection_Count;

					 //if the already 6 connnections or connection already exists
					if ((iterarate_Connection_Count >= 6) ){
						break;
					}
					   //if all the point checks were passed, the reverse connection will be added
					else{
						iterarate_Connection_Count++;
						room[assigned_Room_Number].room_Connection_Count++;
						snprintf(room[assigned_Room_Number].connection[iterarate_Connection_Count], sizeof(room[i].room_Name), room[i].room_Name);
					}
				}
			}
		}

        /**************************Code to Chose Make Files*****************************/
        //one file needs to be created for each room, thus total of 7 files will be created
        //see http://www.thegeekstuff.com/2012/07/c-file-handling/
		for( i = 1; i <= 7; i++){
			int rondomize_Room_Type;
			int j, cons;

			FILE *iteration_File;
			// create a directory with the room name
			snprintf(room[i].file_Room_Name, sizeof(directory_Name)+ sizeof(room[i].room_Name) +1, "./%s/%s", directory_Name, room[i].room_Name);

            //open a file to apend mode
            //The first argument is a pointer to a string containing name of the file to be opened while the second argument is the mode in which the file is to be opened.
            //we will open a file in the a - Open for appending (writing at end of file). The file is created if it does not exist. The stream is positioned at the end of the file.

			iteration_File = fopen(room[i].file_Room_Name, "a");
            // if file cannot be open
			if (iteration_File == 0){
                //The standard error stream, which is used for error messages and diagnostics issued by the program.
                //http://www.gnu.org/software/libc/manual/html_node/Standard-Streams.html
				fprintf(stderr, "Could not open %s\n", room[i].file_Room_Name);
				exit(1);
			}
            //if file is opened successfully, room name will be appended to the end
			fprintf(iteration_File, "ROOM NAME:%s\n", room[i].room_Name);
			//connections that were determined above, will be appended for each file room as well
			//int determined_Connections;
			//count of determined connections/
			cons = room[i].room_Connection_Count;
			for(j = 1; j <= cons; j++){
				fprintf(iteration_File, "CONNECTION %d:%s\n", j, room[i].connection[j]);
			}
                //assignment of the room room_Type, we have 3 categories of rooms: START, MID AND END
                //a number will between 1 to 3 will be chosen randomly
                //1 --start room
                //2--mid room
                //3--end room
                //http://stackoverflow.com/questions/13471241/rand-function-c
                //code a most of the code from the piazza discussion board for cs 161 and cs 261
				rondomize_Room_Type = rand() % 3 + 1;
                //assignment of the start room
                //https://github.com/hinesro/cs344/blob/9d18481422d26906651a7253c24680acc886bbe5/assignment2/VERSION%20CONTROL/am4.47_FUKN_DUN_M8.c
                //the only difference is that here integerst
				if( rondomize_Room_Type == 1){
                    //this is an indication that start room was not assigned yet
					if(start == 0){
						fprintf(iteration_File, "ROOM TYPE:START_ROOM\n");
						room[i].room_Type = 1;
						start++;
					}
					else
						rondomize_Room_Type++;
				}
				//assignment of the end room
                //code taken fro
				if( rondomize_Room_Type ==3){
                    // this is an indication that end room was not assigned yet
					if(end == 0){
						fprintf(iteration_File, "ROOM TYPE:END_ROOM\n");
						room[i].room_Type = 3;
						end++;
					}
                    //if start room is not NULL, the random number will be incremented and room will be assigned to another category
					else
						rondomize_Room_Type--;
				}
				 //assignment of the middle room
				if (rondomize_Room_Type == 2){
                    //if count less than 5 assing to mid room
					if (mid <= 5){
						fprintf(iteration_File, "ROOM TYPE:MID_ROOM\n");
						room[i].room_Type = 2;
						mid++;
					}
                    //if count is 5 and there is no start room. assing to start
					else if(start == 0){
						fprintf(iteration_File, "ROOM TYPE:START_ROOM\n");
						room[i].room_Type = 1;
						start++;
					}
					//if count is 5 and there is no end room assign to end
					else{
						fprintf(iteration_File, "ROOM TYPE:END_ROOM\n");
						room[i].room_Type = 3;
						end++;
					}
				}
				//close file
			fclose(iteration_File);
		}
        /**************************Code for User Interface*****************************/
        //need to find start room
        //there are 7 files in the directory, we will loop looking for the file with room_Type 1
        //as mentioned previously room_Type = 1 is the start room
			for(i=1; i <= 7; i++){

				if(room[i].room_Type == 1){
					snprintf(currrent_Room_File, sizeof(room[i].file_Room_Name), room[i].file_Room_Name);
					steps+=1;
					snprintf(path[steps], sizeof(room[i].room_Name), room[i].room_Name);
					break;
				}
			}

		//until the final room is found, we'll continue looping
		do{
            //http://www.qnx.com/developers/docs/6.5.0/index.jsp?topic=%2Fcom.qnx.doc.neutrino_lib_ref%2Fs%2Fsnprintf.html
			FILE *game_File;
            //open a file using a read mode
			game_File=fopen (currrent_Room_File, "r");
            //The fseek() function is used to set the file position indicator for the stream to a new
			//position. This function accepts three arguments.
			//1 --- the FILE stream pointer returned by the fopen() function.
			//2-- ‘offset’ tells the amount of bytes to seek.
			//3 -- ‘whence’ tells from where the seek of ‘offset’ number of bytes is to be done.
			// The available values for whence are SEEK_SET, SEEK_CUR, or SEEK_END.
			//we will start from SEEK_CUR
			fseek(game_File, 10, SEEK_CUR);
            //Get current position
			//char *fgets(char *str, int n, FILE *stream) reads a line from the specified stream and stores
			//it into the string pointed to by str. It stops when either (n-1) characters are read, the newline
			//character is read, or the end-of-file is reached, whichever comes first.
			fgets(room_Buffer, 13, game_File);
            //display possible connections
			possible_Connections = room[i].room_Connection_Count;
			for(loop_Counter = 1; loop_Counter <= possible_Connections; loop_Counter++){
				fseek(game_File, 13, SEEK_CUR);
				fgets(connection_Display, 13, game_File);
                //The snprintf() function formats and stores a series of characters and values in the array buffer.
				//Any argument-list is converted and put out according to the corresponding format specification
				//in the format-string. The snprintf() function is identical to the sprintf() function with the
				//addition of the n argument, which indicates the maximum number of characters (including the
                //ending null character) to be written to buffer.
				snprintf(current_Room_Connections[loop_Counter], sizeof(connection_Display), "%s", connection_Display);
                //char *strtok(char *str, const char *delim) breaks string str into a series of tokens using the delimiter delim
				//the delim in this case will be new line
				strtok(connection_Display, "\n");
                //need to print in the following format:
                //POSSIBLE CONNECTIONS: France, Macedonia, San Marino, Portugal, Slovakia.
                //look at the code on the following websites
                //http://linux.die.net/man/3/snprintf
                //http://gribblelab.org/CBootcamp/10_Input_and_Output.ht
				if(loop_Counter == 1){
					snprintf(list_Of_Connections, sizeof(connection_Display),"%s, ", connection_Display );
				}
				else if(loop_Counter != possible_Connections){
					snprintf(list_Of_Connections + strlen(list_Of_Connections), sizeof(connection_Display),"%s, ", connection_Display );
				}
				else{
					snprintf(list_Of_Connections + strlen(list_Of_Connections) , sizeof(connection_Display),"%s", connection_Display );

				}
			}
			//print the location, connections, and get user input
			//will stay inside the loop until the user make a valid entry
			do{
				printf("CURRENT LOCATION: %s", room_Buffer);
				printf("POSSIBLE CONNECTIONS: %s.\n", list_Of_Connections);
				printf("WHERE TO? >");
				fgets(next_Room, 255, stdin);
				printf("\n");
				//reset of the  boolean for each iteration to make sure that its not 0
				dublicate_Check = 99;
				//looping from possible_Connections
				for(x = 1; x <= possible_Connections; x++){
                    //compare entry to the rooms in the current-room connections
					dublicate_Check = strcmp(next_Room, current_Room_Connections[x]);
					//if there is a match break from this loop to enter that room
					if (dublicate_Check == 0){
						break;
					}
                    //if the input does not match, give an error message
					if(x == possible_Connections){
						printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
					}
				}
			}while(dublicate_Check !=0);
			fclose(game_File);//close the file
					//compare next_Room with connections in the struct to make sure that its valid
					dublicate_Check = 99;
					strtok(next_Room, "\n");
					for(i = 1; i <= 7; i++){
						dublicate_Check = strcmp(next_Room, room[i].room_Name);

						if(dublicate_Check == 0){
							snprintf(currrent_Room_File, sizeof(room[i].file_Room_Name), room[i].file_Room_Name);
							break;
						}

					}
					steps+=1;
					snprintf(path[steps], sizeof(room[i].room_Name), room[i].room_Name);
			}while(room[i].room_Type != 3);
            //the end
			printf("YOU HAVE FOUND THE END CITY. CONGRATULATIONS!\n");
			printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", steps);

			for(x = 1; x <=steps; x++){
				printf("%s\n", path[x]);
			}
	exit(0);

	}

//FUNCTION DEFINITIONS
/******************************************************************************
 *  Function:  void swap (int * value1, int *value2)
 *  Description: Function to swap 2 numbers
 *  Parameters: value1, value2 - these are 2 elements to be swapped
 ******************************************************************************/
void swap (int * value1, int *value2){
    int temp = *value1; //save 1s value in temp variable
	*value1 = *value2;  // value1 is assigned to be value2
	*value2 = temp;     //value 2 is assigned to temp, which hold old value of value1
}

/******************************************************************************
 *  Function:  void randomize_Rooms(int array[], int n)
 *  Description: Function to generate a random permutations of array[] filled
 *  with room names
 *  Parameters: elements of the array. In or case, elements are rooms
 *  Helper function: void swap (int * value1, int *value2)
 ******************************************************************************/

void randomize_Rooms(int array[], int n){
            //initialize randome
		srand( time(NULL));
    // we will start from the last element and swap one by one
    //we do not need to run for the 1st element thats why  i>0
    int i;
	for (i = n-1; i > 0; i--){
        //pick random index from 0 to i
		int j = rand() % (i+1);
        //swap array[i] with the element at random index
		swap(&array[i], &array[j]);
	}
}

/******************************************************************************
 *  Function:  int file_Permission()
 *  Description: Function to set permission of the file to
 *  0777 - all can read, write and execute and converting it to octal
 *  Parameters: elements of the array. In or case, elements are rooms
 *  Helper function: void swap (int * value1, int *value2)
 ******************************************************************************/
	// The strtol() function converts the initial part of the string in nptr
    //to a long integer value according to the given base
    //in our case the base is 8
    //http://stackoverflow.com/questions/4568681/using-chmod-in-a-c-program
    //https://www.raspberrypi.org/forums/viewtopic.php?f=29&t=87268
    //http://unix.stackexchange.com/questions/103413/is-there-any-difference-between-mode-value-0777-and-777
    //http://www.thinkplexx.com/learn/article/unix/command/chmod-permissions-flags-explained-600-0600-700-777-100-etc
int file_Permission(){
    char mode[] = "0777"; // read the permissian to the character array. the persmision will be set to 0777
	int octal = strtol(mode, 0, 8);  // do the octal conversion
	return octal;
}

/******************************************************************************
 *  Function:  int get_Random_In_Range ( int min , int max )
 *  Description: Generates a random # in the range [min, max]
 *  Parameters: int min and int max
 ******************************************************************************/
int get_Random_In_Range ( int min , int max ) {
    int random = min + (rand() % (max - min +1));
    return random;
}
























