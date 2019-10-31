#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <sstream>
#include <windows.h>
const char *fileName = "C:\\Users\\g.mathieu.ext\\cmd\\resources\\list.txt";

std::string trim(std::string s)
{
    std::ostringstream ss;
    size_t i = 0;
    while (s[i] == ' ' && i < s.length())
    {
        i++;
    }
    while (i < s.length())
    {
        ss << s[i++];
    }
    return ss.str();
}

bool verifyContainX(std::string s)
{
    size_t i = 0;
    while (i < s.length())
    {
        if (s[i] == 'x' || s[i] == 'X')
            return true;
        ++i;
    }
    return false;
}

std::vector<std::pair<std::string, bool>> getAll()
{
    std::vector<std::pair<std::string, bool>> todos;
    std::cout << "_______TODO LIST_______" << std::endl
              << std::endl;
    std::ifstream file(fileName);
    if (!file)
    {
        std::cerr << "Unable to open file";
        exit(1); // call system to stop
    }
    std::string buffer;
    while (std::getline(file, buffer, ']'))
    {
        //[x in buffer
        bool isDone = verifyContainX(buffer);

        std::getline(file, buffer);
        todos.push_back(std::pair(buffer, isDone));
    }
    file.close();

    return todos;
}

void printTaskToBeDone()
{

    auto todos = getAll();
    std::vector<std::pair<std::string, bool>> todoNotDone;
    unsigned count = 0;
    unsigned countTodo = 0;
    std::for_each(todos.begin(), todos.end(), [&](std::pair<std::string, bool> todo) {
        ++count;
        if (!todo.second)
        {
            std::cout << "#" << count << " [ ] " << todo.first << std::endl;
            ++countTodo;
        }
    });
    if (countTodo == 0)
    {
        std::cout << "Nothing to do :(" << std::endl;
    }
}
void printAll()
{

    auto todos = getAll();
    unsigned count = 1;
    std::for_each(todos.begin(), todos.end(), [&count](auto todo) {
        std::cout << "#" << count++ << " " << (todo.second ? "[x]" : "[ ]") << " " << todo.first << std::endl;
    });
}
void declareTaskDone(std::vector<int> number)
{

    std::string line;
    std::ifstream fin;
    fin.open(fileName);
    std::ofstream temp; // contents of path must be copied to a temp file then renamed back to the path file
    temp.open("temp.txt");
    unsigned count = 0;

    while (getline(fin, line))
    {
        // counting task not done
        count++;
        if (std::find(number.begin(), number.end(), count) == number.end()) // write all lines to temp other than the line marked fro erasing
            temp << line << std::endl;
        else
        {
            temp << "[x]" << line.substr(line.find("]") + 1) << std::endl;
        }
    }

    temp.close();
    fin.close();

    remove(fileName);
    rename("temp.txt", fileName);
}

void declareTaskNotDone(std::vector<int> number)
{

    std::string line;
    std::ifstream fin;
    fin.open(fileName);
    std::ofstream temp; // contents of path must be copied to a temp file then renamed back to the path file
    temp.open("temp.txt");
    unsigned count = 0;

    while (getline(fin, line))
    {
        // counting task not done
        count++;
        if (std::find(number.begin(), number.end(), count) == number.end()) // write all lines to temp other than the line marked fro erasing
            temp << line << std::endl;
        else
        {
            temp << "[]" << line.substr(line.find("]") + 1) << std::endl;
        }
    }

    temp.close();
    fin.close();

    remove(fileName);
    rename("temp.txt", fileName);
}

void removeTask(unsigned number)
{

    std::string line;
    std::ifstream fin;
    fin.open(fileName);
    std::ofstream temp; // contents of path must be copied to a temp file then renamed back to the path file
    temp.open("temp.txt");
    unsigned count = 0;

    while (getline(fin, line))
    {
        // counting task not done
        count++;
        if (count != number) // write all lines to temp other than the line marked fro erasing
            temp << line << std::endl;
    }

    temp.close();
    fin.close();

    remove(fileName);
    rename("temp.txt", fileName);
}

std::vector<int> stringToIntVector(std::string &input)
{
    std::vector<int> numbers;
    std::stringstream ss("");
    for (int i(0); i < (int)input.length(); ++i)
    {
        if (input[i] == ' ')
        {
            numbers.emplace_back(atoi(ss.str().c_str()));
            ss.str(std::string());
            ss.clear();
        }
        else
        {
            ss << input[i];
        }
    }
    if (ss.str().length())
    {
        numbers.emplace_back(atoi(ss.str().c_str()));
    }

    return numbers;
}

int main()
{
    int GREEN = 2;
    int BLUE = 9;
    int WHITE = 7;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BLUE);
    printTaskToBeDone();

    bool quit = false;
    std::string answer;
    do
    {

        std::cout << "$ ";
        std::getline(std::cin, answer);

        if (answer == "q")
        {
            quit = true;
        }
        else if (answer.find("+") == 0)
        {
            std::string task = answer.substr(1);
            task = trim(task);
            std::ofstream file;
            file.open(fileName, std::ofstream::app);
            file << "[]" << task << "\n";
            file.close();
        }
        else if (answer == "l")
        {
            printTaskToBeDone();
        }
        else if (answer.find("do") == 0)
        {
            if (answer.length() < 4)
                std::cout << "Please specify the task number to mark done." << std::endl;
            else
            {
                std::string input = answer.substr(3);
                declareTaskDone(stringToIntVector(input));
                printTaskToBeDone();
            }
        }
        else if (answer == "all")
        {
            printAll();
        }
        else if (answer.find("undo") == 0)
        {
            if (answer.length() < 5)
                std::cout << "Please specify the task number to mark todo." << std::endl;
            else
            {
                std::string input = answer.substr(5);
                declareTaskNotDone(stringToIntVector(input));
                printTaskToBeDone();
            }
        }
        else if (answer.find("rm") == 0)
        {
            if (answer.length() < 4)
                std::cout << "Please specify the task number to remove." << std::endl;
            else
            {
                unsigned n = (unsigned)atoi(answer.substr(3).c_str());
                removeTask(n);
            }
        }

    } while (!quit);

    std::cout << "Good work!" << std::endl;
    SetConsoleTextAttribute(hConsole, WHITE);
}