/**********************************************************************
Contributed By: Yash Tomar (LinkedIn: https://www.linkedin.com/in/yash-tomar1708/ || github: https://github.com/1708yash)

This code is for directly recevining and parsing the 3D point data (NO IMU) from the Unitree Lidar using UDP mode.

I made this because while using the Unitree Lidar SDK, I found that the example code provided in the SDK was only giving me IMU data but not always providing the cloud data, so this is for only cloud data receiving and parsing.
***********************************************************************/

#include "unitree_lidar_sdk.h"           // Core interface
#include "unitree_lidar_utilities.h"     // For PointCloudUnitree
#include <unistd.h>                      // For sleep()
#include <iostream>
#include <iomanip>                       // For std::setprecision

using namespace unilidar_sdk2;

int main(int argc, char *argv[])
{
    // 1. Create reader
    UnitreeLidarReader *lreader = createUnitreeLidarReader();

    // 2. UDP initialization parameters
    std::string lidar_ip   = "192.168.1.62";
    std::string local_ip   = "192.168.1.2";
    unsigned short lidar_port = 6101;
    unsigned short local_port = 6201;

    // 3. Initialize UDP
    if (lreader->initializeUDP(lidar_port, lidar_ip, local_port, local_ip) != 0) {
        std::cerr << "Unilidar UDP initialization failed, exiting.\n";
        return -1;
    }
    std::cout << "Unilidar UDP initialization succeeded.\n";

    // 4. Start rotation
    lreader->startLidarRotation();
    sleep(1);

    // 5. Set work mode: 3D mode with IMU disabled (bit 1=0 for 3D, bit 2=1 to disable IMU ⇒ mode=4)
    uint32_t workMode = (1 << 2);  // Disable IMU
    std::cout << "Setting work mode to: " << workMode << "\n";
    lreader->setLidarWorkMode(workMode);
    sleep(1);

    // 6. Reset to apply settings
    lreader->resetLidar();
    sleep(1);

    // 7. Main loop: parse and handle only full 3D cloud packets
    while (true) {
        int ptype = lreader->runParse();
        if (ptype == LIDAR_POINT_DATA_PACKET_TYPE) {
            PointCloudUnitree cloud;
            if (!lreader->getPointCloud(cloud)) continue;

            std::cout << "A Cloud message is parsed!\n";
            std::cout << "\tstamp = " << std::fixed << std::setprecision(6)
                      << cloud.stamp << ", id = " << cloud.id << "\n";
            std::cout << "\tcloud size = " << cloud.points.size()
                      << ", ringNum = " << cloud.ringNum << "\n";

            std::cout << "\tfirst 10 points (x, y, z, intensity, time, ring):\n";
            for (size_t i = 0; i < std::min<size_t>(cloud.points.size(), 10); ++i) {
                const auto &p = cloud.points[i];
                std::cout << "\t  ("
                          << std::fixed << std::setprecision(6)
                          << p.x << ", "
                          << p.y << ", "
                          << p.z << ", "
                          << p.intensity << ", "
                          << p.time << ", "
                          << static_cast<int>(p.ring)
                          << ")\n";
            }
        }
    }

    return 0;
}