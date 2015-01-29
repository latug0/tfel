/*!
 * \file   mfront/include/MFront/RungeKuttaDSL.hxx
 * \brief  This file declares the RungeKuttaDSL class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTRUNGEKUTTAPARSER_H_
#define _LIB_MFRONTRUNGEKUTTAPARSER_H_ 

#include<string>
#include"MFront/BehaviourDSLBase.hxx"

namespace mfront{

  struct RungeKuttaDSL
    : public BehaviourDSLBase<RungeKuttaDSL>
  {
    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    RungeKuttaDSL();

    ~RungeKuttaDSL();

  private:

    enum ErrorEvaluation{
      DEFAULTERROREVALUATION,
      ERRORSUMMATIONEVALUATION,
      MAXIMUMVALUEERROREVALUATION
    }; // end of ErrorEvaluation

    std::string
    computeStressVariableModifier1(const Hypothesis,
				   const std::string&,
				   const bool);

    std::string
    computeStressVariableModifier2(const Hypothesis,
				   const std::string&,
				   const bool);

    virtual void
    treatUnknownVariableMethod(const Hypothesis,
			       const std::string&);

    virtual void treatEpsilon(void);

    virtual void treatMinimalTimeStep(void);

    virtual void treatAlgorithm(void);

    virtual void treatComputeStress(void);

    virtual void
    treatUpdateAuxiliaryStateVariables(void);

    virtual void treatDerivative(void);

    virtual void writeBehaviourParserSpecificIncludes(void);

    virtual void writeBehaviourParserSpecificTypedefs(void);

    virtual void writeBehaviourParserSpecificMembers(const Hypothesis);

    virtual void writeBehaviourEulerIntegrator(const Hypothesis);

    virtual void writeBehaviourRK2Integrator(const Hypothesis);

    virtual void writeBehaviourRK4Integrator(const Hypothesis);

    virtual void writeBehaviourRK42Integrator(const Hypothesis);

    virtual void writeBehaviourRK54Integrator(const Hypothesis);

    virtual void writeBehaviourRKCastemIntegrator(const Hypothesis);

    virtual void writeBehaviourUpdateStateVariables(const Hypothesis);

    virtual void
    writeBehaviourUpdateAuxiliaryStateVariables(const Hypothesis);

    virtual void
    writeBehaviourIntegrator(const Hypothesis);

    virtual void
    endsInputFileProcessing(void);

    virtual void
    setDefaultAlgorithm(void);

    friend struct BehaviourDSLBase<RungeKuttaDSL>;

    // error normalisation factors
    std::map<std::string,std::string> enf;

  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTRUNGEKUTTAPARSER_H */

