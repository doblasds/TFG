#include <ros/ros.h>
#include "tuw_aruco/aruco_node.h"
#include <vector>
#include <cmath>
#include <geometry_msgs/PoseStamped.h>


using namespace std;
ros::Publisher marker_pub;

class CameraReader 
{
    public:

        ros::NodeHandle n;

        
        // double aruco_pose_position_x = 0;
        // double aruco_pose_position_y = 0;
        // double aruco_pose_position_z = 0;

        CameraReader(ros::NodeHandle node, double dx, double dy, double dz, double rotation = 0);
        void callback(const marker_msgs::MarkerDetection& msg);
        void send_result();

    private:
        vector <double> aruco_camera_position;
        vector <double> aruco_world_position;
        unsigned num_markers_read = 0;

        double aruco_pose_orientation_x = 0.0;
        double aruco_pose_orientation_y = 0.0;
        double aruco_pose_orientation_z = 0.0;
        double aruco_pose_orientation_w = 1.0;


        
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

	for(unsigned i = 0; i < matrix.size(); ++i)
	{
        for(unsigned k = 0; k < matrix.size(); ++k)
        {
            aruco_world_position[i] += matrix[i][k] * aruco_camera_position[k];
        }
	}

}

void CameraReader::send_result(){
    geometry_msgs::PoseStamped position;

    position.pose.position.x = aruco_world_position[0];
    position.pose.position.y = aruco_world_position[1];
    position.pose.position.z = aruco_world_position[2];

    position.pose.orientation.x = aruco_pose_orientation_x;
    position.pose.orientation.y = aruco_pose_orientation_y;
    position.pose.orientation.z = aruco_pose_orientation_z;
    position.pose.orientation.w = aruco_pose_orientation_w;

    marker_pub.publish(position);
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
        //cout << "ID MARKER:" << marker.ids.front() << endl;
        
        aruco_camera_position[0] += marker.pose.position.x;
        aruco_camera_position[1] += marker.pose.position.y;
        aruco_camera_position[2] += marker.pose.position.z;

        num_markers_read++;
        // cout << "POSICION CAMARA:\n";
        // for (int i = 0; i< 3; i++){
        //     cout << aruco_camera_position[i] << "  ";
        // }

    }
    if (num_markers_read > 0){
        aruco_camera_position[0] /= num_markers_read;
        aruco_camera_position[1] /= num_markers_read;
        aruco_camera_position[2] /= num_markers_read;

        calculate_world_position();
    }else{
        aruco_world_position[0] = -99.99;
    }


    

    // if (num_markers_read > 0){
    //     //cout << "POSICION CAMARA:\n";
    //     cout << "POSICION WORLD:\n";
    //     for (int i = 0; i< 3; i++){
    //         cout << aruco_world_position[i] << "  ";
    //     }
    //     cout << endl;

    // }



    //No se que hacer

    // aruco_pose_orientation_x = msg.pose.orientation.x;
    // aruco_pose_orientation_y = msg.pose.orientation.y;
    // aruco_pose_orientation_z = msg.pose.orientation.z;
    // aruco_pose_orientation_w = msg.pose.orientation.w;

    send_result();
}




int main( int argc, char** argv )
{

    ros::init(argc, argv, "release_camera");
    ros::NodeHandle n;
    ros::Rate r(1);   

    string camera_id = ros::this_node::getName();
    double dx_camera = 0.0;
    double dy_camera = 0.0;
    double dz_camera = 0.0;
    double rotation_camera = 0.0;

    n.getParam(camera_id + "/dx_camera", dx_camera);
    n.getParam(camera_id + "/dy_camera", dy_camera);
    n.getParam(camera_id + "/dz_camera", dz_camera);
    n.getParam(camera_id + "/rotation_camera", rotation_camera);


    //cout << "dx_cam: " <<  dx_camera << endl;
    // cout << "dy_cam: " <<  dy_camera << endl;
    // cout << "dz_cam: " <<  dz_camera << endl;
    // cout << "rotation_cam: " <<  rotation_camera << endl;

    CameraReader camera(n, dx_camera, dy_camera, dz_camera, rotation_camera);

    marker_pub = n.advertise<geometry_msgs::PoseStamped>(camera_id + "/result", 1);
    ros::Subscriber sub = n.subscribe(camera_id + "/tracker/markers", 1, &CameraReader::callback, &camera);
    
    ros::spin();
    // while(ros::ok())
//     listener.send_marker();
    return 0;
}


