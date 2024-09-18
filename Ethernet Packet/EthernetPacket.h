/*****************************************************************
** Name: Clara Issa Ishac Abdelmessih
** Date: 19/09/2024
** Final Project, Milestone 1
** Ethernet Packet
** Header File
******************************************************************/

// Include guards to prevent multiple inclusions of this header file
#ifndef ETHERNET_PACKET_H
#define ETHERNET_PACKET_H

#include <vector>    // Required for vector

using namespace std;

// Constant defining the IFG symbol used in Ethernet packets
const unsigned char IFGSymbol = 0x07;

// Declaration of the EthernetPacket class
class EthernetPacket
{
public:
    // Public member variables for Ethernet packet components
    unsigned char Preamble[7] = {0xfb, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}; // Preamble of the Ethernet packet (7 bytes)
    unsigned char SDF = 0xd5; // Start Frame Delimiter (1 byte)
    unsigned char DestAddress[6]; // Destination MAC address (6 bytes)
    unsigned char SourceAddress[6]; // Source MAC address (6 bytes)
    unsigned char EtherType[2]; // EtherType field (2 bytes)
    vector<unsigned char> Payload; // Payload data (variable length)
    unsigned char CRC[4]; // CRC field (4 bytes for error checking)
    vector<unsigned char> IFG; // Inter-Frame Gap (variable length)

    // Default constructor
    EthernetPacket() {}

    // Parameterized constructor to initialize Ethernet packet with given values
    EthernetPacket(unsigned char i_DestAddress[6], unsigned char SourceAddress[6], int i_MaxPacketSize, int i_MinNumOfIFGsPerPacket);

    // Public method to calculate the size of the Ethernet packet including payload and IFG
    int Size();

private:
    // Private method to align the total packet size to a multiple of 4 bytes
    void Align_4_Byte(int TotalSizeWithIFG);

    // Private method to calculate the CRC for the payload
    void CRCCalculation(int PayloadSize);
};

// End of include guard
#endif //ETHERNET_PACKET_H
