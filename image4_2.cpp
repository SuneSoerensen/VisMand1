#include "functions.cpp"

int main()
{
    Mat img4_2 = imread("../Images/Image4_2.png", IMREAD_GRAYSCALE);
    
    Analysis("4_2", img4_2, false);
    
    waitKey(0);
    return 0;
}
