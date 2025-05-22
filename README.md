# ClearVision - BBM203 Fall 2024 Assignment

This project is developed as part of the BBM203 Software Practicum I course at Hacettepe University (Fall 2024). ClearVision is a C++11-based image processing application designed to apply various filters on grayscale images and handle encrypted message embedding using least significant bit (LSB) manipulation.

## Features

- Mean, Gaussian, and Unsharp Mask filters
- Image arithmetic: addition, subtraction, comparison
- Secure message hiding and retrieval using LSB
- Upper/lower triangular matrix-based image encoding for secret image storage
- File I/O support with stb_image.h and stb_image_write.h

## Directory Structure

- `src/`: C++ source code files
- `sample_io/`: Example input images and message files
- `README.md`: Project documentation

## Usage

The program is used via command-line arguments:

```bash
./clearvision mean input/image.png 3
./clearvision gauss input/image.png 5 1.0
./clearvision unsharp input/image.png 5 1.5
./clearvision enc input/image.png "Confidential Message"
./clearvision dec input/image.png 24
