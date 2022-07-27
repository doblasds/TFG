#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/aruco.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    
    Mat outputMarker;
    Ptr<aruco::Dictionary> markerDictionary = \
    aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    for (int i = 0; i<50; i++)
    {
        aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
        ostringstream convert; 
        string imageName = "4x4Marker_";
        convert << imageName << i << ".jpg";
        imwrite(convert.str(), outputMarker);
    }

}