//
// Created by lumadev on 25/01/2026.
//

#include "settings.h"
#include "transfer/croc_cli.h"

std::vector<std::string> CrocCLI::CreateSendCommand(const PathList& paths, const std::string& ticket) const
{
    std::vector<std::string> command;

    command[0] = "croc.exe";
    command[1] = "send";
    command[2] = "--code";
    command[3] = ticket;

    for (size_t i = 0; i < paths.size(); ++i)
    {
        command[4 + i] = paths.at(i).string();
    }
    command[4 + paths.size()] = nullptr;

    return command;
}

std::vector<std::string> CrocCLI::CreateReceiveCommand(const std::string& ticket) const
{
    std::vector<std::string> command;

    command[0] = "croc.exe";
    command[1] = "--yes";
    command[2] = "--overwrite";
    command[3] = "--out";
    command[4] = Settings::GetDownloadFolderPath().string();
    command[5] = ticket;
    command[6] = nullptr;

    return command;
}
