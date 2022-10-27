#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <cv_bridge/cv_bridge.h>    

#include "fiducial_msgs/Fiducial.h"
#include "fiducial_msgs/FiducialTransform.h"
#include "fiducial_msgs/FiducialTransformArray.h"

#include <math.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>


using namespace std;
using namespace cv;

ros::Publisher marker_pub;

class CameraNode 
{
    private:
        ros::NodeHandle n;

        geometry_msgs::Pose fiducial_camera_pose;
        geometry_msgs::Pose fiducial_world_pose;

        unsigned num_fiducials;
        vector <Point3d> fiducial_trans_to_center; // TRANSLATION VECTOR OF MARKERS FROM THE CENTER
        vector <Point3d> fiducial_rot_to_ref; //EVERY POINT REPRESENT AXIS ROLL, PITCH, YAW OF THE MARKER
        vector<vector<double>> matrix;  

        vector <double> fiducial_orientation_buff = {0, 0, 0, 0};

        double rotation;

    public:

        CameraNode(ros::NodeHandle node, unsigned num_fiducials, double dx, double dy, double dz, double rotation);
        void callback(const fiducial_msgs::FiducialTransformArray& msg);
        void send_result();
        void load_fiducial_orientation_buff(int id, float x, float y, float z, float w);
        void calculate_world_position();
};

CameraNode::CameraNode(ros::NodeHandle node, unsigned num_fiducials, double dx, double dy, double dz, double rotation){
    n = node;
    this->num_fiducials = num_fiducials;

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

    fiducial_rot_to_ref[0] = Point3d(0, 0, 0);          //FIDUCIAL 0 
    fiducial_rot_to_ref[1] = Point3d(0, 0, -2.09441);  //FIDUCIAL 1  
    fiducial_rot_to_ref[2] = Point3d(0, 0, 2.09441);  //FIDUCIAL 2

    double rot_to_rads = (M_PI * rotation) / 180;

    //CORRECIÓN DE ROT
    for (int i = 0; i < num_fiducials; i++) {
        fiducial_rot_to_ref[i].z -= 1.57 + rot_to_rads ; //+ rotation ;//+ (2 * M_PI)
    }
    
    matrix.resize(4);
    matrix[0] = {cos(rot_to_rads), -sin(rot_to_rads),    0.0f,    dx};
    matrix[1] = {sin(rot_to_rads), cos(rot_to_rads),     0.0f,    dy};
    matrix[2] = {0.0f,                 0.0f,                     1.0f,    dz};
    matrix[3] = {0.0f,                 0.0f,                     0.0f,    1.0f};


}

void CameraNode::callback(const fiducial_msgs::FiducialTransformArray& msg) 
{
    unsigned num_fiducials_read = msg.transforms.size();
    geometry_msgs::Pose pose_cache = fiducial_camera_pose;
    
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

        float x,y,z,w;
        x = marker.transform.rotation.w;
        y = -marker.transform.rotation.y;
        z = marker.transform.rotation.x;
        w = -marker.transform.rotation.z;

        load_fiducial_orientation_buff(marker.fiducial_id, x, y, z, w);

        tf2::Quaternion q_orig (fiducial_orientation_buff[0], fiducial_orientation_buff[1], fiducial_orientation_buff[2], fiducial_orientation_buff[3]);
        tf2::Quaternion q_rot, q_result;

        tf2Scalar roll = 0;
        tf2Scalar pitch = 0;
        tf2Scalar yaw = fiducial_rot_to_ref[marker.fiducial_id].z; //M_PI_2;

        q_rot.setRPY(roll, pitch, yaw);
        q_result = q_rot * q_orig;
        q_result.normalize();
        
        fiducial_camera_pose.orientation.x += q_result.getX();
        fiducial_camera_pose.orientation.y += q_result.getY();
        fiducial_camera_pose.orientation.z += q_result.getZ();
        fiducial_camera_pose.orientation.w = q_result.getW();
    }

    if (num_fiducials_read > 0) {
        fiducial_camera_pose.position.x /= num_fiducials_read;
        fiducial_camera_pose.position.y /= num_fiducials_read;
        fiducial_camera_pose.position.z /= num_fiducials_read;

        fiducial_camera_pose.orientation.x /= num_fiducials_read;
        fiducial_camera_pose.orientation.y /= num_fiducials_read;
        fiducial_camera_pose.orientation.z /= num_fiducials_read;
        // fiducial_camera_pose.orientation.w /= num_fiducials_read;
    }


    calculate_world_position();
    send_result();
}

