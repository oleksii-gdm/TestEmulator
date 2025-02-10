#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <pty.h>
#include <thread>
#include <sys/wait.h>

#include "utils.hpp"

constexpr int delay = 10;

void emularorFork(const std::string& name, const std::string& age)
{
    int master_fd; // file descriptor for the pseudo-terminal
    pid_t pid = forkpty(&master_fd, nullptr, nullptr, nullptr); // create pseudo-terminal and fork the process

    if(pid == -1)
    {
        std::cerr << "ERROR: forkpty failed!" << std::endl;
        return;
    }
    if(pid == 0)
    {
        execl("./PromptProgramm", "PromptProgramm", nullptr); // child process: replace the current process with the PromptProgramm
        std::cerr << "ERROR: execl failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    fcntl(master_fd, F_SETFL, O_NONBLOCK); // make master_fd non-blocking

    char buffer[256]; // buffer for reading output from the pseudo-terminal
    ssize_t read_bytes = 0;
    bool nameEntered = false;
    bool ageEntered = false;

    while (true)
    {
        read_bytes = read(master_fd, buffer, sizeof(buffer) - 1);

        if(read_bytes > 0)
        {
            buffer[read_bytes] = '\0';
            std::cout << buffer; // print PromptProgramm output
            std::cout.flush();

            if(!nameEntered && strstr(buffer, "Please enter Your Name: ")) // if PromptProgramm asks for the name, send it
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                std::string input = name + "\n";
                write(master_fd, input.c_str(), input.size());
                nameEntered = true;
            }
            if(!ageEntered && strstr(buffer, "Please enter Your age: ")) // if PromptProgramm asks for the age, send it
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                std::string input = age + "\n";
                write(master_fd, input.c_str(), input.size());
                ageEntered = true;
            }

            int status;

            if(waitpid(pid, &status, WNOHANG) > 0) // check if the process has finished
            {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // sleep bit before checking again
        }
    }

    close(master_fd); // close pseudo-terminal
    waitpid(pid, nullptr, 0); // ensure PromptProgramm has finished
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        std::cerr << "ERROR: Name and age are required! Usage: " << argv[0] << " Name Age" << std::endl;
        return EXIT_FAILURE;
    }

    std::string name = argv[1];

    if(!isNameValid(argv[1]))
    {
        std::cerr << "ERROR: Name is too long! Maximum length is 49 characters." << std::endl;
        return EXIT_FAILURE;
    }

    emularorFork(argv[1], argv[2]);

    return EXIT_SUCCESS;
}
