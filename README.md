# cartoonify

cartoonizing video in real-time

pre-requisites: openCV

compilation: g++ cartoonize.cpp -o cartoon `pkg-config --cflags --libs opencv`

execution: ./cartoon

This program accesses the webcam and renders the cartoonized version of the person viewing it, we have two types of display. One is sketch mode of display and the other is a colored cartoon display. Using the keyboard inputs we can change the mode of view (sketch or cartoon).

The procedure for cartoonizing is as follows:
1. Convert the actual image into grayscale image.
2. Apply the median blur for noise removal.
3. Detect edges in the image using Laplacian.
4. Threshold the Laplacian.
5. Erode the thresholded image with the kernel mask.
6. Apply median blur to the eroded image
7. Apply bilateral filter to the blurred image for making the image smooth and making the
edges sharp.
8. Convert the bilateral filter image to color image.
9. Using bitwise and operator on both the input frame and the converted color image.

The above steps when executed cartoonizes an input image into its cartoonized version.
If we want to view sketch mode, we can display the bilateral image itself.
