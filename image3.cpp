#include "functions.cpp"

int main()
{
    Mat img3 = imread("../Images/Image3.png", IMREAD_GRAYSCALE);
    
    Analysis("3(0)", img3, true);

    //Apply filter:
    Mat filtered(img3.size(), CV_8U);
    //harmonicMeanFilter(img3, filtered, 3, 3); //Not the best
    midtPointFilter(img3, filtered, 3, 3);
    Analysis("3(1)", filtered, true);
    
    medianBlur(filtered, filtered, 3);
    Analysis("3(2)", filtered, true);
    
    //intensity transform
    filtered -= 25;
    //equalizeHist(filtered, filtered);
    
    //medianBlur(filtered, filtered, 3);
    
    //sharpening
    Mat sharpened(filtered.size(), CV_8U);
    UnsharpMasking(filtered, sharpened, 3);
    
    //equalizeHist(filtered, filtered);
    
    //medianBlur(sharpened, sharpened, 3);
    
    //Mat sharpened2(filtered.size(), CV_8U);
    //UnsharpMasking(sharpened, sharpened2, 2);

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
    
    Analysis("3(3)", sharpened, true);

    waitKey(0);
    return 0;
}
