/**********************************************************************
 Copyright (c) 2020-2024, Unitree Robotics.Co.Ltd. All rights reserved.
***********************************************************************/

#include "example.h"

int main(int argc, char *argv[])
{
    // Parse command line arguments
    std::string lidar_ip = "192.168.1.62";
    std::string local_ip = "192.168.1.2";
    uint16_t lidar_port = 6101;
    uint16_t local_port = 6201;

    if (argc >= 2) lidar_ip = argv[1];
    if (argc >= 3) local_ip = argv[2];
    if (argc >= 4) lidar_port = std::atoi(argv[3]);
    if (argc >= 5) local_port = std::atoi(argv[4]);

    // Initialize
    UnitreeLidarReader *lreader = createUnitreeLidarReader();

    if (lreader->initializeUDP(lidar_port, lidar_ip, local_port, local_ip))
    {
        printf("Unilidar initialization failed! Exit here!\n");
        exit(-1);
    }
    else
    {
        printf("Unilidar initialization succeed!\n");
        printf("LiDAR IP: %s:%d\n", lidar_ip.c_str(), lidar_port);
        printf("Local IP: %s:%d\n", local_ip.c_str(), local_port);
    }

    sleep(1);
    
    // Set lidar work mode to Wide-angle FOV
    // Bit 0 = 1: Wide-angle FOV (192°)
    // Other bits = 0: 3D mode, IMU enabled, Ethernet mode, auto-start
    uint32_t workMode = 1;
    std::cout << "Setting LiDAR to Wide-angle FOV mode (192°)..." << std::endl;
    lreader->setLidarWorkMode(workMode);
    std::cout << "Wide-angle FOV mode enabled successfully!" << std::endl;
    std::cout << "Please restart LiDAR for changes to take effect." << std::endl;
    
    sleep(1);
    
    return 0;
}
