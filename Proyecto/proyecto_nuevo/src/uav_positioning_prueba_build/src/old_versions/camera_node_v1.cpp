#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <cv_bridge/cv_bridge.h>    

#include "fiducial_msgs/Fiducial.h"
#include "fiducial_msgs/FiducialTransform.h"
#include "fiducial_msgs/FiducialTransformArray.h"

using namespace std;
using namespace cv;

ros::Publisher marker_pub;

class CameraNode 
{
    private:
        ros::NodeHandle n;
        geometry_msgs::Pose fiducial_camera_pose;

    public:
        CameraNode(ros::NodeHandle node);
        void callback(const fiducial_msgs::FiducialTransformArray& msg);
        void send_marker();
};

CameraNode::CameraNode(ros::NodeHandle node){
    n = node;

    fiducial_camera_pose.position.x = 0.0;
    fiducial_camera_pose.position.y = 0.0;
    fiducial_camera_pose.position.z = 0.0;

    fiducial_camera_pose.orientation.x = 0.0;
    fiducial_camera_pose.orientation.y = 0.0;
    fiducial_camera_pose.orientation.z = 0.0;
    fiducial_camera_pose.orientation.w = 0.0;
}


void CameraNode::callback(const fiducial_msgs::FiducialTransformArray& msg) 
{
    //Check if the msg contains fiducials
    unsigned num_fiducials_read = msg.transforms.size();
    if (num_fiducials_read > 0) {
        fiducial_msgs::FiducialTransform fiducial_read;
        fiducial_read = msg.transforms[0];

        fiducial_camera_pose.position.x = fiducial_read.transform.translation.x;
        fiducial_camera_pose.position.y = fiducial_read.transform.translation.z;
        fiducial_camera_pose.position.z = -fiducial_read.transform.translation.y;

        fiducial_camera_pose.orientation.x = fiducial_read.transform.rotation.w;
        fiducial_camera_pose.orientation.y = -fiducial_read.transform.rotation.y;
        fiducial_camera_pose.orientation.z = fiducial_read.transform.rotation.x;
        fiducial_camera_pose.orientation.w = -fiducial_read.transform.rotation.z;
    }

    send_marker();
}

void CameraNode::send_marker(){ 
    
    visualization_msgs::Marker marker;
    uint32_t shape = visualization_msgs::Marker::ARROW;

    marker.header.frame_id = "/world";
    marker.header.stamp = ros::Time::now();
    marker.ns = "basic_shapes";
    marker.id = 0;
    marker.type = shape;
    marker.lifetime = ros::Duration();

    marker.action = visualization_msgs::Marker::ADD;

    marker.pose.position.x = fiducial_camera_pose.position.x;
    marker.pose.position.y = fiducial_camera_pose.position.y;
    marker.pose.position.z = fiducial_camera_pose.position.z;

    marker.pose.orientation.x = fiducial_camera_pose.orientation.x;
    marker.pose.orientation.y = fiducial_camera_pose.orientation.y;
    marker.pose.orientation.z = fiducial_camera_pose.orientation.z;
    marker.pose.orientation.w = fiducial_camera_pose.orientation.w;

    marker.scale.x = 2.0;
    marker.scale.y = 1.0;
    marker.scale.z = 0.5;

    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0f;

    // cout << "Enviando marker..." << endl;
    marker_pub.publish(marker);
}

int main( int argc, char** argv )
{
    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    ros::init(argc, argv, "camera_node");
    ros::NodeHandle n;
    ros::Rate r(30);   

    string node_name = ros::this_node::getName();
    std::string ns = ros::this_node::getNamespace();

    cout << "NODE NAME: " << node_name << endl;
    cout << "NAMESPACE: " << ns << endl;

    CameraNode camera(n);
    marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    ros::Subscriber sub = n.subscribe(ns + "/fiducial_transforms", 1, &CameraNode::callback, &camera);

    ros::spin();

    return 0; 
}