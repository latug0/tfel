/*!
 * \file  include/TFEL/Math/Matrix/tmatrix_column_view.hxx
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

#ifndef LIB_TFEL_MATH_TMATRIX_COLUMN_VIEW_HXX
#define LIB_TFEL_MATH_TMATRIX_COLUMN_VIEW_HXX

namespace tfel::math {

  template <unsigned short N,
            unsigned short M,
            unsigned short I,
            unsigned short J,
            unsigned short K,
            typename T>
  struct tmatrix_column_view_expr {
    //! a simple typedef to the tmatrix runtime properties
    /*
     * This is a MatrixConcept requirement.
     */
    typedef EmptyRunTimeProperties RunTimeProperties;
    /*!
     * type of the tmatrix's values.
     * (this is a VectorConcept requirement).
     */
    typedef T NumType;
    /*!
     * type of the tmatrix's values.
     * (this is a stl requirement).
     */
    typedef T value_type;
    /*!
     * type of a pointer to the value contained.
     * (this is a stl requirement).
     */
    typedef value_type* pointer;
    /*!
     * type of a const pointer to the value contained.
     * (this is a stl requirement).
     */
    typedef const value_type* const_pointer;
    /*!
     * type of a reference to the value contained.
     * (this is a stl requirement).
     */
    typedef value_type& reference;
    /*!
     * type of a const reference to the value contained.
     * (this is a stl requirement).
     */
    typedef const value_type& const_reference;
    /*!
     * type of the size of the container.
     * (this is a stl requirement).
     */
    typedef unsigned short size_type;
    /*!
     * type of the difference between two iterators.
     * (this is a stl requirement).
     */
    typedef ptrdiff_t difference_type;
    /*!
     * constructor
     * \param[in] m_ : the underlying matrix
     */
    TFEL_MATH_INLINE
    tmatrix_column_view_expr(tmatrix<N, M, T>&);
    //! Return the RunTimeProperties of the tmatrix.
    /*
     * This is a VectorConcept requirement.
     * \return const RunTimeProperties, the runtime properties of
     * the tmatrix
     */
    TFEL_MATH_INLINE
    RunTimeProperties getRunTimeProperties() const;
    /*!
     * \brief index operator.
     * This is a vector concept requirement.
     * \param[in] i : index.
     * \return a reference to the ith element of the column.
     */
    TFEL_MATH_INLINE
    const T& operator()(const unsigned short i) const;
    /*!
     * \brief index operator.
     * \param[in] i : index.
     * \return a reference to the ith element of the column.
     */
    TFEL_MATH_INLINE
    T& operator()(const unsigned short);

    /*!
     * \brief index operator.
     * This is a vector concept requirement.
     * \param[in] i : index.
     * \return a reference to the ith element of the column.
     */
    TFEL_MATH_INLINE
    const T& operator[](const unsigned short) const;
    /*!
     * \brief index operator.
     * \param[in] i : index.
     * \return a reference to the ith element of the column.
     */
    TFEL_MATH_INLINE
    T& operator[](const unsigned short);

   private:
    //! reference to the underlying matrix
    tmatrix<N, M, T>& m;
  };

  template <unsigned short N,
            unsigned short M,
            unsigned short I,
            unsigned short J,
            unsigned short K,
            typename T>
  struct tmatrix_column_view
      : public Expr<tvector<K, T>, tmatrix_column_view_expr<N, M, I, J, K, T>>,
        public tvector_base<tmatrix_column_view<N, M, I, J, K, T>, K, T> {
    /*!
     * constructor
     * \param[in] m_ : the underlying matrix
     */
    TFEL_MATH_INLINE
    tmatrix_column_view(tmatrix<N, M, T>&);
    //! using tvector_base::operator=
    using tvector_base<tmatrix_column_view, K, T>::operator=;
    //! using tvector_base::operator+=
    using tvector_base<tmatrix_column_view, K, T>::operator+=;
    //! using tvector_base::operator-=
    using tvector_base<tmatrix_column_view, K, T>::operator-=;
    //! using tvector_base::operator*=
    using tvector_base<tmatrix_column_view, K, T>::operator*=;
    //! using tvector_base::operator/=
    using tvector_base<tmatrix_column_view, K, T>::operator/=;
  };

}  // end of namespace tfel::math

namespace tfel::typetraits{

  template <unsigned short N,
            unsigned short M,
            unsigned short I,
            unsigned short J,
            unsigned short K,
            typename T,
            typename T2>
  struct IsAssignableTo<tfel::math::tmatrix_column_view<N, M, I, J, K, T>,
                        tfel::math::tvector<K, T2>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<T, T2>();
  };
}

#include "TFEL/Math/Matrix/tmatrix_column_view.ixx"

#endif /* LIB_TFEL_MATH_TMATRIX_COLUMN_VIEW_HXX */
