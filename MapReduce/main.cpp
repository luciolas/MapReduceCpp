#include "MapReduce/master.h"
#include <Scheduler/JobScheduler.h>
#include <common.h>
#include <MapReduce_Customs/ii.h>
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
	auto* s = ::MapReduce::GetScheduler();
	int max_files = 4;
	Master m;
	std::vector<std::string> input_files{};
	auto curr_path = fs::current_path();
	int i = 0;
	for (auto& entry : fs::directory_iterator(curr_path))
	{
		if (i >= max_files)
		{
			break;
		}
		if (entry.path().extension() == ".txt")
		{
			input_files.push_back(entry.path().filename().string());
			++i;
		}
	}
	std::cout << "Files: " << i << std::endl;
	m.Init();
	s->Schedule(&Master::Start, &m);
	auto start_wait = m.StartRPCs(input_files, iimapF, iireduceF);
	start_wait.wait();
	/*while (1)
	{
		if (workers.size() == i)
		{
			break;
		}
	}*/
	m.StopAllRPCs();

}