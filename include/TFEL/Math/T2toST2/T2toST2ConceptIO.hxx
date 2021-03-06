/*! 
 * \file  T2toST2ConceptIO.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24 janv. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_T2TOST2CONCEPTIO_HXX
#define LIB_TFEL_MATH_T2TOST2CONCEPTIO_HXX 

#include<ostream>
#include"TFEL/Math/T2toST2/T2toST2Concept.hxx"

namespace tfel{

  namespace math{

    // Serialisation operator
    template<typename T>
    std::ostream &
    operator << (std::ostream &,
		 const T2toST2Concept<T>&);

    template<typename T>
    std::ostream &
    operator << (std::ostream & os,
		 const T2toST2Concept<T>& s)
    {
      unsigned short i;
      unsigned short j;
      os << "[";
      for(i=0;i<StensorDimeToSize<getSpaceDimension<T>()>::value;++i){
	if(i!=0){
	  os << " [";
	} else {
	  os << "[";
	}
	for(j=0;j<TensorDimeToSize<getSpaceDimension<T>()>::value;++j){
	  os << s(i,j);
	  if(j!=TensorDimeToSize<getSpaceDimension<T>()>::value-1){
	    os << ",";
	  }
	}
	if(i!=StensorDimeToSize<getSpaceDimension<T>()>::value-1){
	  os << "]\n";
	} else {
	  os << "]";
	}
      }
      os << "]";
      return os;
    } // end of operator <<

  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOST2CONCEPTIO_HXX */

