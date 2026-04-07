#include "p2p/p2p.h"

#include <librats.h>
#include <filesystem>
#include <memory>

namespace P2P {
	namespace {
		std::unique_ptr<librats::RatsClient> client;

	}

	void Init(const int port) {
		librats::FileTransferConfig config;
		config.allow_resume = true;
		config.chunk_size = 128 * 1024;
		config.max_concurrent_chunks = 4;
		config.temp_directory = std::filesystem::temp_directory_path().string();
		config.verify_checksums = true;
		config.max_retries = 20;
		config.timeout_seconds = 60;

		client = librats::create_rats_client(port);
		client->set_file_transfer_config(config);
		client->start();
		std::cout << "PEER ID: " << client->get_our_peer_id() << std::endl;
	}


}
