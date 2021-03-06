/*!
 * \file  StandardOperations.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03 févr. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_EXPRESSIONTEMPLATES_STANDARDOPERATIONS_HXX
#define LIB_TFEL_MATH_EXPRESSIONTEMPLATES_STANDARDOPERATIONS_HXX

#include <type_traits>
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"
#include "TFEL/Math/General/ComputeBinaryResult.hxx"

namespace tfel::math {

  /*!
   * \brief negate a mathematical object
   * \tparam T1: type of the left  hand side argument
   * \param  a : argument
   * \return an handle which allow delayed evaluation of the
   * operation.
   *
   * This operator is defined if the
   * ComputeUnaryResult<T1,T2,OpPlus> metafunctions returns valid
   * Result and Handle.
   * \see ComputeBinaryResult
   * \see IsInvalidType
   */
  template <typename T1>
  TFEL_MATH_INLINE auto operator-(T1&& a) -> std::enable_if_t<
      isUnaryOperationResultTypeValid<decltype(a), OpNeg>::value,
      UnaryOperationHandler<decltype(a), OpNeg>> {
    return UnaryOperationHandler<decltype(a), OpNeg>(std::forward<T1>(a));
  }

  /*!
   * \brief add to mathematical object
   * \tparam T1: type of the left  hand side argument
   * \tparam T2: type of the right hand side argument
   * \param  a : left  hand side argument
   * \param  b : right hand side argument
   * \return an handle which allow delayed evaluation of the
   * operation
   *
   * This operator is defined if the
   * ComputeBinaryResult<T1,T2,OpPlus> metafunctions returns valid
   * Result and Handle.
   * \see ComputeBinaryResult
   * \see IsInvalidType
   */
  template <typename T1, typename T2>
  TFEL_MATH_INLINE auto operator+(T1&& a, T2&& b) -> std::enable_if_t<
      isBinaryOperationResultTypeValid<decltype(a), decltype(b), OpPlus>::
              value &&
          (!((isScalar<T1>()) && (isScalar<T2>()))),
      BinaryOperationHandler<decltype(a), decltype(b), OpPlus>> {
    return BinaryOperationHandler<decltype(a), decltype(b), OpPlus>(
        std::forward<T1>(a), std::forward<T2>(b));
  }

  /*!
   * \brief substract to mathematical object
   * \tparam T1: type of the left  hand side argument
   * \tparam T2: type of the right hand side argument
   * \param  a : left  hand side argument
   * \param  b : right hand side argument
   * \return an handle which allow delayed evaluation of the
   * operation
   *
   * This operator is defined if the
   * ComputeBinaryResult<T1,T2,OpMinus> metafunctions returns valid
   * Result and Handle.
   * \see ComputeBinaryResult
   * \see IsInvalidType
   */
  template <typename T1, typename T2>
  TFEL_MATH_INLINE auto operator-(T1&& a, T2&& b) -> std::enable_if_t<
      isBinaryOperationResultTypeValid<decltype(a), decltype(b), OpMinus>::
              value &&
          (!((isScalar<T1>()) && (isScalar<T2>()))),
      BinaryOperationHandler<decltype(a), decltype(b), OpMinus>> {
    return BinaryOperationHandler<decltype(a), decltype(b), OpMinus>(
        std::forward<T1>(a), std::forward<T2>(b));
  }

  /*!
   * \brief multiply to mathematical object
   * \tparam T1: type of the left  hand side argument
   * \tparam T2: type of the right hand side argument
   * \param  a : left  hand side argument
   * \param  b : right hand side argument
   * \return an handle which allow delayed evaluation of the
   * operation
   *
   * This operator is defined if the
   * ComputeBinaryResult<T1,T2,OpMult> metafunctions returns valid
   * Result and Handle.
   * \see ComputeBinaryResult
   * \see IsInvalidType
   */
  template <typename T1, typename T2>
  TFEL_MATH_INLINE auto operator*(T1&& a, T2&& b) -> std::enable_if_t<
      isBinaryOperationResultTypeValid<decltype(a), decltype(b), OpMult>::
              value &&
          (!((isScalar<T1>()) && (isScalar<T2>()))),
      BinaryOperationHandler<decltype(a), decltype(b), OpMult>> {
    return BinaryOperationHandler<decltype(a), decltype(b), OpMult>(
        std::forward<T1>(a), std::forward<T2>(b));
  }

  /*!
   * \brief divide a mathematical object by another
   * \tparam T1: type of the left  hand side argument
   * \tparam T2: type of the right hand side argument
   * \param  a : left  hand side argument
   * \param  b : right hand side argument
   * \return an handle which allow delayed evaluation of the
   * operation
   *
   * This operator is defined if the
   * ComputeBinaryResult<T1,T2,OpDiv> metafunctions returns valid
   * Result and Handle.
   * \see ComputeBinaryResult
   * \see IsInvalidType
   */
  template <typename T1, typename T2>
  TFEL_MATH_INLINE auto operator/(T1&& a, T2&& b) -> std::enable_if_t<
      isBinaryOperationResultTypeValid<decltype(a), decltype(b), OpDiv>::
              value &&
          (!((isScalar<T1>()) && (isScalar<T2>()))),
      BinaryOperationHandler<decltype(a), decltype(b), OpDiv>> {
    return BinaryOperationHandler<decltype(a), decltype(b), OpDiv>(
        std::forward<T1>(a), std::forward<T2>(b));
  }

  /*!
   * \brief diadic product between two mathematical objects
   * \tparam T1: type of the left  hand side argument
   * \tparam T2: type of the right hand side argument
   * \param  a : left  hand side argument
   * \param  b : right hand side argument
   * \return an handle which allow delayed evaluation of the
   * operation
   *
   * This operator is defined if the
   * ComputeBinaryResult<T1,T2,OpDiadicProduct> metafunctions
   * returns valid Result and Handle.
   * \see ComputeBinaryResult
   * \see IsInvalidType
   */
  template <typename T1, typename T2>
  TFEL_MATH_INLINE auto operator^(T1&& a, T2&& b) -> std::enable_if_t<
      isBinaryOperationResultTypeValid<decltype(a),
                                       decltype(b),
                                       OpDiadicProduct>::value &&
          (!((isScalar<T1>()) && (isScalar<T2>()))),
      BinaryOperationHandler<decltype(a), decltype(b), OpDiadicProduct>> {
    return BinaryOperationHandler<decltype(a), decltype(b), OpDiadicProduct>(
        std::forward<T1>(a), std::forward<T2>(b));
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_EXPRESSIONTEMPLATES_STANDARDOPERATIONS_HXX */
