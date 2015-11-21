//Todd Larson's rshell
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>
#include <queue>
#include <string.h>
using namespace std;

void run_cmd(char *arr[]){
    int status;
    pid_t pid, c_pid;
    pid = fork();
        
    if(pid < 0){
        perror("fork failed");              //fork the commands into execvp
        exit(1);
    } 
    else if(pid == 0){
        execvp(arr[0],arr);
        // cout << s << endl;
        perror("execvp failed");
        exit(1);
    }
    else if(pid > 0){
        if((c_pid = wait(&status)) < 0){
            perror("wait");
            exit(1);
        }
    }
}

bool check_cmd(char *arr[]){
    int status;
    pid_t pid, c_pid;
    pid = fork();
        
    if(pid < 0){
        perror("fork failed");              //fork the commands into execvp
        exit(1);
        return false;
    } 
    else if(pid == 0){
        execvp(arr[0],arr);
        // cout << s << endl;
        perror("execvp failed");
        return false;
    }
    else if(pid > 0){
        if((c_pid = wait(&status)) < 0){
            perror("wait");
            exit(1);
            return false;
        }
    }
    return true;
}

int is_comment(std::string line){
    for(unsigned i = 0; i <line.length(); i++){   //finds the location of the comment
        if(line.at(i) == '#'){
            return i;
        }
    }
    return -1;
}
int run_test(char *t_arr[]){
  
   struct stat buf;
   if(strcmp(t_arr[0], "test") == 0){                    //runs the test command
       if(strcmp(t_arr[1], "-d") == 0){
          stat(t_arr[2], &buf);
          if(S_ISDIR(buf.st_mode))
             return 0;      
          else 
             return 1;
       }
       else if(strcmp(t_arr[1], "-f") == 0){                // then i check if it is -e -f or -d
         stat(t_arr[2], &buf);
         if(S_ISREG(buf.st_mode))
             return 0;
         else 
             return 1;
       }
       else{
           if(strcmp(t_arr[1], "-e") == 0){
               stat(t_arr[2], &buf);                        // or i check if they didnt put anything in so i default it to -e
               if(S_ISREG(buf.st_mode) || S_ISDIR(buf.st_mode))
                   return 0;
               else
                   return 1;
           }
            else{
               stat(t_arr[1], &buf);                                 // i then check the flags
               if(S_ISREG(buf.st_mode) || S_ISDIR(buf.st_mode))
                   return 0;
               else
                   return 1;
           }

       }
   }
   else{
       if(strcmp(t_arr[1], "-d") == 0){                            //depends on what the user entered i check certain flags
          stat(t_arr[2], &buf);
          if(S_ISDIR(buf.st_mode))
             return 0;
          else 
             return 1;
       }
       else if(strcmp(t_arr[1], "-f") == 0){
         stat(t_arr[2], &buf);
         if(S_ISREG(buf.st_mode))                                   // like if it is a file i check the S_ISREG flag
             return 0;
         else 
             return 1;
       }
       else{
           if(strcmp(t_arr[1], "-e") == 0){
               stat(t_arr[2], &buf);
               if(S_ISREG(buf.st_mode) || S_ISDIR(buf.st_mode))
                   return 0;
               else
                   return 1;
           }
            else{
               stat(t_arr[1], &buf);                                  //returns true or false if the file exists or not
               if(S_ISREG(buf.st_mode) || S_ISDIR(buf.st_mode))
                   return 0;
               else
                   return 1;
           }

       }
   }
   cout<<"Error"<<endl;
   return 1;
}

