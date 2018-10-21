#include "functions.cpp"

int main()
{
    Mat img2 = imread("../Images/Image2.png", IMREAD_GRAYSCALE);
    
    //Analysis("2", img2, false);

    Mat resA;
    FredsAdaptiveMedian(img2, resA, 9);
    
    Mat resB;
    FredsAdaptiveMedian(resA, resB, 9);
    
    Mat resC;
    //FredsAdaptiveMedian(resB, resC, 7);
    //equalizeHist(resB, resC);
    
    Analysis("2", resB, false);
    
    waitKey(0);
    return 0;
}
