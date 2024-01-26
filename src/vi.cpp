#include "vi.h"

Vi::Vi(File _file):file(_file){}

void Vi::run(){
    std::cout<<"vi cmd : "<<std::endl;
    std::cout<<"d-display e-edit q-quit"<<std::endl;

    std::string cmd;
    bool run = true;
    while (run){
        std::cout<<"vi:";
        std::getline(std::cin, cmd);
        if (cmd == "q"){
            run = false;
        }
        else if (cmd == "d"){
            std::cout<<file.data<<std::endl;
        }
        else if (cmd == "e"){
            std::cout<<"enter text, end with \\n$"<<std::endl;
            std::cout<<"text:"<<std::endl;;
            std::string text {read_text()};
            memcpy(file.data,text.c_str(),text.length());
        }
    }
}

std::string Vi::read_text(){
    std::string text;
    bool read = true;

    while (read) {
        std::string line;
        std::getline(std::cin, line);

        if (line == "$") {
            text[text.length()-1] ='\0';
            read = false;
        }
        else{
            text += line + '\n';
        }
    }
    return text;
}