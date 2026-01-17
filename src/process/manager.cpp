#include "process/manager.h"

#include "transfer/croc_cli.h"
#include "transfer/interface_backend.h"

#include <SDL3/SDL_process.h>

namespace ProcessManager {
    namespace {
        Backend currentBackend = Backend::CROC_CLI;
        IBackend *backendInterface = nullptr;

        std::vector<SendProcess> sendProcesses;
        std::vector<ReceiveProcess> receiveProcesses;
    }

    std::vector<std::string> GetAvailableBackends() {
        return {
            BackendToStr(Backend::CROC_CLI),
            BackendToStr(Backend::SENDME_CLI),
        };
    }

    std::string BackendToStr(const Backend backend) {
        switch (backend) {
            case Backend::CROC_CLI:
                return "Croc (CLI)";
            case Backend::SENDME_CLI:
            // return "Sendme (CLI)";
            case Backend::UNSUPPORTED:
                return "Unsupported";
            default:
                return "WTF IS THIS?";
        }
    }

    void Init() {
        ChangeBackend(Backend::CROC_CLI);
    }

    void Quit() {
        for (auto &proc: sendProcesses) {
            SDL_DestroyProcess(proc.sdlProcess);
        }

        for (auto &proc: receiveProcesses) {
            SDL_DestroyProcess(proc.sdlProcess);
        }

        sendProcesses.clear();
        receiveProcesses.clear();

        delete backendInterface;
        backendInterface = nullptr;
    }

    void ChangeBackend(const Backend backend) {
        delete backendInterface;

        switch (backend) {
            case Backend::CROC_CLI:
                currentBackend = Backend::CROC_CLI;
                backendInterface = new CrocCLI;
                break;
            case Backend::SENDME_CLI: [[fallthrough]];
            default:
                currentBackend = Backend::UNSUPPORTED;
                backendInterface = nullptr;
                break;
        }
    }

    void SendFiles(const PathList &filePaths) {
        if (backendInterface == nullptr) return;

        SendProcess proc = backendInterface->SendFiles(filePaths);
        if (!proc.IsWorking()) return;

        sendProcesses.push_back(std::move(proc));
    }

    void SendFolder(const fs::path &folderPath) {
        if (backendInterface == nullptr) return;

        SendProcess proc = backendInterface->SendFolder(folderPath);
        if (!proc.IsWorking()) return;

        sendProcesses.push_back(std::move(proc));
    }

    void ReceiveArchive(const std::string &ticket) {
        if (backendInterface == nullptr) return;

        ReceiveProcess proc = backendInterface->ReceiveArchive(ticket);
        if (!proc.IsWorking()) return;

        receiveProcesses.push_back(std::move(proc));
    }
}
