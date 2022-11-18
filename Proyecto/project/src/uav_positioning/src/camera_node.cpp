#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <cv_bridge/cv_bridge.h>    
#include <marker_msgs/MarkerDetection.h>

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
        void callback(const marker_msgs::MarkerDetection& msg);
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

    // fiducial_trans_to_center[0] = Point3d(0, 0, -0.25f);  //FIDUCIAL 0 
    // fiducial_trans_to_center[1] = Point3d(0, 0, -0.25f);  //FIDUCIAL 1 
    // fiducial_trans_to_center[2] = Point3d(0, 0, -0.25f);  //FIDUCIAL 2
    // fiducial_trans_to_center[3] = Point3d(0, 0, -0.25f);  //FIDUCIAL 3
    // fiducial_trans_to_center[4] = Point3d(0, 0, -0.25f);  //FIDUCIAL 4

    fiducial_trans_to_center[0] = Point3d(0, 0, -0.9f);  //FIDUCIAL 0 
    fiducial_trans_to_center[1] = Point3d(0, 0, -0.9f);  //FIDUCIAL 1 
    fiducial_trans_to_center[2] = Point3d(0, 0, -0.9f);  //FIDUCIAL 2
    fiducial_trans_to_center[3] = Point3d(0, 0, -0.9f);  //FIDUCIAL 3
    fiducial_trans_to_center[4] = Point3d(0, 0, -0.9f);  //FIDUCIAL 4

    fiducial_rot_to_ref[0] = Point3d(0, 0, 0);          //FIDUCIAL 0 CARA FRONTAL
    fiducial_rot_to_ref[1] = Point3d(0, 0, -M_PI/2);    //FIDUCIAL 1 CARA DERECHA
    fiducial_rot_to_ref[2] = Point3d(0, 0, M_PI/2);     //FIDUCIAL 2 CARA IZQUIERDA
    fiducial_rot_to_ref[3] = Point3d(0, 0, M_PI);       //FIDUCIAL 3 CARA TRASERA
    //??????????????????????????????????????????????????????????????????????????????   
    //fiducial_rot_to_ref[4] = Point3d(0, M_PI/2, 0);     //FIDUCIAL 4 CARA SUPERIOR

    double rot_to_rads = (M_PI * rotation) / 180;

    //CORRECIÓN DE ROT
    for (int i = 0; i < num_fiducials; i++) {                                       
        fiducial_rot_to_ref[i].z -= (-M_PI/2 ) - rot_to_rads ; //
    }
    
    matrix.resize(4);
    matrix[0] = {cos(rot_to_rads),      -sin(rot_to_rads),          0.0f,    dx};
    matrix[1] = {sin(rot_to_rads),      cos(rot_to_rads),           0.0f,    dy};
    matrix[2] = {0.0f,                  0.0f,                       1.0f,    dz};
    matrix[3] = {0.0f,                  0.0f,                       0.0f,    1.0f};


}

void CameraNode::callback(const marker_msgs::MarkerDetection& msg) 
{
    unsigned num_fiducials_read = msg.markers.size();
    geometry_msgs::Pose pose_cache = fiducial_camera_pose;
    vector<tf2::Quaternion> quaternions;
    tf2::Quaternion q_result;

    fiducial_camera_pose.position.x = -99.99;
    fiducial_camera_pose.position.y = 0.0;
    fiducial_camera_pose.position.z = 0.0;

    fiducial_camera_pose.orientation.x = 0.0;
    fiducial_camera_pose.orientation.y = 0.0;
    fiducial_camera_pose.orientation.z = 0.0;
    fiducial_camera_pose.orientation.w = 0.0;


    for (auto &marker:msg.markers) {
        
        Point3d translation = fiducial_trans_to_center[marker.ids.front()];

        fiducial_camera_pose.position.x = 0;
        fiducial_camera_pose.position.x += (marker.pose.position.x + translation.x);
        fiducial_camera_pose.position.y += (marker.pose.position.z + translation.z);
        fiducial_camera_pose.position.z += (-marker.pose.position.y + translation.y);

        float x,y,z,w;
        x = marker.pose.orientation.w;
        y = -marker.pose.orientation.y;
        z = marker.pose.orientation.x;
        w = -marker.pose.orientation.z;

        load_fiducial_orientation_buff(marker.ids.front(), x, y, z, w);

        tf2::Quaternion q_orig (fiducial_orientation_buff[0], fiducial_orientation_buff[1], fiducial_orientation_buff[2], fiducial_orientation_buff[3]);
        tf2::Quaternion q_rot;

        tf2Scalar roll = fiducial_rot_to_ref[marker.ids.front()].x;
        tf2Scalar pitch = fiducial_rot_to_ref[marker.ids.front()].y;
        tf2Scalar yaw = fiducial_rot_to_ref[marker.ids.front()].z;

        q_rot.setRPY(roll, pitch, yaw);
        q_result = q_rot * q_orig;
        q_result.normalize();
        
        // cout << "RESULTADO ORIENTACION MARKER: " <<  marker.ids.front() << ", RADS:" << q_result.getAngle() << endl;
        quaternions.push_back(q_result);
    }

    if (num_fiducials_read > 0) {
        fiducial_camera_pose.position.x /= num_fiducials_read;
        fiducial_camera_pose.position.y /= num_fiducials_read;
        fiducial_camera_pose.position.z /= num_fiducials_read;


        if (num_fiducials_read >= 2 ){
            for (unsigned i=1; i < num_fiducials_read; i++){
                tf2::Quaternion q1 (q_result);
                tf2::Quaternion q2 (quaternions[i]);
                q_result = q1.slerp(q2, 0.5);
            }
        }

        fiducial_camera_pose.orientation.x = q_result.getX();
        fiducial_camera_pose.orientation.y = q_result.getY();
        fiducial_camera_pose.orientation.z = q_result.getZ();
        fiducial_camera_pose.orientation.w = q_result.getW();
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
            case 3:
                fiducial_orientation_buff[0] = -x;
                fiducial_orientation_buff[1] = y;
                fiducial_orientation_buff[2] = z;
                fiducial_orientation_buff[3] = w;
                break;
            case 4:
                fiducial_orientation_buff[0] = x;
                fiducial_orientation_buff[1] = y;
                fiducial_orientation_buff[2] = z;
                fiducial_orientation_buff[3] = w;
                break;
        }
}

void CameraNode::calculate_world_position(){
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

    marker_pub = n.advertise<geometry_msgs::PoseStamped>( node_name + "/pose", 1); 
    ros::Subscriber sub = n.subscribe(ns + "/markers", 1, &CameraNode::callback, &camera);
    
    ros::spin();

    return 0; 
}