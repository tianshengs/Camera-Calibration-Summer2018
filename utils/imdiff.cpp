/* imdiff.cpp - visual alignment of two images
 *
 * CS 453 openCV demo
 */

#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat im0, im1;
Mat im1t; // transformed image 1
Mat imd; // "difference" image
const char *win = "imdiff";
float dx = 26;
int dxval = dx+20;
float dy = 0;
float startx = 9999;
float starty = 9999;

void imdiff()
{
    float s = 1;
    Mat T1 = (Mat_<float>(2,3) << s, 0, dx, 0, s, dy); 
    warpAffine(im1, im1t, T1, im1.size());
    addWeighted(im0, 1, im1t, -1, 128, imd);
    char txt[100];
    sprintf(txt, "dx=%g  dy=%g", dx, dy);
    putText(imd, txt, Point(50, 50), FONT_HERSHEY_PLAIN, 2, Scalar(255, 255, 255), 4);
    imshow(win, imd);
}

void changedx(int, void *)
{
    dx = dxval-20;
    imdiff();
}

static void onMouse( int event, int x, int y, int, void* )
{
    //printf("x=%d y=%d    ", x, y);
    //printf("dx=%g dy=%g\n", dx, dy);
    if (event == CV_EVENT_LBUTTONDOWN) {
	startx = x-dx;
	starty = y-dy;
    } else if (event == CV_EVENT_LBUTTONUP) {
	startx = 9999;
	starty = 9999;
	printf("calling imdiff: dx=%g dy=%g\n", dx, dy);
	waitKey(1);
	imdiff();
    } else if (event == CV_EVENT_MOUSEMOVE && startx < 9999) {
	dx = x - startx;
	dy = y - starty;
	imdiff();
    }
}

int main(int argc, char ** argv)
{
    if (argc < 3) {
	fprintf(stderr, "usage: %s im1 im2\n", argv[0]);
	exit(1);
    }

    im0 = imread(argv[1], 1);
    if (!im0.data) { 
	fprintf(stderr, "cannot read image %s\n", argv[1]); 
	exit(1); 
    }
    im1 = imread(argv[2], 1);
    if (!im1.data) { 
	fprintf(stderr, "cannot read image %s\n", argv[2]); 
	exit(1); 
    }

    // convert to gray
    //cvtColor(im0, im0bw, CV_RGB2GRAY );  
    //cvtColor(im1, im1bw, CV_RGB2GRAY );  

    //namedWindow(win, CV_WINDOW_AUTOSIZE);
    namedWindow(win, CV_WINDOW_NORMAL);
    resizeWindow(win, 1480, 920 );
    createTrackbar("dx", win, &dxval, 100, changedx);
    setMouseCallback(win, onMouse);
    imdiff();

    float step = 0.25;
    while(1) {
	int c = waitKey(0);
	switch(c) {
	case 27:// ESC
	case 'q':
	    return 0;
	case 2424832: case 65361: case 119: // left arrow
	    dx -= 1; imdiff(); break;
	case 2555904: case 65363: case 114:  // right arrow
	    dx += 1; imdiff(); break;
	case ',': // small step to the left
	    dx -= step; imdiff(); break;
	case '.': // small step to the right
	    dx += step; imdiff(); break;
	case 2490368: case 65362: case 101: // up arrow
	    dy -= step; imdiff(); break; 
	case 2621440: case 65364: case 100: // down arrow
	    dy += step; imdiff(); break;
	case ' ': // reset
	    dx = 0; dy = 0; imdiff(); break;
	case 'a': // show original left image
	    imshow(win, im0); break;
	case 's': // show original right image
	    imshow(win, im1t); break;
	default:
	    printf("key %d (%c %d) pressed\n", c, (char)c, (char)c);
	}
    }

    return 0;
}
