#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/aruco.hpp>


using namespace cv;
using namespace std;

const float calibrationSquareDimension = 0.025f;
const float arucoSquareDimension = 0.097f; // meters 0.097 
const Size chessboardDimensions = Size(6,9);




void createArucoMarkers()
{
    Mat outputMarker;
    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    for (int i = 0; i<50; i++)
    {
        aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
        ostringstream convert; 
        string imageName = "4x4Marker_";
        convert << imageName << i << ".jpg";
        imwrite(convert.str(), outputMarker);
    }
}

void createKnownBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f>& corners){
    for (int i = 0;i < boardSize.height; i++)
    {
        for (int j = 0; j < boardSize.width; j++)
        {
            corners.push_back(Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
        }


    }    

}

void getChessboardCorners(vector <Mat> images, vector<vector<Point2f>>&allfoundCorners, bool showResults = false){
    for (vector <Mat>::iterator iter = images.begin(); iter != images.end(); iter++ )
    {
        vector<Point2f> pointBuf;
        bool found = findChessboardCorners(*iter, Size(9,6), pointBuf,\
        CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_NORMALIZE_IMAGE);
        if (found)
        {
            allfoundCorners.push_back(pointBuf);
        }
        if (showResults){
            drawChessboardCorners(*iter, Size(9,6), pointBuf, found);
            imshow("Looking for Corners", *iter);
            waitKey(0);

        }
    }
}

void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat & cameraMatrix, Mat& distanceCoefficients){
    vector<vector<Point2f>> checkerboardImageSpacePoints;
    getChessboardCorners(calibrationImages, checkerboardImageSpacePoints, false);
    vector<vector<Point3f>> worldSpaceCornerPoints(1);
    createKnownBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
    worldSpaceCornerPoints.resize(checkerboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);
    vector <Mat> rVectors, tVectors;
    distanceCoefficients = Mat::zeros(8, 1, CV_64F);

    calibrateCamera(worldSpaceCornerPoints, checkerboardImageSpacePoints, boardSize,\
    cameraMatrix, distanceCoefficients, rVectors,tVectors);
}


bool saveCameraCalibration( string name, Mat cameraMatrix, Mat distanceCoefficients)
{
    ofstream outStream(name);
    if (outStream)
    {
        uint16_t rows = cameraMatrix.rows;
        uint16_t columns = cameraMatrix.cols;

        outStream << rows << endl;
        outStream << columns << endl;

        for (int r = 0; r< rows; r++)
        {
            for (int c = 0; c< columns; c++)
            {
                double value = cameraMatrix.at<double>(r,c);
                outStream << value << endl;
            }
        }

        rows = distanceCoefficients.rows;
        columns = distanceCoefficients.cols;

        outStream << rows << endl;
        outStream << columns << endl;


        for (int r = 0; r< rows; r++)
        {
            for (int c = 0; c< columns; c++)
            {
                double value = cameraMatrix.at<double>(r,c);
                outStream << value << endl;
            }
        }
        outStream.close();
        return true;
    }
    return false;
}


void cameraCalibrationProcess(Mat& cameraMatrix, Mat& distanceCoefficients){

    Mat frame;
    Mat drawToFrame;

    vector<Mat> savedImages;

    vector<vector<Point2f>> markerCorners, rejectedCandidates;

    VideoCapture vid(0);

    if (!vid.isOpened())
        cout << "LA CONCHA DE LA LORA";
    
    int framesPerSecond = 20;
    namedWindow("Webcam", CV_WINDOW_AUTOSIZE);

    while (true)
    {
        if (!vid.read(frame))
            break;
        
        vector<Vec2f> foundPoints;
        bool found = false;

        found = findChessboardCorners(frame, chessboardDimensions, foundPoints,\
         CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_FAST_CHECK);

        frame.copyTo(drawToFrame);
        drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
        
        if (found)
            imshow("Webcam", drawToFrame);
        else
            imshow("Webcam", frame);
        char character = waitKey(1000 / framesPerSecond);
        switch (character)
        {
        
        case 'a':
            //saving image
            if (found)
            {
                Mat temp;
                frame.copyTo(temp);
                savedImages.push_back(temp);
            }
            break;
        case 'd':
            //start calibration
            if (savedImages.size() > 15)
            {
                cameraCalibration(savedImages, chessboardDimensions, calibrationSquareDimension,\
                cameraMatrix, distanceCoefficients);
                saveCameraCalibration("IloveCameraCalibration", cameraMatrix, distanceCoefficients);
            }   
            break;

        case ' ':
            //exit
            break;
        }
    }
}




bool loadCameraCalibration (string name, Mat& cameraMatrix, Mat& distanceCoefficients)
{
    ifstream inStream(name);
    if (inStream)
    {
        uint16_t rows = cameraMatrix.rows;
        uint16_t columns= cameraMatrix.cols;

        inStream >> rows;
        inStream >> columns;

        cameraMatrix = Mat (Size(columns, rows), CV_64F);
        for (int r=0; r < rows; r++)
        {
            for (int c=0; c < columns; c++)
            {
                double read = 0.0f;
                inStream >> read;
                cameraMatrix.at<double>(r,c) = read;
                cout << cameraMatrix.at<double>(r,c) << "\n";

            }
        }

        //Distance coefficients

        inStream >> rows;
        inStream >> columns;

        distanceCoefficients = Mat::zeros(rows, columns, CV_64F);

        for (int r=0; r < rows; r++)
        {
            for (int c=0; c < columns; c++)
            {
                double read = 0.0f;
                inStream >> read;
                distanceCoefficients.at<double>(r, c) = read;
                cout << distanceCoefficients.at<double>(r, c) << "\n";
            }
        }
        inStream.close();
        return true;
    }
    return false;
}


int startWebcamMonitoring(const Mat& cameraMatrix, const Mat& distanceCoefficients, float arucoSquareDimensions)
{
    Mat frame;

    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCandidates;
    aruco::DetectorParameters parameters;
    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    VideoCapture vid(0); 

    if (!vid.isOpened())
    {
        return -1;
    }
    namedWindow("Webcam", CV_WINDOW_AUTOSIZE);
    vector<Vec3d> rotationVectors, translationVectors;

    while(true)
    {
        if (!vid.read(frame)){
            break;
        }
            

        aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);
        aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimensions, cameraMatrix,\
        distanceCoefficients, rotationVectors, translationVectors);

        for (int i=0; i<markerIds.size(); i++)
        {
            aruco::drawAxis(frame, cameraMatrix, distanceCoefficients, rotationVectors[i], translationVectors[i], 0.1f);
            cout << translationVectors[i][0] << ", " << translationVectors[i][1] << ", " << translationVectors[i][2] << "\n";
        }
        imshow("Webcam", frame);
        waitKey(30);
    }
    return 1;
}  


int main(int argc, char** argv) {
    
    //createArucoMarkers();

    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
    Mat distanceCoefficients;

    // cameraCalibrationProcess(cameraMatrix, distanceCoefficients); 
    bool done = loadCameraCalibration("IloveCameraCalibration", cameraMatrix, distanceCoefficients);
    if (done)
        cout << "\n LETS GO\n" ;


    startWebcamMonitoring(cameraMatrix, distanceCoefficients, arucoSquareDimension);

}