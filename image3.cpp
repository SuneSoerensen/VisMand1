#include "functions.cpp"

int main()
{
    Mat img3 = imread("../Images/Image3.png", IMREAD_GRAYSCALE);
    
    //Analysis("3", img3, false); //Not needed, for now at least

    Mat filtered;
    harmonicMeanFilter(img3, filtered, 3, 3);
    
    waitKey(0);
    return 0;
}
