#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
#include "Crypto.h"
#include "GrayscaleImage.h"

std::vector<int> Crypto::extract_LSBits(SecretImage &secret_image, int message_length)
{
    std::vector<int> LSB_array;// Vector to store LSBs.

    // Recreates SecretImage as GrayscaleImage.
    GrayscaleImage image=secret_image.reconstruct();

    // Calculates image dimensions (width and height).
    int width=image.get_width();
    int height=image.get_height();
    int total_pixels=width*height;

    // Calculates the total number of required bits based on message length.
    int total=message_length*7;

    // Calculates the starting pixel so that the last LSB coincides with the last pixel.
    int start_pixel=total_pixels-total;

    //Extracts LSBs from pixels and adds them to the LSB_array vector.
    for (int i = start_pixel; i<total_pixels; ++i)
    {
        int pixel_value = image.get_pixel(i/width, i%width);
        LSB_array.push_back(pixel_value & 1);//Extracts the LSB and adds it to the vector.
    }

    return LSB_array;
}

std::string Crypto::decrypt_message(const std::vector<int> &LSB_array)
{
    std::string message;
    // Converts each 7-bit group from the LSB string into a character.
    for (size_t i = 0; i < LSB_array.size(); i += 7)
    {
        int val=0;
        for (int j = 0; j < 7; ++j)
        {
            val =(val<< 1)|LSB_array[i + j];// A 7-bit value is created.
        }
        message.push_back(static_cast<char>(val));// It is added to the message as a character.
    }


    return message;
}


std::vector<int> Crypto::encrypt_message(const std::string &message)
{
    std::vector<int>LSB_array;// LSBs of the encrypted message.
    for (char c:message)
    {
        std::bitset<7>mem(c);// Each character is treated as a 7-bit bitset.

        for (int i = 6; i >= 0; --i)// Adds each of the 7 bits to the LSB_array.
        {
            LSB_array.push_back(mem[i]);
        }
    }

    return LSB_array;
}

SecretImage Crypto::embed_LSBits(GrayscaleImage &image, const std::vector<int> &LSB_array)
{
    int width=image.get_width();
    int height=image.get_height();
    int total=width*height;// Total number of pixels.


    int start= total-LSB_array.size();//Start pixel for LSB

    int index=0;
    for (int i = start; i < total; ++i)// Embeds LSBs into pixels in the image.
    {
        int col=i%width;
        int row=i/width;
        int pixel_value=image.get_pixel(row, col);//Original value of pixel
        pixel_value=(pixel_value&~1)|LSB_array[index++];//Only LSB is updated
        image.set_pixel(row,col,pixel_value);//Update value of pixel
    }


    return SecretImage(image);
}
