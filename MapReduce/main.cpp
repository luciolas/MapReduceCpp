#include "MapReduce/master.h"
#include <Scheduler/JobScheduler.h>
#include <common.h>
#include <MapReduce_Customs/ii.h>
#include <filesystem>
#include <curl/curl.h>

namespace fs = std::filesystem;

int main()
{
	curl_global_init(CURL_GLOBAL_ALL);

	DEFER([]() {curl_global_cleanup(); })
	Master m;
	m.Init();
	m.Start(Master::Mode::DIST);
}