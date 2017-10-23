#include "etuMatrix3x3.h"

/**
 *@brief Constructor
 **Setting default values
 */
etuMatrix3x3::etuMatrix3x3()
{
  int i,j;
  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      m[i][j]=(i==j)? 1:0;
    }
  }
}


/**
 *@brief Destructor
 */
etuMatrix3x3::~etuMatrix3x3()
{
  //do nothing
}

/**
 *@brief Create a pointer to a new object
 */
etuMatrix3x3* etuMatrix3x3::New()
{
  return new etuMatrix3x3();
}

/**
 *@brief Self eraser
 */
void etuMatrix3x3::Delete()
{
  delete this;
}

/**
 *@brief Set matrix element
 *@param i row
 *@param j column
 *@param p value to be set
 *@return none
*/
void etuMatrix3x3::SetElement(int i,int j,float p)
{
  m[i][j]=p;
}

/**
 *@brief Get matrix element
 *@param i row
 *@param j column
 *@return value of element at row i. col j
*/
float etuMatrix3x3::GetElement(int i, int j)
{
  return m[i][j];
}

/**
 *@brief Reset to identity matrix 
 *@return none
*/
void etuMatrix3x3::Identity()
{
  int i,j;
  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      m[i][j]=(i==j)? 1:0;
    }
  }
}

/**
 *@brief Multiply matrix out=M*in
 *@param in Matrix to be multiplied by
 *@param out Pointer to result
 *@return none
*/
void etuMatrix3x3::Multiply3x3(etuMatrix3x3* in, etuMatrix3x3* out)
{
  int i,j,k;
  float tmp;
  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      tmp=0.0;
      for(k=0;k<3;k++){
	tmp+=m[i][k]*in->GetElement(k,j);
      }
      out->SetElement(i,j,tmp);
    }
  }
}

/**
 *@brief Multiply point out=M*in
 *@param in point to be multiplied by
 *@param out Pointer to result
 *@return none
*/
void etuMatrix3x3::MultiplyPoint(float in[3], float* out)
{
  int i,k;
  float tmp;
  for(i=0;i<3;i++){
    out[i]=0.0;
    for(k=0;k<3;k++){
      out[i]+=m[i][k]*in[k];
    } 
  }
}

/**
 *@brief Get the determinant
 *@return none
*/
float etuMatrix3x3::GetDeterminant()
{
  return (m[0][0]*m[1][1]*m[2][2] + m[0][1]*m[1][2]*m[2][0] + m[0][2]*m[1][0]*m[2][1]
	  -m[0][0]*m[1][2]*m[2][1]-m[0][1]*m[1][0]*m[2][2]-m[0][2]*m[1][1]*m[2][0]);
}

/**
 *@brief Get Inverse matrix
 *@param out Pointer to result
 *@return none
*/
void etuMatrix3x3::GetInverse(etuMatrix3x3* out)
{
  float det=GetDeterminant();
  out->SetElement(0,0,(m[1][1]*m[2][2]-m[1][2]*m[2][1])/det);
  out->SetElement(0,1,(m[0][2]*m[2][1]-m[0][1]*m[2][2])/det);
  out->SetElement(0,2,(m[0][1]*m[1][2]-m[0][2]*m[1][1])/det);
  out->SetElement(1,0,(m[1][2]*m[2][0]-m[1][0]*m[2][2])/det);
  out->SetElement(1,1,(m[0][0]*m[2][2]-m[0][2]*m[2][0])/det);
  out->SetElement(1,2,(m[0][2]*m[1][0]-m[0][0]*m[1][2])/det);
  out->SetElement(2,0,(m[1][0]*m[2][1]-m[1][1]*m[2][0])/det);
  out->SetElement(2,1,(m[0][1]*m[2][0]-m[0][0]*m[2][1])/det);
  out->SetElement(2,2,(m[0][0]*m[1][1]-m[0][1]*m[1][0])/det);
}
