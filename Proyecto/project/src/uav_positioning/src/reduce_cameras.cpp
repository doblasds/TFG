#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseStamped.h>
#include <message_filters/sync_policies/approximate_time.h>

#include <math.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>


using namespace message_filters;
using namespace std;

ros::Publisher marker_pub;

void callback(const geometry_msgs::PoseStamped::ConstPtr& msg_cam1, \
              const geometry_msgs::PoseStamped::ConstPtr& msg_cam2)
              //N
{
    geometry_msgs::Pose result;
    //double factor = 1.0;    

    bool found = true;
    bool found_cam1 = msg_cam1->pose.position.x != -99.99;
    bool found_cam2 = msg_cam2->pose.position.x != -99.99;

    // bool found_cam1 = msg_cam1 != nullptr;
    // bool found_cam2 = msg_cam2 != nullptr;


    if ( !found_cam1 && !found_cam2 )
    {
        found = false;
    }
    else if (found_cam1 && !found_cam2)
    {
        result = msg_cam1->pose;
    }
    else if (!found_cam1 && found_cam2)
    {
        result = msg_cam2->pose;
    }
    else
    {
        // cout << "msg 1: x: " << msg_cam1->pose.position.x << " y: " << msg_cam1->pose.position.y << " z: " << msg_cam1->pose.position.x << endl;
        // cout << "msg 2: x: " << msg_cam2->pose.position.x << " y: " << msg_cam2->pose.position.y << " z: " << msg_cam2->pose.position.x << endl;
        
        
        result.position.x = ((msg_cam1->pose.position.x + msg_cam2->pose.position.x) / 2);
        result.position.y = ((msg_cam1->pose.position.y + msg_cam2->pose.position.y) / 2);
        result.position.z = ((msg_cam1->pose.position.z + msg_cam2->pose.position.z) / 2);
    
        // result.orientation.x = ((msg_cam1->pose.orientation.x + msg_cam2->pose.orientation.x) / 2);
        // result.orientation.y = ((msg_cam1->pose.orientation.y + msg_cam2->pose.orientation.y) / 2);
        // result.orientation.z = ((msg_cam1->pose.orientation.z + msg_cam2->pose.orientation.z) / 2);
        result.orientation.x =  msg_cam2->pose.orientation.x;
        result.orientation.y =  msg_cam2->pose.orientation.y;
        result.orientation.z =  msg_cam2->pose.orientation.z;
        result.orientation.w =  msg_cam2->pose.orientation.w;

        cout << "msg 1: x: " << msg_cam1->pose.orientation.x << " y: " << msg_cam1->pose.orientation.y << " z: " << msg_cam1->pose.orientation.x << " w: " << msg_cam1->pose.orientation.w << endl;
        cout << "msg 2: x: " << msg_cam2->pose.orientation.x << " y: " << msg_cam2->pose.orientation.y << " z: " << msg_cam2->pose.orientation.x << " w: " << msg_cam2->pose.orientation.w <<  endl;


        // result.orientation.x = msg_cam2->pose.orientation.x;
        // result.orientation.y = msg_cam2->pose.orientation.y;
        // result.orientation.z = msg_cam2->pose.orientation.z;
        // result.orientation.w = msg_cam2->pose.orientation.w;

    }

    if (found){

        visualization_msgs::Marker marker;
        uint32_t shape = visualization_msgs::Marker::ARROW;

        marker.header.frame_id = "/world";
        marker.header.stamp = ros::Time::now();
        marker.ns = "basic_shapes";
        marker.id = 0;
        marker.type = shape;
        marker.lifetime = ros::Duration();

        marker.action = visualization_msgs::Marker::ADD;

        marker.pose = result;

        marker.scale.x = 0.8;
        marker.scale.y = 0.4;
        marker.scale.z = 0.2;

        marker.color.r = 0.0f;
        marker.color.g = 1.0f;
        marker.color.b = 1.0f;
        marker.color.a = 1.0;

        marker_pub.publish(marker);
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "reduce_cameras");
    ros::NodeHandle nh;
    ros::Rate r(30);   

    string node_name = ros::this_node::getName();

    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    message_filters::Subscriber<geometry_msgs::PoseStamped> \
    cam1_sub(nh, "/cam_1/camera_node/pose", 1);
    message_filters::Subscriber<geometry_msgs::PoseStamped> \
    cam2_sub(nh, "/cam_1/camera_node/pose", 1);

    marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 1);

    typedef sync_policies::ApproximateTime \
    <geometry_msgs::PoseStamped, geometry_msgs::PoseStamped> MySyncPolicy;

    Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), cam1_sub, cam2_sub);
    sync.registerCallback(boost::bind(&callback, _1, _2));

    ros::spin();

    return 0;
}