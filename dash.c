#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
 
#define INITIAL_PATH "/bin"
#define MAX_SIZE_FOR_PATH_DIR 2000
 
char error_message[30] = "An error has occurred\n";
 
ssize_t readinput;
size_t len = 0;
char *inputstr = NULL;
char *parallel_command_token;
char *space_token;
const char *delimiter = "&";
const char *delimiter1=" ";
const char *delimiter2 = ">";
char *saveptr1;
char *saveptr2;
char *saveptr4;
char* path_setup = INITIAL_PATH;
char line[MAX_SIZE_FOR_PATH_DIR];
pid_t pids[200];
  
void change_directory(char *path); // Changes the directory
char* create_path(char *path_recieved); // Sets the path variable with path entered by the user
void process_commands(char *commands); // Checking and executing built in commands and external commands will be passed to execute_commands()
void execute_commands(char *command, char *saveptr); // Executes external commands
void process_redirection(char *commands); // Checks if it is a valid redirection and redirects stderr and stdout to the file mentioned
void waitforpid(int pid_count); // Waits for all children to complete
void process_input(char* command); // Redirects the commands to the respective functions
 
void change_directory(char *path){
    if(chdir(path) != 0){
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
}
 
void waitforpid(int pid_count){
    for (int j = 0; j < pid_count; j++) {
        waitpid(pids[j], NULL, 0);
    }
}
 
char* create_path(char *path_recieved){
    if(path_recieved == NULL || strcmp(path_recieved,"")==0){
        if (path_setup != NULL && strcmp(path_setup,INITIAL_PATH)!=0) {
            path_setup = NULL;
        }
        path_setup = strdup("");  // Set to an empty string if path is null or empty
        return path_setup;
    }
   
    if(path_setup != NULL && strcmp(path_setup,INITIAL_PATH)!=0){  // Cleaning previous path    
        path_setup = NULL;
    }
 
    if(path_recieved != NULL){
        if(path_setup != NULL){
            path_setup =  NULL;
        }
        path_setup = path_recieved;
    }
   
    return path_setup;
}
 
void process_redirection(char *commands) {
    if (strchr(commands, '>') != strrchr(commands, '>')){ // Checking if there are mulitple redirection symbols (>)
        write(STDERR_FILENO, error_message, strlen(error_message));
        return;
    } else {
        char *redirection_commands = strtok_r(commands, delimiter2, &saveptr4);
       
        if (redirection_commands == NULL) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            return;
        }
        char *file_names = strtok_r(NULL, " \t\n", &saveptr4);
       
        if (strlen(saveptr4) != 0) { // Ensure only one filename is provided
            write(STDERR_FILENO, error_message, strlen(error_message));
            return;
        }
 
        int fileopen = open(file_names, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fileopen == -1) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            return;
        }
 
        // Save the original stdout and stderr
        int saved_stdout = dup(STDOUT_FILENO);
        int saved_stderr = dup(STDERR_FILENO);
 
        // Redirect stdout and stderr to the file
        dup2(fileopen, STDOUT_FILENO);
        dup2(fileopen, STDERR_FILENO);
        close(fileopen);
 
        // Process the command
        process_commands(redirection_commands);
 
        // Restore the original stdout and stderr
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stderr, STDERR_FILENO);
        close(saved_stdout);
        close(saved_stderr);
 
        return;
    }
}
 