bool do_op(char * do_array[]){
     unsigned i = 1;
     int fail = 2;
     int count = 0;                          //this function does the operation
     char * an_array[20];                    // it returns true or false 
     unsigned j = 0;                         // I have a flag that tells the function wheither or not the next command should run

     while(do_array[i+1] != NULL){
        // cout<<count<<endl;
         count++;
         if(strcmp(do_array[i], "&&") == 0){
             an_array[j] = NULL;                     // if the flag is not 0 and the conncetor is &&
             if(fail != 1){
                 if(!check_cmd(an_array)){
                     fail = 1;
                 }
             else                                    // then i run the command
                 fail = 0;
             j = 0;
             i++;                                    // else i move on and set the flag to 0
             }
         }
         else if(strcmp(do_array[i], "||") == 0){
             an_array[j] = NULL;
             if(fail != 0){ 
                 if(check_cmd(an_array)){
                     fail = 0;                            // if flag is 0 and conncetor is ||
                 }
             }
             else
                 fail = 1;                                   //run the command
             i++;
             j = 0;
         }                                                  //else set flag to 1
         an_array[j] = do_array[i];
         i++;
         j++;
     }
 
     an_array[j] = NULL;
     if(fail != 1){
         run_cmd(an_array);
         return true;
     }                                              //return true or false so other functions can check if they should 
     return false;                                  // run the next command or skip over it
}

bool check_operator(std::string h){
    std::size_t found1;
    std::size_t found2;
    found1 = h.find("&&");
    found2 = h.find("||");

    if(found1 == std::string::npos && found2 == std::string::npos){
        return false;                                                     //checks if the cmd has || or && and returns false if it doesnt
    }
    
    char *ch_array[50];
    char *token;
    char new_in[100];

    strcpy(new_in,h.c_str());
    token = strtok(new_in," ");
   
    unsigned i =0;
    int count = 0;

    while(token != NULL && i < 100){                                      //else i will tokenize the string it an array
        ch_array[i] = token;
        token = strtok(NULL, " \"");
        i++;       
    } 

    ch_array[i] = NULL;
    char *f_array[20];
    unsigned j =0;
    i = 0;

    while(ch_array[j] != NULL){                                             // i then make a new array that will be sent into the 
        
        if(strcmp(ch_array[j], "(")  ==0){
            while(strcmp(ch_array[j], ")") != 0){
                f_array[i] = ch_array[j]; 
               // cout<<count<<" "<<f_array[i]<<endl;
                count++;                                //newely added part
                i++;
                j++;
            }
            f_array[i] = ch_array[j];
           // cout<<count<<" "<<f_array[i]<<endl;
            j++;
            f_array[i+1] = NULL;                                                          //checks if the user enterd ()
            if(strcmp(ch_array[j], "&&") == 0){
               
                i = 0;
                if(!do_op(f_array)){       //fixme
                   if(strcmp(ch_array[j+1], "(") == 0){
                       while(strcmp(ch_array[j], ")") != 0){
                           j++;
                       }                //collects the arugemnts in () and sends them into do_op()
                      
                       if(ch_array[j+1] == NULL)
                           return true;
                   j++;
                   }
                   else{                      //fixme
                       j++;
                       while(strcmp(ch_array[j], "||") != 0 || strcmp(ch_array[j], "&&") != 0){
                            if(ch_array[j+1] == NULL)
                                return true;
                            j++;
                       }
                   } 
                  // cout<<"hi"<<endl; 
                   j++;
                }
            }
            else if(strcmp(ch_array[j], "||") == 0){
                 i = 0;                                                      //the result will determine if I skip the next line of commands
                 if(do_op(f_array)){
                   // cout<<"hello"<<endl;
                    j++;
                    if(strcmp(ch_array[j], "(") == 0){
                        while(strcmp(ch_array[j], ")") != 0){
                          // cout<<"no"<<endl;
                           j++;
                        }
                    if(ch_array[j+1] == NULL)
                        return true;

                    j++;
                    }
                    else{
                        j++;                                                //or the the rest execute
                       // cout<<"marco"<<endl;
                        while(strcmp(ch_array[j], "||") != 0 || strcmp(ch_array[j], "&&") != 0){
                           // cout<<"polo"<<endl;
                            if(ch_array[j+1] == NULL)
                                return true;
                            j++;
                        }
                    } 
                 }
                 j++;
            }
        }
        else if(strcmp(ch_array[j], "&&") == 0){
            f_array[i] = NULL;

            if(strcmp(f_array[0], "test") ==0 || strcmp(f_array[0], "[") ==0){          //test if the user entered the test command
               if(run_test(f_array) == 0){
                    i = 0;
                    j++;                                                               //if they did i run the command in run_test()
                    while(ch_array[j] != NULL){
                        f_array[i] = ch_array[j];
                        i++;
                        j++;                                                           //it depends on the result i will run the next command
                    }

                    f_array[i] = NULL;
                    run_cmd(f_array);
                    return true;
               } 
               else
                   return true;
               
            }
            else if(!check_cmd(f_array)){                                    // and if i dont get the desired result i ignore the next part of the command
                j++;

                while((strcmp(ch_array[j],"&&") != 0 && strcmp(ch_array[j],"||") != 0)){
                    j++;
                    if(ch_array[j] == NULL)
                        return true;                                   // if the next part is the end i will return back to the main
                }
            }

            j++;
            i = 0;
        }
        else if(strcmp(ch_array[j], "||") == 0){
            f_array[i] = NULL;

            if(strcmp(f_array[0], "test") ==0 || strcmp(f_array[0], "[") ==0){
               if(run_test(f_array) == 1){                                                 //checks the test command with the ||
                   
                    i = 0;
                    j++; 

                    while(ch_array[j] != NULL){
                        f_array[i] = ch_array[j];
                        i++;
                        j++;                                                          //if the file is not there then i run the next command
                    } 

                    f_array[i] = NULL;
                    run_cmd(f_array);
                    return true;
               }
               else
                   return true; 
            }
            else if(check_cmd(f_array)){
                j++;
                while(strcmp(ch_array[j], "||") != 0 && strcmp(ch_array[j], "&&") != 0){
                    j++;
                    if(ch_array[j] == NULL)                                           //the other part of igonoring the command
                        return true;
                }
            }
            j++;
            i= 0;
        }
        if(strcmp(ch_array[j],"exit") == 0)
            exit(1);

        f_array[i] = ch_array[j];
        i++;
        j++;
    }
    f_array[i] = NULL;
    run_cmd(f_array);
    return true;                                             //if i have reached the last command i just run it
}