void CameraNode::send_result(){ 
    
    geometry_msgs::PoseStamped position_msg;
    position_msg.header.frame_id = "world";
    position_msg.header.stamp = ros::Time::now();

    position_msg.pose.position.x = fiducial_world_pose.position.x;
    position_msg.pose.position.y = fiducial_world_pose.position.y;
    position_msg.pose.position.z = fiducial_world_pose.position.z;
    position_msg.pose.orientation.x = fiducial_world_pose.orientation.x;
    position_msg.pose.orientation.y = fiducial_world_pose.orientation.y;
    position_msg.pose.orientation.z = fiducial_world_pose.orientation.z;
    position_msg.pose.orientation.w = fiducial_world_pose.orientation.w;
    
    marker_pub.publish(position_msg);
}



void CameraNode::load_fiducial_orientation_buff(int id, float x, float y, float z, float w){
        //FIDUCIAL ORIENTATION CONFIGURATION
        switch (id) {
            case 0:
                fiducial_orientation_buff[0] = x;
                fiducial_orientation_buff[1] = y;
                fiducial_orientation_buff[2] = z;
                fiducial_orientation_buff[3] = w;
                break;
            case 1:
                fiducial_orientation_buff[0] = -y; 
                fiducial_orientation_buff[1] = x; 
                fiducial_orientation_buff[2] = z; 
                fiducial_orientation_buff[3] = w; 
                break;
            case 2:
                fiducial_orientation_buff[0] = y;
                fiducial_orientation_buff[1] = -x;
                fiducial_orientation_buff[2] = z;
                fiducial_orientation_buff[3] = w;
                break;
        }
}

void CameraNode::calculate_world_position(){
    // cout << "MATRIX: \n";
    // for (int i=0; i< 4 ; i++){
    //     for (int j=0; j< 4 ; j++){
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "aruco_camera_position: << \n";

    // for (int u=0; u< 3 ; u++){
    //     cout << aruco_camera_position[u] << " ";
    // }
    // cout << endl;
    if (fiducial_camera_pose.position.x != -99.99)
    {

        fiducial_world_pose.position.x = 0.0;
        fiducial_world_pose.position.y = 0.0;
        fiducial_world_pose.position.z = 0.0;
    
        vector <double> cam_pose_to_vector(4);
        cam_pose_to_vector[0] = fiducial_camera_pose.position.x;
        cam_pose_to_vector[1] = fiducial_camera_pose.position.y;
        cam_pose_to_vector[2] = fiducial_camera_pose.position.z;
        cam_pose_to_vector[3] = 1.0;

        vector <double> result = {0.0, 0.0, 0.0, 1};


        for(unsigned i = 0; i < matrix.size(); ++i)
        {
            for(unsigned k = 0; k < matrix.size(); ++k)
            {
                result[i] += matrix[i][k] * cam_pose_to_vector[k];
            }
        }

        fiducial_world_pose.position.x = result[0];
        fiducial_world_pose.position.y = result[1];
        fiducial_world_pose.position.z = result[2];
        //Have to calculate the orientation of the fiducial
        fiducial_world_pose.orientation = fiducial_camera_pose.orientation;
    }else{
        fiducial_world_pose.position.x = -99.99;
    }
}

int main( int argc, char** argv )
{
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    int num_fids = 0;
    double dx_camera = 0.0;
    double dy_camera = 0.0;
    double dz_camera = 0.0;
    double rotation_camera = 0.0;
    // double roll, pitch, yaw;
    // roll = pitch = yaw = 0;

    ros::init(argc, argv, "camera_node");
    ros::NodeHandle n;
    ros::Rate r(30);   

    string node_name = ros::this_node::getName();
    std::string ns = ros::this_node::getNamespace();

    cout << "NODE NAME: " << node_name << endl;
    cout << "NAMESPACE: " << ns << endl;

    n.getParam(node_name + "/num_fiducials", num_fids);
    n.getParam(node_name + "/dx_camera", dx_camera);
    n.getParam(node_name + "/dy_camera", dy_camera);
    n.getParam(node_name + "/dz_camera", dz_camera);

    //ROTACION CAMARA YAW
    n.getParam(node_name + "/rotation_camera", rotation_camera);

    CameraNode camera(n, num_fids, dx_camera, dy_camera, dz_camera, rotation_camera);

    marker_pub = n.advertise<geometry_msgs::PoseStamped>(node_name + "/pose", 1);
    ros::Subscriber sub = n.subscribe(ns + "/fiducial_transforms", 1, &CameraNode::callback, &camera);
    
    ros::spin();

    return 0; 
}