#include "functions.cpp"
#include <iostream>

using namespace std;

int main()
{
    Mat img1 = imread("../Images/Image1.png", IMREAD_GRAYSCALE);

    Analysis("1(0)", img1, true);

    Mat filtered;
    //Mat filtered2;
    adaptiveMaxFilter(img1, filtered, 9);
//    namedWindow("adaptiveMax", WINDOW_NORMAL);
//    resizeWindow("adaptiveMax", WIDTH, HEIGHT);
//    imshow("adaptiveMax",filtered);
    Analysis("1(1)", filtered, true);
    filtered += 50;
    //medianBlur(filtered, filtered, 9);
    
    //Mat sharpened;
    //UnsharpMasking(filtered, sharpened, 10);
    
    Analysis("1(2)", filtered, true);

    waitKey(0);
    return 0;
}
