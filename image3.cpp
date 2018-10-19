#include "functions.cpp"

int main()
{
    Mat img3 = imread("../Images/Image3.png", IMREAD_GRAYSCALE);
    
    //Analysis("3", img3, false);

    //Apply filter:
    Mat filtered(img3.size(), CV_8U);
    //harmonicMeanFilter(img3, filtered, 3, 3); //Not the best
    midtPointFilter(img3, filtered, 3, 3);

//    //Show result of first filtering:
//    namedWindow("Filtered once", WINDOW_NORMAL);
//    imshow("Filtered once", filtered);
//    resizeWindow("Filtered once", Size(WIDTH, HEIGHT));
    
//    //Show original:
//    namedWindow("Original image", WINDOW_NORMAL);
//    imshow("Original image", img3);
//    resizeWindow("Original image", Size(WIDTH, HEIGHT));

//    //Show hist. of filtered:
//    CalcAndShowHist("Histogram of filtered", filtered, false);

//    //ROI-hist of filtered:
//    Rect rect(825, 1424, 675, 384); //(corner_x, corner_y, length_x, length_y)
//    Mat roi = filtered(rect);
//    CalcAndShowHist("ROI-filtered", roi, false);
    
    Analysis("3", filtered, true);

    waitKey(0);
    return 0;
}
