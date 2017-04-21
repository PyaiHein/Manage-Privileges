#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

using namespace std;

#define studentID 7008064

int main(int argc, char **argv)
{
	int counter = 0;
	int status;
	char myFile[] = "sniff";
	time_t timer;
	struct stat sb;
	pid_t pid;
	unsigned long shift = 0b111111111;
	int rightStudent = 0;
	int rightPassword = 0;
	int fileAvail = 0;
	int rightOwner = 0;
	int rightPermission = 0;
	int rightTime = 0;

	if(studentID != getuid())
	{
		cout << "Incorrect User." << endl;
		exit(1);
	}
	else if(studentID == getuid())
	{
		rightStudent = 1;
	}

	if(rightStudent == 1)
	{
	    pid = fork();
	    if (pid == -1)
	    {
	        // fork failed
	        printf("fork() failed!\n");
	        return 1;
	    }
	    else if (pid == 0) // child process
	    {
	        char * args[] = {(char*)"kinit", (char*)NULL};
	        char *envp[] =
	   		{
		        (char*) "/usr/bin/kinit",
		        (char*) " \t\n",
		        (char*) "/pkg/bin/tcsh",
		        (char*)NULL
	    	};

	    	execve("/usr/bin/kinit", args, envp);
	    	fprintf(stderr, "Password Failed!\n");
	    }
	    else // parent process
	    {
	    	wait(&status);
	    	if(status != 0)
	    		exit(1);
	    	else
	    		rightPassword = 1;
	    }
	}

	if(rightPassword == 1)
	{

        ifstream infile(myFile);
  
        if(!infile.good())
        {
        	cout << "Could not find file: " << myFile << endl;
        	exit(1);
        }
    	else
    	{
    		if (stat(myFile, &sb) < 0) 
    		{
	        	perror("stat");
	        	exit(EXIT_FAILURE);
   			}
   			else
   			{
   				fileAvail = 1;
   			}
   		}
   	}

   	if(fileAvail == 1)
   	{
   		if(sb.st_uid != studentID)
	    {
	    	printf("Incorrect file owner\n");
	    	exit(1);
	    }
	    else
	    {
	    	rightOwner = 1;
	    }
	}

	if(rightOwner == 1)
	{
		unsigned long temp = ((unsigned long) sb.st_mode) & shift;
        int rem, i=1, octal=0;
		while (temp!=0)
		{
	        rem=temp%8;
	        temp/=8;
	        octal+=rem*i;
	        i*=10;
	    }
	    if( ((octal / 100) % 2 != 1) || ((octal % 100) / 10 > 0) || ((octal % 10) > 0))
	    {
	    	printf("Incorrect file permissions\n");
	    	exit(1);
	    }
	    else
	    {
	    	rightPermission = 1;
	    }
	}

	if(rightPermission == 1)
	{
		time(&timer);
	    double timediff = difftime(timer,sb.st_mtime);
	    if(timediff > 60)
	    {
	    	printf("Error: File was created or modified over a minute ago.\n");
	    	exit(1);
	    }
	    else
	    {
	    	rightTime = 1;
	    }
	}

	if(rightTime == 1)
	{
		pid = fork();
	    if (pid == -1)
	    {
	        // fork failed
	        printf("fork() failed!\n");
	        return 1;
	    }
		else if (pid == 0) // child process
		{
			char * arg[] = {(char*)"chown", (char*)"root:proj", (char*)"./sniff", (char*)NULL};
			char *env[] = {(char*)NULL};

	    	execve("chown", arg, env);
	    	fprintf(stderr, "Chown failed!\n");
		}
		else
		{
			wait(&status);
			if(status != 0)
				exit(1);

			if(chmod("sniff", 04550) < 0)
			{
				perror("chmod error");
				exit(1);
			}
		}
	}
		return 0;

}
