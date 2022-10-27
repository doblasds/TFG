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
        }
    }else{
        fiducial_camera_pose.position.x = -99.99;
    }
