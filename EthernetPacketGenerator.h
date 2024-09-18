/*****************************************************************
** Name: Clara Issa Ishac Abdelmessih
** Date: 19/09/2024
** Final Project, Milestone 1
** Ethernet Packet Generator
** Header File
******************************************************************/

// Include guards to prevent multiple inclusions of this header file
#ifndef ETHERNET_PACKET_GENERATOR_H
#define ETHERNET_PACKET_GENERATOR_H

#include "EthernetPacketConfig.h" // Include the EthernetPacketConfig class definition
#include "EthernetPacket.h"      // Include the EthernetPacket class definition
#include <string>
#include <fstream>

using namespace std;

// Class for generating Ethernet packets based on configuration and packet details
class EthernetPacketGenerator
{
public:
    EthernetPacketConfig Config; // Configuration settings for Ethernet packet generation
    EthernetPacket Packet;       // Ethernet packet to be generated

    // Constructor to Initializes the EthernetPacketGenerator with configuration, packet, and output file name
    EthernetPacketGenerator(const EthernetPacketConfig& i_Config, const EthernetPacket& i_Packet, string& output_file);

private:
    // Function to generate packets and write them to the specified output file
    void GeneratePackets(string& output_file);

    // Function to send a single packet to the output file
    void SendPacket(ofstream& file);

    // Function to send Inter-Frame Gaps (IFGs) to the output file
    // NumOfIFGs specifies the number of IFG symbols to be sent
    void SendIFG(double NumOfIFGs, ofstream& file);
};

// End of include guard
#endif // ETHERNET_PACKET_GENERATOR_H
