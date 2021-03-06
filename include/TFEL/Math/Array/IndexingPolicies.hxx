/*!
 * \file  include/TFEL/Math/Array/IndexingPolicies.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_INDEXINGPOLICIES_HXX
#define LIB_TFEL_MATH_ARRAY_INDEXINGPOLICIES_HXX

#include <tuple>
#include <utility>
#include <type_traits>

namespace tfel::math {

  /*!
   * `IndexingPolicy` concept
   *
   * A class matching the `IndexingPolicy` concept must provide:
   *
   * - An alias called `size_type` to the type used to index data.
   * - An alias called `RowMajorIndexingPolicy` which can be used to import data
   *   from raw `C` pointers.
   * - A static member data called `arity` of type `size_type`.
   * - A static boolean data member called `hasFixedSizes`. If this data
   *   member is `true`, the class must be stateless, i.e. empty.
   * - A static boolean data member called `areDataContiguous` which states if
   *   the data are continous
   * - A member function called `size` which takes no argument returning the
   *   number of data values accessible by the indexing policy.  This member
   *   function must be `constexpr` if `hasFixedSizes` is true.
   * - A member function called `size` which takes a `size_type` argument
   *   returning range in the given dimension. This member function must
   *   be `constexpr` if `hasFixedSizes` is true.
   * - A member function called `getUnderlyingArrayMinimalSize` which returns
   *   the minimal size of an array required by the indexing policy. This member
   *   function must be `constexpr` if `hasFixedSizes` is true.
   */

  /*!
   * \brief a simple function to check that the type of the indices are
   * compatible with the given indexing policy
   * \tparam IndexingPolicy: the indexing policy
   * \tparam Indices: list of indices types
   */
  template <typename IndexingPolicy, typename... Indices>
  constexpr void checkIndicesValiditity() noexcept {
    static_assert(sizeof...(Indices) == IndexingPolicy::arity,
                  "invalid number of indices");
    static_assert(
        (std::is_convertible_v<Indices, typename IndexingPolicy::size_type> &&
         ...) &&
            (std::is_integral_v<Indices> && ...),
        "invalid indices type");
  }  // end of checkIndicesValidity

  /*!
   * \brief a base class to compare indexing policies which provide default
   * implementations.
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \note the default implementations are very conservative. For example,
   * the memory layout are reported to be different by default.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  struct IndexingPoliciesCompatiblityCheckBase {
    /*!
     * \return if the given indexing policies may *a priori* be compatible.
     */
    static constexpr bool performCompileTimeCheck();
    /*!
     * \return if the indexing policies have the same memory layouts.
     *
     * Two indexing policies can be compatible (at compile-time and at runtime),
     * i.e. one may transfer data from one indexing policy to another, but the
     * memory layout of the data may differ. For example, one may assign a
     * matrix stored in column-major format to a matrix having a row-major
     * format.
     */
    static constexpr bool haveTheSameMemoryLayout();
    /*!
     * \return if the given policies are compatible at runtime time.
     * \pre this method is only meaningful if the `compileTimeCheck` method
     * returns `true`.
     * \param[in] p1: first indexing policy
     * \param[in] p2: second indexing policy
     * \note This methods does nothing if both indexing policies have fixed size
     * (i.e. known at compile-time).
     */
    static constexpr bool performRuntimeTimeCheck(const IndexingPolicy1&,
                                                  const IndexingPolicy2&);
  };  // end of struct IndexingPoliciesCompatiblityCheck

  /*!
   * \brief a structure used to compare two indexing policies
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \note this structure is meant to be specialised in specific cases.
   * \note this structure is no meant to be used directly in most cases and the
   * use the for `checkIndexingPoliciesCompatiblity`,
   * `checkIndexingPoliciesRuntimeCompatiblity`,
   * `haveIndexingPoliciesTheSameMemoryLayout` helper functions.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  struct IndexingPoliciesCompatiblityCheck
      : IndexingPoliciesCompatiblityCheckBase<IndexingPolicy1,
                                              IndexingPolicy2> {
  };  // end of struct IndexingPoliciesCompatiblityCheck

  /*!
   * \brief check if two indexing policies can be compatible.
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \note this function is a simple wrapper around
   * `IndexingPoliciesCompatiblityCheck::checkIndexingPoliciesCompatiblity`.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr bool checkIndexingPoliciesCompatiblity();
  /*!
   * \return if two indexing policies have the same memory layout.
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \note this function is a simple wrapper around
   * `IndexingPoliciesCompatiblityCheck::haveTheSameMemoryLayout`.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr bool haveIndexingPoliciesTheSameMemoryLayout();
  /*!
   * \return if two indexing policies are compatible at runtime.
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \param[in] p1: first indexing policy
   * \param[in] p2: second indexing policy
   * \note this function is a simple wrapper around
   * `IndexingPoliciesCompatiblityCheck::runtimeTimeCheck`.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr bool areIndexingPoliciesCompatibleAtRunTime(const IndexingPolicy1&,
                                                        const IndexingPolicy2&);

  /*!
   * \brief check if two indexing policies are compatible at runtime.
   * \tparam IndexingPolicy1: first indexing policy
   * \tparam IndexingPolicy2: second indexing policy
   * \param[in] p1: first indexing policy
   * \param[in] p2: second indexing policy
   * \note this function simply calls
   * `areIndexingPoliciesCompatibleAtRunTime` and reports a contract violation
   * if the two indexing policies are not compatible.
   */
  template <typename IndexingPolicy1, typename IndexingPolicy2>
  constexpr void checkIndexingPoliciesRuntimeCompatiblity(
      const IndexingPolicy1&, const IndexingPolicy2&);

  template <typename IndexingPolicy, typename... T, std::size_t... I>
  constexpr auto buildIndexingPolicyFromTuple(const std::tuple<T...>&,
                                              const std::index_sequence<I...>&);

  template <typename IndexingPolicy, typename... Args>
  constexpr auto buildIndexingPolicyAndExtractPointerToData(Args&... args);

}  // end of namespace tfel::math

#include "TFEL/Math/Array/IndexingPolicies.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_INDEXINGPOLICIES_HXX */
