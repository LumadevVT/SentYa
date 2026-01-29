//
// Created by lumadev on 25/01/2026.
//

#include "settings.h"
#include "transfer/croc_cli.h"

std::vector<std::string> CrocCLI::CreateSendCommand(const PathList& paths, const std::string& ticket) const
{
    std::vector<std::string> command;

    command.emplace_back((binariesPath / "croc.exe").string());
    command.emplace_back("send");
    command.emplace_back("--code");
    command.emplace_back(ticket);

    for (const auto& path : paths)
        command.emplace_back(path.string());

    return command;
}

std::vector<std::string> CrocCLI::CreateReceiveCommand(const std::string& ticket) const
{
    std::vector<std::string> command;

    command.emplace_back((binariesPath / "croc.exe").string());
    command.emplace_back("--yes");
    command.emplace_back("--overwrite");
    command.emplace_back("--out");
    command.emplace_back(Settings::GetDownloadFolderPath().string());
    command.emplace_back(ticket);

    return command;
}
