/*! 
 * \file  mfront/src/BehaviourDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 mars 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>
#include<string>
#include<sstream>
#include<stdexcept>
#include<algorithm>

#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/LocalDataStructure.hxx"
#include"MFront/BehaviourDescription.hxx"

namespace mfront
{

  template<typename Arg1>
  void
  BehaviourDescription::callBehaviourData(const Hypothesis h,
					  void (BehaviourData:: *m)(const Arg1&),
					  const Arg1& a,
					  const bool b)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->d.*m)(a);
      if(b){
	for(auto md : this->sd){
	  (md.second.get()->*m)(a);
	}
      }
    } else {
      (this->getBehaviourData2(h).*m)(a);
    }
  } // end of BehaviourDescription::callBehaviourData

  template<typename Arg1>
  void
  BehaviourDescription::callBehaviourData(const Hypothesis h,
					  void (BehaviourData:: *m)(const Arg1),
					  const Arg1 a,
					  const bool b)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->d.*m)(a);
      if(b){
	for(const auto& ptr : this->sd){
	  auto& bdata = *(ptr.second);
	  (bdata.*m)(a);
	}
      }
    } else {
      (this->getBehaviourData2(h).*m)(a);
    }
  } // end of BehaviourDescription::callBehaviourData

  template<typename Arg1,typename Arg2>
  void
  BehaviourDescription::callBehaviourData(const Hypothesis h,
					  void (BehaviourData:: *m)(const Arg1&,
								    const Arg2),
					  const Arg1& a1,
					  const Arg2  a2,
					  const bool b)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->d.*m)(a1,a2);
      if(b){
	for(const auto& ptr : this->sd){
	  auto& bdata = *(ptr.second);
	  (bdata.*m)(a1,a2);
	}
      }
    } else {
      (this->getBehaviourData2(h).*m)(a1,a2);
    }
  } // end of BehaviourDescription::callBehaviourData

  template<typename Arg1,
	   typename Arg2>
  void
  BehaviourDescription::callBehaviourData(const Hypothesis h,
					  void (BehaviourData:: *m)(const Arg1&,
								    const Arg2&),
					  const Arg1& a1,
					  const Arg2& a2,
					  const bool b)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      (this->d.*m)(a1,a2);
      if(b){
	for(const auto& ptr : this->sd){
	  auto& bdata = *(ptr.second);
	  (bdata.*m)(a1,a2);
	}
      }
    } else {
      (this->getBehaviourData2(h).*m)(a1,a2);
    }
  } // end of BehaviourDescription::callBehaviourData

  template<typename Res,typename Arg1>
  Res
  BehaviourDescription::getData(const Hypothesis h,
				Res (BehaviourData:: *m)(const Arg1&) const,
				const Arg1& a) const
  {
    return (this->getBehaviourData(h).*m)(a);
  } // end of BehaviourDescription::getData

  static void
  declareParameter(BehaviourDescription& bd,
		   BehaviourDescription::MaterialProperty& mp,
		   const tfel::glossary::GlossaryEntry& e,
		   const std::string& n){
    const auto h = tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(mp.is<BehaviourDescription::ConstantMaterialProperty>()){
      auto& cmp = mp.get<BehaviourDescription::ConstantMaterialProperty>();
      cmp.name  = n;
      // declare associated parameter
      VariableDescription m("real",n,1u,0u);
      bd.addParameter(h,m);
      bd.setParameterDefaultValue(h,n,cmp.value);
      bd.setGlossaryName(h,n,e.getKey());
    }
  } // end of BehaviourDescription::declareParameter
  
  static void
  checkElasticMaterialProperty(BehaviourDescription& bd,
			       BehaviourDescription::MaterialProperty& emp,
			       const tfel::glossary::GlossaryEntry& e,
			       const std::string& n2){
    if(emp.is<BehaviourDescription::ComputedMaterialProperty>()){
      const auto& mpd = *(emp.get<BehaviourDescription::ComputedMaterialProperty>().mpd);
      
      const auto ename = [&mpd](){
	auto p = mpd.glossaryNames.find(mpd.output);
	if(p!=mpd.glossaryNames.end()){
	  return p->second;
	}
	p = mpd.entryNames.find(mpd.output);
	if(p!=mpd.entryNames.end()){
	  return p->second;
	}
	return mpd.output;
      }();
      if(ename!=e){
	auto& log = getLogStream();
	log << "checkElasticMaterialProperty: inconsistent external name for "
	    << "material property '"+e.getKey()+"': external name of mfront file "
	    << "output  is '" << ename << "'\n";
      }
    }
    declareParameter(bd,emp,e,n2);
  }
  
  static void
  checkThermalExpansionCoefficientArgument(BehaviourDescription& bd,
							       BehaviourDescription::MaterialProperty& a,
							       const tfel::glossary::GlossaryEntry& e,
							       const std::string& n)
  {
    declareParameter(bd,a,e,n);
    if(!a.is<BehaviourDescription::ComputedMaterialProperty>()){
      return;
    }
    const auto& mpd = *(a.get<BehaviourDescription::ComputedMaterialProperty>().mpd);
    if(!((mpd.inputs.size())||(mpd.inputs.size()!=1u))){
      throw(std::runtime_error("checkThermalExpansionCoefficientArgument: "
			       "thermal expansion shall only depend on temperature or be constant"));
    }
    if(mpd.inputs.size()==1u){
      const auto& v = mpd.inputs.front();
      const auto& vn = v.getExternalName(mpd.glossaryNames,mpd.entryNames);
      if(vn!="Temperature"){
	throw(std::runtime_error("checkThermalExpansionCoefficientArgument: "
				 "thermal expansion shall only depend on temperature"));
      }
    }
  } // end of checkThermalExpansionCoefficientArgument

  const std::string
  BehaviourDescription::requiresStiffnessTensor("requiresStiffnessTensor");

  const std::string
  BehaviourDescription::computesStiffnessTensor("computesStiffnessTensor");
  
  const std::string
  BehaviourDescription::requiresUnAlteredStiffnessTensor("requiresUnAlteredStiffnessTensor");
  
  const std::string
  BehaviourDescription::requiresThermalExpansionCoefficientTensor("requiresThermalExpansionCoefficientTensor");
  
  void
  BehaviourDescription::throwUndefinedAttribute(const std::string& n)
  {
    throw(std::runtime_error("BehaviourDescription::getAttribute: "
			     "no attribute named '"+n+"'"));
  } // end of BehaviourDescription::throwUndefinedAttribute

  BehaviourDescription::BehaviourDescription()
    : use_qt(false),
      type(BehaviourDescription::GENERALBEHAVIOUR),
      // By default, a behaviour is isotropic 
      stype(mfront::ISOTROPIC),
      stypeIsDefined(false),
      // By default, a behaviour is isotropic 
      estype(mfront::ISOTROPIC),
      estypeIsDefined(false),
      ischeme(UNDEFINEDINTEGRATIONSCHEME)
  {} // end of BehaviourDescription::BehaviourDescription()

  const BehaviourData&
  BehaviourDescription::getBehaviourData(const Hypothesis& h) const
  {
    // check that the given hypothesis is supported
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return this->d;
    }
    this->checkModellingHypothesis(h);
    this->requestedHypotheses.insert(h);
    // check if a specialised version of the behaviour
    // description has been defined
    const auto p = this->sd.find(h);
    if(p!=this->sd.end()){
      return *(p->second);
    }
    // return the default...
    return this->d;
  } // end of BehaviourDescription::getBehaviourData

  BehaviourData&
  BehaviourDescription::getBehaviourData2(const ModellingHypothesis::Hypothesis& h)
  {
    // check that the given hypothesis is supported
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return this->d;
    }
    if(this->areModellingHypothesesDefined()){
      this->checkModellingHypothesis(h);
    }
    this->requestedHypotheses.insert(h);
    auto p = this->sd.find(h);
    if(p==this->sd.end()){
      // copy of the default description
      p=this->sd.insert({h,MBDPtr{new BehaviourData{this->d}}}).first;
    }
    return *(p->second);
  } // end of BehaviourDescription::getBehaviourData2

  void
  BehaviourDescription::setBehaviourName(const std::string& m)
  {
    if(!this->behaviour.empty()){
      throw(std::runtime_error("BehaviourDescription::setBehaviourName: "
			       "behaviour name already defined"));
    }
    this->behaviour = m;
    this->updateClassName();
  } // end of BehaviourDescription::setBehaviourName

  const std::string&
  BehaviourDescription::getBehaviourName(void) const
  {
    if(this->behaviour.empty()){
      throw(std::runtime_error("BehaviourDescription::getBehaviourName: "
			       "behaviour name not defined"));
    }
    return this->behaviour;
  } // end of BehaviourDescription::getBehaviourName

  void
  BehaviourDescription::setDSLName(const std::string& m)
  {
    if(!this->dsl.empty()){
      throw(std::runtime_error("BehaviourDescription::setDSLName: "
			       "dsl name already defined"));
    }
    this->dsl = m;
    this->updateClassName();
  } // end of BehaviourDescription::setDSLName

  const std::string&
  BehaviourDescription::getDSLName(void) const
  {
    if(this->dsl.empty()){
      throw(std::runtime_error("BehaviourDescription::getDSLName: "
			       "dsl name not defined"));
    }
    return this->dsl;
  } // end of BehaviourDescription::getDSLName

  std::vector<BehaviourDescription::MaterialPropertyInput>
  BehaviourDescription::getMaterialPropertyInputs(const MaterialPropertyDescription& mpd) const
  {
    auto getVariableType = [](const BehaviourDescription& bd,
			      const Hypothesis h,
			      const std::string& v){
      if(bd.isExternalStateVariableName(h,v)){
	return MaterialPropertyInput::EXTERNALSTATEVARIABLE;
      } else if(bd.isMaterialPropertyName(h,v)){
	return MaterialPropertyInput::MATERIALPROPERTY;
      } else if(bd.isParameterName(h,v)){
	return MaterialPropertyInput::PARAMETER;
      } else {
	throw(std::runtime_error("BehaviourDescription::getMaterialPropertyInputs: "
				 "unsupported variable: variable '"+v+"' is "
				 "neither an external state variable, a material "
				 "property nor a parameter"));
      }
    };
    auto inputs = std::vector<MaterialPropertyInput>{};
    for(const auto& v : mpd.inputs){
      if((mpd.glossaryNames.find(v.name)==mpd.glossaryNames.end())&&
    	 (mpd.entryNames.find(v.name)==mpd.entryNames.end())){
    	throw(std::runtime_error("BehaviourDescription::getMaterialPropertyInputs : "
				 "no glossary nor entry name declared for variable "
				 "'"+v.name+"' used by the material property "
				 "'"+mpd.law+"'"));
      }
      const auto& vn = v.getExternalName(mpd.glossaryNames,mpd.entryNames);
      if(vn==tfel::glossary::Glossary::Temperature){
	inputs.push_back({"T",tfel::glossary::Glossary::Temperature,
	      MaterialPropertyInput::TEMPERATURE});
      } else {
	auto hs = this->getDistinctModellingHypotheses();
	const auto n =
	  this->getVariableNameFromGlossaryNameOrEntryName(*(hs.begin()),vn);
	const auto t = getVariableType(*this,*(hs.begin()),n);
	for(const auto h:hs){
	  if(this->getVariableNameFromGlossaryNameOrEntryName(h,vn)!=n){
	    throw(std::runtime_error("BehaviourDescription::getMaterialPropertyInputs: "
				    "the external name '"+vn+"' is associated with "
				    "two differents variables in two distinct "
				     "modelling hypotheses. This is not supported."));
	  }
	  if(getVariableType(*this,h,n)!=t){
	    throw(std::runtime_error("BehaviourDescription::getMaterialPropertyInputs: "
				    "the external name '"+vn+"' has two different "
				    "types in two distinct modelling hypotheses. "
				     "This is not supported."));
	  }
	}
	inputs.push_back({n,vn,t});
      }
    }
    return inputs;
  } // end of BehaviourDescription::getMaterialPropertyInputs
  
  void
  BehaviourDescription::setIntegrationScheme(const BehaviourDescription::IntegrationScheme s)
  {
    if(this->ischeme!=UNDEFINEDINTEGRATIONSCHEME){
      throw(std::runtime_error("BehaviourDescription::setIntegrationScheme: "
			       "integration scheme already defined"));
    }
    this->ischeme = s;
  } // end of BehaviourDescription::setIntegrationScheme

  BehaviourDescription::IntegrationScheme
  BehaviourDescription::getIntegrationScheme(void) const
  {
    if(this->ischeme==UNDEFINEDINTEGRATIONSCHEME){
      throw(std::runtime_error("BehaviourDescription::getIntegrationScheme: "
			       "the integration scheme is undefined"));
    }
    return this->ischeme;
  } // end of BehaviourDescription::getIntegrationScheme

  void
  BehaviourDescription::setLibrary(const std::string& l)
  {
    if(!this->library.empty()){
      throw(std::runtime_error("BehaviourDescription::setLibrary: "
			       "library alreay defined"));
    }
    this->library = l;
  } // end of BehaviourDescription::setLibrary

  const std::string&
  BehaviourDescription::getLibrary(void) const
  {
    return this->library;
  } // end of BehaviourDescription::getLibrary

  void
  BehaviourDescription::setMaterialName(const std::string& m)
  {
    if(!this->material.empty()){
      throw(std::runtime_error("BehaviourDescription::setMaterialName: "
			       "material name alreay defined"));
    }
    this->material = m;
    this->updateClassName();
  } // end of BehaviourDescription::setMaterialName

  const std::string&
  BehaviourDescription::getMaterialName(void) const
  {
    return this->material;
  } // end of BehaviourDescription::getMaterialName

  void
  BehaviourDescription::setClassName(const std::string& n)
  {
    if(!this->className.empty()){
      throw(std::runtime_error("BehaviourDescription::setClassName: "
			       "class name alreay defined"));
    }
    this->className = n;
  } // end of BehaviourDescription::setClassName

  const std::string&
  BehaviourDescription::getClassName(void) const
  {
    if(this->className.empty()){
      throw(std::runtime_error("BehaviourDescription::getClassName: "
			       "class name not defined"));
    }
    return this->className;
  } // end of BehaviourDescription::getClassName

  void
  BehaviourDescription::appendToIncludes(const std::string& c)
  {
    this->includes+=c;
    if(!this->includes.empty()){
      if(*(this->includes.rbegin())!='\n'){
	this->includes+='\n';
      }
    }
  } // end of BehaviourDescription::appendToIncludes

  const std::string&
  BehaviourDescription::getIncludes(void) const
  {
    return this->includes;
  } // end of BehaviourDescription::getIncludes

  void
  BehaviourDescription::appendToMembers(const Hypothesis h,
					const std::string& c,
					const bool b)
  {
    this->callBehaviourData(h,&BehaviourData::appendToMembers,c,b);
  } // end of BehaviourDescription::appendToMembers

  const std::string BehaviourDescription::getMembers(const Hypothesis h) const
  {
    return this->getBehaviourData(h).getMembers();
  } // end of BehaviourDescription::getMembers

  void BehaviourDescription::appendToPrivateCode(const Hypothesis h,
						 const std::string& c,
						 const bool b)
  {
    this->callBehaviourData(h,&BehaviourData::appendToPrivateCode,c,b);
  } // end of BehaviourDescription::appendToPrivateCode

  const std::string BehaviourDescription::getPrivateCode(const Hypothesis h) const
  {
    return this->getBehaviourData(h).getPrivateCode();
  } // end of BehaviourDescription::getPrivateCode

  void
  BehaviourDescription::appendToSources(const std::string& c)
  {
    this->sources+=c;
    if(!this->sources.empty()){
      if(*(this->sources.rbegin())!='\n'){
	this->sources+='\n';
      }
    }
  } // end of BehaviourDescription::appendToSources

  const std::string&
  BehaviourDescription::getSources(void) const
  {
    return this->sources;
  } // end of BehaviourDescription::getSources

  BehaviourDescription::BehaviourType
  BehaviourDescription::getBehaviourType() const
  {
    return this->type;
  } // end of BehaviourDescription::getBehaviourType

  std::string
  BehaviourDescription::getBehaviourTypeFlag(void) const
  {
    std::string btype;
    if(this->getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      btype="MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR";
    } else if (this->getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      btype="MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR";
    } else if (this->getBehaviourType()==BehaviourDescription::COHESIVEZONEMODEL){
      btype="MechanicalBehaviourBase::COHESIVEZONEMODEL";
    } else {
      throw(std::runtime_error("BehaviourDescription::getBehaviourTypeFlag: "
			       "unsupported behaviour type"));
    }
    return btype;
  } // end of BehaviourDescription::getBehaviourTypeFlag

  bool
  BehaviourDescription::areElasticMaterialPropertiesDefined(void) const
  {
    return !this->elasticMaterialProperties.empty();
  } // end of BehaviourDescription::areElasticMaterialPropertiesDefined

  bool
  BehaviourDescription::isMaterialPropertyConstantDuringTheTimeStep(const MaterialProperty& mp) const{
    if(mp.is<ComputedMaterialProperty>()){
      const auto& cmp = mp.get<ComputedMaterialProperty>();
      for(const auto& i : this->getMaterialPropertyInputs(*(cmp.mpd))){
	if(!((i.type==BehaviourDescription::MaterialPropertyInput::MATERIALPROPERTY)||
	     (i.type==BehaviourDescription::MaterialPropertyInput::PARAMETER))){
	  return false;
	}
      }
      return true;
    }
    return true;
  } // end of BehaviourDescription::isMaterialPropertyConstantDuringTheTimeStep
  
  bool
  BehaviourDescription::areElasticMaterialPropertiesConstantDuringTheTimeStep(void) const{
    if(!this->areElasticMaterialPropertiesDefined()){
      throw(std::runtime_error("BehaviourDescription::getElasticMaterialProperties: "
			       "no elastic material property defined"));
    }
    for(const auto& mp : this->elasticMaterialProperties){
      if(!this->isMaterialPropertyConstantDuringTheTimeStep(mp)){
	return false;
      }
    }
    return true;
  } // end of BehaviourDescription::areElasticMaterialPropertiesConstantDuringTheTimeStep
  
  const std::vector<BehaviourDescription::MaterialProperty>&
  BehaviourDescription::getElasticMaterialProperties(void) const
  {
    if(!this->areElasticMaterialPropertiesDefined()){
      throw(std::runtime_error("BehaviourDescription::getElasticMaterialProperties: "
			       "no elastic material property defined"));
    }
    return this->elasticMaterialProperties;
  }
  
  void
  BehaviourDescription::setElasticMaterialProperties(const std::vector<MaterialProperty>& emps)
  {
    if((this->getBehaviourType()!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)&&
       (this->getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)){
      throw(std::runtime_error("BehaviourDescription::setElasticMaterialProperties: "
			       "only finite and strain behaviour are supported"));
    }
    this->setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
		       BehaviourDescription::requiresStiffnessTensor,false);
    if(!this->elasticMaterialProperties.empty()){
      throw(std::runtime_error("BehaviourDescription::setElasticMaterialProperties: "
			       "elastic material property already declared"));
    }
    auto lemps = emps; // local copy, swap to data member if no exceptions is thrown
    if(emps.size()==2u){
      if(this->isElasticSymmetryTypeDefined()){
	if(this->getElasticSymmetryType()!=mfront::ISOTROPIC){
	  throw(std::runtime_error("BehaviourDescription::setElasticMaterialProperties: "
				   "inconsistent elastic symmetry type"));
	}
      } else {
	this->setElasticSymmetryType(mfront::ISOTROPIC);
      }
      checkElasticMaterialProperty(*this,lemps[0],tfel::glossary::Glossary::YoungModulus,"young");
      checkElasticMaterialProperty(*this,lemps[1],tfel::glossary::Glossary::PoissonRatio,"nu");
    } else if(emps.size()==9u){
      if(this->getSymmetryType()!=mfront::ORTHOTROPIC){
	throw(std::runtime_error("BehaviourDescription::setElasticMaterialProperties: "
				 "the behaviour is not orthotropic."));
      }
      if(this->isElasticSymmetryTypeDefined()){
	if(this->getElasticSymmetryType()!=mfront::ORTHOTROPIC){
	  throw(std::runtime_error("BehaviourDescription::setElasticMaterialProperties: "
				   "inconsistent elastic symmetry type"));
	}
      } else {
	this->setElasticSymmetryType(mfront::ORTHOTROPIC);
      }
      checkElasticMaterialProperty(*this,lemps[0],tfel::glossary::Glossary::YoungModulus1,"young1");
      checkElasticMaterialProperty(*this,lemps[1],tfel::glossary::Glossary::YoungModulus2,"young2");
      checkElasticMaterialProperty(*this,lemps[2],tfel::glossary::Glossary::YoungModulus3,"young3");
      checkElasticMaterialProperty(*this,lemps[3],tfel::glossary::Glossary::PoissonRatio12,"nu12");
      checkElasticMaterialProperty(*this,lemps[4],tfel::glossary::Glossary::PoissonRatio23,"nu23");
      checkElasticMaterialProperty(*this,lemps[5],tfel::glossary::Glossary::PoissonRatio13,"nu13");
      checkElasticMaterialProperty(*this,lemps[6],tfel::glossary::Glossary::ShearModulus12,"mu12");
      checkElasticMaterialProperty(*this,lemps[7],tfel::glossary::Glossary::ShearModulus23,"mu23");
      checkElasticMaterialProperty(*this,lemps[8],tfel::glossary::Glossary::ShearModulus13,"mu13");
    } else {
      throw(std::runtime_error("BehaviourDescription::setElasticMaterialProperties: "
			       "unsupported behaviour type"));
    }
    this->elasticMaterialProperties.swap(lemps);
  } // end of BehaviourDescription::setElasticMaterialProperties
    
  BehaviourSymmetryType
  BehaviourDescription::getElasticSymmetryType() const
  {
    if(!this->estypeIsDefined){
      this->estype = this->getSymmetryType();
      this->estypeIsDefined=true;
    }
    return this->estype;
  } // end of BehaviourDescription::getElasticSymmetryType

  void
  BehaviourDescription::setElasticSymmetryType(const BehaviourSymmetryType t)
  {
    if(this->estypeIsDefined){
      throw(std::runtime_error("BehaviourDescription::setElasticSymmetryType: "
			       "elastic symmetry type already declared"));
    }
    const auto s = this->getSymmetryType();
    if((s==mfront::ISOTROPIC)&&(t==mfront::ORTHOTROPIC)){
      throw(std::runtime_error("BehaviourDescription::setElasticSymmetryType: "
			       "can't define an orthotropic elastic symmetry for "
			       "an isotropic material"));
    }
    this->estype = t;
    this->estypeIsDefined=true;
  } // end of BehaviourDescription::setElasticSymmetryType

  bool
  BehaviourDescription::isElasticSymmetryTypeDefined() const
  {
    return this->estypeIsDefined;
  } // end of BehaviourDescription::isElasticSymmetryTypeDefined

  BehaviourSymmetryType
  BehaviourDescription::getSymmetryType() const
  {
    if(!this->stypeIsDefined){
      this->stype = mfront::ISOTROPIC;
      this->stypeIsDefined=true;
    }
    return this->stype;
  } // end of BehaviourDescription::getSymmetryType

  void
  BehaviourDescription::setSymmetryType(const BehaviourSymmetryType t)
  {
    if(this->stypeIsDefined){
      throw(std::runtime_error("BehaviourDescription::setSymmetryType: "
			       "symmetry type already declared"));
    }
    this->stype = t;
    this->stypeIsDefined=true;
  } // end of BehaviourDescription::setSymmetryType
  
  bool
  BehaviourDescription::isSymmetryTypeDefined() const
  {
    return this->stypeIsDefined;
  } // end of BehaviourDescription::setSymmetryType
  
  void
  BehaviourDescription::declareAsASmallStrainStandardBehaviour(void)
  {
    if(!this->mvariables.empty()){
      throw(std::runtime_error("BehaviourDescription::declareAsASmallStrainStandardBehaviour: "
			       "some driving variables are already declared"));
    }
    DrivingVariable eto;
    eto.name = "eto";
    eto.type = "StrainStensor";
    eto.increment_known = true;
    ThermodynamicForce sig;
    sig.name = "sig";
    sig.type = "StressStensor";
    this->mvariables.insert({eto,sig});
    this->type = BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR;
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"eto");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"deto");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"sig");
  }

  void
  BehaviourDescription::declareAsAFiniteStrainStandardBehaviour(void)
  {
    if(!this->mvariables.empty()){
      throw(std::runtime_error("BehaviourDescription::declareAsAFiniteStrainStandardBehaviour: "
			       "some driving variables are already declared"));
    }
    DrivingVariable F;
    F.name = "F";
    F.type = "DeformationGradientTensor";
    F.increment_known = false;
    ThermodynamicForce sig;
    sig.name = "sig";
    sig.type = "StressStensor";
    this->mvariables.insert({F,sig});
    this->type = BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR;
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"F0");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"F1");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"sig");
  }
  
  void
  BehaviourDescription::declareAsACohesiveZoneModel(void)
  {
    if(!this->mvariables.empty()){
      throw(std::runtime_error("BehaviourDescription::declareAsACohesiveZoneModel: "
			       "some driving variables are already declared"));
    }
    DrivingVariable u;
    u.name = "u";
    u.type = "DisplacementTVector";
    u.increment_known = true;
    ThermodynamicForce t;
    t.name = "t";
    t.type = "ForceTVector";
    this->mvariables.insert({u,t});
    this->type = BehaviourDescription::COHESIVEZONEMODEL;
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"u");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"du");
    this->registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,"t");
  }

  void
  BehaviourDescription::addLocalDataStructure(const LocalDataStructure& lds,
					      const BehaviourData::RegistrationStatus s){
    auto gs = [](const std::vector<LocalDataStructure::Variable>& vars){
      auto r = std::string{};
      r = "struct{\n";
      for(const auto& v : vars){
	r += v.type +' '+v.name+";\n";
      }
      r += "}";
      return r;
    };
    if(!tfel::utilities::CxxTokenizer::isValidIdentifier(lds.name,true)){
      throw(std::runtime_error("BehaviourDSLCommon::addLocalDataStructure: "
			       "invalid local structure name '"+lds.name+"'"));
    }
    const auto mh = lds.getSpecialisedHypotheses();
    for(const auto h:mh){
      if(!lds.get(h).empty()){ // paranoiac checks, this can't occur
	this->addLocalVariable(h,{gs(lds.get(h)),lds.name,1u,0u},s);
      }
    }
    const auto v = lds.get(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    if(v.empty()){
      return;
    }
    auto vd = VariableDescription{gs(v),lds.name,1u,0u};
    if(!this->areAllMechanicalDataSpecialised()){
      this->d.addLocalVariable(vd,s);
    }
    for(auto& ld : this->sd){
      if(std::find(mh.begin(),mh.end(),ld.first)==mh.end()){
	ld.second->addLocalVariable(vd,s);
      }
    }
  }
  
  void
  BehaviourDescription::addMainVariable(const DrivingVariable&    v,
					const ThermodynamicForce& f)
  {
    if(this->type!=BehaviourDescription::GENERALBEHAVIOUR){
      throw(std::runtime_error("BehaviourDescription::addMainVariables: "
			       "one can not add a main variable if the behaviour "
			       "don't have a general behaviour type"));
    }
    if(!this->mvariables.insert({v,f}).second){
      throw(std::runtime_error("BehaviourDescription::addMainVariables: "
			       "a driving variable '"+v.name+"' has "
			       "already been declared"));
    }
  } // end of BehaviourDescription::addMainVariables

  const std::map<DrivingVariable,ThermodynamicForce>&
  BehaviourDescription::getMainVariables(void) const
  {
    return this->mvariables;
  } // end of BehaviourDescription::getMainVariables
  
  bool
  BehaviourDescription::isDrivingVariableName(const std::string& n) const
  {
    for(const auto& v : this->getMainVariables()){
      if(v.first.name==n){
	return true;
      }
    }
    return false;
  } // end of BehaviourDescription::isDrivingVariableName

  bool
  BehaviourDescription::isDrivingVariableIncrementName(const std::string& n) const
  {
    for(const auto& v : this->getMainVariables()){
      const auto& dv = v.first;
      if(dv.increment_known){
	if("d"+dv.name==n){
	  return true;
	}
      }
    }
    return false;
  } // end of BehaviourDescription::isDrivingVariableIncrementName

  std::pair<SupportedTypes::TypeSize,SupportedTypes::TypeSize>
  BehaviourDescription::getMainVariablesSize(void) const
  {
    auto ov = SupportedTypes::TypeSize{};
    auto of = SupportedTypes::TypeSize{};
    for(const auto& v : this->getMainVariables()){
      ov += this->getTypeSize(v.first.type,1u);
      of += this->getTypeSize(v.second.type,1u);
    }
    return {ov,of};
  } // end of BehaviourDescription::getMainVariablesSize

  void
  BehaviourDescription::setThermalExpansionCoefficient(MaterialProperty a)
  {
    using tfel::glossary::Glossary;
    if(this->areThermalExpansionCoefficientsDefined()){
      throw(std::runtime_error("BehaviourDescription::setThermalExpansionCoefficient: "
			       "thermal expansion coefficient already defined"));
    }
    this->setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
		       BehaviourDescription::requiresThermalExpansionCoefficientTensor,false);
    checkThermalExpansionCoefficientArgument(*this,a,Glossary::ThermalExpansion,"alpha");
    this->thermalExpansionCoefficients.push_back(a);
  } // end of BehaviourDescription::setThermalExpansionCoefficient

  void
  BehaviourDescription::setThermalExpansionCoefficients(MaterialProperty a1,
							MaterialProperty a2,
							MaterialProperty a3)
  {
    using tfel::glossary::Glossary;
    if(this->areThermalExpansionCoefficientsDefined()){
      throw(std::runtime_error("BehaviourDescription::setThermalExpansionCoefficient: "
			       "thermal expansion coefficient already defined"));
    }
    if(this->getSymmetryType()!=mfront::ORTHOTROPIC){
      throw(std::runtime_error("BehaviourDescription::setThermalExpansionCoefficient: "
			       "the behaviour is not orthotropic."));
    }
    this->setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
		       BehaviourDescription::requiresThermalExpansionCoefficientTensor,false);
    checkThermalExpansionCoefficientArgument(*this,a1,Glossary::ThermalExpansion1,"alpha1");
    checkThermalExpansionCoefficientArgument(*this,a2,Glossary::ThermalExpansion2,"alpha2");
    checkThermalExpansionCoefficientArgument(*this,a3,Glossary::ThermalExpansion3,"alpha3");
    this->thermalExpansionCoefficients.push_back(a1);
    this->thermalExpansionCoefficients.push_back(a2);
    this->thermalExpansionCoefficients.push_back(a3);
  } // end of BehaviourDescription::setThermalExpansionCoefficients
  
  bool
  BehaviourDescription::areThermalExpansionCoefficientsDefined(void) const
  {
    return !this->thermalExpansionCoefficients.empty();
  } // end of BehaviourDescription::areThermalExpansionCoefficientsDefined

  const std::vector<BehaviourDescription::MaterialProperty>&
  BehaviourDescription::getThermalExpansionCoefficients(void) const
  {
    if(!this->areThermalExpansionCoefficientsDefined()){
      throw(std::runtime_error("BehaviourDescription::getThermalExpansionCoefficients: "
			       "no thermal expansion coefficients defined"));
    }
    return this->thermalExpansionCoefficients;
  }

  void
  BehaviourDescription::setUseQt(const bool b)
  {
    if (this->use_qt) {
      throw(std::runtime_error("BehaviourDescription::setUseQt: "
			       "setUseQt already called"));
    }
    this->use_qt = b;
  } // end of BehaviourDescription::setUseQt

  bool
  BehaviourDescription::useQt(void) const
  {
    return this->use_qt;
  } // end of BehaviourDescription::useQt

  std::string
  BehaviourDescription::getTangentOperatorType(void) const
  {
    if(this->type==GENERALBEHAVIOUR){
      auto msizes = this->getMainVariablesSize();
      std::ostringstream t;
      t << "tfel::math::tmatrix<"
	<< msizes.first  << "," 
	<< msizes.second << ",real>";
      return t.str();
    } else if(this->type==SMALLSTRAINSTANDARDBEHAVIOUR){
      return "StiffnessTensor";
    } else if(this->type==FINITESTRAINSTANDARDBEHAVIOUR){
      return "FiniteStrainBehaviourTangentOperator<N,stress>";
    } else if(this->type==COHESIVEZONEMODEL){
      return "tfel::math::tmatrix<N,N,stress>";
    }
    throw(std::runtime_error("BehaviourDescription::getStiffnessOperatorType: "
			     "internal error (unsupported behaviour type)"));
  } // end of BehaviourDescription::getStiffnessOperatorType

  std::string
  BehaviourDescription::getStressFreeExpansionType(void) const
  {
    if((this->type==SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->type==FINITESTRAINSTANDARDBEHAVIOUR)){
      return "StrainStensor";
    }
    throw(std::runtime_error("BehaviourDescription::getStressFreeExpansionType: "
			     "internal error (unsupported behaviour type)"));
  } // end of BehaviourDescription::getStressFreeExpansionType

  void
  BehaviourDescription::checkModellingHypothesis(const Hypothesis& h) const
  {
    if(this->getModellingHypotheses().find(h)==this->getModellingHypotheses().end()){
      std::ostringstream msg;
      msg << "BehaviourDescription::checkModellingHypothesis: "
	  << "modelling hypothesis '" << ModellingHypothesis::toString(h) 
	  << "' is not supported. Refer to the documentation of "
	  << "the '@ModellingHypothesis' or "
	  << "the '@ModellingHypotheses' keywords for details.\n";
      msg << "Supported modelling hypotheses are :";
      for(const auto & elem : this->hypotheses){
	msg << "\n- '" << ModellingHypothesis::toString(elem) << "'";
      }
      throw(std::runtime_error(msg.str()));
    }
  } // end of BehaviourDescription::checkModellingHypothesis

  bool
  BehaviourDescription::areModellingHypothesesDefined(void) const
  {
    return !this->hypotheses.empty();
  } // end of BehaviourDescription::areModellingHypothesesDefined

  const std::set<BehaviourDescription::Hypothesis>&
  BehaviourDescription::getModellingHypotheses(void) const
  {
    if(this->hypotheses.empty()){
      throw(std::runtime_error("BehaviourDescription::getModellingHypotheses: "
			       "hypothesis undefined yet"));
    }
    return this->hypotheses;
  } // end of BehaviourDescription::getModellingHypotheses

  std::set<BehaviourDescription::Hypothesis>
  BehaviourDescription::getDistinctModellingHypotheses(void) const
  {
    const auto& mh = this->getModellingHypotheses();
    if(mh.size()==1u){
      // if only one modelling hypothesis is supported, it is not
      // considered as specialised, so we return it.
      return mh;
    }
    std::set<Hypothesis> dh;
    if(!this->areAllMechanicalDataSpecialised()){
      // We return UNDEFINEDHYPOTHESIS to take into account all the
      // modelling hypotheses that were not specialised
      dh.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(const auto & h : mh){
      if(this->hasSpecialisedMechanicalData(h)){
	dh.insert(h);
      }
    }
    return dh;
  } // end of BehaviourDescription::getDistinctModellingHypotheses


  bool
  BehaviourDescription::isModellingHypothesisSupported(const Hypothesis h) const
  {
    return this->getModellingHypotheses().count(h)!=0u;
  }

  void
  BehaviourDescription::setModellingHypotheses(const std::set<Hypothesis>& mh,
					       const bool b)
  {
    auto throw_if = [](const bool c,const std::string& m){
      if(c){throw(std::runtime_error("BehaviourDescription::setHypotheses: "+m));}
    };
    // never ever trust a user
    throw_if(mh.empty(),"empty set of modelling hypotheses specificied");
    // check that the user did not already set the modelling hypotheses
    throw_if(!this->hypotheses.empty(),
	     "supported modelling hypotheses have already been declared");
    // check that if a specialised version of the behaviour
    // is defined, it is present in the set of hypotheses defined here
    for(const auto& ld : this->sd){
      throw_if(mh.find(ld.first)==mh.end(),
	       "partial specialisation of the behaviour exists for "
	       "the hypothesis '"+ModellingHypothesis::toString(ld.first)+"' "
	       "which is not in the set of hypotheses which have to be "
	       "supported by the behaviour.");
    }
    for(const auto h : this->requestedHypotheses){
      throw_if(mh.find(h)==mh.end(),
	       "a description of the behaviour for "
	       "the hypothesis '"+ModellingHypothesis::toString(h)+"' "
	       "has been requested earlier, but this hypothesis is not "
	       "in the set of hypotheses which will to be "
	       "supported by the behaviour. This may lead to inconsistencies. "
	       "Cowardly aborting.");
    }
    if(this->hypotheses.empty()){
      this->hypotheses.insert(mh.begin(),mh.end());
    } else {
      if(b){
	// find the intersection of the given hypotheses and the
	// existing one
	std::set<Hypothesis> nh;
	for(const auto h : this->hypotheses){
	  if(mh.find(h)!=mh.end()){
	    nh.insert(h);
	  }
	}
	throw_if(nh.empty(),"intersection of previously modelling hypotheses "
		 "with the new ones is empty");
	this->hypotheses.swap(nh);
      } else {
	throw_if(true,"supported modelling hypotheses have already been declared");
      }
    }
  } // end of BehaviourDescription::setModellingHypotheses

  void
  BehaviourDescription::addMaterialProperties(const Hypothesis h,
					      const VariableDescriptionContainer& v,
					      const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addMaterialProperty;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addMaterialProperty(const Hypothesis h,
					    const VariableDescription& v,
					    const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addMaterialProperty;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addIntegrationVariables(const Hypothesis h,
						const VariableDescriptionContainer& v,
						const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addIntegrationVariable;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addStateVariables(const Hypothesis h,
					  const VariableDescriptionContainer& v,
					  const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addStateVariable;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addIntegrationVariable(const Hypothesis h,
					       const VariableDescription& v,
					       const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addIntegrationVariable;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addStateVariable(const Hypothesis h,
					 const VariableDescription& v,
					 const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addStateVariable;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addAuxiliaryStateVariables(const Hypothesis h,
						   const VariableDescriptionContainer& v,
						   const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addAuxiliaryStateVariable;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addAuxiliaryStateVariable(const Hypothesis h,
						  const VariableDescription& v,
						  const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addAuxiliaryStateVariable;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addExternalStateVariables(const Hypothesis h,
						  const VariableDescriptionContainer& v,
						  const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addExternalStateVariable;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addExternalStateVariable(const Hypothesis h,
						 const VariableDescription& v,
						 const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addExternalStateVariable;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addLocalVariables(const Hypothesis h,
					  const VariableDescriptionContainer& v,
					  const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addLocalVariable;
    this->addVariables(h,v,s,f);
  }

  void
  BehaviourDescription::addLocalVariable(const Hypothesis h,
					 const VariableDescription& v,
					 const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addLocalVariable;
    this->addVariable(h,v,s,f);
  }

  void
  BehaviourDescription::addParameter(const Hypothesis h,
				     const VariableDescription& v,
				     const BehaviourData::RegistrationStatus s)
  {
    typedef void (BehaviourData::* mptr)(const VariableDescription&,
					 const BehaviourData::RegistrationStatus);
    mptr f = &BehaviourData::addParameter;
    this->addVariable(h,v,s,f);
  }

  bool
  BehaviourDescription::hasGlossaryName(const Hypothesis h,
					const std::string& v) const
  {
    return this->getData(h,&BehaviourData::hasGlossaryName,v);
  } // end of BehaviourDescription::hasGlossaryName
  
  bool
  BehaviourDescription::hasEntryName(const Hypothesis h,
				     const std::string& v) const
  {
    return this->getData(h,&BehaviourData::hasEntryName,v);
  } // end of BehaviourDescription::hasEntryName
  
  bool
  BehaviourDescription::hasParameter(const Hypothesis h,
				     const std::string& v) const
  {
    return this->getData(h,&BehaviourData::hasParameter,v);
  } // end of BehaviourDescription::hasParameter

  bool
  BehaviourDescription::hasParameters(const Hypothesis h) const
  {
    return this->getBehaviourData(h).hasParameters();
  } // end of BehaviourDescription::hasParameters

  bool
  BehaviourDescription::hasParameters(void) const
  {
    if(this->d.hasParameters()){
      return true;
    }
    for(auto p=this->sd.begin();p!=this->sd.end();++p){
      if(p->second->hasParameters()){
	return true;
      }
    }
    return false;
  } // end of BehaviourDescription::hasParameters
    
  void
  BehaviourDescription::setParameterDefaultValue(const Hypothesis h,
						 const std::string& n,
						 const double v)
  {
    void (BehaviourData:: *mptr)(const std::string&,
				 const double);
    mptr = &BehaviourData::setParameterDefaultValue;
    this->callBehaviourData(h,mptr,n,v,true);
  }

  void
  BehaviourDescription::setParameterDefaultValue(const Hypothesis h,
						 const std::string& n,
						 const int v)
  {
    void (BehaviourData:: *mptr)(const std::string&,
				 const int v);
    mptr = &BehaviourData::setParameterDefaultValue;
    this->callBehaviourData(h,mptr,n,v,true);
  }

  void
  BehaviourDescription::setParameterDefaultValue(const Hypothesis h,
						 const std::string& n,
						 const unsigned short v)
  {
    void (BehaviourData:: *mptr)(const std::string&,
				 const unsigned short v);
    mptr = &BehaviourData::setParameterDefaultValue;
    this->callBehaviourData(h,mptr,n,v,true);
  }

  unsigned short
  BehaviourDescription::getUnsignedShortParameterDefaultValue(const Hypothesis h,
							      const std::string& n) const
  {
    return this->getData(h,&BehaviourData::getUnsignedShortParameterDefaultValue,n);
  } // end of BehaviourDescription::getUnsignedShortParameterDefaultValue

  int
  BehaviourDescription::getIntegerParameterDefaultValue(const Hypothesis h,
							const std::string& n) const
  {
    return this->getData(h,&BehaviourData::getIntegerParameterDefaultValue,n);
  } // end of BehaviourDescription::getIntegerParameterDefaultValue

  double
  BehaviourDescription::getFloattingPointParameterDefaultValue(const Hypothesis h,
							       const std::string& n) const
  {
    return this->getData(h,&BehaviourData::getFloattingPointParameterDefaultValue,n);
  } // end of BehaviourDescription::getFloattingPointParameterDefaultValue

  void
  BehaviourDescription::addStaticVariable(const Hypothesis h,
					  const StaticVariableDescription& v,
					  const BehaviourData::RegistrationStatus s)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->d.addStaticVariable(v,s);
      for(const auto& md : this->sd){
	md.second->addStaticVariable(v,s);
      }
    } else {
      this->getBehaviourData2(h).addStaticVariable(v,s);
    }
  } // end of BehaviourDescription::addStaticVariable

  void
  BehaviourDescription::addVariables(const Hypothesis h,
				     const VariableDescriptionContainer& v,
				     const BehaviourData::RegistrationStatus s,
				     void (BehaviourData::* m)(const VariableDescription&,
							       const BehaviourData::RegistrationStatus))
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->addVariables(this->d,v,s,m);
      for(auto& md: this->sd){
	this->addVariables(*(md.second),v,s,m);
      }
    } else {
      this->addVariables(this->getBehaviourData2(h),v,s,m);
    }
  }

  void
  BehaviourDescription::addVariable(const Hypothesis h,
				    const VariableDescription& v,
				    const BehaviourData::RegistrationStatus s,
				    void (BehaviourData::* m)(const VariableDescription&,
							      const BehaviourData::RegistrationStatus))
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->addVariable(this->d,v,s,m);
      for(auto& md : this->sd){
	this->addVariable(*(md.second),v,s,m);
      }
    } else {
      this->addVariable(this->getBehaviourData2(h),v,s,m);
    }
  }

  void
  BehaviourDescription::addVariables(BehaviourData& b,
				     const VariableDescriptionContainer& v,
				     const BehaviourData::RegistrationStatus s,
				     void (BehaviourData::* m)(const VariableDescription&,
							       const BehaviourData::RegistrationStatus))
  {
    for(const auto & e : v){
      this->addVariable(b,e,s,m);
    }
  }

  void
  BehaviourDescription::addVariable(BehaviourData& b,
				    const VariableDescription& v,
				    const BehaviourData::RegistrationStatus s,
				    void (BehaviourData::* m)(const VariableDescription&,
							      const BehaviourData::RegistrationStatus))
  {
    (b.*m)(v,s);
  }

  bool BehaviourDescription::areAllMechanicalDataSpecialised(void) const
  {
    return this->getModellingHypotheses().size()==this->sd.size();
  } // end of BehaviourDescription::areAllMechanicalDataSpecialised

  bool BehaviourDescription::areAllMechanicalDataSpecialised(const std::set<Hypothesis>& h) const
  {
    for(const auto & mh : h){
      if(!this->hasSpecialisedMechanicalData(mh)){
	return false;
      }
    }
    return true;
  } // end of BehaviourDescription::areAllMechanicalDataSpecialised

  bool
  BehaviourDescription::hasSpecialisedMechanicalData(const Hypothesis h) const
  {
    if(this->getModellingHypotheses().find(h)==this->getModellingHypotheses().end()){
      throw(std::runtime_error("BehaviourDescription::areAllMechanicalDataSpecialised: "
			       "hypothesis '"+ModellingHypothesis::toString(h)+
			       "' is not supported"));
    }
    return this->sd.find(h)!=this->sd.end();
  }

  void
  BehaviourDescription::requiresTVectorOrVectorIncludes(bool& b1,
							bool& b2) const
  {
    b1 = b2 = false;
    if(this->hypotheses.empty()){
      throw(std::runtime_error("BehaviourDescription::areAllMechanicalDataSpecialised: "
			       "no hypothesis defined"));
    }
    if(!this->areAllMechanicalDataSpecialised()){
      this->d.requiresTVectorOrVectorIncludes(b1,b2);
    }
    for(const auto& md : this->sd){
      md.second->requiresTVectorOrVectorIncludes(b1,b2);
    }
  } // end of BehaviourDescription::requiresTVectorOrVectorIncludes

  void
  BehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const Hypothesis h,
											       const std::string& n)
  {
    void (BehaviourData::* m)(const std::string&) =
      &BehaviourData::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution;
    this->callBehaviourData(h,m,n,true);
  } // end of BehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  void
  BehaviourDescription::setUsableInPurelyImplicitResolution(const Hypothesis h,
							    const bool b)
  {
    void (BehaviourData::* m)(const bool) =
      &BehaviourData::setUsableInPurelyImplicitResolution;
    this->callBehaviourData(h,m,b,true);
  } // end of BehaviourDescription::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  bool
  BehaviourDescription::isMemberUsedInCodeBlocks(const Hypothesis h,
						   const std::string& v) const
  {
    return this->getData(h,&BehaviourData::isMemberUsedInCodeBlocks,v);
  } // end of BehaviourDescription::isMaterialPropertyName

  bool
  BehaviourDescription::isMaterialPropertyName(const Hypothesis h,
					       const std::string& v) const
  {
    return this->getData(h,&BehaviourData::isMaterialPropertyName,v);
  } // end of BehaviourDescription::isMaterialPropertyName

  bool
  BehaviourDescription::isLocalVariableName(const Hypothesis h,
					    const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isLocalVariableName,n);
  }  // end of BehaviourDescription::isLocalVariableName

  bool
  BehaviourDescription::isPersistentVariableName(const Hypothesis h,
						 const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isPersistentVariableName,n);
  }  // end of BehaviourDescription::isPersistentVariableName

  bool
  BehaviourDescription::isIntegrationVariableName(const Hypothesis h,
						  const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isIntegrationVariableName,n);
  }  // end of BehaviourDescription::isIntegrationVariableName

  bool
  BehaviourDescription::isIntegrationVariableIncrementName(const Hypothesis h,
							   const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isIntegrationVariableIncrementName,n);
  }  // end of BehaviourDescription::isIntegrationVariableIncrementName

  bool
  BehaviourDescription::isStateVariableName(const Hypothesis h,
					    const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isStateVariableName,n);
  }  // end of BehaviourDescription::isStateVariableName

  bool
  BehaviourDescription::isStateVariableIncrementName(const Hypothesis h,
						     const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isStateVariableIncrementName,n);
  }  // end of BehaviourDescription::isStateVariableIncrementName

  bool
  BehaviourDescription::isAuxiliaryStateVariableName(const Hypothesis h,
						     const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isAuxiliaryStateVariableName,n);
  }  // end of BehaviourDescription::isAuxiliaryStateVariableName

  bool
  BehaviourDescription::isExternalStateVariableName(const Hypothesis h,
						    const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isExternalStateVariableName,n);
  }  // end of BehaviourDescription::isExternalStateVariableName

  bool
  BehaviourDescription::isExternalStateVariableIncrementName(const Hypothesis h,
							     const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isExternalStateVariableIncrementName,n);
  }  // end of BehaviourDescription::isExternalStateVariableIncrementName

  bool
  BehaviourDescription::isParameterName(const Hypothesis h,
					const std::string& v) const
  {
    return this->getData(h,&BehaviourData::isParameterName,v);
  } // end of BehaviourDescription::isParameterName

  bool
  BehaviourDescription::isStaticVariableName(const Hypothesis h,
					     const std::string& n) const
  {
    return this->getData(h,&BehaviourData::isStaticVariableName,n);
  }  // end of BehaviourDescription::isStaticVariableName

  void
  BehaviourDescription::updateClassName(void)
  {
    if((!this->behaviour.empty())||
       (!this->material.empty())){
      this->className = this->material+this->behaviour;
    }
  } // end of BehaviourDescription::updateClassName
  
  void
  BehaviourDescription::setCode(const Hypothesis h,
				const std::string& n,
				const CodeBlock& c,
				const Mode m,
				const Position p,
				const bool b)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(getVerboseMode()>=VERBOSE_DEBUG){
	auto& log = getLogStream();
	log << "BehaviourDescription::setCode : setting '"
	    << n << "' on default hypothesis"  << std::endl;
      }
      this->d.setCode(n,c,m,p,b);
      for(const auto& pd : sd){
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  auto& log = getLogStream();
	  log << "BehaviourDescription::setCode : setting '"
	      << n << "' on hypothesis '" 
	      << ModellingHypothesis::toString(pd.first) << "'" << std::endl;
	}
	pd.second->setCode(n,c,m,p,b);
      }
    } else {
      if(getVerboseMode()>=VERBOSE_DEBUG){
	auto& log = getLogStream();
	log << "BehaviourDescription::setCode : setting '"
	    << n << "' on hypothesis '" 
	    << ModellingHypothesis::toString(h) << "'" << std::endl;
      }
      this->getBehaviourData2(h).setCode(n,c,m,p,b);
    }
  } // end of BehaviourDescription::setCode

  const CodeBlock&
  BehaviourDescription::getCodeBlock(const Hypothesis h,
				     const std::string& n) const
  {
    return this->getBehaviourData(h).getCodeBlock(n);
  } // end of BehaviourDescription::getCode

  std::string
  BehaviourDescription::getCode(const Hypothesis h,
				const std::string& n) const
  {
    const auto b = this->getAttribute(BehaviourData::profiling,false);
    return this->getBehaviourData(h).getCode(n,this->getClassName(),b);
  } // end of BehaviourDescription::getCode

  bool
  BehaviourDescription::hasCode(const Hypothesis h,
				const std::string& n) const
  {
    return this->getBehaviourData(h).hasCode(n);
  } // end of BehaviourDescription::getCode

  void
  BehaviourDescription::setBounds(const Hypothesis h,
				  const BoundsDescription& b)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->d.setBounds(b);
      for(const auto& md : this->sd){
	md.second->setBounds(b);
      }
    } else {
      this->getBehaviourData2(h).setBounds(b);
    }
  } // end of BehaviourDescription::setBounds

  void
  BehaviourDescription::setAttribute(const Hypothesis h,
				     const std::string& n,
				     const BehaviourAttribute& a,
				     const bool b)
  {
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->d.setAttribute(n,a,b);
      for(const auto& md : this->sd){
  	BehaviourData& bdata = *(md.second);
  	bdata.setAttribute(n,a,b);
      }
    } else {
      this->getBehaviourData2(h).setAttribute(n,a,b);
    }
  } // end of BehaviourDescription::setAttribute

  bool
  BehaviourDescription::hasAttribute(const Hypothesis h,
				     const std::string& n) const
  {
    return this->getBehaviourData(h).hasAttribute(n);
  } // end of BehaviourDescription::hasAttribute

  std::vector<std::string>
  BehaviourDescription::getCodeBlockNames(const Hypothesis h) const
  {
    return this->getBehaviourData(h).getCodeBlockNames();
  }

  std::vector<std::string>
  BehaviourDescription::getExternalNames(const Hypothesis h,
					 const VarContainer& v) const
  {
    return this->getBehaviourData(h).getExternalNames(v);
  }

  void
  BehaviourDescription::getExternalNames(std::vector<std::string>& n,
					 const Hypothesis h,
					 const VarContainer& v) const
  {
    return this->getBehaviourData(h).getExternalNames(n,v);
  } // end of BehaviourDescription::getExternalNames

  void
  BehaviourDescription::appendExternalNames(std::vector<std::string>& n,
					    const Hypothesis h,
					    const VarContainer& v) const
  {
    return this->getBehaviourData(h).appendExternalNames(n,v);
  } // end of BehaviourDescription::appendExternalNames

  void
  BehaviourDescription::setGlossaryName(const Hypothesis h,
					const std::string& n,
					const std::string& g)
  {
    this->callBehaviourData(h,&BehaviourData::setGlossaryName,n,g,true);
  } // end of BehaviourDescription::setGlossaryName

  bool
  BehaviourDescription::isGlossaryNameUsed(const Hypothesis h,
					   const std::string& n) const
  {
    return this->getBehaviourData(h).isGlossaryNameUsed(n);
  } // end of BehaviourDescription::isGlossaryName

  void
  BehaviourDescription::setEntryName(const Hypothesis h,
				     const std::string& n,
				     const std::string& g)
  {
    this->callBehaviourData(h,&BehaviourData::setEntryName,n,g,true);
  } // end of BehaviourDescription::setEntryName

  bool
  BehaviourDescription::isUsedAsEntryName(const Hypothesis h,
					const std::string& n) const
  {
    return this->getBehaviourData(h).isUsedAsEntryName(n);
  } // end of BehaviourDescription::isEntryName

  std::string
  BehaviourDescription::getExternalName(const Hypothesis h,
					const std::string& n) const
  {
    return this->getData(h,&BehaviourData::getExternalName,n);
  } // end of BehaviourDescription::getGlossaryName

  std::string
  BehaviourDescription::getVariableNameFromGlossaryNameOrEntryName(const Hypothesis h,
								   const std::string& n) const
  {
    return this->getBehaviourData(h).getVariableNameFromGlossaryNameOrEntryName(n);
  } // end of BehaviourDescription::getVariableNameFromGlossaryNameOrEntryName

  void
  BehaviourDescription::setAttribute(const std::string& n,
				     const BehaviourAttribute& a,
				     const bool b)
  {
    if(b){
      auto p=this->attributes.find(n);
      if(p!=this->attributes.end()){
	if(a.getTypeIndex()!=p->second.getTypeIndex()){
	  throw(std::runtime_error("BehaviourDescription::setAttribute: "
				   "attribute already exists with a different type"));
	}
	return;
      }
    }
    if(!this->attributes.insert({n,a}).second){
      throw(std::runtime_error("BehaviourDescription::setAttribute: "
			       "attribute '"+n+"' already declared"));
    }
  } // end of BehaviourDescription::setAttribute

  bool
  BehaviourDescription::hasAttribute(const std::string& n) const
  {
    return this->attributes.count(n)!=0u;
  } // end of BehaviourDescription::hasAttribute
  
  const std::map<std::string,BehaviourAttribute>&
  BehaviourDescription::getAttributes() const
  {
    return this->attributes;
  } // end of BehaviourDescription::getAttributes

  void
  BehaviourDescription::reserveName(const Hypothesis h,
				    const std::string& n)
  {
    this->callBehaviourData(h,&BehaviourData::reserveName,n,true);
  }
  
  void
  BehaviourDescription::registerMemberName(const Hypothesis h,
					   const std::string& n)
  {
    this->callBehaviourData(h,&BehaviourData::registerMemberName,n,true);
  } // end of BehaviourDescription::registerMemberName

  void
  BehaviourDescription::registerStaticMemberName(const Hypothesis h,
						 const std::string& n)
  {
    this->callBehaviourData(h,&BehaviourData::registerStaticMemberName,n,true);
  } // end of BehaviourDescription::registerMemberName
  
  void
  BehaviourDescription::addMaterialLaw(const std::string& m)
  {
    if(find(this->materialLaws.begin(),
	    this->materialLaws.end(),m)==this->materialLaws.end()){
      this->materialLaws.push_back(m);
    }
  } // end of BehaviourDescription::getMaterialLaws

  const std::vector<std::string>&
  BehaviourDescription::getMaterialLaws(void) const
  {
    return this->materialLaws;
  }

  std::pair<bool,bool>
  BehaviourDescription::checkVariableExistence(const std::string& v) const
  {
    const auto& h = this->getDistinctModellingHypotheses();
    std::pair<bool,bool> r{true,false};
    for(const auto & elem : h){
      const auto& bdata = this->getBehaviourData(elem);
      const auto& vn = bdata.getVariablesNames();
      const bool b = vn.find(v)!=vn.end();
      r.first  = r.first  && b;
      r.second = r.second || b;
    }
    if(!r.second){
      r.first = false;
    }
    return r;
  } // end of checkVariableExistence

  std::pair<bool,bool>
  BehaviourDescription::checkVariableExistence(const std::string& n,
					       const std::string& c,
					       const bool b) const
  {
    const auto& mh = this->getDistinctModellingHypotheses();
    std::pair<bool,bool> r = {true,false};
    for(const auto h : mh){
      const auto& bd = this->getBehaviourData(h);
      const auto f = bd.getVariables(c).contains(n);
      if(!f&&b){
	throw(std::runtime_error("BehaviourDescription::checkVariableExistence: "
				 "no '"+c+"' named '"+n+"' found for at "
				 "least one modelling hypothesis"));
      }
      r.first  = r.first  && f;
      r.second = r.second || f;
    }
    if(!r.second){
      r.first = false;
    }
    return r;
  }

  void
  BehaviourDescription::checkVariableGlossaryName(const std::string& n,
						  const std::string& g) const
  {
    for(const auto & h : this->getDistinctModellingHypotheses()){
      const auto& bdata = this->getBehaviourData(h);
      if(!bdata.hasGlossaryName(n)){
	throw(std::runtime_error("BehaviourDescription::VariableGlossaryName: "
				 "no glossary name associated with variable '"+n+"'"));
      }
      const auto& en = bdata.getExternalName(n);
      if(en!=g){
	throw(std::runtime_error("BehaviourDescription::VariableGlossaryName: "
				 "the glossary name associated with "
				 "variable '"+n+"' is not '"+g+"', but '"+en+"'"));
      }
    }
  } // end of BehaviourDescription::checkVariableGlossaryName

  void
  BehaviourDescription::checkVariablePosition(const std::string& n,
					      const std::string& c,
					      const size_t p)
  {
    for(const auto & h : this->getDistinctModellingHypotheses()){
      const auto& bdata = this->getBehaviourData(h);
      const auto& vc = bdata.getVariables(c);
      if(p>=vc.size()){
	throw(std::runtime_error("BehaviourDescription::checkVariablePosition: "
				 "position given is greater than the number "
				 "of variables of category '"+c+"'"));
      }
      const auto& v = vc[p];
      if(v.name!=n){
	throw(std::runtime_error("BehaviourDescription::checkVariablePosition: "
				 "variable at the given position is not named '"+n+
				 "' but '"+v.name+"'"));
      }
    }
  } // end of BehaviourDescription::checkVariablePosition

  void
  BehaviourDescription::setOrthotropicAxesConvention(const tfel::material::OrthotropicAxesConvention c)
  {
    if(this->oacIsDefined){
      throw(std::runtime_error("BehaviourDescription::setOrthotropicAxesConvention: "
			       "orthotropic axes convention already defined"));
    }
    if(this->getSymmetryType()!=mfront::ORTHOTROPIC){
      throw(std::runtime_error("BehaviourDescription::setOrthotropicAxesConvention: "
			       "the behaviour is not orthotropic."));
    }
    this->oacIsDefined = true;
    this->oac = c;
  }

  tfel::material::OrthotropicAxesConvention
  BehaviourDescription::getOrthotropicAxesConvention(void) const
  {
    if(this->getSymmetryType()!=mfront::ORTHOTROPIC){
      throw(std::runtime_error("BehaviourDescription::getOrthotropicAxesConvention: "
			       "the behaviour is not orthotropic."));
    }
    if(!this->oacIsDefined){
      this->oacIsDefined = true;
    }
    return this->oac;
  }
  
  BehaviourDescription::~BehaviourDescription()
  {}

  void
  setElasticSymmetryType(BehaviourDescription& bd,
			 const BehaviourSymmetryType s)
  {
    if(bd.isElasticSymmetryTypeDefined()){
      if(bd.getElasticSymmetryType()!=s){
	throw(std::runtime_error("setElasticSymmetryType: "
				 "the elastic symmetry type defined for "
				 "the behaviour is inconsistent."));
      }
    } else {
      bd.setElasticSymmetryType(s);
    }
  } // end of setElasticSymmetryType
  
} // end of namespace mfront
