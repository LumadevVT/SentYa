#include "transfer/croc_cli.h"

#include "utils/log.h"
#include "uuid_v4.h"
#include "utils/time.h"

#include <string>
#include <deque>
#include <random>
#include <SDL3/SDL_process.h>


using namespace std::string_literals;

UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;

Process CrocCLI::SendFiles(const PathList filePaths)
{
    return SendFiles(filePaths, "");
}

Process CrocCLI::SendFolder(const fs::path folderPath)
{
    return SendFolder(folderPath, "");
}

Process CrocCLI::SendFiles(const PathList filePaths, const std::string& oldTicket)
{
    std::string ticket = "SentYa_"s;
    const UUIDv4::UUID uuid = uuidGenerator.getUUID();
    ticket.append(uuid.str());
    if (!oldTicket.empty())
    {
        ticket = oldTicket;
    }

    std::vector<std::string> processCommand = CreateSendCommand(filePaths, ticket);

    const auto processCommandCStr = new const char*[processCommand.size() + 1];
    for (size_t i = 0; i < processCommand.size(); i++)
    {
        processCommandCStr[i] = processCommand[i].c_str();
    }
    processCommandCStr[processCommand.size()] = nullptr;

    Process newProcess = {
        .type = Process::Type::SEND,
        .ticket = ticket,
        .data = Process::SendData{
            .archivePaths = filePaths,
        },
        .command = std::move(processCommand)
    };

    const SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, processCommandCStr);

    SDL_SetStringProperty(props, SDL_PROP_PROCESS_CREATE_WORKING_DIRECTORY_STRING, binariesPath.c_str());

    SDL_Environment* env = SDL_CreateEnvironment(true);
    SDL_SetEnvironmentVariable(env, "CROC_SECRET", newProcess.ticket.c_str(), true);
    SDL_SetPointerProperty(props, SDL_PROP_PROCESS_CREATE_ENVIRONMENT_POINTER, env);
    SDL_SetNumberProperty(props, SDL_PROP_PROCESS_CREATE_STDIN_NUMBER, SDL_PROCESS_STDIO_NULL);
    SDL_SetNumberProperty(props, SDL_PROP_PROCESS_CREATE_STDOUT_NUMBER, SDL_PROCESS_STDIO_INHERITED);
    newProcess.sdlProcess = SDL_CreateProcessWithProperties(props);
    if (newProcess.sdlProcess == nullptr)
    {
        Log::Error("Could not create process! "s + SDL_GetError());
    }

    delete[] processCommandCStr;
    return newProcess;
}

Process CrocCLI::SendFolder(fs::path folderPath, const std::string& oldTicket)
{
    std::string ticket = "SentYa_"s;
    const UUIDv4::UUID uuid = uuidGenerator.getUUID();
    ticket.append(uuid.str());
    if (!oldTicket.empty())
    {
        ticket = oldTicket;
    }

    std::vector<std::string> processCommand = CreateSendCommand({folderPath}, ticket);

    const auto processCommandCStr = new const char*[processCommand.size() + 1];
    for (size_t i = 0; i < processCommand.size(); i++)
    {
        processCommandCStr[i] = processCommand[i].c_str();
    }
    processCommandCStr[processCommand.size()] = nullptr;


    Process newProcess = {
        .type = Process::Type::SEND,
        .ticket = ticket,
        .data = Process::SendData{
            .archivePaths = {folderPath},
        },
        .command = std::move(processCommand)
    };

    const SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, processCommandCStr);

    SDL_SetStringProperty(props, SDL_PROP_PROCESS_CREATE_WORKING_DIRECTORY_STRING, binariesPath.c_str());

    SDL_Environment* env = SDL_CreateEnvironment(true);
    SDL_SetEnvironmentVariable(env, "CROC_SECRET", newProcess.ticket.c_str(), true);
    SDL_SetPointerProperty(props, SDL_PROP_PROCESS_CREATE_ENVIRONMENT_POINTER, env);
    SDL_SetNumberProperty(props, SDL_PROP_PROCESS_CREATE_STDIN_NUMBER, SDL_PROCESS_STDIO_NULL);
    SDL_SetNumberProperty(props, SDL_PROP_PROCESS_CREATE_STDOUT_NUMBER, SDL_PROCESS_STDIO_INHERITED);
    newProcess.sdlProcess = SDL_CreateProcessWithProperties(props);
    if (newProcess.sdlProcess == nullptr)
    {
        Log::Error("Could not create process! "s + SDL_GetError());
    }

    delete[] processCommandCStr;
    return newProcess;
}

Process CrocCLI::ReceiveArchive(const std::string& ticket)
{
    std::vector<std::string> processCommand = CreateReceiveCommand(ticket);

    const char** processCommandCStr = new const char*[processCommand.size() + 1];
    for (size_t i = 0; i < processCommand.size(); i++)
    {
        processCommandCStr[i] = processCommand[i].c_str();
    }
    processCommandCStr[processCommand.size()] = nullptr;

    Process newProcess = {
        .type = Process::Type::RECEIVE,
        .ticket = ticket,
        .data = Process::ReceiveData{
            .timerSeconds = 0.f,
        },
        .command = std::move(processCommand)
    };

    const SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, processCommandCStr);

    SDL_SetStringProperty(props, SDL_PROP_PROCESS_CREATE_WORKING_DIRECTORY_STRING, binariesPath.c_str());

    SDL_Environment* env = SDL_CreateEnvironment(true);
    SDL_SetEnvironmentVariable(env, "CROC_SECRET", newProcess.ticket.c_str(), true);
    SDL_SetPointerProperty(props, SDL_PROP_PROCESS_CREATE_ENVIRONMENT_POINTER, env);
    SDL_SetNumberProperty(props, SDL_PROP_PROCESS_CREATE_STDIN_NUMBER, SDL_PROCESS_STDIO_NULL);
    SDL_SetNumberProperty(props, SDL_PROP_PROCESS_CREATE_STDOUT_NUMBER, SDL_PROCESS_STDIO_INHERITED);
    newProcess.sdlProcess = SDL_CreateProcessWithProperties(props);
    if (newProcess.sdlProcess == nullptr)
    {
        Log::Error("Could not create process! "s + SDL_GetError());
    }

    delete[] processCommandCStr;
    return newProcess;
}

void CrocCLI::UpdateProcess(Process& process)
{
    switch (process.type)
    {
    case Process::Type::SEND:
        break;
    case Process::Type::RECEIVE:
        {
            auto& data = std::get<Process::ReceiveData>(process.data);
            data.timerSeconds += Time::GetDeltaTimeSec();
        }
        break;
    default:
        throw std::runtime_error("Not possible!");
    }
}

void CrocCLI::OnProcessFinish(Process& process)
{
    if (process.type == Process::Type::RECEIVE)
    {
        SDL_KillProcess(process.sdlProcess, true);
        SDL_DestroyProcess(process.sdlProcess);
        process.sdlProcess = nullptr;
        return;
    }

    const auto& data = std::get<Process::SendData>(process.data);
    if (!fs::is_directory(data.archivePaths.front()))
    {
        process = SendFiles(data.archivePaths, process.ticket);
        return;
    }
    process = SendFolder(data.archivePaths.front(), process.ticket);
}
