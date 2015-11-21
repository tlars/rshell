#include <string.h>
#include <iostream>


bool test1(std::string h){
    std::size_t found1;
    std::size_t found2;
    found1 = h.find("&&");
    found2 = h.find("||");
    if(found1 == std::string::npos && found2 == std::string::npos){
        return false;
    }
  

   return true;
}


int main(){

    std::string yes = " slfjsfsj || lsjfsljfj";
    std::string yes2 = "lsfjlsjdf lsdjflsjf && fjlsjf";
    std::string no = "lsdjfsdjf&lsjjbdfbd|fj";
 
    if(test1(yes))
       std::cout<<"yes 1"<<std::endl;
    if(test1(yes2))
       std::cout<<" yes 2"<<std::endl;
    if(test1(no))
       std::cout<<" wfjwe;";
    return 0;
}   
