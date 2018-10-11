#include "functions.cpp"

int main()
{
    Mat img1 = imread("../Images/Image1.png", IMREAD_GRAYSCALE);
    
    Analysis("1", img1, false);
    
    /*Mat filtered;
    MedianFilter(img1, filtered, 5);
    Mat filtered2;
    MedianFilter(filtered, filtered2, 5);
    namedWindow("Filtered image", WINDOW_NORMAL);
    resizeWindow("Filtered image", WIDTH, HEIGHT);
    imshow("Filtered image", filtered2);*/
    
    
    waitKey(0);
    return 0;
}
