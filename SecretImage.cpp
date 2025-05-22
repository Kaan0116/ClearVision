#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {

    this->height=image.get_height();//Keeps the height of the image
    this->width=image.get_width();//Keeps the width of the image
    int u_index=0;//The upper triangle holds the index of the matrix
    int l_index=0;//The lower triangle holds the index of the matrix
    this->lower_triangular=new int[(height*(height-1))/2];
    this->upper_triangular=new int[(width*(width+1))/2];

    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            if(i>j){// For lower triangular matrix, when i > j
                lower_triangular[l_index]=image.get_pixel(i,j);
                l_index++;
            }
            if(i<=j){// For upper triangular matrix, when i <= j
                upper_triangular[u_index]=image.get_pixel(i,j);
                u_index++;
            }
        }
    }
    


    
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {

    this->height=h;
    this->width=w;
    int l_size=height*(height-1)/2;
    int u_size=width*(width+1)/2;
    this->lower_triangular=new int[l_size];
    this->upper_triangular=new int[u_size];
    for(int i=0;i<l_size;++i){// Lower triangular matrix values are copied.
        this->lower_triangular[i]=lower[i];
    }
    for(int j=0; j<u_size;++j){// Upper triangular matrix values are copied.
        this->upper_triangular[j]=upper[j];
    }




}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete[] this->lower_triangular;
    delete[] this->upper_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);
    int l_size=0;
    int u_size=0;
    for(int i=0;i<height;++i) {// Fills each pixel to recreate the image.
        for(int j=0;j<width;++j) {
            if(i>j){
                image.set_pixel(i,j,lower_triangular[l_size++]);
            }
            else{
                image.set_pixel(i,j,upper_triangular[u_size++]);
        }
    }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image){
    int l_size=0;
    int u_size=0;
    for(int i=0;i<height;++i) {// Writes each pixel of the image back into triangular matrices.
        for(int j=0;j<width;++j) {
            if(i>=j) {lower_triangular[l_size++]=image.get_pixel(i,j);}
            if(i<j) {upper_triangular[u_size++]=image.get_pixel(i,j);}
        }
    }
}


// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {

    int u_size=(width*(width +1)) / 2;
    int l_size=((height*(height-1))/2);

    
    std::ofstream outfile(filename);
    outfile<<width<<" "<<height<<std::endl;
    for(int i=0;i<u_size;++i) {
        outfile<<upper_triangular[i];
        if(i!=(u_size-1)) {
            outfile<<" ";
        }
    }
    outfile<<std::endl;
    for(int j=0;j<l_size;++j) {
        outfile<<lower_triangular[j];
        if(j!=(l_size-1)) {
            outfile<<" ";
        }
    }
    outfile<<std::endl;


    outfile.close();

}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {


    std::ifstream infile(filename);
    int w,h;
    infile>>w>>h;
    int l_size=(h*(h-1)) / 2;
    int u_size=(w*(w +1)) / 2;
    int *lower=new int[l_size];
    int *upper=new int[u_size];
    for(int i=0;i<u_size;++i) {
        infile>>upper[i];
    }
    for(int j=0;j<l_size;++j) {
        infile>>lower[j];
    }
    infile.close();
    SecretImage secret_image(w,h,upper,lower);
    delete[] upper;
    delete[] lower;
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
