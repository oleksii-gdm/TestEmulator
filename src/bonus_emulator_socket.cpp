#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "utils.hpp"

constexpr int delay = 1000;

void sendSlowly(int sockfd, const std::string& text)
{
    for (char c : text)
    {
        if (send(sockfd, &c, 1, 0) == -1)
        {
            std::cerr << "ERROR: Failed to send data: " << strerror(errno) << std::endl;
            return;
        }
        usleep(delay);  // small delay to simulate real typing
    }

    if (send(sockfd, "\n", 1, 0) == -1)
    {
        std::cerr << "ERROR: Failed to send newline: " << strerror(errno) << std::endl;
    }
    usleep(delay);
}

void emulatorSocket(const std::string& socket_path, const std::string& name, const std::string& age)
{
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0); // create socket
    if (sockfd == -1)
    {
        std::cerr << "ERROR: Failed to create socket: " << strerror(errno) << std::endl;
        return;
    }

    struct sockaddr_un addr{}; // set up socket address struct
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path.c_str(), sizeof(addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) // connect to the server
    {
        std::cerr << "ERROR: Failed to connect to UNIX socket (" << socket_path << "): " << strerror(errno) << std::endl;
        close(sockfd);
        return;
    }

    char buffer[256];
    ssize_t bytesRead;
    std::string outputBuffer;

    while ((bytesRead = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) // read until "Please enter Your Name:"
    {
        buffer[bytesRead] = '\0';
        std::cout << buffer << std::flush; // print output immediately
        outputBuffer += buffer;

        if (outputBuffer.find("Please enter Your Name:") != std::string::npos)
        {
            std::cout << name << std::endl << std::flush; // show name immediately
            sendSlowly(sockfd, name); // send name character by character
            break;
        }
    }

    if (bytesRead == -1)
    {
        std::cerr << "ERROR: Failed to read from socket: " << strerror(errno) << std::endl;
        close(sockfd);
        return;
    }

    outputBuffer.clear(); 

    while ((bytesRead = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) // read until "Please enter Your age:"
    {
        buffer[bytesRead] = '\0';
        std::cout << buffer << std::flush;
        outputBuffer += buffer;

        if (outputBuffer.find("Please enter Your age:") != std::string::npos)
        {
            std::cout << age << std::endl << std::flush;
            sendSlowly(sockfd, age);
            break;
        }
    }

    if (bytesRead == -1)
    {
        std::cerr << "ERROR: Failed to read from socket: " << strerror(errno) << std::endl;
        close(sockfd);
        return;
    }

    while ((bytesRead = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) // read the remaining output
    {
        buffer[bytesRead] = '\0';
        std::cout << buffer << std::flush;
    }

    if (bytesRead == -1)
    {
        std::cerr << "ERROR: Failed to read final output from socket: " << strerror(errno) << std::endl;
    }

    close(sockfd); // close socket
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "ERROR: Name and age are required! Usage: " << argv[0] << " <Socket Path> <Name> <Age>" << std::endl;
        return EXIT_FAILURE;
    }

    if (!isNameValid(argv[2]))
    {
        std::cerr << "ERROR: Name is too long! Maximum length is 49 characters." << std::endl;
        return EXIT_FAILURE;
    }

    if(!isDigitString(argv[3]))
    {
        std::cerr << "ERROR: Age must be a positive integer." << std::endl;
        return EXIT_FAILURE;
    }

    emulatorSocket(argv[1], argv[2], argv[3]);

    return EXIT_SUCCESS;
}
