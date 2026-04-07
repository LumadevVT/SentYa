#include "process/manager.h"

#include <fstream>
#include <list>
#include <utility>

#include <nlohmann/json.hpp>
#include <SDL3/SDL_process.h>

#include "transfer/croc_cli.h"
#include "transfer/interface_backend.h"


#include "settings.h"
#include "process/types.h"

using json = nlohmann::json;

using namespace std::string_literals;

namespace ProcessManager {
	namespace {
		auto currentBackend = Backend::CROC_CLI;
		IBackend* backendInterface = nullptr;

		std::list<Process> activeTasks;

		constexpr char TICKETS_DISK_FILE[] = "SentYa_Tickets.json";
		std::fstream ticketsFile;
		std::list<std::string> tasksToErase;
	}

	std::vector<Backend> GetAvailableBackends() {
		return {
			Backend::CROC_CLI,
			// Backend::SENDME_CLI
		};
	}

	const char* BackendToStr(const Backend backend) {
		switch (backend) {
			case Backend::CROC_CLI:
				return "Croc (CLI)";
			// case Backend::SENDME_CLI:
			//     return "Sendme (CLI)";
			default:
				return "WTF IS THIS?";
		}
	}

	Backend GetCurrentBackend() {
		return currentBackend;
	}

	const std::list<Process>& GetActiveProcesses() {
		return activeTasks;
	}

	void SaveTicketsToDisk(const std::list<Process>& tasks) {
		ticketsFile.open(Settings::GetConfigFolderPath() / TICKETS_DISK_FILE, std::ios::out);
		json ticketsJson;
		for (auto& task : tasks) {
			json archivesList(std::get<Process::SendData>(task.data).archivePaths);
			ticketsJson[task.ticket] = archivesList;
		}

		ticketsFile << std::setw(4) << ticketsJson << std::endl;

		ticketsFile.close();
	}

	void LoadTicketsFromDisk() {
		ticketsFile.open(Settings::GetConfigFolderPath() / TICKETS_DISK_FILE, std::ios::in);
		if (!ticketsFile.is_open())
			return;

		try {
			json ticketsJson = json::parse(ticketsFile);
			for (auto& [ticket, value] : ticketsJson.items()) {
				auto paths = value.get<PathList>();
				if (is_directory(paths.front()))
					activeTasks.emplace_back(backendInterface->SendFolder(paths.front(), ticket));
				else
					activeTasks.emplace_back(backendInterface->SendFiles(paths, ticket));
			}
		} catch (std::exception& e) {
		}

		ticketsFile.close();
	}

	void Init() {
		ChangeBackend(Backend::CROC_CLI);
		LoadTicketsFromDisk();
	}

	void Update() {
		if (backendInterface == nullptr)
			return;

		for (auto& ticket : tasksToErase) {
			for (auto it = activeTasks.begin(); it != activeTasks.end(); ++it) {
				if (it->ticket == ticket) {
					activeTasks.erase(it);
					break;
				}
			}
		}

		tasksToErase.clear();

		for (auto it = activeTasks.begin(); it != activeTasks.end();) {
			backendInterface->UpdateProcess(*it);

			const bool finished = SDL_WaitProcess(it->sdlProcess, false, nullptr);

			if (finished)
				backendInterface->OnProcessFinish(*it);
			if (it->sdlProcess == nullptr)
				it = activeTasks.erase(it);
			else
				++it;
		}
	}

	void Quit() {
		SaveTicketsToDisk(activeTasks);

		for (auto& proc : activeTasks) {
			if (proc.sdlProcess == nullptr)
				continue;

			SDL_KillProcess(proc.sdlProcess, true);
			SDL_DestroyProcess(proc.sdlProcess);
			proc.sdlProcess = nullptr;
		}

		activeTasks.clear();

		delete backendInterface;
		backendInterface = nullptr;
	}

	void ChangeBackend(const Backend backend) {
		delete backendInterface;
		backendInterface = nullptr;

		switch (backend) {
			case Backend::CROC_CLI:
				currentBackend = Backend::CROC_CLI;
				backendInterface = new CrocCLI();
				break;
			// case Backend::SENDME_CLI:
			//     currentBackend = Backend::SENDME_CLI;
			default:
				break;
		}
	}

	void SendFiles(PathList filePaths) {
		if (backendInterface == nullptr)
			return;

		const Process newProc = backendInterface->SendFiles(std::move(filePaths));

		activeTasks.push_back(newProc);
	}

	void SendFolder(fs::path folderPath) {
		if (backendInterface == nullptr)
			return;

		const Process newProc = backendInterface->SendFolder(std::move(folderPath));

		activeTasks.push_back(newProc);
	}

	void ReceiveArchive(const std::string& ticket) {
		if (backendInterface == nullptr)
			return;

		const Process newProc = backendInterface->ReceiveArchive(ticket);

		activeTasks.push_back(newProc);
	}

	void CloseProcess(const std::string& ticket) {
		tasksToErase.emplace_back(ticket);
	}
}
