#pragma once
#include "src/MainUI.hpp"
#include <memory>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <unistd.h>

struct StdinInput
{
    StdinInput() = default;
    StdinInput(std::string pipedInput)
        : input(pipedInput)
    {
        if (!pipedInput.empty())
            valid = true;
    }
    std::string input = {};
    bool valid = false;
};

struct ArgumentInput
{
    ArgumentInput() = default;
    ArgumentInput(std::vector<std::string> arguments)
        : arguments(arguments)
    {
        if (arguments.size() > 1)
            valid = true;
    }
    std::vector<std::string> arguments = {};
    bool valid = false;
};

void HandleArgs(int argc, const char * argv[])
{
    // COLLECT ARGUMENTS
    std::vector<std::string> argumentsList = {};
    for (int i = 0; i < argc; i++)
    {
//        std::cout << "arg: " << i << " - " << argv[i] << std::endl;
        argumentsList.push_back(argv[i]);
    }
    ArgumentInput argumentInput(argumentsList);

    // COLLECT STDIN
    std::string stdinInputString = {};
    std::string lineInput = {};
    while (!isatty(STDIN_FILENO) // stdin is not a terminal
           && std::cin >> lineInput)
    {
        stdinInputString += lineInput + " ";
    }
//    std::cout << "piped: " << stdinInputString << std::endl;
    StdinInput stdInput(stdinInputString);
    
    // GET MAIN UI
    MainUI* mainUI = MainUI::Get();
    
    // PARSE STDIN
    if (stdInput.valid)
    {
        std::shared_ptr<otter::Queue> queueDeserialized = std::dynamic_pointer_cast<otter::Queue>(otter::SerializableFactory::createPtr(stdInput.input));
        if (queueDeserialized)
            mainUI->state = State(queueDeserialized);
        else
            std::cout << "ERROR: failed to deserialize Queue from stdin" << std::endl;
    }
    
    // PARSE ARGUMENTS
    std::unordered_map<std::string, std::string> arguments = {};
    if (argumentInput.valid)
    {
        const std::unordered_set<std::string> acceptedArguments =
        {
            "--path"
        };
        std::string lastFlag = {};
        for (std::string argument : argumentInput.arguments)
        {
            if (!lastFlag.empty())
            {
                arguments.insert({ lastFlag, argument });
                lastFlag = {};
//                std::cout << "argument: " << lastFlag << " -> " << argument << std::endl;
            }
            else if (acceptedArguments.contains(argument))
            {
                lastFlag = argument;
            }
        }
    }
    
    // ACT ON ARGUMENTS
    if (arguments.contains("--path"))
    {
        const std::string& queuePath = arguments["--path"];
        
        std::ifstream inFile;
        inFile.open(queuePath);
        std::string inFileString((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();
        
        std::shared_ptr<otter::Queue> queueDeserialized = dynamic_pointer_cast<otter::Queue>(otter::SerializableFactory::createPtr(inFileString));
        if (queueDeserialized)
            mainUI->state = State(queueDeserialized);
        else
            std::cout << "ERROR: failed to deserialize Queue from file: " << queuePath << std::endl;
    }
}

void handleOpenFile(std::string filePath)
{
    MainUI* mainUI = MainUI::Get();
    
    std::ifstream inFile;
    inFile.open(filePath);
    std::string inFileString((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    
    std::shared_ptr<otter::Queue> queueDeserialized = dynamic_pointer_cast<otter::Queue>(otter::SerializableFactory::createPtr(inFileString));
    if (queueDeserialized)
        mainUI->state = State(queueDeserialized);
    else
        std::cout << "ERROR: failed to deserialize Queue opened from file: " << filePath << std::endl;
}
