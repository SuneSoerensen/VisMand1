#include "functions.cpp"

int main()
{
    Mat img4_1 = imread("../Images/Image4_1.png", IMREAD_GRAYSCALE);

    //initial analysis
    //Analysis("4_1", img4_1, false);

    //-----------
    // Filtering
    //-----------
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

    //Construct filter (Notch)
    int order = 1;
    int cutoffFreq = 42;
    
    Mat_<Vec2f> complexFilter(img2Channel.size());
    Mat_<Vec2f> tempFilter(img2Channel.size());

    ButterworthHighpass(complexFilter, 1778, 2140, order, cutoffFreq);
    
    ButterworthHighpass(tempFilter, height - 1778, width - 2140, order, cutoffFreq);
    mulSpectrums(complexFilter, tempFilter, complexFilter, 0);
    
    ButterworthHighpass(tempFilter, 2607, 1737, order, cutoffFreq);
    mulSpectrums(complexFilter, tempFilter, complexFilter, 0);
    
    ButterworthHighpass(tempFilter, height - 2607, width - 1737, order, cutoffFreq);
    mulSpectrums(complexFilter, tempFilter, complexFilter, 0);
    
//    ButterworthLowpass(tempFilter, height / 2, width / 2, 1, 1500); //test to see if removing the small dots at high frequencies does anything signifigant
//    mulSpectrums(complexFilter, tempFilter, complexFilter, 0);
    
    //Visualize filter (for debug purposses)
    Mat filter2channel[2];
    split(complexFilter, filter2channel);
    namedWindow("Filter", WINDOW_NORMAL);
    resizeWindow("Filter", Size(WIDTH, HEIGHT));
    imshow("Filter", filter2channel[0]);
    
    //multiply in frequency domain to apply filter
    mulSpectrums(img2Channel, complexFilter, img2Channel, 0);
    dftshift(img2Channel);

    //crop to remove padding
    Mat filtered;
    idft(img2Channel, filtered, (DFT_SCALE | DFT_REAL_OUTPUT));
    filtered = Mat(filtered, Rect(Point((width - img4_1.cols) / 2, (height - img4_1.rows) / 2), img4_1.size()));
    
    //convert
    normalize(filtered, filtered, 0, 1, NORM_MINMAX);
    filtered.convertTo(filtered, CV_8U, 255);

    //2nd analysis
    Analysis("4_1", filtered, true);
    
    waitKey(0);
    return 0;
}
