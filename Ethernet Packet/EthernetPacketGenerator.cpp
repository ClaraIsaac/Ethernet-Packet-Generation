/*****************************************************************
** Name: Clara Issa Ishac Abdelmessih
** Date: 19/09/2024
** Final Project, Milestone 1
** Ethernet Packet Generator
** Source File
******************************************************************/

#include "EthernetPacketGenerator.h"
#include "EthernetPacket.h"
#include <fstream>  // Required for file handling
#include <iostream>
#include <iomanip>  // For formatting output
#include <cmath>    // For ceil

// Constructor for EthernetPacketGenerator class
EthernetPacketGenerator::EthernetPacketGenerator(const EthernetPacketConfig& i_Config, const EthernetPacket& i_Packet, string& output_file)
{
    Config = i_Config;
    Packet = i_Packet;

    // Call the GeneratePackets function to generate packets and write to the output file
    GeneratePackets(output_file);
}

// Function to generate Ethernet packets and write them to an output file
void EthernetPacketGenerator::GeneratePackets(string& output_file)
{
    // Calculate the line rate in gigabytes per microsecond
    double LineRateGigaBytePerMicroSec = (Config.LineRate / 8.0) * 1000;

    // Initialize variables
    double CurrentTime = 0;
    double TimePerPacket = static_cast<double>(Packet.Size()) / LineRateGigaBytePerMicroSec;
    int CaptureSizeUs = Config.CaptureSizeMs * 1000; // Convert capture size to microseconds
    int PacketPerBurstCount = 0;
    int NumOfPackets = 0;

    // Open the output file for writing
    ofstream file(output_file);
    if (!file)
    {
        cerr << "File could not be opened!\n"; // Print error if file can't be opened
        return;
    }

    // Main loop to generate packets until the total time reaches the capture size
    while (CurrentTime < CaptureSizeUs)
    {
        // If there's enough time for another packet and the burst hasn't reached its limit
        if (CurrentTime + TimePerPacket <= CaptureSizeUs && PacketPerBurstCount < Config.BurstSize)
        {
            PacketPerBurstCount++;
            NumOfPackets++;
            SendPacket(file);      // Write a packet to the output file
            CurrentTime = CurrentTime + TimePerPacket; // Update the current time
        }
        // If the burst size is reached, send the Inter-Frame Gap (IFG) for a burst periodicity duration
        else if (CurrentTime + Config.BurstPeriodicity_us <= CaptureSizeUs && PacketPerBurstCount == Config.BurstSize)
        {
            PacketPerBurstCount = 0; // Reset the packet per burst count
            SendIFG(ceil(LineRateGigaBytePerMicroSec * Config.BurstPeriodicity_us), file); // Write IFG to the output file
            CurrentTime = CurrentTime + Config.BurstPeriodicity_us; // Update the current time
        }
        // If time is running out, send a final IFG for the remaining time
        else
        {
            SendIFG(ceil((CaptureSizeUs - CurrentTime) * LineRateGigaBytePerMicroSec), file); // Send the remaining IFG
            CurrentTime = CaptureSizeUs; // Set current time to capture size to end the loop
        }
    }

    // Output the total number of packets sent
    cout << "The number of Packets sent: " << NumOfPackets << " packets" << endl;

    // Close the output file after writing
    file.close();
}

// Function to write a single packet to the output file
void EthernetPacketGenerator::SendPacket(ofstream& file)
{
    // Loop through each byte of the packet and write it to the file in hexadecimal format
    for (size_t i = 0; i < Packet.Size(); ++i) {
        // Insert a newline every 4 bytes
        if (i > 0 && i % 4 == 0) {
            file << '\n';
        }

        // Write different parts of the packet: Preamble, SDF, DestAddress, SourceAddress, EtherType, Payload, CRC, and IFG
        if (i < 7)
            file << hex << setw(2) << setfill('0') << static_cast<int>(Packet.Preamble[i]); // Write Preamble
        else if (i < 8)
            file << hex << setw(2) << setfill('0') << static_cast<int>(Packet.SDF); // Write Start Frame Delimiter (SDF)
        else if (i < 14)
            file << hex << setw(2) << setfill('0') << static_cast<int>(Packet.DestAddress[i - 8]); // Write Destination MAC Address
        else if (i < 20)
            file << hex << setw(2) << setfill('0') << static_cast<int>(Packet.SourceAddress[i - 14]); // Write Source MAC Address
        else if (i < 22)
            file << hex << setw(2) << setfill('0') << static_cast<int>(Packet.EtherType[i - 20]); // Write EtherType field
        else if (i < Packet.Payload.size() + 22)
            file << hex << setw(2) << setfill('0') << static_cast<int>(Packet.Payload[i - 22]); // Write Payload
        else if (i < Packet.Payload.size() + 26)
            file << hex << setw(2) << setfill('0') << static_cast<int>(Packet.CRC[i - Packet.Payload.size() - 22]); // Write CRC
        else
            file << hex << setw(2) << setfill('0') << static_cast<int>(Packet.IFG[i - Packet.Payload.size() - 26]); // Write Inter-Frame Gap (IFG)
    }
    file << '\n';  // Insert a final newline after the entire packet
}

// Function to write the Inter-Frame Gap (IFG) to the output file
void EthernetPacketGenerator::SendIFG(double NumOfIFGs, ofstream& file)
{
    // Write the specified number of IFG symbols to the file
    for (size_t i = 0; i < NumOfIFGs; ++i) {
        // Insert a newline every 4 bytes for better readability
        if (i > 0 && i % 4 == 0) {
            file << '\n';
        }
        // Write the IFG symbol in hexadecimal format
        file << hex << setw(2) << setfill('0') << static_cast<int>(IFGSymbol);
    }
    file << '\n';  // Insert a final newline after writing the IFG
}
