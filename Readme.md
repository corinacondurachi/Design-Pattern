# Design Pattern

Takes an image which contains digits written by hand and tries to recognise the digits by colouring them differently.


# Files

 - main.c
 - test.bmp (the image which we are going to work with )
 - cifra0.bmp
 - cifra1.bmp
 -  cifra2.bmp
 -  cifra3.bmp
 - cifra4.bmp
 - cifra5.bmp
 - cifra6.bmp
 - cifra7.bmp
 - cifra8.bmp
 - cifra9.bmp


## Describing the code
W=image's width
H=image's height

 - int image_width(char *name,FILE *f): returns the width of the image
 -  int image_height(char *name,FILE *f): returns the height of the image
 - void grayscale_image(char* nume_fisier_sursa,char* nume_fisier_destinatie): turns a colored image into a grayscale one
 - unsigned char** matrix_template(char *nume, int w, int h): builds a matrix with the pixels of the template
 - void matrix_image(char *name,int w,int h,unsigned char **L):  builds a matrix with the pixels of the image
 - double sum_average_template(int h,int w,unsigned char **s): calculates the average of the values of grayscale intensity of template's pixels
 - double deviation_standard_template(int h,int w,unsigned char **s): calculates the standard deviation of the template
 - double sum_average_fI(int k,int l,int h,int w,unsigned char **s): calculates the average of the values of grayscale intensity of image's pixels on a portion equal to the template
 - double deviation_standard_fI(int k,int l,int h,int w,unsigned char **s): calculates the standard deviation of image's pixels on a portion equal to the template
 - double cross_correlation(int w,int h,int w_sablon,int h_sablon,int x,int y,unsigned char **s,unsigned char **I): calculates the value of cross-correlation between the template and the image
 - int template_matching(int w,int h,int w_sablon,int h_sablon,coordonate *v,unsigned char **I,unsigned char **s,float ps,int cifra): returns the number of correlations which have a higher value than ps(which in this case will be 0.5)
 - void print_contour(int w,int h,int w_sablon,int h_sablon,char *nume,unsigned char **L,int x,int y,int p1,int p2,int p3): paints a square of different colors
 - void free_matrix(int inaltime,unsigned char **A): frees the memory
 - int cmp(const void *a,const void *b): compares two coordinates
 - double intersection_area(int x1,int y1,int x2,int y2): calculates the area of two squares' intersection 
 - int non_maxi_removal(int n,coordonate *v): eliminates the ones the have a non maxim value
 - void project(): calls every function used









