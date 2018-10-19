#include "functions.cpp"

int main()
{
    Mat img5 = imread("../Images/Image5_optional.png", IMREAD_GRAYSCALE);

    //Analysis("5", img5, false);

    //calc optimal size for DTF
    int height = getOptimalDFTSize((2 * img5.rows) - 1);
    int width = getOptimalDFTSize((2 * img5.cols) - 1);

    //pad image equally on all edges to make it optimally sized
    Mat imgWithPad;
    copyMakeBorder(img5, imgWithPad, (height - img5.rows) / 2, (height - img5.rows) / 2, (width - img5.cols) / 2, (width - img5.cols) / 2, BORDER_CONSTANT, 0);

    //Merge:
    Mat img2Channel;
    Mat temp[2] = {Mat_<float>(imgWithPad), (Mat_<float>(imgWithPad)) = 0};
    merge(temp, 2, img2Channel);

    //DFT:
    dft(img2Channel, img2Channel, DFT_COMPLEX_OUTPUT);
    dftshift(img2Channel);

    //build invers filter
    Mat_<Vec2f> MotionBlurFilter(img2Channel.size());
    motionBlurFilter(MotionBlurFilter,1,0.0001,0.001);

    mulSpectrums(img2Channel, MotionBlurFilter, img2Channel, 0);
    dftshift(img2Channel);

    //Multiply filter with image2channel:
    Mat filtered;
    idft(img2Channel, filtered, (DFT_SCALE | DFT_REAL_OUTPUT));
    filtered = Mat(filtered, Rect(Point((width - img5.cols) / 2, (height - img5.rows) / 2), img5.size()));

    //Visualize filter:
    Mat filter2channel[2];
    split(MotionBlurFilter, filter2channel);
    normalize(filter2channel[0], filter2channel[0], 0, 1, NORM_MINMAX);
    namedWindow("Filter", WINDOW_NORMAL);
    imshow("Filter", filter2channel[0]);
    resizeWindow("Filter", Size(WIDTH, HEIGHT));

    //Normalize and show final result:
    normalize(filtered, filtered, 0, 1, NORM_MINMAX);
    namedWindow("Filtered image", WINDOW_NORMAL);
    imshow("Filtered image", filtered);
    resizeWindow("Filtered image", Size(WIDTH, HEIGHT));

    //Show magnitude spectrum for original;
    CalcAndShowFourierMag("Mag_org", img5, false);

    filtered.convertTo(filtered, CV_8U, 255); //Needed to show power spectrum of filtered
    //And for filtered:
    CalcAndShowFourierMag("Mag_filtered", filtered, false);

    waitKey(0);
    return 0;
}
