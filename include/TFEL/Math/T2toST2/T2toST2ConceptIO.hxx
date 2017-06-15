/*! 
 * \file  T2toST2ConceptIO.hxx
 * \brief
 * \author Helfer Thomas
 * \date   24 janv. 2015
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
      for(i=0;i<StensorDimeToSize<T2toST2Traits<T>::dime>::value;++i){
	if(i!=0){
	  os << " [";
	} else {
	  os << "[";
	}
	for(j=0;j<TensorDimeToSize<T2toST2Traits<T>::dime>::value;++j){
	  os << s(i,j);
	  if(j!=TensorDimeToSize<T2toST2Traits<T>::dime>::value-1){
	    os << ",";
	  }
	}
	if(i!=StensorDimeToSize<T2toST2Traits<T>::dime>::value-1){
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
