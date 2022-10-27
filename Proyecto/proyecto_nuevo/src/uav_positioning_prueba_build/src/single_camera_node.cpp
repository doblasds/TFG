#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
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
unsigned buffer_size = 1;
vector <geometry_msgs::Pose> pose_buffer;


class Camera 
{
    private:
        ros::NodeHandle n;

        geometry_msgs::Pose fiducial_camera_pose;

        vector <Point3d> fiducial_trans_to_center; // TRANSLATION VECTOR OF MARKERS FROM THE CENTER
        vector <Point3d> fiducial_rot_to_ref; //EVERY POINT REPRESENT AXIS ROLL, PITCH, YAW OF THE MARKER
        
        vector <double> fiducial_camera_orientation_buff = {0, 0, 0, 0};

    public:

        Camera(ros::NodeHandle node);
        void callback(const fiducial_msgs::FiducialTransformArray& msg);
        void send_marker();

};

Camera::Camera(ros::NodeHandle node){
    n = node;

    fiducial_camera_pose.position.x = -99.99;
    fiducial_camera_pose.position.y = 0.0;
    fiducial_camera_pose.position.z = 0.0;

    fiducial_camera_pose.orientation.x = 0.0;
    fiducial_camera_pose.orientation.y = 0.0;
    fiducial_camera_pose.orientation.z = 0.0;
    fiducial_camera_pose.orientation.w = 0.0;


    fiducial_trans_to_center.resize(num_fiducials);
    fiducial_rot_to_ref.resize(num_fiducials);

    fiducial_trans_to_center[0] = Point3d(0, 0, -0.025f);  //FIDUCIAL 0 
    fiducial_trans_to_center[1] = Point3d(0, 0, -0.025f);  //FIDUCIAL 1 
    fiducial_trans_to_center[2] = Point3d(0, 0, -0.025f);  //FIDUCIAL 2

    fiducial_rot_to_ref[0] = Point3d(0, 0, 1.884956);   //FIDUCIAL 0 
    fiducial_rot_to_ref[1] = Point3d(0, 0, 0);          //FIDUCIAL 1  
    fiducial_rot_to_ref[2] = Point3d(0, 0, -1.884956);  //FIDUCIAL 2
}



void Camera::callback(const fiducial_msgs::FiducialTransformArray& msg) 
{
    unsigned num_fiducials_read = msg.transforms.size();
    geometry_msgs::Pose pose_cache = fiducial_camera_pose;
    unsigned failed_fiducials = 0;
    
    fiducial_camera_pose.position.x = -99.99;
    fiducial_camera_pose.position.y = 0.0;
    fiducial_camera_pose.position.z = 0.0;

    fiducial_camera_pose.orientation.x = 0.0;
    fiducial_camera_pose.orientation.y = 0.0;
    fiducial_camera_pose.orientation.z = 0.0;
    fiducial_camera_pose.orientation.w = 0.0;

    for (auto &marker:msg.transforms) {
        
        Point3d translation = fiducial_trans_to_center[marker.fiducial_id];

        fiducial_camera_pose.position.x = 0;

        fiducial_camera_pose.position.x += (marker.transform.translation.x + translation.x);
        fiducial_camera_pose.position.y += (marker.transform.translation.z + translation.y);
        fiducial_camera_pose.position.z += (-marker.transform.translation.y + translation.z);

        //cout << "ROT: x: " << marker.transform.rotation.x << " y: " << marker.transform.rotation.y << " z: " << marker.transform.rotation.z << " w: " << marker.transform.rotation.w << endl;
        float x,y,z,w;
        x = marker.transform.rotation.w;
        y = -marker.transform.rotation.y;
        z = marker.transform.rotation.x;
        w = -marker.transform.rotation.z;

        //FIDUCIAL ORIENTATION CONFIGURATION
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

        tf2::Quaternion q_orig (fiducial_camera_orientation_buff[0], fiducial_camera_orientation_buff[1], fiducial_camera_orientation_buff[2], fiducial_camera_orientation_buff[3]);
        tf2::Quaternion q_rot, q_result;

        tf2Scalar roll = fiducial_rot_to_ref[marker.fiducial_id].x;
        tf2Scalar pitch = fiducial_rot_to_ref[marker.fiducial_id].y;
        tf2Scalar yaw = fiducial_rot_to_ref[marker.fiducial_id].z;

        q_rot.setRPY(roll, pitch, yaw);
        q_result = q_rot * q_orig;
        q_result.normalize();
                
        bool started = pose_cache.orientation.x != 0.0 || pose_cache.orientation.y != 0.0 || pose_cache.orientation.z != 0.0 || pose_cache.orientation.w != 0.0;
        //CALCULO DEL ERROR DEL MARCADOR EN CIERTAS OCASIONES
        bool error = abs( pose_cache.orientation.x - q_result.getX() ) > 0.8 || abs( pose_cache.orientation.y - q_result.getY() ) > 0.8 || abs( pose_cache.orientation.z - q_result.getZ() ) > 0.8 || abs( pose_cache.orientation.w - q_result.getW() ) > 0.8;
        bool founds = pose_cache.position.x != 99.99;
        float threshold = 0.8; 
        if ( error > threshold && started && founds ) {
            failed_fiducials++;
            cout << "FAILED";
        } else {
            fiducial_camera_pose.orientation.x += q_result.getX();
            fiducial_camera_pose.orientation.y += q_result.getY();
            fiducial_camera_pose.orientation.z += q_result.getZ();
            fiducial_camera_pose.orientation.w += q_result.getW();
        }    

    }

    if ( (num_fiducials_read - failed_fiducials) > 0) {


        fiducial_camera_pose.position.x /= num_fiducials_read;
        fiducial_camera_pose.position.y /= num_fiducials_read;
        fiducial_camera_pose.position.z /= num_fiducials_read;

        fiducial_camera_pose.orientation.x /= (num_fiducials_read - failed_fiducials);
        fiducial_camera_pose.orientation.y /= (num_fiducials_read - failed_fiducials);
        fiducial_camera_pose.orientation.z /= (num_fiducials_read - failed_fiducials);
        fiducial_camera_pose.orientation.w /= (num_fiducials_read - failed_fiducials);


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

                
                // if (abs(average_pose.orientation.x - pose.orientation.x) > 0.8) {
                //     cout << "tontito";
                //     //average_pose.orientation.x = pose.orientation.x;
                // } 
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
        }
    }else{
        fiducial_camera_pose.position.x = -99.99;
    }

    send_marker();
}

void Camera::send_marker(){ 
    
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
    ros::init(argc, argv, "camera_test");
    ros::NodeHandle n;

    ros::Rate r(30);   

    std::cout << std::fixed;
    std::cout << std::setprecision(2);


    string node_name = ros::this_node::getName();
    std::string ns = ros::this_node::getNamespace();

    cout << "NODE NAME: " << node_name << endl;
    cout << "NAMESPACE: " << ns << endl;

    string cam;
    double dx_camera = 0.0;
    double dy_camera = 0.0;
    double dz_camera = 0.0;
    double rotation_camera = 0.0;
    double roll, pitch, yaw;
    roll = pitch = yaw = 0;

    n.getParam(node_name + "/num_fiducials", num_fiducials);



    Camera camera(n, dx_camera, dy_camera, dz_camera, rotation_camera);

    marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    ros::Subscriber sub = n.subscribe(ns + "/fiducial_transforms", 1, &Camera::callback, &camera);

    
    ros::spin();

    return 0; 


}