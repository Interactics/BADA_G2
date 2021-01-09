#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char **argv){
    ros::init(argc, argv, "bada_g2_mobile");
    ros::NodeHandle node;
    tf::TransformBroadcaster br;
    tf::Transform transform;
    ros::Rate rate(10.0);

    while (node.ok()) {
        transform.setOrigin(tf::Vector3(-0.70, 0, 0.79));
        transform.setRotation(tf::Quaternion(0, 0, 0, 1));
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "mobile_base", "imu_link"));
        rate.sleep();
    }
    return 0;
}