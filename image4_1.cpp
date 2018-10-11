#include "functions.cpp"

int main()
{
    Mat img4_1 = imread("../Images/Image4_1.png", IMREAD_GRAYSCALE);
    
    Analysis("4_1", img4_1, false);

    waitKey(0);
    return 0;
}
