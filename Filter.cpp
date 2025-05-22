#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {

    GrayscaleImage CopyImage=image;//Create a copy of original image
    int rad=kernelSize/2;//Calculate a kernel radius
    int width=image.get_width();
    int height=image.get_height();

    for(int j=0;j<height;++j){
        for(int i=0;i<width;++i){
            int sum=0;
            int count=0;
            for(int kernel_j=-rad;kernel_j<=rad;++kernel_j){
                for(int kernel_i=-rad;kernel_i<=rad;++kernel_i){
                    int neighbour_j=kernel_j+j;
                    int neighbour_i=kernel_i+i;
                    if (neighbour_i<0 || neighbour_i>=width || neighbour_j<0||neighbour_j>=height) {// Ignores out-of-bounds pixels.
                        sum += 0;
                    } else {
                        sum += CopyImage.get_pixel(neighbour_i, neighbour_j);

                    }
                    count++;

                }
            }
            if (count>0) {
                int changeValue=sum/count;
                if(changeValue<0){changeValue=0;}
                if(changeValue>255){changeValue=255;}
                image.set_pixel(i,j,changeValue);
            }
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    int rad=kernelSize/2;//Calculate a kernel radius
    GrayscaleImage CopyImage=image;//Create a copy of original image
    int width=image.get_width();
    int height=image.get_height();
    double sum=0.0;//Total value of in kernel

    std::vector<std::vector<double>>kernel(kernelSize,std::vector<double>(kernelSize));// Creates the Gaussian kernel matrix.
    for(int j=-rad;j<=rad;++j) {
        for(int i=-rad;i<=rad;++i) {
            kernel[j+rad][i+rad]=(1/(2*M_PI*sigma*sigma))*exp(-((i*i+j*j)/(2*sigma*sigma)));
            sum+=kernel[j+rad][i+rad];
        }
    }
    for(int j=0;j<kernelSize;++j) {
        for(int i=0;i<kernelSize;++i) {
            kernel[j][i]/=sum;
        }
    }
    for(int j=0;j<height;++j) {
        for(int i=0;i<width;++i) {
            double sum1=0;
            for(int kernel_j=-rad;kernel_j<=rad;++kernel_j) {
                for(int kernel_i=-rad;kernel_i<=rad;++kernel_i) {
                    int neighbour_j=j+kernel_j;
                    int neighbour_i=i+kernel_i;


                    if(neighbour_j>=0&&neighbour_j<height && neighbour_i>=0&&neighbour_i<width) {//Control of bounds
                        sum1+=CopyImage.get_pixel(neighbour_i,neighbour_j)*kernel[kernel_j+rad][kernel_i+rad];
                    }
                }
            }
            int changeValue=static_cast<int>(sum1);
            if(changeValue<0) {
                changeValue=0;
            }
            if(changeValue>255) {
                changeValue=255;
            }
            image.set_pixel(i,j,changeValue);
        }
    }
    
}


// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    GrayscaleImage CopyImage=image;//Create a copy of original image
    int width=image.get_width();
    int height=image.get_height();
    apply_gaussian_smoothing(CopyImage,kernelSize,1.0);
    for(int j=0;j<height;++j) {
        for(int i=0;i<width;++i) {
            int orPixel=image.get_pixel(i,j);
            int copyPixel=CopyImage.get_pixel(i,j);

            int sharPixel=orPixel+amount*(orPixel-copyPixel);// New pixel value is calculated with unsharp masking.
            if(sharPixel<0) {sharPixel=0;}
            if(sharPixel>255) {sharPixel=255;}
            image.set_pixel(i,j,sharPixel);
        }
    }

}
