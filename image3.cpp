#include "functions.cpp"

int main()
{
    Mat img3 = imread("../Images/Image3.png", IMREAD_GRAYSCALE);
    
    Analysis("3", img3, false);
    
    waitKey(0);
    return 0;
}
