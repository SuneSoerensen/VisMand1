#include "functions.cpp"

int main()
{
    Mat img2 = imread("../Images/Image2.png", IMREAD_GRAYSCALE);
    
    Analysis("2", img2, false);
    
    waitKey(0);
    return 0;
}
