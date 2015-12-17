/*! 
 * \file  mtest/include/MTest/AsterSmallStrainBehaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_MTESTASTERSMALLSTRAINBEHAVIOUR_H_
#define LIB_MTEST_MTESTASTERSMALLSTRAINBEHAVIOUR_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MTest/AsterStandardBehaviour.hxx"

namespace mtest
{

  /*!
   * A class to handle mechanical beheaviours written using the aster
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL AsterSmallStrainBehaviour
    : public AsterStandardBehaviour
  {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AsterSmallStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
				   const std::string&,
				   const std::string&);
    /*!
     * \param[out] v : initial values of the driving variables
     */
    virtual void
    getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>&) const override;
    //! destructor
    virtual ~AsterSmallStrainBehaviour();
  protected:
    /*!
     * \brief call the mechanical behaviour
     * \param[out]    Kt    : tangent operator
     * \param[in/out] s     : current state
     * \param[out]    wk    : workspace
     * \param[in]     h     : modelling hypothesis
     * \param[in]     dt    : time increment
     * \param[in]     ktype : type of the stiffness matrix
     * \param[in]     b     : if true, integrate the behaviour over the time
     * step, if false compute a prediction of the stiffness matrix
     */
    virtual bool
    call_behaviour(tfel::math::matrix<real>&,
		   CurrentState&,
		   BehaviourWorkSpace&,
		   const tfel::material::ModellingHypothesis::Hypothesis,
		   const real,
		   const StiffnessMatrixType,
		   const bool) const override;
  }; // end of struct Behaviour
  
} // end of namespace mtest

#endif /* LIB_MTEST_MTESTASTERSMALLSTRAINBEHAVIOUR_H_ */
