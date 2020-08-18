#include "worker.h"

int main(int argc, char ** argv)
{
  char *  port = nullptr;
  char* myPort = nullptr;
  char* id = nullptr;
  // get args
  if (argc > 1)
  {
    port = argv[1];
    myPort = argv[2];
    id = argv[3];
  }
  std::string addr{ "localhost:" };
  Worker wrker{ grpc::CreateChannel(addr + port,grpc::InsecureChannelCredentials()), addr + port ,atoi(id)};
   
  wrker.Start(atoi(myPort));
}