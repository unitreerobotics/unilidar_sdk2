#include "unitree_lidar_sdk.h"

using namespace unilidar_sdk2;

int main(int argc, char *argv[])
{

    // Initialize
    UnitreeLidarReader *lreader = createUnitreeLidarReader();

    std::string lidar_ip = "192.168.1.62";
    std::string local_ip = "192.168.1.2";

    unsigned short lidar_port = 6101;
    unsigned short local_port = 6201;

    if (lreader->initializeUDP(lidar_port, lidar_ip, local_port, local_ip))
    {
        printf("Unilidar initialization failed! Exit here!\n");
        exit(-1);
    }
    else
    {
        printf("Unilidar initialization succeed!\n");
    }

    sleep(1);

    std::cout << "stop lidar rotation ..." << std::endl;
    lreader->stopLidarRotation();
    std::cout << "done" << std::endl;
    sleep(1);

    return 0;
}
