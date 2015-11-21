#include<string.h>
#include<stdio.h>

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

bool check_operator(std::string h){
    std::size_t found1;
    std::size_t found2;
    found1 = h.find("&&");
    found2 = h.find("||");
    if(found1 == std::string::npos && found2 == std::string::npos){
        return false;
    }
    std::queue<int> num_op;
    std::queue<std::string> cmd;
    while(found1 != std::string::npos && found2 != std::string::npos){
        if(found2 > found1 && found1 != std::string::npos){
            cmd.push(h.substr(0,found1));
            num_op.push(0); 
            h.erase(h.begin()+ found1 + 1);
        }  
        else{
            cmd.push(h.substr(0,found2));
            num_op.push(1); 
            h.erase(h.begin()+ found2 + 1);
        }
        if(found1 > found2 && found2 != std::string::npos){
            cmd.push(h.substr(0,found2));
            num_op.push(1); 
            h.erase(h.begin()+ found2 + 1);
        }  
        else{
            cmd.push(h.substr(0,found1));
            num_op.push(0); 
            h.erase(h.begin()+ found1 + 1);
        }
        found1 = h.find("&&");
        found2 = h.find("||");
    } 
    char * new_token;
    char new_temp[100];
    char * char_cmd[10];
        
    while(!cmd.empty()){
        strcpy(new_temp,cmd.front().c_str());
        new_token = strtok(new_temp," |&");
        cmd.pop(); 
        unsigned i =0;
        while(new_token != NULL && i < 100){
            char_cmd[i] = new_token;
            new_token = strtok(NULL, " |&");
            i++; 
        }      
        char_cmd[i] = NULL;
        if(num_op.front() == 0){
            num_op.pop();
            if(!check_cmd(char_cmd)){
                cmd.pop();
            }
        }
        else{
            num_op.pop();
            if(check_cmd(char_cmd)){
                cmd.pop();
            }
        } 
    } 
    return true;
}
