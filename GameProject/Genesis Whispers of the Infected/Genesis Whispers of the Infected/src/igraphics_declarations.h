#ifndef IGRAPHICS_DECLARATIONS_H
#define IGRAPHICS_DECLARATIONS_H

// Standard GLUT font pointers (matching glut.h definitions)
#ifndef GLUT_BITMAP_TIMES_ROMAN_24
#define GLUT_BITMAP_8_BY_13             ((void*)0x0001)
#define GLUT_BITMAP_9_BY_15             ((void*)0x0002)
#define GLUT_BITMAP_TIMES_ROMAN_10      ((void*)0x0003)
#define GLUT_BITMAP_TIMES_ROMAN_24      ((void*)0x0004)
#define GLUT_BITMAP_HELVETICA_10        ((void*)0x0005)
#define GLUT_BITMAP_HELVETICA_12        ((void*)0x0006)
#define GLUT_BITMAP_HELVETICA_18        ((void*)0x0007)
#endif

// Declare external globals defined in iGraphics.h
extern int iScreenHeight;
extern int iScreenWidth;
extern int iMouseX;
extern int iMouseY;

// iGraphics function declarations
void iClear();
void iSetColor(double r, double g, double b);
void iFilledRectangle(double left, double bottom, double dx, double dy);
void iRectangle(double left, double bottom, double dx, double dy);
void iText(double x, double y, char *str, void* font = GLUT_BITMAP_8_BY_13);
void iFilledCircle(double x, double y, double r, int slices = 100);
void iCircle(double x, double y, double r, int slices = 100);
void iShowBMP(double x, double y, char filename[]);
void iShowBMP2(double x, double y, char filename[], int ignoreColor);

#endif
