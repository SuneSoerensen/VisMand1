#include "functions.cpp"

int main()
{
    Mat img4_1 = imread("../Images/Image4_1.png", IMREAD_GRAYSCALE);

    //Analysis("4_2", img4_1, false); //Not needed, for now

    //calc optimal size for DTF
    int height = getOptimalDFTSize((2 * img4_1.rows) - 1);
    int width = getOptimalDFTSize((2 * img4_1.cols) - 1);

    //pad image equally on all edges to make it optimally sized
    Mat imgWithPad;
    copyMakeBorder(img4_1, imgWithPad, (height - img4_1.rows) / 2, (height - img4_1.rows) / 2, (width - img4_1.cols) / 2, (width - img4_1.cols) / 2, BORDER_CONSTANT, 0);

    //Merge:
    Mat img2Channel;
    Mat temp[2] = {Mat_<float>(imgWithPad), (Mat_<float>(imgWithPad)) = 0};
    merge(temp, 2, img2Channel);

    //DFT:
    dft(img2Channel, img2Channel, DFT_COMPLEX_OUTPUT);
    dftshift(img2Channel);

    //Construct filter:
    Mat_<Vec2f> complexFilter(img2Channel.size());
    float fc = 250.0; //cutoff frequency
    float n = 2.0; //order

    for (int i = 0; i < complexFilter.rows; i++)
    {
       for (int j = 0; j < complexFilter.cols; j++)
       {
           //float dist = sqrt(pow(i - ((filter.rows - 1) / 2.0 ), 2) + pow(j - ((filter.cols - 1) / 2.0 ), 2));
           complexFilter(i, j)[0] = 1.0;
       }
    }

    //Multiply filter with image2channel:
    mulSpectrums(img2Channel, complexFilter, img2Channel, 0);
    dftshift(img2Channel);

    Mat filtered;
    idft(img2Channel, filtered, (DFT_SCALE | DFT_REAL_OUTPUT));
    filtered = Mat(filtered, Rect(Point((width - img4_1.cols) / 2, (height - img4_1.rows) / 2), img4_1.size()));

    //Visualize filter:
    Mat filter2channel[2];
    split(complexFilter, filter2channel);
    namedWindow("Filter", WINDOW_NORMAL);
    imshow("Filter", filter2channel[0]);
    resizeWindow("Filter", Size(WIDTH, HEIGHT));

    //Normalize and show final result:
    normalize(filtered, filtered, 0, 1, NORM_MINMAX);
    namedWindow("Filtered image", WINDOW_NORMAL);
    imshow("Filtered image", filtered);
    resizeWindow("Filtered image", Size(WIDTH, HEIGHT));

    //Show input image:
    namedWindow("Image 4_2", WINDOW_NORMAL);
    imshow("Image 4_2", img4_1);
    resizeWindow("Image 4_2", Size(WIDTH, HEIGHT));
    waitKey(0);

    //Show magnitude spectrum for original;
    CalcAndShowFourierMag("Mag_org", img4_1, false);

    filtered.convertTo(filtered, CV_8U, 255); //Needed to show power spectrum of filtered
    //And for filtered:
    CalcAndShowFourierMag("Mag_filtered", filtered, false);
    waitKey(0);

    return 0;
}
