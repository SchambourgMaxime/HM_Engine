#include "HM_GameMaster.h"
#include "HM_Log.h"

int main(int argc, char* argv[])
{

	HM_GameMaster::instance()->initGame();

	HM_GameMaster::instance()->launchGame();

	delete HM_GameMaster::instance();

	return 0;

}