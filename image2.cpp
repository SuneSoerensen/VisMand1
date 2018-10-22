#include "functions.cpp"

int main()
{
    Mat img2 = imread("../Images/Image2.png", IMREAD_GRAYSCALE);
    
    //Analysis("2", img2, false);

    Mat resA;
    FredsAdaptiveMedian(img2, resA, 9);
    
    Mat resB;
    FredsAdaptiveMedian(resA, resB, 9);
    
    //equalizeHist(resB, resB);
    medianBlur(resB, resB, 3);
    
    Mat resC(img2.size(), CV_8U);
    //midtPointFilter(resB, resC, 3, 3);
    //equalizeHist(resB, resC);
    //UnsharpMasking(resB, resC, 2);
    //medianBlur(resA, resC, 3);
    
    Mat resD(img2.size(), CV_8U);
    //midtPointFilter(resC, resD, 3, 3);
    //UnsharpMasking(resC, resD, 4);
    
    Mat resF(img2.size(), CV_8U);
    //midtPointFilter(resD, resF, 5, 5);
    //UnsharpMasking(resB, resF, 2);
    
    Analysis("2", resB, true);
    
    waitKey(0);
    return 0;
}
