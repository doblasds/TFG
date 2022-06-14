#include <ros/ros.h>
#include "tuw_aruco/aruco_node.h"
#include <vector>
#include <cmath>


using namespace std;
//ros::Publisher marker_pub;

vector <double> multiply (vector <vector<double>> a, vector <double> b){
    vector <double> result(4);
    result = {0,0,0,1};
    int i, j, k;

	for(i = 0; i < a.size(); ++i)
	{
        for(k=0; k < a.size(); ++k)
        {
            result[i] += a[i][k] * b[k];
        }
	}
    return result;

}




class Listener 
{
  public:

    //uint32_t shape = visualization_msgs::Marker::SPHERE_LIST;
    ros::NodeHandle n;


    double aruco_pose_position_x = 0;
    double aruco_pose_position_y = 0;
    double aruco_pose_position_z = 0;
    double aruco_pose_orientation_x = 0.0;
    double aruco_pose_orientation_y = 0.0;
    double aruco_pose_orientation_z = 0.0;
    double aruco_pose_orientation_w = 1.0;

    Listener(ros::NodeHandle node);
    void callback(const marker_msgs::MarkerDetection& msg);
    //void send_marker();
};

Listener::Listener(ros::NodeHandle node){
    n = node;
    //aruco_pose_position_y = 2;
}



void Listener::callback(const marker_msgs::MarkerDetection& msg) 
{
    //std::vector<aruco::Marker> markers;
    //markers = msg.markers;
    for (auto &marker:msg.markers) {
        cout << "ID MARKER:" << marker.ids.front() << endl;
        cout << marker.pose.position.x << endl;
    }
    // aruco_pose_position_x = msg.pose.position.x;
    // aruco_pose_position_y = msg.pose.position.y;
    // aruco_pose_position_z = msg.pose.position.z;
    // aruco_pose_orientation_x = msg.pose.orientation.x;
    // aruco_pose_orientation_y = msg.pose.orientation.y;
    // aruco_pose_orientation_z = msg.pose.orientation.z;
    // aruco_pose_orientation_w = msg.pose.orientation.w;

    //cout << "CONCHITA X: " << msg.pose.position.x << endl;
    //cout << "CONCHITA X: " << msg.markers[0].pose.position.x << endl;
    cout << "ELEMENTOS: " << msg.markers.size() << endl;

    //send_marker();
}

// void Listener::send_marker(){ //tring topic ="visualization_marker"
    

//     visualization_msgs::Marker marker;
//     uint32_t shape = visualization_msgs::Marker::SPHERE;

//     marker.header.frame_id = "/my_frame";
//     marker.header.stamp = ros::Time::now();
//     marker.ns = "basic_shapes";
//     marker.id = 0;
//     marker.type = shape;
//     marker.lifetime = ros::Duration();


//     marker.action = visualization_msgs::Marker::ADD;

//     marker.pose.position.x = aruco_pose_position_x*20;
//     marker.pose.position.y = aruco_pose_position_y*20;
//     marker.pose.position.z = aruco_pose_position_z*20;
//     marker.pose.orientation.x = aruco_pose_orientation_x*20;
//     marker.pose.orientation.y = aruco_pose_orientation_y*20;
//     marker.pose.orientation.z = aruco_pose_orientation_z*20;
//     marker.pose.orientation.w = aruco_pose_orientation_w*20;

//     marker.scale.x = 1.0;
//     marker.scale.y = 1.0;
//     marker.scale.z = 1.0;

//     marker.color.r = 0.0f;
//     marker.color.g = 1.0f;
//     marker.color.b = 0.0f;
//     marker.color.a = 1.0;

//     marker_pub.publish(marker);
// }




int main( int argc, char** argv )
{
    ros::init(argc, argv, "combinate");
    ros::NodeHandle n;
    ros::Rate r(1);   
    Listener listener(n);

    //marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    ros::Subscriber sub = n.subscribe("/cam/1/tracker/markers", 1, &Listener::callback, &listener);
    
    ros::spin();
    // while(ros::ok())
    //     listener.send_marker();



  return 0;


}