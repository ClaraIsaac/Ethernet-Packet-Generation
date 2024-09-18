/*****************************************************************
** Name: Clara Issa Ishac Abdelmessih
** Date: 19/09/2024
** Final Project, Milestone 1
** Main Program
******************************************************************/

#include "EthernetPacketConfig.h"    // Include the EthernetPacketConfig class definition
#include "EthernetPacket.h"          // Include the EthernetPacket class definition
#include "EthernetPacketGenerator.h" // Include the EthernetPacketGenerator class definition
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    // Check if the correct number of command-line arguments are provided
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <config_file> <output_file>" << endl;
        return 1;
    }

    string ConfigFile = argv[1]; // First argument: path to the configuration file
    string OutputFile = argv[2]; // Second argument: path to the output file

    // Create an instance of the EthernetPacketConfig class
    EthernetPacketConfig Config;
    // Load configuration settings from the specified file
    Config.SetConfig(ConfigFile);

    // Create an instance of the EthernetPacket class using the loaded configuration
    EthernetPacket Packet(Config.DestAddress, Config.SourceAddress, Config.MaxPacketSize, Config.MinNumOfIFGsPerPacket);

    // Create an instance of the EthernetPacketGenerator class to generate packets and write them to the output file
    EthernetPacketGenerator Generator(Config, Packet, OutputFile);

    cout << "Ethernet packets generated and written to " << OutputFile << endl;

    return 0;
}
