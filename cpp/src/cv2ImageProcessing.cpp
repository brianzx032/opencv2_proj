#include "cv2ImageProcessing.h"
#include <iostream>
#include <vector>

cv2ImageProcessing::cv2ImageProcessing(void)
{
    std::cout << "Object is being created" << std::endl;
}
cv2ImageProcessing::~cv2ImageProcessing(void)
{
    std::cout << "Object is being deleted" << std::endl;
}

void cv2ImageProcessing::showInfo(CvImage& cvImg, const std::string& filename)
{
    std::cout << filename << " Info" << std::endl;
    std::cout << "Size:" << cvImg.size() << std::endl;
    std::cout << "Type:" << cvImg.type() << std::endl;
    std::cout << "Depth:" << cvImg.depth() << std::endl;
    std::cout << "Channels:" << cvImg.channels() << std::endl;
}

CvImage cv2ImageProcessing::ImRead(const std::string& filename)
{
    return cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);
}

void cv2ImageProcessing::ImWrite(const std::string& filename, CvImage& cvImg)
{
    cv::imwrite(filename, cvImg);
}

void cv2ImageProcessing::ImShow(const std::string& winname, CvImage& cvImg)
{
    cv::imshow(winname, cvImg);
}

void cv2ImageProcessing::Resize(CvImage& newImg1, CvImage& newImg2, const CvImage& SrcImg1, const CvImage& SrcImg2)
{
    int rows, cols;
    rows = std::max(SrcImg1.rows,SrcImg2.rows);
    cols = std::max(SrcImg1.cols,SrcImg2.cols);

    newImg1.create(rows, cols, CV_32FC3);
    newImg2.create(rows, cols, CV_32FC3);
    
    cv::resize(SrcImg1,newImg1,newImg1.size());
    cv::resize(SrcImg2,newImg2,newImg2.size());
    // showInfo(newImg1,"1");
    // showInfo(newImg2,"2");
}

void cv2ImageProcessing::SplitAlpha(CvImage& Foreground, CvImage& Alpha, const CvImage& SrcImg)
{
    std::vector<CvImage> channels_src, rgb_img;
    cv::split(SrcImg, channels_src);

    rgb_img.push_back(channels_src.at(0));
    rgb_img.push_back(channels_src.at(1));
    rgb_img.push_back(channels_src.at(2));

    cv::merge(rgb_img, Foreground);
    channels_src.at(3).copyTo(Alpha);
}

CvImage cv2ImageProcessing::AlphaBlend(const CvImage& Foreground, const CvImage& Background, const CvImage& Alpha)
{
    CvImage fore, back, alph, out_img;
    std::vector<CvImage> alph_mask;
    Foreground.convertTo(fore, CV_32FC3, 1.0/255);
    Background.convertTo(back, CV_32FC3, 1.0/255);
    Alpha.convertTo(alph, CV_32FC3, 1.0/255);
    
    alph_mask.push_back(alph);
    alph_mask.push_back(alph);
    alph_mask.push_back(alph);
    cv::merge(alph_mask, alph);

    out_img=CvImage::zeros(fore.size(),fore.type());
    cv::multiply(alph, fore, fore);
    cv::multiply(cv::Scalar::all(1.0)-alph, back, back);
    cv::add(fore, back, out_img);
    return out_img;
}