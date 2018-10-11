#include "functions.cpp"

int main()
{
    Mat img5 = imread("../Images/Image5_optional.png", IMREAD_GRAYSCALE);
    
    Analysis("5", img5, false);

    waitKey(0);
    return 0;
}
