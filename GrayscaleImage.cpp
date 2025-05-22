#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);// Loads the image in grayscale using STB




    data=new int*[height];// Allocates space in memory and copies data to int matrix
    for(int i=0;i<height;++i ){
        data[i]=new int[width];
        for (int j=0;j<width;++j){
            data[i][j]=static_cast<int>(image[i * width + j]);
            
        }
        
    }



    
    stbi_image_free(image);//Releases the image loaded by the STB
    

}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) {
    //Copies from the existing data matrix
    data=new int*[h];
    for(int i=0;i<h;++i){
        data[i]=new int[w];
        std::copy(inputData[i],inputData[i]+w,data[i]);
    
    }

}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {

    data=new int*[height];
    for(int i=0;i<height;++i ){
        data[i]=new int[width];
        for (int j=0;j<width;++j){
            data[i][j]=0;
        }
    }
}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {
    //Copies the current image
    height=other.height;
    width=other.width;
    data=new int*[height];
    for(int i=0;i<height;++i){
        data[i]=new int[width];
        std::copy(other.data[i],other.data[i]+width,data[i]);
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    // Frees memory
    for(int i=0;i<height;++i){
        delete[] data[i];
    }
    delete[] data;

}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {

    if(height!= other.height ||width!=other.width){// Checks whether the size and pixel values of two images are the same
        return false;
    }


    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            if(data[i][j]!=other.data[i][j]){
                return false;
            }
        }
    }
    
    return true;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    //Adds two images and returns the result
    GrayscaleImage result(width, height);




    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            int p1;
            int p2;
            if(i<height && j<width) {
                p1=data[i][j];
            }
            else {
                p1=0;
            }

            if(i<other.height && j<other.width) {
                p2=other.data[i][j];
            }
            else {
                p2=0;
            }
            int sum=p1+p2;
            if(sum>255){
                result.data[i][j]=255;
            }
            else{
                result.data[i][j]=sum;
            }
        }
    }
    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    //Extracts two images and returns the result
    GrayscaleImage result(width, height);

    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){


            int p1;
            int p2;
            if(i<height && j<width) {
                p1=data[i][j];
            }
            else {
                p1=0;
            }

            if(i<other.height && j<other.width) {
                p2=other.data[i][j];
            }
            else {
                p2=0;
            }

            int subs=p1-p2;
            if(subs<0){
                result.data[i][j]=0;
            }
            else{
                result.data[i][j]=subs;
            }
        }
    }
    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Saves the image data to a file in PNG format
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Converts pixel data to unsigned char type and copies it to buffer
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        throw std::runtime_error("Error saving the image to file");
    }


    // Clean up the allocated buffer
    delete[] imageBuffer;
}
