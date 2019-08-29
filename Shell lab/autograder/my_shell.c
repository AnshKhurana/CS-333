#include  <stdio.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;
    int num_cmd = 0;
    int bgprocs[64];
    for (i = 0; i < 64; ++i)
    {
        bgprocs[i] = -1;
    }
	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {			
        // Parent code before the fork call
		/* BEGIN: TAKING INPUT */
        bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}

		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

        for (i = 0; i < 64; ++i)
            {
                int cpid;
                if (bgprocs[i] != -1)
                {
                    cpid = bgprocs[i];
                    int wt_suc =  waitpid(cpid, NULL, WNOHANG);
                    if (wt_suc > 0)
                    {
                        printf("Shell: Background process finished\n"); 
                    }    
                }
                
            }
   
       //do whatever you want with the commands, here we just print them
        int len = 0;
		for(i=0;tokens[i]!=NULL;i++){
            len++;
		}
       
       if (len == 0)
       {
        continue;
       }
       int mode = 0;

       if (strcmp(tokens[len-1], "&") == 0)
       {
           mode = 1;
           tokens[len-1] = NULL;
       }
       else
       {
        for (i = 0; i < len; ++i)
        {
            if (strcmp(tokens[i], "&&")==0)
            {
                mode = 2; 
                num_cmd++;
                tokens[i] = NULL;
            }
            else if (strcmp(tokens[i], "&&&")==0)
            {
                mode = 3;
                tokens[i] = NULL;
                num_cmd++;
            }

        }
       }


       if (strcmp(tokens[0], "cd")==0)
       {
           chdir(tokens[1]); // Irrespective mode, corner case
           // Freeing the allocated memory  
        for(i=0;tokens[i]!=NULL;i++)
        {
            free(tokens[i]);
        }
        free(tokens);
        continue;
       }
       if (mode == 0 || mode == 1)
       {
       int rc = fork();
       if (rc == 0)
       {
            if (mode == 0)
            {
                int status  =  execvp(tokens[0], tokens);     
                if (status < 0)
                {
                    printf("Shell: Incorrect Command\n");
                }
            }
           
            if (mode == 1)
            {
                int status  =  execvp(tokens[0], tokens);     
                if (status < 0)
                {
                    printf("Shell: Incorrect Command\n");
                }
            }

            if (mode == 2)
            {
                /* code */
            }

            if (mode == 3)
            {

            }
       }
       else if (rc < 0)
       {
           fprintf(stdout, "Fork failed\n");
       }
       else
       {
            
            
            if (mode == 0)
            {
                waitpid(rc, NULL, 0);
            }

            if (mode == 1)
            {
                
                for (i = 0; i < 64; ++i)
                {
                    if (bgprocs[i]==-1)
                    {
                        bgprocs[i] = rc;
                        break;
                    }
                }
            }
       }


		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
    }
    else
    {
        if (mode == 2)
        {
            num_cmd = num_cmd + 1;
        
        while (num_cmd != 0)
        {
            num_cmd--;

            int rc = fork();
            if (rc == 0)
            {
                int status  =  execvp(tokens[0], tokens);     
                if (status < 0)
                {
                    printf("Shell: Incorrect Command\n");
                }
            }
            else if (rc < 0)
            {
                printf("Fork failed!\n");
            }
            else
            {
                if (num_cmd == 0)
                {
                    for(i=0;tokens[i]!=NULL;i++)
                    {
                        free(tokens[i]);
                    }
                    // free(tokens);
                }
                else
                {
                    while (tokens[0] != NULL)
                    {
                        free(tokens[0]);
                        tokens++;
                    }
                    // free(tokens[0]);
                    tokens++;
                }

                waitpid(rc, NULL, 0);
            }
        }
        }
        else
        {
            num_cmd = num_cmd + 1;
            while (num_cmd != 0)
        {
            num_cmd--;

            int rc = fork();
            if (rc == 0)
            {
                int status  =  execvp(tokens[0], tokens);     
                if (status < 0)
                {
                    printf("Shell: Incorrect Command\n");
                }
            }
            else if (rc < 0)
            {
                printf("Fork failed!\n");
            }
            else
            {
                if (num_cmd == 0)
                {
                    for(i=0;tokens[i]!=NULL;i++)
                    {
                        free(tokens[i]);
                    }
                    // free(tokens);
                }
                else
                {
                    while (tokens[0] != NULL)
                    {
                        free(tokens[0]);
                        tokens++;
                    }
                    // free(tokens[0]);
                    tokens++;
                }

                for (i = 0; i < 64; ++i)
                {
                    if (bgprocs[i]==-1)
                    {
                        bgprocs[i] = rc;
                        break;
                    }
                }
            }
        }   
        }
    }
	}
	return 0;
}
