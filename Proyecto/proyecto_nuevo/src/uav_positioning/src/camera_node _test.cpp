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

        vector<vector<double>> translation_matrix;  

        geometry_msgs::Pose fiducial_camera_pose;
        geometry_msgs::Pose fiducial_world_pose;

        vector <Point3d> fiducial_trans_to_center; // TRANSLATION VECTOR OF MARKERS FROM THE CENTER
        vector <float> fiducial_rot_to_ref; // YAX AXIS, IN RADS 
        vector <double> fiducial_camera_orientation_buff = {0, 0, 0, 0};

    public:

        Camera(ros::NodeHandle node, double dx, double dy, double dz, double rotation);
        void calculate_world_position();
        void callback(const fiducial_msgs::FiducialTransformArray& msg);
        void send_marker();

};

Camera::Camera(ros::NodeHandle node, double dx, double dy, double dz, double rotation){
    n = node;

    fiducial_camera_pose.position.x = 0.0;
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

    fiducial_rot_to_ref[0] = 1.884956;    
    fiducial_rot_to_ref[1] = 0;           
    fiducial_rot_to_ref[2] = -1.884956;  

    translation_matrix.resize(4);

    translation_matrix[0] = {cos(M_PI * rotation),    -sin(M_PI * rotation),    0.0f,           dx};
    translation_matrix[1] = {sin(M_PI * rotation),    cos(M_PI * rotation),     0.0f,           dy};
    translation_matrix[2] = {0.0f,                    0.0f,                     1.0f,           dz};
    translation_matrix[3] = {0.0f,                    0.0f,                     0.0f,           1.0f};


}

void Camera::calculate_world_position(){
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

    fiducial_world_pose.position.x = 0.0;
    fiducial_world_pose.position.y = 0.0;
    fiducial_world_pose.position.z = 0.0;
 

    vector <double> cam_pose_to_vector(4);
    cam_pose_to_vector[0] = fiducial_camera_pose.position.x;
    cam_pose_to_vector[1] = fiducial_camera_pose.position.y;
    cam_pose_to_vector[2] = fiducial_camera_pose.position.z;
    cam_pose_to_vector[3] = 1.0;


    vector <double> result = {0.0, 0.0, 0.0, 1};


	for(unsigned i = 0; i < translation_matrix.size(); ++i)
	{
        for(unsigned k = 0; k < translation_matrix.size(); ++k)
        {
            result[i] += translation_matrix[i][k] * cam_pose_to_vector[k];
        }
	}

    fiducial_world_pose.position.x = result[0];
    fiducial_world_pose.position.y = result[1];
    fiducial_world_pose.position.z = result[2];

}


void Camera::callback(const fiducial_msgs::FiducialTransformArray& msg) 
{
    float factor = 3.0;
    unsigned num_fiducials_read = msg.transforms.size();
    geometry_msgs::Pose pose_cache = fiducial_camera_pose;
    unsigned failed_fiducials = 0;
    
    fiducial_camera_pose.position.x = 0.0;
    fiducial_camera_pose.position.y = 0.0;
    fiducial_camera_pose.position.z = 0.0;

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
        cout << "FIDUCIAL_CAMERA_ORIENTATION marker_id: " << marker.fiducial_id << endl << "x: " << fiducial_camera_orientation_buff[0] << " y: " << fiducial_camera_orientation_buff[1] << " z: " << fiducial_camera_orientation_buff[2] << " w: " << fiducial_camera_orientation_buff[3] << endl; 


        tf2::Quaternion q_orig (fiducial_camera_orientation_buff[0], fiducial_camera_orientation_buff[1], fiducial_camera_orientation_buff[2], fiducial_camera_orientation_buff[3]);
        
        tf2::Quaternion q_rot, q_result;

        tf2Scalar roll = 0;
        tf2Scalar pitch = 0;
        tf2Scalar yaw = fiducial_rot_to_ref[marker.fiducial_id]; //M_PI_2;

        q_rot.setRPY(roll, pitch, yaw);
        // q_orig.normalize();
        // q_rot.normalize();
        q_result = q_rot * q_orig;
        q_result.normalize();
        
        // cout << "FIDUCIAL_CAMERA_ORIENTATION marker_id: " << marker.fiducial_id << endl << "x: " << q_result.x() << " y: " << q_result.y() << " z: " << q_result.z() << " w: " << q_result.w() << endl; 

        
        bool caca = pose_cache.orientation.x == 0.0 && pose_cache.orientation.y == 0.0 && pose_cache.orientation.z == 0.0 && pose_cache.orientation.w == 0.0;
        //CALCULO DEL ERROR DEL MARCADOR EN CIERTAS OCASIONES
        bool error = abs( pose_cache.orientation.x - q_result.getX() ) > 0.8 || abs( pose_cache.orientation.y - q_result.getY() ) > 0.8 || abs( pose_cache.orientation.z - q_result.getZ() ) > 0.8 || abs( pose_cache.orientation.w - q_result.getW() ) > 0.8;
        if ( error > 0.8 && !caca ) {
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

            send_marker();  
        }


        //send_marker();

        //calculate_world_position
    }else{
        // fiducial_camera_pose.position.x = -99.99;
        // fiducial_camera_pose.orientation.x = -99.99;
    }


     //send marker to rviz




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
    ros::init(argc, argv, "camera_node");
    ros::NodeHandle n;

    // ros::Rate r(1);   

    std::cout << std::fixed;
    std::cout << std::setprecision(2);


    string node_name = ros::this_node::getName();
    double dx_camera = 0.0;
    double dy_camera = 0.0;
    double dz_camera = 0.0;
    double rotation_camera = 0.0;

    n.getParam(node_name + "/num_fiducials", num_fiducials);
    n.getParam(node_name + "/dx_camera", dx_camera);
    n.getParam(node_name + "/dy_camera", dy_camera);
    n.getParam(node_name + "/dz_camera", dz_camera);
    n.getParam(node_name + "/rotation_camera", rotation_camera);

    Camera camera(n, dx_camera, dy_camera, dz_camera, rotation_camera);

    //Publisher definivo
    // marker_pub = n.advertise<geometry_msgs::PoseStamped>(node_name + "/result", 1);


    marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    ros::Subscriber sub = n.subscribe("/fiducial_transforms", 1, &Camera::callback, &camera);
    
    ros::spin();

    return 0; 


}