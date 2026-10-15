import os
import subprocess

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    cloud_topic_arg = DeclareLaunchArgument(
        'cloud_topic',
        default_value='unilidar/cloud',
        description='Topic for published point cloud',
    )
    imu_topic_arg = DeclareLaunchArgument(
        'imu_topic',
        default_value='unilidar/imu',
        description='Topic for published IMU messages',
    )
    rviz_arg = DeclareLaunchArgument(
        'rviz',
        default_value='false',
        description='Launch RViz visualization',
    )

    cloud_topic = LaunchConfiguration('cloud_topic')
    imu_topic = LaunchConfiguration('imu_topic')
    rviz = LaunchConfiguration('rviz')

    # Run unitree lidar
    node1 = Node(
        package='unitree_lidar_ros2',
        executable='unitree_lidar_ros2_node',
        name='unitree_lidar_ros2_node',
        output='screen',
        parameters=[
            {'initialize_type': 2},
            {'work_mode': 0},
            {'use_system_timestamp': True},
            {'range_min': 0.0},
            {'range_max': 100.0},
            {'cloud_scan_num': 18},

            {'serial_port': '/dev/ttyACM0'},
            {'baudrate': 4000000},

            {'lidar_port': 6101},
            {'lidar_ip': '192.168.1.62'},
            {'local_port': 6201},
            {'local_ip': '192.168.1.2'},

            {'cloud_frame': 'unilidar_lidar'},
            {'cloud_topic': cloud_topic},
            {'imu_frame': 'unilidar_imu'},
            {'imu_topic': imu_topic},
        ],
    )

    # Run Rviz
    package_path = subprocess.check_output(['ros2', 'pkg', 'prefix', 'unitree_lidar_ros2']).decode('utf-8').rstrip()
    rviz_config_file = os.path.join(package_path, 'share', 'unitree_lidar_ros2', 'view.rviz')
    print('rviz_config_file = ' + rviz_config_file)
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', rviz_config_file],
        output='log',
        condition=IfCondition(rviz),
    )
    return LaunchDescription([cloud_topic_arg, imu_topic_arg, rviz_arg, node1, rviz_node])
