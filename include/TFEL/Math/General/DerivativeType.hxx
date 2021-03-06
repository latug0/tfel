/*!
 * \file   include/TFEL/Math/General/DerivativeType.hxx
 * \brief
 * \author Thomas Helfer
 * \date   24/06/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_DERIVATIVETYPE_HXX
#define LIB_TFEL_MATH_DERIVATIVETYPE_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/ComputeObjectTag.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"

namespace tfel::math {

  /*!
   * \brief an helper metafunction meant to be specialised
   */
  template <typename TagA, typename TagB, typename A, typename B>
  struct DerivativeTypeDispatcher {
    //! \brief result
    using type = tfel::meta::InvalidType;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief a meta function returning the type of the derivative of
   * a variable of type T1 with respect to a variable of type T2
   * \tparam T1: type
   * \tparam T2: type
   */
  template <typename T1, typename T2>
  struct DerivativeType {
    //! boolean stating if both variables are scalar
    static constexpr auto are_scalars = isScalar<T1>() && isScalar<T2>();
    //! \brief the result of the derivative type dispatcher
    using derivative_type_dispatcher_result =
        typename tfel::math::DerivativeTypeDispatcher<
            typename ComputeObjectTag<T1>::type,
            typename ComputeObjectTag<T2>::type,
            T1,
            T2>::type;
    //! the result
    using type = std::conditional_t<are_scalars,
                                    result_type<T1, T2, OpDiv>,
                                    derivative_type_dispatcher_result>;
  };  // end of DerivativeType

  //! a simple alias
  template <typename T1, typename T2>
  using derivative_type = typename DerivativeType<T1, T2>::type;

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_DERIVATIVETYPE_HXX */
