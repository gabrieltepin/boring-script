#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <limits>
using namespace std;

//build it with: g++ -std=c++11 cleanarticles.cpp -o cleanarticles
//run it with: ./cleanarticles

DIR *dpdf;
struct dirent *epdf;

int main() {
    fstream new_file; 
    string str, substr, inlineStyleBegin("style=\""), boldTagBegin("<b"), boldTagEnd("</b");
    char c;

    dpdf = opendir("./");
    if (dpdf != NULL){
        //iterating over all files in this script directory
        while ((epdf = readdir(dpdf))){
            string fileName(epdf->d_name);
            if(fileName.compare(".")==0) continue;
            else if(fileName.compare("..")==0) continue; 
            else {
                new_file.open(fileName, ios::in); 
                if(!new_file) 
                    cout<<"FILE NOT FOUND\n"; 
                else{ 
                    setbuf(stdin, NULL);
                    ofstream temp;
                    temp.open("temp.txt"); 
                    //iterating over every file character 
                    while(new_file.get(c)){
                        //if it is ' ', then update this word in output file
                        if(c == ' ') {
                            temp << str + " ";
                            str = "";
                        }  
                        else {
                            str+=c;
                        } 
                        //if the word style is found, then it should be an inline style in the html file, so ignore its content until it ends
                        if(str.find(inlineStyleBegin) != string::npos){
                            new_file.ignore(std::numeric_limits<int>::max(), '"');
                            str = "";
                        }
                        //if the bold tag is found, then it should be replaced by a h2 tag according to the Zeplin layout
                        if(int pos = str.find(boldTagBegin) != string::npos){
                            str[pos]='h';
                            str+='2';
                        }
                        //replacing above bold tag end with h2
                        if(int pos = str.find(boldTagEnd) != string::npos){
                            str[pos+1]='h';
                            str+='2';
                        }
                    }
                    //finish task by reinitilizing variables and closing files
                    temp << str;
                    str = "";
                    temp.close();
                    new_file.close();    
                    //generate the new cleaned file
                    string out = "_";
                    out += fileName;
                    rename("temp.txt", out.c_str());
                    cout << "New cleaned file: " << out << "\n";
                }
            }
        }
    }
    closedir(dpdf);

    return 0;
}