#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

#include <ros/ros.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf/transform_datatypes.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2/LinearMath/Scalar.h>

#include <visualization_msgs/Marker.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <dynamic_reconfigure/server.h>
#include <std_srvs/SetBool.h>
#include <std_msgs/String.h>

#include "fiducial_msgs/Fiducial.h"
#include "fiducial_msgs/FiducialArray.h"
#include "fiducial_msgs/FiducialTransform.h"
#include "fiducial_msgs/FiducialTransformArray.h"
#include "aruco_detect/DetectorParamsConfig.h"

#include <vision_msgs/Detection2D.h>
#include <vision_msgs/Detection2DArray.h>
#include <vision_msgs/ObjectHypothesisWithPose.h>

#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>

#include <list>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>


using namespace std;
using namespace cv;


using namespace std;
ros::Publisher marker_pub;

double num_fiducials = 0;
unsigned buffer_size = 3;
vector <geometry_msgs::Pose> pose_buffer;


class Listener 
{
    private:


    //uint32_t shape = visualization_msgs::Marker::SPHERE_LIST;
        ros::NodeHandle n;
        // vector <double> fiducial_camera_position;
        // vector <double> fiducial_camera_orientation;

        // vector <double> fiducial_world_position;
        // vector <double> fiducial_world_orientation;

        geometry_msgs::Pose fiducial_camera_pose;
        geometry_msgs::Pose fiducial_world_pose;


        vector <Point3d> fiducial_trans_to_center; // TRANSLATION VECTOR OF MARKERS FROM THE CENTER
        vector <float> fiducial_rot_to_ref; // YAX AXIS, IN RADS 

    public:

        Listener(ros::NodeHandle node);
        void callback(const fiducial_msgs::FiducialTransformArray& msg);
        void send_marker();
};

Listener::Listener(ros::NodeHandle node){
    n = node;

    // fiducial_camera_position.resize(4);
    fiducial_camera_pose.position.x = 0.0;
    fiducial_camera_pose.position.y = 0.0;
    fiducial_camera_pose.position.z = 0.0;
    //fiducial_camera_pose.position.? = 1.0;

    // fiducial_camera_orientation.resize(4);
    fiducial_camera_pose.orientation.x = 0.0;
    fiducial_camera_pose.orientation.y = 0.0;
    fiducial_camera_pose.orientation.z = 0.0;
    fiducial_camera_pose.orientation.w = 1.0;


    fiducial_trans_to_center.resize(num_fiducials);
    fiducial_rot_to_ref.resize(num_fiducials);

    fiducial_trans_to_center[0] = Point3d(0,0,-0.025f);    // fiducial_trans_to_center.push_back(Point3d(0,0,-0.025f)); //FIDUCIAL 0
    fiducial_trans_to_center[1] = Point3d(0,0,-0.025f);    // fiducial_trans_to_center.push_back(Point3d(0,0,-0.025f)); //FIDUCIAL 1
    fiducial_trans_to_center[2] = Point3d(0,0,-0.025f);    // fiducial_trans_to_center.push_back(Point3d(0,0,-0.025f)); //FIDUCIAL 2

    fiducial_rot_to_ref[0] = 1.884956;    // fiducial_rotation.push_back(1.884956); //FIDUCIAL 0 -> FRENTE 120º
    fiducial_rot_to_ref[1] = 0;           // fiducial_rotation.push_back(0); //FIDUCIAL 1
    fiducial_rot_to_ref[2] = -1.884956;   // fiducial_rotation.push_back(-1.884956); //FIDUCIAL 2


    // cout << "[0]: " << fiducial_rotation[0] << "[1]: " << fiducial_rotation[1] << "[2]: " << fiducial_rotation[2] << endl;

}



