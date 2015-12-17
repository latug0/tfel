/*! 
 * \file  mtest/include/MTest/AsterStandardBehaviour.hxx
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

#ifndef LIB_MTEST_MTESTASTERSTANDARDBEHAVIOUR_H_
#define LIB_MTEST_MTESTASTERSTANDARDBEHAVIOUR_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MTest/UmatBehaviourBase.hxx"

namespace mtest
{

  /*!
   * A class to handle mechanical beheaviours written using the aster
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL AsterStandardBehaviour
    : public UmatBehaviourBase
  {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    AsterStandardBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
				const std::string&,
				const std::string&);
    /*!
     * \brief compute the *real* rotation matrix
     * \param[in] mp : material properties
     * \param[in] r  : rotation matrix defined by the user
     * \note this method is only meaningfull for the umat (Cast3M)
     * interface
     */
    virtual tfel::math::tmatrix<3u,3u,real>
    getRotationMatrix(const tfel::math::vector<real>&,
		      const tfel::math::tmatrix<3u,3u,real>&) const override;
    /*!
     * \return the size of the array of internal variables
     * \param[in] d : space dimension
     */
    virtual size_t
    getInternalStateVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const override;
    /*!
     * \return the descriptions the internal variables
     * \param[in] d : space dimension
     */
    virtual std::vector<std::string>
    getInternalStateVariablesDescriptions(const tfel::material::ModellingHypothesis::Hypothesis) const override;
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual StiffnessMatrixType
    getDefaultStiffnessMatrixType(void) const override;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return true if the integration was successfull, false otherwise
     * \param[out] wk    : behaviour workspace
     * \param[in]  s     : current state
     * \param[in]  dt    : time increment
     * \param[in]  ktype : type of the stiffness matrix
     */
    virtual bool
    computePredictionOperator(BehaviourWorkSpace&,
			      const CurrentState&,
			      const tfel::material::ModellingHypothesis::Hypothesis,
			      const StiffnessMatrixType) const override;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return true if the integration was successfull, false otherwise
     * \param[out/in] s     : current state
     * \param[out]    wk    : behaviour workspace
     * \param[in]     h     : modelling hypothesis
     * \param[in]     dt    : time increment
     * \param[in]     ktype : type of the stiffness matrix
     */
    virtual bool
    integrate(CurrentState&,
	      BehaviourWorkSpace&,
	      const tfel::material::ModellingHypothesis::Hypothesis,
	      const real,
	      const StiffnessMatrixType) const override;
    /*!
     * \brief allocate internal workspace
     * \param[out] wk : behaviour workspace
     * \param[in]  h  : modelling hypothesis
     */
    virtual void
    allocate(BehaviourWorkSpace&,
	     const tfel::material::ModellingHypothesis::Hypothesis) const override;
    //! destructor
    virtual ~AsterStandardBehaviour();
  protected:
    /*!
     * \brief call the mechanical behaviour
     * \param[out]    Kt    : tangent operator
     * \param[in/out] s     : current state
     * \param[out]    wk    : behaviour workspace
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
		   const bool) const = 0;
    //! the aster fonction
    tfel::system::AsterFctPtr fct;
    //! save tangent operator
    bool savesTangentOperator;
  }; // end of struct Behaviour
  
} // end of namespace mtest

#endif /* LIB_MTEST_MTESTASTERSTANDARDBEHAVIOUR_H_ */