int main(int argc, char * argv[]){
    string  in;    // user input for the command line
    
    char *char_array[50];
    char *token;
    char new_in[100];
    while(1){
        cout << "$ ";
        getline(cin,in); 
        if(in.compare("exit") == 0){
           exit(1);
        }
        if(is_comment(in) >= 0){
           in = in.substr(0,is_comment(in));   // if i found a comment
        }                                     // then i ignore the rest of the input
        strcpy(new_in, in.c_str());
        token  = strtok(new_in, ";"); // breaks up the input into single commands
        unsigned i = 0;
        while(token != NULL && i < 100){
           char_array[i] = token;
           token = strtok(NULL,";");      //this will tokenize the array by seperating them if there is a ;
           i++;
        }  
       
        queue<string> cmd_queue;
 
        for(unsigned j = 0; j < i; j++){
           cmd_queue.push(char_array[j]);    //makes a queue full of commands
        }               
        char * new_token;
        char new_temp[100];
        char * char_cmd[10];
        string temp;
        unsigned k =0;
        while(!cmd_queue.empty()){
           temp = cmd_queue.front();     //run the queue of commands
           if(!check_operator(temp)){
               cmd_queue.pop();
               strcpy(new_temp,temp.c_str());
               new_token = strtok(new_temp," ");   //will run until the all the commands have been run
               k =0;
               while(new_token != NULL && k < 100){
                   if(strcmp(new_token, "exit") == 0){           //double check the exit
                       exit(1);
                   }
                   char_cmd[k] = new_token;
                   new_token = strtok(NULL," \"");
                   k++;
               } 
               char_cmd[k] = NULL;
               if((strcmp(char_cmd[0], "[") ==0) || (strcmp(char_cmd[0], "test") == 0)){ 
                  if( run_test(char_cmd) == 0)
                      cout<<"works"<<endl;
               }
               else
                   run_cmd(char_cmd);
           }                                        //takes the commands and forks them
           else{
               cmd_queue.pop();
           } 
       }
    }
    return 0;

}
