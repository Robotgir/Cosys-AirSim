#!/usr/bin/env python3

from sensor_msgs_py import point_cloud2
from sensor_msgs.msg import PointCloud2, PointField
import std_msgs.msg
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSHistoryPolicy, QoSReliabilityPolicy

class AddIntensityAndTimeFieldNode(Node):
    def __init__(self):
        super().__init__('add_intensity_and_time_field_node')

        # QoS Profile (optional)
        qos_profile = QoSProfile(
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=10,
            reliability=QoSReliabilityPolicy.BEST_EFFORT
        )

        # Create a publisher for the modified PointCloud2 message
        self.publisher_ = self.create_publisher(PointCloud2, 'velodyne_points', 10)

        # Create a subscriber to the original PointCloud2 topic
        self.create_subscription(
            PointCloud2,
            # '/airsim_node/SimpleFlight/lidar/Lidar1',  # Adjust topic as needed
            '/airsim_node/SimpleFlight/lidar/points/Lidar1',  # Adjust topic as needed
            self.listener_callback,
            qos_profile
        )
        self.get_logger().info('Node initialized and subscribing to /airsim_node/SimpleFlight/lidar/points/Lidar1')

    def listener_callback(self, msg):
        # Extract timestamp from the header
        timestamp = msg.header.stamp.sec + msg.header.stamp.nanosec * 1e-9

        # Parse the PointCloud2 message into a list of points
        points = list(point_cloud2.read_points(msg, field_names=("x", "y", "z"), skip_nans=True))

        # Create a new list to store points with the added intensity and time (t) fields
        new_points = []

        for point in points:
            # Add intensity as a fixed value (10.0) and the timestamp (t)
            new_point = list(point) + [10.0, timestamp]  # Add intensity field with value 10 and time (t)
            new_points.append(new_point)

        # Create a new PointCloud2 message with the added intensity and time fields
        header = std_msgs.msg.Header()
        header.stamp = self.get_clock().now().to_msg()
        header.frame_id = 'SimpleFlight/Lidar1'  # Use the same frame_id as the input message

        # Define the new fields in the PointCloud2 message
        fields = msg.fields + [
            PointField(name='intensity', offset=12, datatype=PointField.FLOAT32, count=1),
            PointField(name='t', offset=16, datatype=PointField.FLOAT64, count=1)  # Add time field (t)
        ]

        # Create the PointCloud2 message with custom fields
        modified_pc = point_cloud2.create_cloud(header, fields, new_points)
        
        # Publish the modified PointCloud2 message
        self.publisher_.publish(modified_pc)

def main(args=None):
    rclpy.init(args=args)
    node = AddIntensityAndTimeFieldNode()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
