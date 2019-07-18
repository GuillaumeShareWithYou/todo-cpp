#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

const char* fileName = "C:\\Users\\g.mathieu.ext\\cmd\\resources\\list.txt";

bool verifyContainX(std::string s) {
    unsigned i = 0; 
    while(i < s.length()) {
        if(s[i] == 'x' || s[i] == 'X') return true;    
        ++i;
    }
    return false;
}

std::vector<std::pair<std::string, bool>> getAll() {
  std::vector<std::pair<std::string, bool>> todos;
    std::cout << "_______TODO LIST_______" << std::endl << std::endl;
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Unable to open file";
        exit(1);   // call system to stop
    }
    std::string buffer;
    while(std::getline(file, buffer, ']')) {
        //[x in buffer
        bool isDone = verifyContainX(buffer);
            
        std::getline(file, buffer);
        todos.push_back(std::pair(buffer, isDone));
    }
    file.close();

    return todos;
}

void printTaskToBeDone() {
  
    auto todos = getAll();
    std::vector<std::pair<std::string, bool>> todoNotDone;

    std::copy_if(todos.begin(), todos.end(), std::back_insert_iterator(todoNotDone), [](auto todo) {return !todo.second;});

    if(todoNotDone.empty()){
        std::cout << "Nothing to do :)" << std::endl;
    } else {
        unsigned count = 1;
        std::for_each(todoNotDone.begin(), todoNotDone.end(), [&count](auto todo){
            std::cout << count++ <<"- " << todo.first << std::endl;
        });     
    }
}
void printAll() {

    auto todos = getAll();

    std::for_each(todos.begin(), todos.end(), [](auto todo) {
        std::cout << (todo.second ? "[x]" : "[ ]") << " " << todo.first << std::endl;
    });

}
void declareTaskDone(unsigned number) {

std::string line;
std::ifstream fin;
fin.open(fileName);
std::ofstream temp; // contents of path must be copied to a temp file then renamed back to the path file
temp.open("temp.txt");
unsigned count = 0;

while (getline(fin, line)) {
    // counting task not done
    if(line.find("[ ]") == 0) count++;
    if (count != number) // write all lines to temp other than the line marked fro erasing
        temp << line << std::endl;
    else {
        temp << "[x]" << line.substr(3) << std::endl;
        std::cout << "removing line " << count << std::endl;
    }
}

temp.close();
fin.close();

remove(fileName);
rename("temp.txt", fileName);

}

int main() {

   
    printTaskToBeDone();

    bool quit = false;
    std::string answer;
    do {
        
        std::cout << "What do yo want ? (l,+,rm,q)" << std::endl;
        std::getline(std::cin, answer);
        
        if(answer == "q"){
            quit = true;
        } else if(answer.find("+") == 0) {
            std::string task = answer.substr(2);
            std::ofstream file;
            file.open(fileName, std::ofstream::app);
            file << "[ ]" << task << "\n";
            file.close();
        } else if(answer == "l") {
            printTaskToBeDone();
        } else if(answer.find("rm") == 0) {
            std::cout << "what task is done ?" << std::endl;
            unsigned n = (unsigned) atoi(answer.substr(3).c_str());
            std::cout <<"line ask to remove : " <<  n << std::endl;
            declareTaskDone(n);
            printTaskToBeDone();
        } else if(answer == "lall") {
            printAll();
        }


    } while (!quit);

    std::cout << "Good work!" << std::endl;
    
}