#include <iostream>

#include <Gadgeteering.h>

#include <Mainboards/FEZLynxS4.h>
#include <Modules/Gyro.h>

using namespace std;
using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

int main(int argc, char** argv)
{
	fez_lynx_s4 board;
	gyro gyroscope(1);

	while(true)
	{
		gyro::sensor_data data = gyroscope.request_measurement();

		cout << "X: " << data.x << " Y: " << data.y << " Z: " << data.z << " Temp: " << data.temperature << endl;

		system::sleep(100);
	}

	return 0;
}
