/*****************************************************************
** Name: Clara Issa Ishac Abdelmessih
** Date: 19/09/2024
** Final Project, Milestone 1
** Ethernet Packet Configuration
** Header File
******************************************************************/

// Include guards to prevent multiple inclusions of this header file
#ifndef ETHERNET_PACKET_CONFIG_H
#define ETHERNET_PACKET_CONFIG_H

#include <string>

using namespace std;

// Declaration of the EthernetPacketConfig class
class EthernetPacketConfig
{
public:
    // Variables representing Ethernet configuration parameters
    unsigned int LineRate;
    unsigned int CaptureSizeMs;
    unsigned int MinNumOfIFGsPerPacket;
    unsigned char DestAddress[6];
    unsigned char SourceAddress[6];
    unsigned int MaxPacketSize;
    unsigned int BurstSize;
    unsigned int BurstPeriodicity_us;

    // Constructor declaration
    EthernetPacketConfig();

    // Function to set the Ethernet configuration from a configuration file
    void SetConfig(string& file_name);
};

// End of include guard
#endif // ETHERNET_PACKET_CONFIG_H
