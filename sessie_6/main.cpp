#include <iostream>
#include <opencv2/opencv.hpp>   //`pkg-config --libs opencv`

using namespace std;
using namespace cv;

void detectFace(Mat frame);

CascadeClassifier face_cascade, face_lbp;

int main(int argc, const char**argv)
{
    CommandLineParser parser(argc, argv,
    "{ help h || show this message }"
    "{ @movie m || path to movie }"
    );

    string movPath = parser.get<string>("@movie");
    string cascadePath = "/home/student/Github/2018_beeldinterpretatie_Maxim_Aelterman/sessie_6/bin/Debug/haarcascade_frontalface_alt.xml";
    string lbpPath = "/home/student/Github/2018_beeldinterpretatie_Maxim_Aelterman/sessie_6/bin/Debug/lbpcascade_frontalface_improved.xml";

    if(movPath.empty()){
        cerr << "Something went wrong with your arguments: --h for help" << endl;
        return -1;
    }

    VideoCapture mov(movPath);
    if(!mov.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    while(1)
    {
        Mat frame;
        mov >> frame;

        if(frame.empty()) break;

        face_cascade.load(cascadePath);
        face_lbp.load(lbpPath);
        detectFace(frame);

            //exit when esc is pressed
        char c=(char)waitKey(25);
        if(c==27) break;
    }

    mov.release();
    destroyAllWindows();
}

void detectFace(Mat frame)
{
    vector<Rect> faces_cascade, faces_lbp;
    Mat frame_gray;
    vector<int> cascade_rej, lbp_rej;
    vector<double> cascade_weight, lbp_weight;
    bool cascade_outpRej, lbp_outputRej;

    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

      //detect faces with HAAR
    face_cascade.detectMultiScale(frame_gray, faces_cascade, cascade_rej, cascade_weight, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30), true);

    face_lbp.detectMultiScale(frame_gray, faces_lbp, lbp_rej, lbp_weight, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30), true);

    for(size_t i = 0; i < faces_cascade.size(); i++)
    {
        Point center(faces_cascade[i].x + faces_cascade[i].width*0.5, faces_cascade[i].y + faces_cascade[i].height*0.5);
        ellipse(frame, center, Size( faces_cascade[i].width*0.5, faces_cascade[i].height*0.5), 0, 0, 360, Scalar(0, 255, 0), 3, LINE_8, 0);
        putText(frame, "test", cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);

        Mat faceROI = frame_gray(faces_cascade[i]);
    }

    for(size_t i = 0; i < faces_lbp.size(); i++)
    {
        Point center(faces_lbp[i].x + faces_lbp[i].width*0.5, faces_lbp[i].y + faces_lbp[i].height*0.5);
        ellipse(frame, center, Size( faces_lbp[i].width*0.5, faces_lbp[i].height*0.5), 0, 0, 360, Scalar(0, 0, 255), 3, LINE_8, 0);

        Mat faceROI = frame_gray(faces_lbp[i]);
    }

    imshow("Faces", frame);
    return;
}
