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
    medianBlur(resB, resB, 9);
    
    Mat resC = resB;
//    Mat resC(img2.size(), CV_8U);
//    midtPointFilter(resB, resC, 5, 5);
    
    //laplacian
    Mat laplace;
    Laplacian(resC, laplace, CV_16S, 1, 3, 0, BORDER_DEFAULT);
    //resB += laplace;
    //medianBlur(laplace, laplace, 5);
    Mat laplaceVis = laplace + 128;
    //convertScaleAbs(laplace, laplace);
    
//    Mat cvtLaplace;
    convertScaleAbs(laplaceVis, laplaceVis);
//    medianBlur(cvtLaplace, cvtLaplace, 5);
    namedWindow("laplace", WINDOW_NORMAL);
    imshow("laplace", laplaceVis);
    
    //sobel
    Mat grad_x, grad_y;
    //Mat abs_grad_x, abs_grad_y;
    
    Sobel(resC, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    Sobel(resC, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    
//    convertScaleAbs(grad_x, abs_grad_x);
//    convertScaleAbs(grad_y, abs_grad_y);
    
    Mat grad;
    addWeighted(grad_x, 0.5, grad_y, 0.5, 0, grad);
    //medianBlur(grad, grad, 5);
    GaussianBlur(grad, grad, Size(5, 5), 0, 0);
    
    Mat gradVis = grad + 128;
    convertScaleAbs(gradVis, gradVis);
    namedWindow("sobel", WINDOW_NORMAL);
    imshow("sobel", gradVis);
    
    for (int i = 0; i < resC.rows; i++)
    {
        for (int j = 0; j < resC.cols; j++)
        {
            resC.at<uchar>(i, j) = resC.at<uchar>(i, j) + laplace.at<uchar>(i, j) * grad.at<uchar>(i, j);
        }
    }
    
//    convertScaleAbs(sharpMask, sharpMask);
//    namedWindow("mask", WINDOW_NORMAL);
//    imshow("mask", sharpMask);
    
    //resB += sharpMask;
    
    //equalizeHist(resB, resC);
    //UnsharpMasking(resB, resC, 2);
    //medianBlur(resA, resC, 3);
    
    Mat resD(img2.size(), CV_8U);
    //midtPointFilter(resC, resD, 3, 3);
    //UnsharpMasking(resC, resD, 4);
    
    Mat resF(img2.size(), CV_8U);
    //midtPointFilter(resD, resF, 5, 5);
    //UnsharpMasking(resB, resF, 2);
    
    Analysis("2", resC, true);
    
    waitKey(0);
    return 0;
}
