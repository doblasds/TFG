#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseStamped.h>


using namespace message_filters;
using namespace std;

ros::Publisher marker_pub;

void action(const geometry_msgs::PoseStamped::ConstPtr& msg_cam1, const geometry_msgs::PoseStamped::ConstPtr& msg_cam2)
{
    geometry_msgs::Pose result;
    bool found = true;
    if (msg_cam1->pose.position.x == -99.99 && msg_cam2->pose.position.x == -99.99 )
    {
        result.position.x = 0;
        result.position.y = 0;
        result.position.z = 0;
        found = false;
    }
    else if (msg_cam1->pose.position.x != -99.99 && msg_cam2->pose.position.x == -99.99)
    {
        result.position.x = msg_cam1->pose.position.x;
        result.position.y = msg_cam1->pose.position.y;
        result.position.z = msg_cam1->pose.position.z;
    }
    else if (msg_cam2->pose.position.x != -99.99 && msg_cam1->pose.position.x == -99.99)
    {
        result.position.x = msg_cam2->pose.position.x;
        result.position.y = msg_cam2->pose.position.y;
        result.position.z = msg_cam2->pose.position.z;
    }
    else
    {
        result.position.x = (msg_cam1->pose.position.x + msg_cam2->pose.position.x) / 2;
        result.position.y = (msg_cam1->pose.position.y + msg_cam2->pose.position.y) / 2;
        result.position.z = (msg_cam1->pose.position.z + msg_cam2->pose.position.z) / 2;

    }

    if (found){
        visualization_msgs::Marker marker;
        uint32_t shape = visualization_msgs::Marker::SPHERE;

        marker.header.frame_id = "/my_frame";
        marker.header.stamp = ros::Time::now();
        marker.ns = "basic_shapes";
        marker.id = 0;
        marker.type = shape;
        marker.lifetime = ros::Duration();

        marker.action = visualization_msgs::Marker::ADD;

        marker.pose = result;

        marker.scale.x = 0.1;
        marker.scale.y = 0.1;
        marker.scale.z = 0.1;

        marker.color.r = 0.0f;
        marker.color.g = 0.0f;
        marker.color.b = 1.0f;
        marker.color.a = 1.0;

        marker_pub.publish(marker);
    }

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "reduce_cameras");

    ros::NodeHandle nh;

    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 1);

    message_filters::Subscriber<geometry_msgs::PoseStamped> cam1_sub(nh, "/cam_1/result", 1);
    message_filters::Subscriber<geometry_msgs::PoseStamped> cam2_sub(nh, "/cam_2/result", 1);
    TimeSynchronizer<geometry_msgs::PoseStamped, geometry_msgs::PoseStamped> sync(cam1_sub, cam2_sub, 10);
    sync.registerCallback(boost::bind(&action, _1, _2));

    ros::spin();

    return 0;
}