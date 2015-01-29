/*! 
 * \file  StensorConceptIO.hxx
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

#ifndef _LIB_TFEL_MATH_STENSORCONCEPTIO_H_
#define _LIB_TFEL_MATH_STENSORCONCEPTIO_H_ 

#include<ostream>
#include"TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel
{

  namespace math
  {

    //! Serialisation operator
    template<typename T>
    std::ostream&
    operator << (std::ostream &,const StensorConcept<T>&);

    template<typename T>
    std::ostream&
    operator << (std::ostream & os,const StensorConcept<T>& s)
    {
      unsigned short i;
      os << "[ ";
      for(i=0;i!=StensorDimeToSize<StensorTraits<T>::dime>::value;++i){
	os << s(i) << " ";
      }
      os << "]";
      return os;
    } // end of operator << 

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_STENSORCONCEPTIO_H_ */