void Listener::callback(const fiducial_msgs::FiducialTransformArray& msg) 
{
    float factor = 3.0;
    unsigned num_fiducials_read = msg.transforms.size();
    vector <double> fiducial_camera_orientation_buff = {0, 0, 0, 0};

    // fiducial_camera_position[0] = 0.0;
    // fiducial_camera_position[1] = 0.0;
    // fiducial_camera_position[2] = 0.0;
    // fiducial_camera_position[3] = 1.0;

    fiducial_camera_pose.position.x = 0.0;
    fiducial_camera_pose.position.y = 0.0;
    fiducial_camera_pose.position.z = 0.0;
    // fiducial_camera_position[3] = 1.0;

    // fiducial_camera_orientation[0] = 0.0;
    // fiducial_camera_orientation[1] = 0.0;
    // fiducial_camera_orientation[2] = 0.0;
    // fiducial_camera_orientation[3] = 0.0;

    fiducial_camera_pose.orientation.x = 0.0;
    fiducial_camera_pose.orientation.y = 0.0;
    fiducial_camera_pose.orientation.z = 0.0;
    fiducial_camera_pose.orientation.w = 0.0;



    for (auto &marker:msg.transforms) {
        
        Point3d translation = fiducial_trans_to_center[marker.fiducial_id];

        fiducial_camera_pose.position.x += (marker.transform.translation.x + translation.x) * factor;
        fiducial_camera_pose.position.y += (marker.transform.translation.z + translation.y) * factor;
        fiducial_camera_pose.position.z += (-marker.transform.translation.y + translation.z) * factor;

        //cout << "ROT: x: " << marker.transform.rotation.x << " y: " << marker.transform.rotation.y << " z: " << marker.transform.rotation.z << " w: " << marker.transform.rotation.w << endl;
        float x,y,z,w;
        x = marker.transform.rotation.w;
        y = -marker.transform.rotation.y;
        z = marker.transform.rotation.x;
        w = -marker.transform.rotation.z;

        switch (marker.fiducial_id) {
            case 0:
                fiducial_camera_orientation_buff[0] = x;
                fiducial_camera_orientation_buff[1] = y;
                fiducial_camera_orientation_buff[2] = z;
                fiducial_camera_orientation_buff[3] = w;
                break;
            case 1:
                fiducial_camera_orientation_buff[0] = -y; // X
                fiducial_camera_orientation_buff[1] = x; // Y
                fiducial_camera_orientation_buff[2] = z; // Z
                fiducial_camera_orientation_buff[3] = w; // W
                break;
            case 2:
                fiducial_camera_orientation_buff[0] = y;
                fiducial_camera_orientation_buff[1] = -x;
                fiducial_camera_orientation_buff[2] = z;
                fiducial_camera_orientation_buff[3] = w;
                break;

        }
        // fiducial_camera_orientation_buff[0] = marker.transform.rotation.w;
        // fiducial_camera_orientation_buff[1] = -marker.transform.rotation.y;
        // fiducial_camera_orientation_buff[2] = marker.transform.rotation.x;
        // fiducial_camera_orientation_buff[3] = -marker.transform.rotation.z;

        tf2::Quaternion q_orig (fiducial_camera_orientation_buff[0], fiducial_camera_orientation_buff[1], fiducial_camera_orientation_buff[2], fiducial_camera_orientation_buff[3]);
        
        tf2::Quaternion q_rot, q_result;

        tf2Scalar roll = 0;
        tf2Scalar pitch = 0;
        tf2Scalar yaw = fiducial_rot_to_ref[marker.fiducial_id]; //M_PI_2;

        q_rot.setRPY(roll, pitch, yaw);
        q_result = q_rot * q_orig;
        
        cout << "FIDUCIAL_CAMERA_ORIENTATION marker_id: " << marker.fiducial_id << endl << "x: " << q_result.x() << " y: " << q_result.y() << " z: " << q_result.z() << " w: " << q_result.w() << endl; 

        fiducial_camera_pose.orientation.x += q_result.getX();
        fiducial_camera_pose.orientation.y += q_result.getY();
        fiducial_camera_pose.orientation.z += q_result.getZ();
        fiducial_camera_pose.orientation.w += q_result.getW();

    }

    if (num_fiducials_read > 0) {

        fiducial_camera_pose.position.x /= num_fiducials_read;
        fiducial_camera_pose.position.y /= num_fiducials_read;
        fiducial_camera_pose.position.z /= num_fiducials_read;

        fiducial_camera_pose.orientation.x /= num_fiducials_read;
        fiducial_camera_pose.orientation.y /= num_fiducials_read;
        fiducial_camera_pose.orientation.z /= num_fiducials_read;
        fiducial_camera_pose.orientation.w /= num_fiducials_read;

        //calculate_world_position
    }else{
        fiducial_camera_pose.position.x = -99.99;
        fiducial_camera_pose.orientation.x = -99.99;
    }

    pose_buffer.push_back(fiducial_camera_pose);
    
    if (pose_buffer.size() == buffer_size) {

        geometry_msgs::Pose average_pose;
        average_pose.position.x = 0.0;
        average_pose.position.y = 0.0;
        average_pose.position.z = 0.0;
        average_pose.orientation.x = 0.0;
        average_pose.orientation.y = 0.0;
        average_pose.orientation.z = 0.0;
        average_pose.orientation.w = 0.0;

        for (auto &pose:pose_buffer) {
            average_pose.position.x += pose.position.x;
            average_pose.position.y += pose.position.y;
            average_pose.position.z += pose.position.z;
            average_pose.orientation.x += pose.orientation.x;
            average_pose.orientation.y += pose.orientation.y;
            average_pose.orientation.z += pose.orientation.z;
            average_pose.orientation.w += pose.orientation.w;
        }

        average_pose.position.x /= buffer_size;
        average_pose.position.y /= buffer_size;
        average_pose.position.z /= buffer_size;
        average_pose.orientation.x /= buffer_size;
        average_pose.orientation.y /= buffer_size;
        average_pose.orientation.z /= buffer_size;
        average_pose.orientation.w /= buffer_size;

        fiducial_camera_pose = average_pose;
        pose_buffer.clear();

        send_marker();  
    }



}

void Listener::send_marker(){ 
    


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

    // marker.scale.x = 0.5;
    // marker.scale.y = 0.5;
    // marker.scale.z = 0.5;

    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0f;

    //CALCULATE WORLD POSITION

    marker_pub.publish(marker);
}




int main( int argc, char** argv )
{
    ros::init(argc, argv, "marker");
    ros::NodeHandle n;

    n.getParam("marker/num_fiducials", num_fiducials);
    ros::Rate r(30);   
    Listener listener(n);

    std::cout << std::fixed;
    std::cout << std::setprecision(2);


    marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    ros::Subscriber sub = n.subscribe("/fiducial_transforms", 1, &Listener::callback, &listener);
    
    ros::spin();

    return 0; 


}