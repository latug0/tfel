/*! 
 * \file  include/TFEL/Math/ST2toST2/ST2toST2Expr.hxx
 * \brief
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _MATH_ST2TOST2_EXPR_LIB_
#define _MATH_ST2TOST2_EXPR_LIB_ 1

#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/EmptyClass.hxx"
#include"TFEL/Metaprogramming/HasIterator.hxx"
#include"TFEL/Metaprogramming/HasConstIterator.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Concept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    struct ST2toST2Expr;

    template<typename T_type, typename Expr>
    struct ST2toST2Traits<ST2toST2Expr<T_type,Expr> >{
      typedef typename ST2toST2Traits<T_type>::NumType   NumType;
      typedef unsigned short IndexType;
      static constexpr unsigned short dime = ST2toST2Traits<T_type>::dime;
    };

    template<typename T_type, typename Expr>
    struct ST2toST2Expr
      : public ST2toST2Concept<ST2toST2Expr<T_type,Expr> >,
	public Expr
    {

      typedef typename Expr::RunTimeProperties RunTimeProperties;
      typedef typename Expr::value_type      value_type;      
      typedef typename Expr::pointer	     pointer;	    
      typedef typename Expr::const_pointer   const_pointer; 
      typedef typename Expr::reference	     reference;	    
      typedef typename Expr::const_reference const_reference;
      typedef typename Expr::size_type 	     size_type;	    
      typedef typename Expr::difference_type difference_type;

      explicit TFEL_MATH_INLINE ST2toST2Expr()
	: Expr()
      {}

      explicit TFEL_MATH_INLINE ST2toST2Expr(const typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE ST2toST2Expr(const typename Expr::first_arg& a_, 
					    const typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}

      using Expr::operator();

      using Expr::operator=;
    };

    template<typename T_type, typename Expr>
    struct ST2toST2Type<ST2toST2Expr<T_type,Expr> >
    {
      typedef T_type type;
    };

  } // end of namespace math

  namespace typetraits{

    template<typename T_type, typename Expr>
    struct IsTemporary<tfel::math::ST2toST2Expr<T_type,Expr> >
    {
      static constexpr bool cond = true;
    };

  }// end of namespace typetraits

} // end of TFEL namespace

#endif /* _MATH_ST2TOST2_EXPR_LIB_ */
