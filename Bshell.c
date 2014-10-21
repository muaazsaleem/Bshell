#include <stdlib.h>
#include <stdio.h>
#include <string.h>	// Mainly for strtok
#include <unistd.h>


#include <fcntl.h>


#define MAX_ARGS 2

char * PATH = "MyShell:$:";
static char * Path_array[10];
extern char **environ;

//char *path; //for the command path

typedef struct{
	char *cmd_path;	//path of command
	int argc;	//number of args for the cmd
	char *argv[MAX_ARGS];
	//char cmd_suffix;
}commandStruct;

//void free_struct (commandStruct * command1)
//{
	
	
//	command1=NULL;

//}

int runCommand(commandStruct * command1);


void path_token()
{
	
	 
	int i = 0;

	
	char *pathstr= getenv("PATH");
	
	
	Path_array[i] = strtok(pathstr,":");
	
	
	while(Path_array[i]!=NULL)
	{
		//fprintf(stderr, "%s\n",Path_array[i] );
		
		i++;
		Path_array[i] = strtok(NULL,":");
		
		
	}
	//printf("%s\n","out" );
	Path_array[i]='/0';
	//return 0;
}



char* attach_path(char *cmd)
{
	char ret[100];
	int index=10;
	int fd;
	bzero(ret, 100);
	
	//fprintf(stderr, "%s\n", "I'm here");
	
	
	
	for(index=0;index<10;index++) 
		{ 
				
				//fprintf(stderr, "%s\n", " I'm ");


				strcpy(ret, Path_array[index]);
				strncat(ret,"/",1);

				//printf("%s\n",ret );

				//fprintf(stderr, "%s\n", "I'm here");
				
				strncat(ret, cmd, strlen(cmd));
				printf("%s\n",ret );

				
				if (fd = access (ret, F_OK)==0)
					{
	    		printf( "File Found!!!\n");
	    		return ret;
			}

		
		}
	
		printf("%s\n","file not found" );
		return; //exit(EXIT_FALIURE);
}


int main() {
	
	system("clear");
	int flag = 1;
	do {
		char buffer[256], *input;
		pid_t child;
		int childstatus, status;	//For Fork
	
		int x = 1;
		char *cmd;			//For strtok

		char *command;			//The command
		int cmdlen;

		commandStruct *command1 = (commandStruct*) malloc(sizeof(commandStruct));	
		command1->argc = 1;
		
		path_token(); //Saves $PATH into an array

		printf("\n%s", PATH);
		fflush(stdout);
		input = fgets(buffer, 256, stdin);

		cmdlen = strlen(input);		//Take out new line char
		if (input[cmdlen-1] == '\n') {	//
			input[cmdlen-1] = '\0';	//
		}				//

		//Get first token (the command) from input
       		cmd = strtok(input, " ");
		command = cmd;

		if (strcmp(command, "exit") == 0){
			flag = 0;
		}	

		command1->cmd_path = command;
		command1->argv[0] = command1->cmd_path;
	
		//Loop thru until whole command is tokenized
	        while (1)
	        {
	               	//Check command has no more than 2 args
				if (command1->argc > 3) {
					printf("\nError! Too many command arguements!\n");
					break;
				}

					//Extract next part of command
	                cmd = strtok(NULL, " ");

	                //Check that there is nothing else to extract
	                if (cmd == NULL)
		                {
		                        //printf("Command Tokenised");
					break;
				}

			command1->argv[command1->argc] = cmd;	//put arg into arg array
			command1->argc = command1->argc+1;	//increase count of args
		         	
	        }

		printf("Number of args: %d\n", command1->argc);
		strcpy(command1->cmd_path,attach_path(command1->cmd_path));
		//printf("%s\n","Printing cmd_path" );
		//printf("%s\n",command1->cmd_path );
	
		childstatus = fork();
		if (childstatus != 0) { //if parent process is running
			wait(&status);
		} else {
			/* This area is child process */
			runCommand(command1);
			return(0);
		}	
		free(command1);

	}while (flag == 1);

	return 0;
	exit(0);
}


int runCommand(commandStruct * command1) {
	
	//printf("Path: %s\n", command1->cmd_path);
	
	execve(command1->cmd_path, command1->argv, environ);
	//printf("%s\n%s\n%s\n", command1->argv[0], command1->argv[1], command1->argv[2]);
	return 1;

}
