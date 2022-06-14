#include <ros/ros.h>
#include "tuw_aruco/aruco_node.h"
#include <vector>
#include <cmath>


using namespace std;
//ros::Publisher marker_pub;

class CameraReader 
{
    public:

        ros::NodeHandle n;

        
        // double aruco_pose_position_x = 0;
        // double aruco_pose_position_y = 0;
        // double aruco_pose_position_z = 0;
        // double aruco_pose_orientation_x = 0.0;
        // double aruco_pose_orientation_y = 0.0;
        // double aruco_pose_orientation_z = 0.0;
        // double aruco_pose_orientation_w = 1.0;

        CameraReader(ros::NodeHandle node, double dx, double dy, double dz, double rotation = 0);
        void callback(const marker_msgs::MarkerDetection& msg);
        //void send_marker();

    private:
        vector <double> aruco_camera_position;
        vector <double> aruco_world_position;
        unsigned num_markers_read = 0;

        
        vector<vector<double>> matrix;  
        void calculate_world_position();
        // float rotation;
        // double dx, dy, dz; 

    
};

CameraReader::CameraReader(ros::NodeHandle node, double dx, double dy, double dz, double rotation){
    //GUARDAR VARIABLES APARTE?
    // this->rotation = rotation;
    // this->dx = dx;
    // this->dy = dy;
    // this->dz = dz;
    aruco_camera_position.resize(4);
    aruco_camera_position[3] = 1;
    aruco_world_position.resize(4);

    matrix.resize(4);
    //double rotation_rad = rotation * 180 / M_PI;

    matrix[0] = {cos(M_PI * rotation),    -sin(M_PI * rotation),    0.0f,           dx};
    matrix[1] = {sin(M_PI * rotation),    cos(M_PI * rotation),     0.0f,           dy};
    matrix[2] = {0.0f,                    0.0f,                     1.0f,           dz};
    matrix[3] = {0.0f,                    0.0f,                     0.0f,           1.0f};

    n = node;
}

void CameraReader::calculate_world_position(){
    int i, k;
    aruco_world_position.clear();
    aruco_world_position[0] = 0;
    aruco_world_position[1] = 0;
    aruco_world_position[2] = 0;
    aruco_world_position[3] = 1;

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

	for(i = 0; i < matrix.size(); ++i)
	{
        for(k=0; k < matrix.size(); ++k)
        {
            aruco_world_position[i] += matrix[i][k] * aruco_camera_position[k];
        }
	}

}

void CameraReader::callback(const marker_msgs::MarkerDetection& msg) 
{
    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    num_markers_read = 0;

    aruco_camera_position[0] = 0;
    aruco_camera_position[1] = 0;
    aruco_camera_position[2] = 0;
    aruco_camera_position[3] = 1;

    for (auto &marker:msg.markers) {

        cout << "ID MARKER:" << marker.ids.front() << endl;
        
        aruco_camera_position[0] += marker.pose.position.x;
        aruco_camera_position[1] += marker.pose.position.y;
        aruco_camera_position[2] += marker.pose.position.z;

        num_markers_read++;


        // cout << "POSICION CAMARA:\n";
        // for (int i = 0; i< 3; i++){
        //     cout << aruco_camera_position[i] << "  ";
        // }

    }

    aruco_camera_position[0] /= num_markers_read;
    aruco_camera_position[1] /= num_markers_read;
    aruco_camera_position[2] /= num_markers_read;

    calculate_world_position();

    //cout << endl;
    cout << "POSICION WORLD:\n";
    for (int i = 0; i< 3; i++){
        cout << aruco_world_position[i] << "  ";
    }
    cout << endl;

    // aruco_pose_position_x = msg.pose.position.x;
    // aruco_pose_position_y = msg.pose.position.y;
    // aruco_pose_position_z = msg.pose.position.z;
    // aruco_pose_orientation_x = msg.pose.orientation.x;
    // aruco_pose_orientation_y = msg.pose.orientation.y;
    // aruco_pose_orientation_z = msg.pose.orientation.z;
    // aruco_pose_orientation_w = msg.pose.orientation.w;

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

    
    ros::init(argc, argv, "release_camera");
    ros::NodeHandle n;
    ros::Rate r(1);   

    double dx_camera = 0.0;
    double dy_camera = 0.0;
    double dz_camera = 0.0;
    double rotation_camera = 0.0;

    n.getParam("dx_camera", dx_camera);
    n.getParam("dy_camera", dy_camera);
    n.getParam("dz_camera", dz_camera);
    n.getParam("rotation_camera", rotation_camera);

    CameraReader camera(n, dx_camera, dy_camera, dz_camera, rotation_camera);

    //marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
    ros::Subscriber sub = n.subscribe("/cam/1/tracker/markers", 1, &CameraReader::callback, &camera);
    
    ros::spin();
    // while(ros::ok())
//     listener.send_marker();
    return 0;
}