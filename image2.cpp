#include "functions.cpp"

int main()
{
    Mat img2 = imread("../Images/Image2.png", IMREAD_GRAYSCALE);
    
    //Analysis("2", img2, false);

    Mat resA;
    FredsAdaptiveMedian(img2, resA, 9);
    Analysis("2", resA, true);
    
    waitKey(0);
    return 0;
}
