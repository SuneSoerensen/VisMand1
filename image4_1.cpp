#include "functions.cpp"

int main()
{
    Mat img4_1 = imread("../Images/Image4_1.png", IMREAD_GRAYSCALE);
    
    //Analysis("4_1", img4_1, false);

    //-----------
    // Filtering
    //-----------
    //calc optimal size
    int height = getOptimalDFTSize((2 * img4_1.rows) - 1);
    int width = getOptimalDFTSize((2 * img4_1.cols) - 1);

    //pad image to make it optimally sized
    Mat imgWithPad;
    copyMakeBorder(img4_1, imgWithPad, 0, height - img4_1.rows, 0, width - img4_1.cols, BORDER_CONSTANT, 0);

    //merge
    Mat img2Channel;
    Mat temp[2] = {Mat_<float>(imgWithPad), (Mat_<float>(imgWithPad)) = 0};
    merge(temp, 2, img2Channel);

    //dft
    dft(img2Channel, img2Channel, DFT_COMPLEX_OUTPUT);

    //create filter
    Mat_<float> filter(height, width);
    float fc = 250.0; //cutoff frequency
    float n = 2.0; //order

    for (int i = 0; i < filter.rows; i++)
    {
        for (int j = 0; j < filter.cols; j++)
        {
            float dist = sqrt(pow(i - ((filter.rows - 1) / 2.0 ), 2) + pow(j - ((filter.cols - 1) / 2.0 ), 2));
            filter.at<float>(i, j) = 1.0;
        }
    }

    Mat complexFilter;
    Mat temp2[2] = {Mat_<float>(filter), (Mat_<float>(filter)) = 0};
    merge(temp2, 2, complexFilter);

    //shift the filter quadrants
    dftshift(complexFilter);

    //multiply in frequency domain to apply filter
    Mat resFreq = img2Channel.clone();
    mulSpectrums(img2Channel, complexFilter, resFreq, 0, false);

    //inverse Fourier transform
    Mat res;
    dft(resFreq, res, DFT_INVERSE + DFT_SCALE + DFT_REAL_OUTPUT);

    namedWindow("res", WINDOW_NORMAL);
    imshow("res", res);

    //crop to remove padding
    res = Mat(res, Rect(0, 0, img4_1.cols, img4_1.rows)); //(corner_x, corner_y, length_x, length_y)

    //save
    res.copyTo(img4_1);

    Analysis("filtered", img4_1, false);

    waitKey(0);
    return 0;
}
