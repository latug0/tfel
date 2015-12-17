/*! 
 * \file   mfront/mtest/AsterSmallStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  07 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterComputeStiffnessTensor.hxx"

#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/UmatNormaliseTangentOperator.hxx"
#include"MTest/AsterSmallStrainBehaviour.hxx"

namespace mtest
{

  AsterSmallStrainBehaviour::AsterSmallStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& l,
								 const std::string& b)
    : AsterStandardBehaviour(h,l,b)
  {}

  void
  AsterSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  }

  bool
  AsterSmallStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
					    CurrentState& s,
					    BehaviourWorkSpace& wk,
					    const tfel::material::ModellingHypothesis::Hypothesis h,
					    const real dt,
					    const StiffnessMatrixType ktype,
					    const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace aster;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    static const real sqrt2 = sqrt(real(2));
    unsigned short dimension;
    AsterInt ntens;
    AsterInt nprops = s.mprops1.size() == 0 ? 1 : static_cast<AsterInt>(s.mprops1.size());
    AsterInt nstatv;
    AsterInt nummod;
    if (h==MH::AXISYMMETRICAL){
      ntens = 4;
      dimension = 2u;
      nummod = 4u;
    } else if (h==MH::PLANESTRESS){
      ntens = 4;
      dimension = 2u;
      nummod = 5u;
    } else if (h==MH::PLANESTRAIN){
      ntens = 4;
      dimension = 2u;
      nummod = 6u;
    } else if (h==MH::TRIDIMENSIONAL){
      ntens = 6;
      dimension = 3u;
      nummod = 3u;
    } else {
      string msg("AsterSmallStrainBehaviour::call_beahviour : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    fill(wk.D.begin(),wk.D.end(),0.);
    // choosing the type of stiffness matrix
    UmatBehaviourBase::initializeTangentOperator(wk,ktype,b);
    // using a local copy of material properties to handle the
    // case where s.mprops1 is empty
    copy(s.mprops1.begin(),s.mprops1.end(),wk.mps.begin());
    if(s.mprops1.empty()){
      wk.mps[0] = real(0);
    }
    // using a local copy of internal state variables to handle the
    // case where iv0 is empty
    copy(s.iv0.begin(),s.iv0.end(),wk.ivs.begin());
    if(s.iv0.empty()){
      wk.ivs[0] = real(0);
    }
    nstatv = static_cast<AsterInt>(wk.ivs.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot = transpose(s.r);
    stensor<3u,real> ue0(real(0));
    stensor<3u,real> ude(real(0));
    copy(s.e0.begin(),s.e0.end(),ue0.begin());
    for(decltype(s.e1.size()) i=0;i!=s.e1.size();++i){
      ude(i) = s.e1(i)-s.e0(i);
    }
    copy(s.s0.begin(),s.s0.end(),s.s1.begin());
    for(AsterInt i=3;i!=static_cast<unsigned short>(ntens);++i){
      s.s1(i) /= sqrt2;
      ue0(i)  *= sqrt2;
      ude(i)  *= sqrt2;
    }
    AsterReal ndt(1.);
    (this->fct)(&(s.s1(0)),&(wk.ivs(0)),&(wk.D(0,0)),
		&ue0(0),&ude(0),&dt,
		&(s.esv0(0)),&(s.desv(0)),
		&(s.esv0(0))+1,&(s.desv(0))+1,
		&ntens,&nstatv,&(wk.mps(0)),
		&nprops,&drot(0,0),&ndt,&nummod);
    if(ndt<0.){
      return false;
    }
    if(ktype!=StiffnessMatrixType::NOSTIFFNESS){
      UmatNormaliseTangentOperator::exe(Kt,wk.D,dimension);
    }
    if(b){
      if(!s.iv0.empty()){
	copy_n(wk.ivs.begin(), s.iv1.size(),s.iv1.begin());
      }
      // turning things in standard conventions
      for(AsterInt i=3;i!=static_cast<unsigned short>(ntens);++i){
	s.s1(i) *= sqrt2;
      }
    }
    return true;
  }

  AsterSmallStrainBehaviour::~AsterSmallStrainBehaviour()
  {}
  
} // end of namespace mtest








