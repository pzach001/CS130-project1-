/**
 * minigl.cpp
 * -------------------------------
 * Implement miniGL here.
 *
 * You may include minigl.h and any of the standard C++ libraries.
 * No other includes are permitted.  Other preprocessing directives
 * are also not permitted.  These requirements are strictly
 * enforced.  Be sure to run a test grading to make sure your file
 * passes the sanity tests.
 */
//#include "vec.h"  //This was given to us in assignment 2, which the professor allowed us to use
//#include <vector>
#include "minigl.h"
#include <stack>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>
//using namespace std;
using namespace std;
//-----------Global variables----------//
stack<vector<vector<MGLfloat>>>  ProjectionStack;
stack<vector<vector<MGLfloat>>>  ModelViewStack;
vector<vector<MGLfloat>> CurrProjectionMatrix;
vector<vector<MGLfloat>> CurrModelViewMatrix;
vector<vector<MGLpixel>> PixelFramebuffer;
int glcheck = 0;
//vector<float> FirstRow;
//FirstRow.push_back(one);
//FirstRow.push_back(zero);
//Firstrow.push_back(zero);
//vector<float> SecondRow;
//SecondRow.push_back(zero);
//SecondRow.push_back(one);
//SecondRow.push_back(zero);
//vector<float> ThirdRow;
//ThirdRow.push_back(zero);
//ThirdRow.push_back(zero);
//ThirdRow.push_back(one);
vector<vector<MGLfloat>> IdentityMatrix;
vector<MGLfloat>         VertexHold;
int                   NumOfVert = 0;
//IdentityMatrix.push_back(Firstrow);
//IdentityMatrix.push_back(Secondrow);
//IdentityMatrix.push_back(Thirdrow);


MGLmatrix_mode CurrMode;
MGLpoly_mode   CurrPolyMode;
//-------------------------------------//
/**
 * Standard macro to report errors
 */
inline void MGL_ERROR(const char* description) {
    printf("%s\n", description);
    exit(1);
}


/**
 * Read pixel data starting with the pixel at coordinates
 * (0, 0), up to (width,  height), into the array
 * pointed to by data.  The boundaries are lower-inclusive,
 * that is, a call with width = height = 1 would just read
 * the pixel at (0, 0).
 *
 * Rasterization and z-buffering should be performed when
 * this function is called, so that the data array is filled
 * with the actual pixel values that should be displayed on
 * the two-dimensional screen.
 */
void mglReadPixels(MGLsize width,
                   MGLsize height,
                   MGLpixel *data)
{
    for(int  x = 0; x <  width; x++)
       {
           for( int y = 0; y < height ; y++)
           {
             Make_Pixel(255,255,255);
           }
       }  
}

/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode)
{
   glcheck = 1;
   CurrPolyMode = mode;

}


/**
 * Stop specifying the vertices for a group of primitives.
 */
void mglEnd()
{
  glcheck = 0;
}

/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x,
                MGLfloat y)
{
  if(glcheck == 0)
  {
    printf("error: did not call glBegin");
  }
  if(glcheck == 1)
  {
   	vector<MGLfloat>  vertex;
   	vertex.push_back(x);
   	vertex.push_back(y);
   	vertex.push_back(0);
        VertexHold = vertex;
        NumOfVert++;
        if(NumOfVert == 3)
        {
        
        }
  }
}

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x,
                MGLfloat y,
                MGLfloat z)
{
   if(glcheck == 0)
   {
       printf("error: did not call  glBegin");
   } 
   if(glcheck ==1)
   {
       vector<MGLfloat> vertex;
       vertex.push_back(x);
       vertex.push_back(y);
       vertex.push_back(z);
       VertexHold = vertex;
       NumOfVert++;   
   }
}

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode)
{
   CurrMode = mode;
}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix()
{
    
}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix()
{
}

/**
 * Replace the current matrix with the identity
 */
void mglLoadIdentity()  //currMode Does not have the actual mode yet, in which you need to find it.
{
   vector<MGLfloat>FirstRow;
   vector<MGLfloat>SecondRow;
   vector<MGLfloat>ThirdRow;

   
   FirstRow.push_back(1);
   FirstRow.push_back(0);
   FirstRow.push_back(0);
  // FirstRow.push_bacl(0);

   SecondRow.push_back(0);
   SecondRow.push_back(1);
   SecondRow.push_back(0);
  // Secondrow.push_back(0);

   ThirdRow.push_back(0);
   ThirdRow.push_back(0);
   ThirdRow.push_back(1);
  // ThirdRow.push_back(0);
    
  // FourthRow.push_back(0);
  // FourthRow.push_back(0);
  // FourthRow.push_back(0);
  // FourthRow.push_back(1);
  IdentityMatrix.push_back(FirstRow);
  IdentityMatrix.push_back(SecondRow);
  IdentityMatrix.push_back(ThirdRow);
  //IdentityMatrix.push_back(FourthRow);  
  if( CurrMode == MGL_MODELVIEW )
  {
      ModelViewStack.top()   = IdentityMatrix;
  }
  if( CurrMode == MGL_PROJECTION)
  {
     ProjectionStack.top() = IdentityMatrix;
  }

}

/**
 * Replace the current matrix with an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglLoadMatrix(const MGLfloat *matrix)
{
}

/**
 * Multiply the current matrix by an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglMultMatrix(const MGLfloat *matrix)
{
}

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x,
                  MGLfloat y,
                  MGLfloat z)
{
}

/**
 * Multiply the current matrix by the rotation matrix
 * for a rotation of (angle) degrees about the vector
 * from the origin to the point (x, y, z).
 */
void mglRotate(MGLfloat angle,
               MGLfloat x,
               MGLfloat y,
               MGLfloat z)
{
}

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x,
              MGLfloat y,
              MGLfloat z)
{
}

/**
 * Multiply the current matrix by the perspective matrix
 * with the given clipping plane coordinates.
 */
void mglFrustum(MGLfloat left,
                MGLfloat right,
                MGLfloat bottom,
                MGLfloat top,
                MGLfloat near,
                MGLfloat far)
{
}

/**
 * Multiply the current matrix by the orthographic matrix
 * with the given clipping plane coordinates.
 */
void mglOrtho(MGLfloat left,
              MGLfloat right,
              MGLfloat bottom,
              MGLfloat top,
              MGLfloat near,
              MGLfloat far)
{
}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLfloat red,
              MGLfloat green,
              MGLfloat blue)
{
}
