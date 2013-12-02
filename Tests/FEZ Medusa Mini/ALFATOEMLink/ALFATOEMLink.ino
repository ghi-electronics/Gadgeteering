#include <Wire.h>
#include <SPI.h>

#include <Gadgeteering.h>

#include <Mainboards/FEZMedusaMini.h>
#include <Modules/ALFATOEMLink.h>

using namespace gadgeteering;
using namespace gadgeteering::mainboards;
using namespace gadgeteering::modules;

void loop()
{

}

void setup()
{
	Serial.begin(9600);

	fez_medusa_mini board;
	alfat_oem_link alfat(1);

	unsigned char major, minor, rev;
	alfat.get_version(major, minor, rev);

	alfat.mount(alfat_oem_link::drives::SD);

	alfat_oem_link::file* file1 = alfat.open_file("\\foo.txt", alfat_oem_link::file::modes::WRITE);
	alfat_oem_link::file* file2 = alfat.open_file("\\bar.txt", alfat_oem_link::file::modes::READ);

	unsigned char buffer[14] = "Hello, World!";
	unsigned int written, read;
	file1->write(buffer, 13, &written);
	file1->flush();
	file2->read(buffer, 13, &read);
	
	file1->close();
	file2->close();

	Serial.print(static_cast<int>(major)); Serial.print(" "); Serial.print(static_cast<int>(minor)); Serial.print(" "); Serial.print(static_cast<int>(rev)); Serial.println("");
	Serial.print(written); Serial.print(" "); Serial.print(read); Serial.println("");
	Serial.print(reinterpret_cast<char*>(buffer)); Serial.println("");

	return;
}
