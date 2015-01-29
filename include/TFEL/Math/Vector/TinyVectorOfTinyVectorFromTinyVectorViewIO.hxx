/*! 
 * \file  TinyVectorOfTinyVectorFromTinyVectorViewIO.hxx
 * \brief
 * \author Helfer Thomas
 * \date   24 janv. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_TINYVECTOROFTINYVECTORFROMTINYVECTORVIEWIO_H_
#define _LIB_TFEL_MATH_TINYVECTOROFTINYVECTORFROMTINYVECTORVIEWIO_H_ 

#include<ostream>
#include"TFEL/Math/TinyVectorOfTinyVectorFromTinyVectorView.hxx"

namespace tfel{
  
  namespace math{

    // Serialisation operator
    template<unsigned short N, unsigned short Mn,
	     unsigned short In,unsigned short Nn,
	     typename T>
    std::ostream&
    operator << (std::ostream &,
		 const VectorExpr<tvector<Nn,tvector<N,T> >,TinyVectorOfTinyVectorFromTinyVectorViewExpr<N,Mn,In,Nn,T> >&);

    template<unsigned short N, unsigned short Mn,
	     unsigned short In,unsigned short Nn,
	     typename T>
    std::ostream&
    operator << (std::ostream & os,
		 const VectorExpr<tvector<Nn,tvector<N,T> >,TinyVectorOfTinyVectorFromTinyVectorViewExpr<N,Mn,In,Nn,T> >& s)
    {
      os << "[ ";
      for(unsigned short i=0;i<Nn;++i){
	os << s(i) << " ";
      }
      os << "]";
      return os;
    } // end of operator << 
    
  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TINYVECTOROFTINYVECTORFROMTINYVECTORVIEWIO_H */

