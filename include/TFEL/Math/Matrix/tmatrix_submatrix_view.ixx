/*! 
 * \file  include/TFEL/Math/Matrix/tmatrix_submatrix_view.ixx
 * \brief
 * \author Thomas Helfer
 * \brief 27 déc. 2012
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_TMATRIX_SUBMATRIX_VIEWIXX
#define LIB_TFEL_MATH_TMATRIX_SUBMATRIX_VIEWIXX 

namespace tfel
{

  namespace math
  {

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    tmatrix_submatrix_view<N,M,I,J,R,C,T>::tmatrix_submatrix_view(tmatrix<N,M,T>& m_)
      : Expr<tmatrix<R,C,T>, tmatrix_submatrix_view_expr<N,M,I,J,R,C,T> >(m_)
    {}

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>::tmatrix_submatrix_view_expr(tmatrix<N,M,T>& m_)
      : m(m_)
    {} // end of tmatrix_submatrix_view_expr
    
    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    typename tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>::RunTimeProperties
    tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>::getRunTimeProperties() const
    {
      return RunTimeProperties();
    }

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    const T& 
    tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>::operator()(const unsigned short i,
							 const unsigned short j) const
    {
      return m(static_cast<unsigned short>(I+i),
	       static_cast<unsigned short>(J+j));
    }

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    T&
    tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>::operator()(const unsigned short i,
							 const unsigned short j)
    {
      return m(static_cast<unsigned short>(I+i),
	       static_cast<unsigned short>(J+j));
    }

  } // end of namespace math
  
} // end of namespace tfel

#endif /* LIB_TFEL_MATH_TMATRIX_SUBMATRIX_VIEWIXX */
