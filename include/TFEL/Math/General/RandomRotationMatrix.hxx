/*! 
 * \file  RandomRotationMatrix.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 16 mai 2013
 */

#ifndef _LIB_TFEL_MATH_RANDOMROTATIONMATRIX_H_
#define _LIB_TFEL_MATH_RANDOMROTATIONMATRIX_H_ 

#include<cmath>
#include<cstdlib>
#include"TFEL/Math/tmatrix.hxx"

namespace tfel{

  namespace math{
    
    template<typename T>
    tmatrix<3u,3u,T>
    getRandomRotationMatrix(void)
    {
      using namespace std;
      const T rmax = static_cast<T>(RAND_MAX);
      const T pi = real(4)* atan(real(1));  
      tmatrix<3u,3u,T> drot;
      T psi=T(2)*pi*(static_cast<T>(rand())/rmax);
      T cthe=T(2)*((static_cast<T>(rand())/rmax)-0.5);
      T the=acos(cthe);
      T phi=T(2)*pi*(static_cast<T>(rand())/rmax);
      T cospsi=cos(psi);
      T costhe=cos(the);
      T cosphi=cos(phi);
      T sinpsi=sin(psi);
      T sinthe=sin(the);
      T sinphi=sin(phi);
      drot(0,0) =  cosphi*cospsi - sinphi*costhe*sinpsi;
      drot(0,1) =  cosphi*sinpsi + sinphi*costhe*cospsi;
      drot(0,2) =  sinphi*sinthe;
      drot(1,0) = -sinphi*cospsi - cosphi*costhe*sinpsi;
      drot(1,1) = -sinphi*sinpsi + cosphi*costhe*cospsi;
      drot(1,2) =  cosphi*sinthe;
      drot(2,0) =  sinthe*sinpsi;
      drot(2,1) = -sinthe*cospsi;
      drot(2,2) =  costhe;
      return drot;
    } // end of getRandomRotationMatrix

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_RANDOMROTATIONMATRIX_H */
