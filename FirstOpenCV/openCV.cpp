//
//  openCV.cpp
//  FirstOpenCV
//
//  Created by Stan on 2023/8/12.
//

#include "openCV.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>



using namespace std;
using namespace cv;



const string PATH_IMAGE = "/Users/stan/Desktop/a.png";


Point2f findCorrespondingFisheyePoint(int Xe, int Ye, int We, int He, float FOV){
    Point2f fisheyePoint;
    float theta, phi, r;
    Point3f sphericalPoint;

    theta = CV_PI * (Xe / ( (float) We ) - 0.5);
    phi = CV_PI * (Ye / ( (float) He ) - 0.5);

    sphericalPoint.x = cos(phi) * sin(theta);
    sphericalPoint.y = cos(phi) * cos(theta);
    sphericalPoint.z = sin(phi);

    theta = atan2(sphericalPoint.z, sphericalPoint.x);
    phi = atan2(sqrt(pow(sphericalPoint.x,2) + pow(sphericalPoint.z,2)), sphericalPoint.y);
    r = ( (float) We ) * phi / FOV;

    fisheyePoint.x = (int) ( 0.5 * ( (float) We ) + r * cos(theta) );
    fisheyePoint.y = (int) ( 0.5 * ( (float) He ) + r * sin(theta) );

    return fisheyePoint;
}

void dddd() {

    Mat fisheyeImage, equirectangularImage;
    int Wf, Hf;
    float FOV;
    int We, He;

    
    
    fisheyeImage = imread(PATH_IMAGE, IMREAD_COLOR);

    Wf = fisheyeImage.size().width;
    Hf = fisheyeImage.size().height;
    FOV = (180 * CV_PI ) / 180;

    We = Wf;
    He = Hf;


    equirectangularImage.create(He, We, CV_8UC3);

    for (int Xe = 0; Xe < We; Xe++){

        for (int Ye = 0; Ye < He; Ye++){

            Point2f fisheyePoint = findCorrespondingFisheyePoint(Xe, Ye, We, He, FOV);

            if (fisheyePoint.x >= We || fisheyePoint.y >= He)

                continue;

            if (fisheyePoint.x < 0 || fisheyePoint.y < 0)
                continue;

                equirectangularImage.at<Vec3b>(Point(Xe, Ye)) = fisheyeImage.at<Vec3b>(fisheyePoint);

        }

    }

    imwrite("/Users/stan/Desktop/b.png", equirectangularImage);

}



using namespace cv;
using namespace std;


const double PI = 3.141592653589793;


//Find the corresponding fisheye outpout point corresponding to an input cartesian point
Point2f findFisheye(int Xe, int Ye, double R, double Cfx, double Cfy, double He, double We){
    Point2f fisheyePoint;
    double theta, r, Xf, Yf; //Polar coordinates

    r = Ye/He*R;
    theta = Xe/We*2.0*PI;
    Xf = Cfx+r*sin(theta);
    Yf = Cfy+r*cos(theta);
    fisheyePoint.x = Xf;
    fisheyePoint.y = Yf;

    return fisheyePoint;
}

void cc(){

    Mat fisheyeImage, equirectangularImage;
    fisheyeImage = imread(PATH_IMAGE, IMREAD_COLOR);

    float Hf, Wf, He, We;
    double R, Cfx, Cfy;

    Hf = fisheyeImage.size().height;
    Wf = fisheyeImage.size().width;
    R = Hf/2.0; //The fisheye image is a square of 1400x1400 pixels containing a circle so the radius is half of the width or height size
    Cfx = Wf/2.0; //The fisheye image is a square so the center in x is located at half the distance of the width
    Cfy = Hf/2.0; //The fisheye image is a square so the center in y is located at half the distance of the height

    He = (int)R * 1.5;
    We = (int)2*PI*R;

    equirectangularImage.create(He, We, fisheyeImage.type());

    for (int Xe = 0; Xe <equirectangularImage.size().width; Xe++){
        for (int Ye = 0; Ye <equirectangularImage.size().height; Ye++){
            equirectangularImage.at<Vec3b>(Point(Xe, Ye)) =    fisheyeImage.at<Vec3b>(findFisheye(Xe, Ye, R, Cfx, Cfy, He, We));
        }
    }


    imwrite("/Users/stan/Desktop/cccc.png", equirectangularImage);
}
