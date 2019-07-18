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
    unsigned count = 0;
    unsigned countTodo = 0;
    std::for_each(todos.begin(), todos.end(), [&](auto todo) {
        ++count;
        if(!todo.second) {
               std::cout << "#" << count << " [ ] " << todo.first << std::endl;
               ++countTodo;
        }
    });
    if(countTodo == 0) {
        std::cout << "Nothing to do :(" << std::endl;
    }
}
void printAll() {

    auto todos = getAll();
    unsigned count = 1;
    std::for_each(todos.begin(), todos.end(), [&count](auto todo) {
        std::cout << "#" << count++ << " " << (todo.second ? "[x]" : "[ ]") << " " << todo.first << std::endl;
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
    count++;
    if (count != number) // write all lines to temp other than the line marked fro erasing
        temp << line << std::endl;
    else {
        temp << "[x]" << line.substr(3) << std::endl;
    }
}

temp.close();
fin.close();

remove(fileName);
rename("temp.txt", fileName);

}

void declareTaskNotDone(unsigned number) {

std::string line;
std::ifstream fin;
fin.open(fileName);
std::ofstream temp; // contents of path must be copied to a temp file then renamed back to the path file
temp.open("temp.txt");
unsigned count = 0;

while (getline(fin, line)) {
    // counting task not done
    count++;
    if (count != number) // write all lines to temp other than the line marked fro erasing
        temp << line << std::endl;
    else {
        temp << "[ ]" << line.substr(3) << std::endl;
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
        
        std::cout << "$ ";
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
        } else if(answer.find("done") == 0) {
            unsigned n = (unsigned) atoi(answer.substr(5).c_str());
            declareTaskDone(n);
            printTaskToBeDone();
        } else if(answer == "all") {
            printAll();
        } else if(answer.find("undo") == 0) {
            unsigned n = (unsigned) atoi(answer.substr(5).c_str());
            declareTaskNotDone(n);
            printTaskToBeDone();
        }


    } while (!quit);

    std::cout << "Good work!" << std::endl;
    
}