void execute_commands(char *command, char *saveptr){
    if(path_setup == NULL){
        path_setup = strdup(INITIAL_PATH);
    }      
   
    char *path_copy;
    char path[MAX_SIZE_FOR_PATH_DIR]; // Buffer to hold the full path
    char *args[MAX_SIZE_FOR_PATH_DIR];
    int i = 0;
 
    path_copy = strdup(path_setup);
 
    if(command == NULL){
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    char *saveptr3;
    args[i] = command;
 
    char *cmd_token = strtok_r(NULL, " ", &saveptr);
   
    for (i = 1; cmd_token != NULL && i < MAX_SIZE_FOR_PATH_DIR - 1; i++) {
        args[i] = cmd_token;  // store the token in args array  
        cmd_token = strtok_r(NULL, " ", &saveptr);  // continue tokenizing with strtok_r
    }
    args[i] = NULL;
    int pid_count = 0;
    char *token;
 
    // Forking and executing in parallel
    for(token = strtok_r(path_copy," ",&saveptr3); token != NULL; token = strtok_r(NULL," ",&saveptr3)){
        snprintf(path, sizeof(path), "%s/%s", token, command);
        if (access(path, X_OK) == 0) {
            int rc = fork();
            if (rc < 0) {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            } else if (rc == 0) {
                // Child process executes the command
                if(execv(path, args) == -1){
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    exit(1); // child exits after error
                }
            } else {
                // Parent creates an array of child pids
                pids[pid_count++] = rc;
            }
        }
    }
 
    // Wait for all child processes after forking
    if (pid_count > 0) {
        int status;
        for (int j = 0; j < pid_count; j++) {
            while (waitpid(pids[j], &status, 0) == 0) {
                // Checking the status
            }
        }
    }
}
 
void process_commands(char *commands){
    if(commands == NULL){
        write(STDERR_FILENO, error_message, strlen(error_message));
        return;
    }
 
    char *space_token = strtok_r(commands, delimiter1, &saveptr2);
    if (space_token == NULL) {
        return;
    }
    else if(strcmp(space_token, "cd") == 0) {
        space_token = strtok_r(NULL, delimiter1, &saveptr2);
        if (space_token != NULL && strtok_r(NULL, delimiter1, &saveptr2) == NULL) {
            change_directory(space_token);
        }
        else {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        return;
    }
    else if (strcmp(space_token, "exit") == 0) {
        space_token = strtok_r(NULL, delimiter2, &saveptr2);
        if(space_token != NULL){
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        else{
            exit(0);
        }
    }
    else if (strcmp(space_token, "path") == 0) {
        space_token = strtok_r(NULL, "path", &saveptr2);
        create_path(space_token);
    }
    else{
        execute_commands(space_token, saveptr2);
    }
}
 
void compile_commands(char *inputstr) {
    for (parallel_command_token = strtok_r(inputstr, delimiter, &saveptr1); parallel_command_token != NULL;
         parallel_command_token = strtok_r(NULL, delimiter, &saveptr1)) {
        parallel_command_token = strtok(parallel_command_token, "\n");
        
        if (inputstr[0] == '&') {
            write(STDERR_FILENO, error_message, strlen(error_message));
            return;
        } else {
            // Fork for each command to execute in parallel
            pid_t pid = fork();
            if (pid == 0) {
                // In child process, execute the command
                if (strchr(parallel_command_token, '>')) {
                    process_redirection(parallel_command_token);
                } else {
                    process_commands(parallel_command_token);
                }
                exit(0);  // Child process exits after execution
            } else if (pid < 0) {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        }
    }
    // Parent waits for all child processes to finish
    int status;
    while (wait(&status) > 0);
}
 
void process_input(char* command){
    command[strcspn(command, "\n")] = '\0';
    if (strchr(command,'&') == NULL && strchr(command,'>') == NULL) {
        process_commands(command);
    } else {
        compile_commands(command);
    }
    return;
}
 
int main(int argc, char **argv){
    if(argc > 2){
        write(STDERR_FILENO,error_message, strlen(error_message));
        exit(1);
    }
    // Entering Batch Mode
    if(argc == 2){
        FILE *file_to_read = fopen(argv[1], "r");
        if (file_to_read == NULL) {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        while (fgets(line, MAX_SIZE_FOR_PATH_DIR, file_to_read)) {
            process_input(line);
        }
        fclose(file_to_read);
        if (feof(file_to_read)) {
            exit(0);
        }
    }
    // Entering Interactive mode
    if (argc == 1){
        while(1){
            printf("dash> ");
            readinput = getline(&inputstr, &len, stdin);
            if(readinput == -1){
                write(STDERR_FILENO,error_message , strlen(error_message));
                exit(1);
            }
            if(feof(stdin)){
                break;
            }
            process_input(inputstr);
        }
        free(inputstr);
        return 0;
    }
}
