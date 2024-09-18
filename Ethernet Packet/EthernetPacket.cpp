/*****************************************************************
** Name: Clara Issa Ishac Abdelmessih
** Date: 19/09/2024
** Final Project, Milestone 1
** Ethernet Packet
** Source File
******************************************************************/

#include "EthernetPacket.h" // Include the EthernetPacket class definition
#include <iostream>

// CRC polynomial used in Ethernet CRC calculation (standard for Ethernet frames)
const uint32_t CRCPolynomial = 0x04C11DB7;

// Parameterized constructor for EthernetPacket class
EthernetPacket::EthernetPacket(unsigned char i_DestAddress[6], unsigned char i_SourceAddress[6], int i_MaxPacketSize, int i_MinNumOfIFGsPerPacket)
{
    // Calculate the payload size based on the maximum packet size minus fixed Ethernet header fields
    int PayloadSize = i_MaxPacketSize - 26; // Fixed header size: 7 (Preamble) + 1 (SDF) + 6 (DestAddress) + 6 (SourceAddress) + 2 (EtherType) + 4 (CRC)

    // Initialize the destination and source MAC addresses
    for(int i = 0; i < 6; i++)
    {
        DestAddress[i]   = i_DestAddress[i];
        SourceAddress[i] = i_SourceAddress[i];
    }

    // Initialize the payload with zeros
    for(int i = 0; i < PayloadSize; i++)
    {
        Payload.push_back(0x00);
    }

    // Initialize the Inter-Frame Gap (IFG) with the specified number of IFG symbols
    for(int i = 0; i < i_MinNumOfIFGsPerPacket; i++)
    {
        IFG.push_back(IFGSymbol);
    }

    // Set the EtherType field to represent the payload size
    EtherType[0] = static_cast<unsigned char>(PayloadSize & 0xFF);        // Low byte
    EtherType[1] = static_cast<unsigned char>((PayloadSize >> 8) & 0xFF); // High byte

    // Calculate the CRC for the packet
    CRCCalculation(PayloadSize);

    // Align the packet size to a multiple of 4 bytes by adding IFG symbols if necessary
    Align_4_Byte(i_MaxPacketSize + i_MinNumOfIFGsPerPacket);
}

// Function to align the total packet size to a multiple of 4 bytes
void EthernetPacket::Align_4_Byte(int TotalSizeWithIFG)
{
    // If the total size is not a multiple of 4, add IFG symbols to align it
    if (TotalSizeWithIFG % 4 != 0)
    {
        for (int i = 0; i < TotalSizeWithIFG % 4; i++)
            IFG.push_back(IFGSymbol);
    }
}

// Function to calculate the CRC for the Ethernet packet
void EthernetPacket::CRCCalculation(int PayloadSize)
{
    uint32_t CRC_temp = 0xffffffff; // Initialize CRC to all ones

    // Process each byte of the destination address
    for (int i = 0; i < 6; ++i)
    {
        CRC_temp ^= static_cast<uint32_t>(DestAddress[i]) << 24; // XOR byte into the top of the 32-bit CRC

        // Process each bit of the CRC
        for (int j = 0; j < 8; ++j)
        {
            if (CRC_temp & 0x80000000) // If the top bit is set
            {
                CRC_temp = (CRC_temp << 1) ^ CRCPolynomial; // Shift and XOR with polynomial
            }
            else
            {
                CRC_temp <<= 1; // Just shift
            }
        }
    }

    // Process each byte of the source address
    for (int i = 0; i < 6; ++i)
    {
        CRC_temp ^= static_cast<uint32_t>(SourceAddress[i]) << 24; // XOR byte into the top of the 32-bit CRC

        // Process each bit of the CRC
        for (int j = 0; j < 8; ++j)
        {
            if (CRC_temp & 0x80000000) // If the top bit is set
            {
                CRC_temp = (CRC_temp << 1) ^ CRCPolynomial; // Shift and XOR with polynomial
            }
            else
            {
                CRC_temp <<= 1; // Just shift
            }
        }
    }

    // Process each byte of the EtherType field
    for (int i = 0; i < 2; ++i)
    {
        CRC_temp ^= static_cast<uint32_t>(EtherType[i]) << 24; // XOR byte into the top of the 32-bit CRC

        // Process each bit of the CRC
        for (int j = 0; j < 8; ++j)
        {
            if (CRC_temp & 0x80000000) // If the top bit is set
            {
                CRC_temp = (CRC_temp << 1) ^ CRCPolynomial; // Shift and XOR with polynomial
            }
            else
            {
                CRC_temp <<= 1; // Just shift
            }
        }
    }

    // Process each byte of the payload
    for (int i = 0; i < PayloadSize; ++i)
    {
        CRC_temp ^= static_cast<uint32_t>(Payload[i]) << 24; // XOR byte into the top of the 32-bit CRC

        // Process each bit of the CRC
        for (int j = 0; j < 8; ++j)
        {
            if (CRC_temp & 0x80000000) // If the top bit is set
            {
                CRC_temp = (CRC_temp << 1) ^ CRCPolynomial; // Shift and XOR with polynomial
            }
            else
            {
                CRC_temp <<= 1; // Just shift
            }
        }
    }

    // Store the final CRC value in the CRC array
    CRC[0] = static_cast<unsigned char>(CRC_temp & 0xFF);
    CRC[1] = static_cast<unsigned char>((CRC_temp >> 8) & 0xFF);
    CRC[2] = static_cast<unsigned char>((CRC_temp >> 16) & 0xFF);
    CRC[3] = static_cast<unsigned char>((CRC_temp >> 24) & 0xFF);
}

// Function to calculate the total size of the Ethernet packet including payload and IFG
int EthernetPacket::Size()
{
    return 26 + Payload.size() + IFG.size(); // Fixed header size + payload size + IFG size
}
