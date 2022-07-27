#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseStamped.h>
#include <message_filters/sync_policies/approximate_time.h>



using namespace message_filters;
using namespace std;

ros::Publisher marker_pub;

void callback(const geometry_msgs::PoseStamped::ConstPtr& msg_cam1, const geometry_msgs::PoseStamped::ConstPtr& msg_cam2)
{
    geometry_msgs::Pose result;
    geometry_msgs::Pose pose_cam_1, pose_cam_2;
    pose_cam_1 = msg_cam1->pose;
    pose_cam_2 = msg_cam2->pose;
    double factor = 1.0;
        
    bool found = true;
    if (msg_cam1->pose.position.x == -99.99 && msg_cam2->pose.position.x == -99.99 )
    {
        found = false;
    }
    else if (msg_cam1->pose.position.x != -99.99 && msg_cam2->pose.position.x == -99.99)
    {
        //result.position = pose_cam_1.pose.position;
        result = pose_cam_1;

    }
    else if (msg_cam2->pose.position.x != -99.99 && msg_cam1->pose.position.x == -99.99)
    {
        result = pose_cam_2;
    }
    else
    {
        result.position.x = ((pose_cam_1.position.x + pose_cam_2.position.x) / 2) * factor;
        result.position.y = ((pose_cam_1.position.y + pose_cam_2.position.y) / 2) * factor;
        result.position.z = ((pose_cam_1.position.z + pose_cam_2.position.z) / 2) * factor;
    
        result.orientation.x = ((pose_cam_1.orientation.x + pose_cam_2.orientation.x) / 2);
        result.orientation.y = ((pose_cam_1.orientation.y + pose_cam_2.orientation.y) / 2);
        result.orientation.z = ((pose_cam_1.orientation.z + pose_cam_2.orientation.z) / 2);
        result.orientation.w = ((pose_cam_1.orientation.w + pose_cam_2.orientation.w) / 2);

    }

    //cout << "FOUND: " << found << endl;


    if (found){

        cout << "MSG 1: " << msg_cam1->pose.position.x << " " << msg_cam1->pose.position.y << " " << msg_cam1->pose.position.z << endl;
        cout << "Orientation:" << msg_cam1->pose.orientation.x << " " << msg_cam1->pose.orientation.y << " " << msg_cam1->pose.orientation.z << " " << msg_cam1->pose.orientation.w << endl;
        cout << "MSG 2: " << msg_cam2->pose.position.x << " " << msg_cam2->pose.position.y << " " << msg_cam2->pose.position.z << endl;
        cout << "Orientation:" << msg_cam2->pose.orientation.x << " " << msg_cam2->pose.orientation.y << " " << msg_cam2->pose.orientation.z << " " << msg_cam2->pose.orientation.w << endl;
        visualization_msgs::Marker marker;
        uint32_t shape = visualization_msgs::Marker::ARROW;

        marker.header.frame_id = "/world";
        marker.header.stamp = ros::Time::now();
        marker.ns = "basic_shapes";
        marker.id = 0;
        marker.type = shape;
        marker.lifetime = ros::Duration();

        marker.action = visualization_msgs::Marker::ADD;

        // cout << "RESULTADO: x: " << result.position.x << " y: " << result.position.y << " z: " << result.position.z << endl;

        marker.pose = result;

        marker.scale.x = 2;
        marker.scale.y = 1;
        marker.scale.z = 0.5;

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

    string node_name = ros::this_node::getName();

    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 1);

    message_filters::Subscriber<geometry_msgs::PoseStamped> cam1_sub(nh, "/cam_2/camera_node/pose", 1);
    message_filters::Subscriber<geometry_msgs::PoseStamped> cam2_sub(nh, "/cam_1/camera_node/pose", 1);

    typedef sync_policies::ApproximateTime<geometry_msgs::PoseStamped, geometry_msgs::PoseStamped> MySyncPolicy;
  // ApproximateTime takes a queue size as its constructor argument, hence MySyncPolicy(10)
    Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), cam1_sub, cam2_sub);
    //TimeSynchronizer<geometry_msgs::PoseStamped, geometry_msgs::PoseStamped> sync(cam1_sub, cam2_sub, 10);
    sync.registerCallback(boost::bind(&callback, _1, _2));

    ros::spin();

    return 0;
}