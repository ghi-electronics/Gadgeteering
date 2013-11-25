#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/HubAP5.h>
#include <Modules/Tunes.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	hub_ap5 hub(0);
	tunes piezo(hub.socket_4);
	
	piezo.set(500, 0.5);
	
	return 0;
}
