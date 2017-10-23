#ifndef _XMATRIX3X3X_H
#define _XMATRIX3X3X_H
#include <math.h>
class etuMatrix3x3 {
 public:
  static etuMatrix3x3* New();
  void Delete();
 public:
  void SetElement(int i,int j,float p);
  float GetElement(int i, int j);
  void Identity();
  void Multiply3x3(etuMatrix3x3* in, etuMatrix3x3* out);
  void MultiplyPoint(float in[3], float* out);
  float GetDeterminant();
  void GetInverse(etuMatrix3x3* out);
 private:
  float m[3][3];
 protected:
  etuMatrix3x3();
  ~etuMatrix3x3();
};

#endif
