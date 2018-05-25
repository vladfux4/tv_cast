#include "common/logger.h"

#include "com_ctrl.h"
#include "logic/server_logic.h"
#include "logic/client_logic.h"

int main(int argc, char* argv[]) {
  InitLogger(argv[0]);

  LOG(INFO) << "Start";

  try {
    CommunicationController com_ctr;
    logic::ServerLogic server_logic;
    logic::ClientLogic client_logic(com_ctr);

    com_ctr.RegisterHttpServerObserver(server_logic);
    com_ctr.RegisterHttpClientObserver(client_logic);

//    client_logic.SendGetRequest();

    com_ctr.Run();
  } catch (std::exception& e) {
    LOG(ERROR) << "Exception: " << e.what();
  }

  return 0;
}
