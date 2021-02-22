#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define BUFFER_LEN 1024

int main();     //main function declaration
int i = 0;      // declaring i and y for loops for -ansi
int y = 0;
char* BASH_VAR[20][2];        //for bash variables

//---------------------------------------------------------------------------------------------

void BASH(char* argv[100], int argc){
    
    if (strcmp(argv[1] , "=") == 0){                    //check if bash variable
        char* variable = argv[0];           //assign the variable
        char* number, flag = '\0';
        number = argv[2];                   //assign the number
        for ( i = 0; i<20; i++) {            //loop to update the variable if exist
            if (BASH_VAR[i][0] == variable){
                printf("truee\n");
                BASH_VAR[i][1] = number;
                flag = 1;
            }
        }

        if(flag == 0){                        //loop to add new bash variable if don't exist
        for ( i = 0; i<20; i++) {
            if(BASH_VAR[i][0] == NULL){
                BASH_VAR[i][0] = variable;
                BASH_VAR[i][1] = number;
                i = 20;                     //to update only once
            }
        }
        }
    }
}

//---------------------------------------------------------------------------------------------

void echo(char* argv[100], int argc){           //function to print in echo
    
    if( (strcmp(argv[0] , "echo") == 0) || (strcmp(argv[0] , "ECHO") == 0)){

        int flag = 0;
        for ( i = 0; i<20; i++) {
            if(BASH_VAR[i][0] == argv[1]){
                printf("%s","I LOVE you\n");
                printf("%s", BASH_VAR[i][1]);     //print the saved variable
                flag = 1;
            }
        }
        if(flag == 0){
            printf("The BASH variable doesn't EXIST!");
        }
    }
    
}

//---------------------------------------------------------------------------------------------

void pipee(char* argv[100], int argc , int place){      //DONE
    
    char* C1[5] = {0,0,0,0,0};    //for first command
    char* C2[5] = {0,0,0,0,0};     //for second command
    
    for ( i = 0; i<place; i++) {         //copy the first command in C1
        C1[i] = argv[i];
    }
    int point = 0;
    for ( y = (place+1); y<argc; y++) {      //copy the seconnd command in C2
        C2[point] = argv[y];
        point++;
    }
    
    int fd[2];      //array of two elements for the pipe
    pipe(fd);       //we send the array as an argument and if succes it will contain two new file descripteros
    pid_t pid;
    pid = fork();       //create pid then fork a child
    
    
    if (pid == 0){              //child
        dup2(fd[0], 0);         //duplicate the input
        execvp(C2[0] , C2);        //execute it
        char *er = strerror(errno);     //for error
        printf("Child error!");
        printf("%s",er);
        main();
    }else if (pid == -1){               //error
        char *er = strerror(errno);     //then pid = -1, then error
        printf("%s",er);                //to print the error
        printf("\n");
        exit(0);
    }else{              //parent
        dup2(fd[1], 1);         //duplicate pipe
        execvp(C1[0] , C1);     //execute parent first then take the output to child
        char *er = strerror(errno);     //if error generated
        printf("Parent error!");
        printf("%s",er);
        main();
        
    }
    
}
    
//---------------------------------------------------------------------------------------------

int redirection(char* argv[100], int argc ){        //DONE
    int flag = 0;
    for ( i = 0; i<argc; i++) {
        if (strcmp(argv[i],"<") == 0) flag = 1;     //check id redirection flages are present
        if (strcmp(argv[i],">") == 0) flag = 1;
    }
    return flag;                        //return 1 if the command contains redirection and 0 otherwise
}

//---------------------------------------------------------------------------------------------

void exe_redirection(char* argv[100], int argc){        //needs more working
    printf("program couldn't do redirection!");
}

//---------------------------------------------------------------------------------------------

void check_cd(char* argv[100] , int argc ){
    
    if ((strcmp(argv[0], "cd") == 0))
    {
        if (argc > 1){      //check if user entered the directory to be envoked or not
            chdir(argv[1]);     //change directory
            //main();         //return to main for another new command
        }
        else {
            printf("Please enter the directory after cd!");
          //  main();
        }
    }
}
//---------------------------------------------------------------------------------------------#
size_t read_command(char *cmd){
    if(!fgets(cmd, BUFFER_LEN, stdin)) //get command and put it in line
        return 0; //if user hits CTRL+D break
    size_t length = strlen(cmd); // get command length
    if (cmd[length - 1] == '\n') cmd[length - 1] = '\0'; // clear new line
        return strlen(cmd); // return length of the command read
}

//---------------------------------------------------------------------------------------------#

int build_args(char * cmd, char ** argv){
    char *token; //split command into separate strings
    token = strtok(cmd," ");
    int i=0;
    while(token!=NULL){// loop for all tokens
        argv[i]=token; // store token
        token = strtok(NULL," "); // get next token
        i++; // increment number of tokens
    }
    argv[i]=NULL; //set last value to NULL for execvp
    return i; // return number of tokens
}

//---------------------------------------------------------------------------------------------#

void set_program_path (char * path, char * bin, char * prog){
    memset (path,0,1024); // intialize buffer
    strcpy(path, bin); //copy /bin/ to file path
    strcat(path, prog); //add program to path
    for( i=0; i<strlen(path); i++) //delete newline
        if(path[i]=='\n') path[i]='\0';
}

//---------------------------------------------------------------------------------------------#


int main(){
    
    char line[BUFFER_LEN];  //get command line
    char* argv[100];        //holds the arguments passed
    char* bin= "/bin/";
    char path[1024];
    char dir[1024];         //to save in it current directory
    int argc;               //arg count
    
    int size_dir = sizeof(dir); //get size of the directory
    getcwd(dir,size_dir);       //get the directory

    while(1){
        printf("\n%s", dir);        //print current directory
        printf(" $: ");
        if (read_command(line) == 0 ){  //user command //set path at bin //full file pat
            printf("\n");
            exit(0);
        }                               // CRTL+D pressed
    
        if (strcmp(line, "exit") == 0 || strcmp(line, "Exit") == 0){
            exit(0);                    //exit entered
        }
        
        argc = build_args (line,argv); // build program argument
        set_program_path (path,bin,argv[0]); // set program full path
        
    //**************************************************************************
        
        check_cd( argv , argc );        //check if cd becauce it is not covered by execv
        
        if  (redirection( argv , argc ) == 1) exe_redirection( argv , argc );       //if redirection exist then execute redirection
        
        for ( i = 0; i<argc; i++) {          //check if the command have pipe, then call pipe function
            if (strcmp(argv[i],"|") == 0){
                int place = i;                  //state the place of the | sign
                pipee(argv,argc,place);         //call the pipee function to execute
            }
        }
        if(argv[1] != 0){
        BASH(argv, argc);       //check if BASH
        echo(argv, argc);       //check if echo
        }
    //**************************************************************************

        //for testing
       // for (int i = 0; i<20; i++) {
         //   printf("%s", BASH_VAR[i][0]);
          //  printf("  ");
           // printf("%s", BASH_VAR[i][1]);
            //printf("\n");
       // }

        int pid= fork(); //fork child
        if(pid==0){ //Child
            execve(path,argv,0); // if failed process is not replaced // then print error message
            fprintf(stderr, "Child process could not do execve\n");
        }else wait(NULL); //Parent
        
    }
    return 0;
}



