/*!
 * \file  mtest/include/MTest/CastemFiniteStrainBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR2_HXX
#define LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR2_HXX

#include "MTest/CastemSmallStrainBehaviour.hxx"

namespace mtest {

  /*!
   * \brief a class to handle finite strain mechanical beheaviours written
   * using the castem interface based on the ETO/PK1 kinematic
   */
  struct TFEL_VISIBILITY_LOCAL CastemFiniteStrainBehaviour2 : public CastemSmallStrainBehaviour {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    CastemFiniteStrainBehaviour2(const Hypothesis, const std::string&, const std::string&);
    //! destructor
    ~CastemFiniteStrainBehaviour2() override;

  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR2_HXX */
