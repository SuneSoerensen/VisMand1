#include "functions.cpp"

int main()
{
    Mat img4_2 = imread("../Images/Image4_2.png", IMREAD_GRAYSCALE);
    
    //initial analysis
    //Analysis("4_2", img4_2, false);

//    //calc optimal size for DTF
//    int height = getOptimalDFTSize((2 * img4_2.rows) - 1);
//    int width = getOptimalDFTSize((2 * img4_2.cols) - 1);

//    //pad image equally on all edges to make it optimally sized
//    Mat imgWithPad;
//    copyMakeBorder(img4_2, imgWithPad, (height - img4_2.rows) / 2, (height - img4_2.rows) / 2, (width - img4_2.cols) / 2, (width - img4_2.cols) / 2, BORDER_CONSTANT, 0);

//    //Merge:
//    Mat img2Channel;
//    Mat temp[2] = {Mat_<float>(imgWithPad), (Mat_<float>(imgWithPad)) = 0};
//    merge(temp, 2, img2Channel);

//    //DFT:
//    dft(img2Channel, img2Channel, DFT_COMPLEX_OUTPUT);
//    dftshift(img2Channel);

//    //Construct gaussian bandreject filter:
//    Mat_<Vec2f> gaussBandReject(img2Channel.size());
//    gaussianBandReject(gaussBandReject, 300, 820);

//    //Multiply filter with image2channel:
//    mulSpectrums(img2Channel, gaussBandReject, img2Channel, 0);
//    dftshift(img2Channel);

//    Mat filtered;
//    idft(img2Channel, filtered, (DFT_SCALE | DFT_REAL_OUTPUT));
//    filtered = Mat(filtered, Rect(Point((width - img4_2.cols) / 2, (height - img4_2.rows) / 2), img4_2.size()));

//    //Visualize filter:
//    Mat filter2channel[2];
//    split(gaussBandReject, filter2channel);
//    normalize(filter2channel[0], filter2channel[0], 0, 1, NORM_MINMAX);
//    namedWindow("Filter", WINDOW_NORMAL);
//    imshow("Filter", filter2channel[0]);
//    resizeWindow("Filter", Size(WIDTH, HEIGHT));

//    //Normalize and show final result:
//    normalize(filtered, filtered, 0, 1, NORM_MINMAX);
//    namedWindow("Filtered image", WINDOW_NORMAL);
//    imshow("Filtered image", filtered);
//    resizeWindow("Filtered image", Size(WIDTH, HEIGHT));

//    //Show input image:
//    namedWindow("Image 4_2", WINDOW_NORMAL);
//    imshow("Image 4_2", img4_2);
//    resizeWindow("Image 4_2", Size(WIDTH, HEIGHT));
//    waitKey(0);

//    //Show magnitude spectrum for original;
//    CalcAndShowFourierMag("Mag_org", img4_2, false);

//    filtered.convertTo(filtered, CV_8U, 255); //Needed to show power spectrum of filtered
//    //And for filtered:
//    CalcAndShowFourierMag("Mag_filtered", filtered, false);
//    waitKey(0);
    
    //-----------
    // Filtering
    //-----------
    //calc optimal size for DTF
    int height = getOptimalDFTSize((2 * img4_2.rows) - 1);
    int width = getOptimalDFTSize((2 * img4_2.cols) - 1);

    //pad image equally on all edges to make it optimally sized
    Mat imgWithPad;
    copyMakeBorder(img4_2, imgWithPad, (height - img4_2.rows) / 2, (height - img4_2.rows) / 2, (width - img4_2.cols) / 2, (width - img4_2.cols) / 2, BORDER_CONSTANT, 0);

    //Merge:
    Mat img2Channel;
    Mat temp[2] = {Mat_<float>(imgWithPad), (Mat_<float>(imgWithPad)) = 0};
    merge(temp, 2, img2Channel);

    //DFT:
    dft(img2Channel, img2Channel, DFT_COMPLEX_OUTPUT);
    dftshift(img2Channel);

    //Construct gaussian bandreject filter:
    //Mat_<Vec2f> gaussBandReject(img2Channel.size());
    //gaussianBandReject(gaussBandReject, 300, 820);
    //Mat filter = gaussBandReject;
    
    //construct Notch filter
    int order = 1;
    int cutoffFreq = 50;
    
    Mat_<Vec2f> filter(img2Channel.size());
    Mat_<Vec2f> tempFilter(img2Channel.size());

    //from the top, clockwise...
    ButterworthHighpass(filter, 1571, 1536, order, cutoffFreq); //...first pair
    
    ButterworthHighpass(tempFilter, height - 1571, width - 1536, order, cutoffFreq);
    mulSpectrums(filter, tempFilter, filter, 0);
    
    ButterworthHighpass(tempFilter, 1815, 2104, order, cutoffFreq); //...second pair
    mulSpectrums(filter, tempFilter, filter, 0);
    
    ButterworthHighpass(tempFilter, height - 1815, width - 2104, order, cutoffFreq);
    mulSpectrums(filter, tempFilter, filter, 0);
    
    ButterworthHighpass(tempFilter, 2401, 2342, order, cutoffFreq); //...third pair
    mulSpectrums(filter, tempFilter, filter, 0);
    
    ButterworthHighpass(tempFilter, height - 2401, width - 2342, order, cutoffFreq);
    mulSpectrums(filter, tempFilter, filter, 0);
    
    ButterworthHighpass(tempFilter, 2986, 2104, order, cutoffFreq); //...fourth pair
    mulSpectrums(filter, tempFilter, filter, 0);
    
    ButterworthHighpass(tempFilter, height - 2986, width - 2104, order, cutoffFreq);
    mulSpectrums(filter, tempFilter, filter, 0);
    
    //Visualize filter (for debug purposses)
    Mat filter2channel[2];
    split(filter, filter2channel);
    namedWindow("Filter", WINDOW_NORMAL);
    resizeWindow("Filter", Size(WIDTH, HEIGHT));
    imshow("Filter", filter2channel[0]);
    
    //multiply in frequency domain to apply filter
    mulSpectrums(img2Channel, filter, img2Channel, 0);
    dftshift(img2Channel);

    //crop to remove padding
    Mat filtered;
    idft(img2Channel, filtered, (DFT_SCALE | DFT_REAL_OUTPUT));
    filtered = Mat(filtered, Rect(Point((width - img4_2.cols) / 2, (height - img4_2.rows) / 2), img4_2.size()));
    
    //convert
    normalize(filtered, filtered, 0, 1, NORM_MINMAX);
    filtered.convertTo(filtered, CV_8U, 255);

    //2nd analysis
    Analysis("4_2", filtered, true);
    
    waitKey(0);
    return 0;
}
