#include "functions.cpp"

int main()
{
    Mat img3 = imread("../Images/Image3.png", IMREAD_GRAYSCALE);
    
    //Analysis("3", img3, false); //Not needed, for now at least

    //Apply filter:
    Mat filtered(img3.size(), CV_8U);
    harmonicMeanFilter(img3, filtered, 7, 7);

    //Show result:
    namedWindow("Filtered image", WINDOW_NORMAL);
    imshow("Filtered image", filtered);
    resizeWindow("Filtered image", Size(WIDTH, HEIGHT));
    
    waitKey(0);
    return 0;
}
