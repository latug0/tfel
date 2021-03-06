/*!
 * \file   include/TFEL/Math/Tensor/TensorFromTinyMatrixRowView.hxx
 * \brief
 * Tensor From Tensor Matrix Row View
 * \author Thomas Helfer
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSORFROMTINYMATRIXROWVIEW_HXX
#define LIB_TFEL_MATH_TENSORFROMTINYMATRIXROWVIEW_HXX

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel::math {

  template <unsigned short N,
            unsigned short Mn,
            unsigned short Mm,
            unsigned short In,
            unsigned short Im,
            typename T>
  struct TensorFromTinyMatrixRowViewExpr {
  };  // end of struct TensorFromTinyMatrixRowViewExpr

  template <unsigned short N,
            unsigned short Mn,
            unsigned short Mm,
            unsigned short In,
            unsigned short Im,
            typename T>
  struct Expr<tensor<N, T>,
              TensorFromTinyMatrixRowViewExpr<N, Mn, Mm, In, Im, T>>
      : public TensorConcept<
            Expr<tensor<N, T>,
                 TensorFromTinyMatrixRowViewExpr<N, Mn, Mm, In, Im, T>>>,
        public tensor_base<
            Expr<tensor<N, T>,
                 TensorFromTinyMatrixRowViewExpr<N, Mn, Mm, In, Im, T>>> {
    static_assert((N == 1u) || (N == 2u) || (N == 3u));
    static_assert(In < Mn);
    static_assert(TensorDimeToSize<N>::value <= Mm - Im);

    typedef EmptyRunTimeProperties RunTimeProperties;

    RunTimeProperties getRunTimeProperties() const {
      return RunTimeProperties();
    }

    Expr(tmatrix<Mn, Mm, T>& m_) : m(m_) {}  // end of Expr

    const T& operator()(const unsigned short i) const {
      return this->m(In, Im + i);
    }  // end of operator() const

    T& operator()(const unsigned short i) {
      return this->m(In, Im + i);
    }  // end of operator()

    const T& operator[](const unsigned short i) const {
      return this->m(In, Im + i);
    }  // end of operator[] const

    T& operator[](const unsigned short i) {
      return this->m(In, Im + i);
    }  // end of operator[]

    //! assignement operator
    using tensor_base<Expr>::operator=;

   protected:
    tmatrix<Mn, Mm, T>& m;

  };  // end of struct Expr

  template <unsigned short N,
            unsigned short Mn,
            unsigned short Mm,
            unsigned short In,
            unsigned short Im,
            typename T = double>
  struct TensorFromTinyMatrixRowView {
    typedef Expr<tensor<N, T>,
                 TensorFromTinyMatrixRowViewExpr<N, Mn, Mm, In, Im, T>>
        type;
  };  // end of struct TensorFromTinyMatrixRowView

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TENSORFROMTINYMATRIXROWVIEW_HXX */
