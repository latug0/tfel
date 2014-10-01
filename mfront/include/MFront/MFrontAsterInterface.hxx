/*!
 * \file   mfront/include/MFront/MFrontAsterInterface.hxx
 * \brief  This file declares the MFrontAsterInterface class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTASTERINTERFACE_H_
#define _LIB_MFRONTASTERINTERFACE_H_ 

#include<string>
#include<fstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"MFront/MFrontUMATInterfaceBase.hxx"

namespace mfront{

  struct MFrontAsterInterface
    : public MFrontUMATInterfaceBase
  {
    static std::string 
    getName(void);
    
    MFrontAsterInterface();
    
    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] mb : mechanical behaviour description
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const MechanicalBehaviourDescription&) const;

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);
    /*!
     * \brief write output files
     * \param[in] mb        : mechanical behaviour description
     * \param[in] fd        : mfront file description
     */
    virtual void
    endTreatement(const MechanicalBehaviourDescription&,
		  const MFrontFileDescription&) const;

    virtual void
    reset(void);

    virtual ~MFrontAsterInterface();
    
  protected:

    virtual std::string
    getLibraryName(const MechanicalBehaviourDescription&) const;

    virtual std::string
    getInterfaceName(void) const;
    /*!
     * \return the name of the function generated by the Cyrano interface
     * \param[in] n : name of the behaviour as defined by interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     */
    virtual std::string
    getFunctionName(const std::string&) const;
    /*!
     * \return a pair which first member gives the position of the
     * material properties in the values given through the interface
     * and whose second members is an offset giving the number of
     * imposed material properties.
     * \param[in] mb : mechanical behaviour description
     * \param[in] h  : modelling hypothesis
     */
    virtual std::pair<std::vector<UMATMaterialProperty>,
		      SupportedTypes::TypeSize>
    buildMaterialPropertiesList(const MechanicalBehaviourDescription&,
				const Hypothesis) const;
    /*!
     * \param[in] out : output file
     */
    virtual void
    writeMTestFileGeneratorSetModellingHypothesis(std::ostream&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void
    writeUMATxxSpecificSymbols(std::ostream&,
			       const std::string&,
			       const MechanicalBehaviourDescription&,
			       const MFrontFileDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void
    writeUMATxxAdditionalSymbols(std::ostream&,
				 const std::string&,
				 const Hypothesis,
				 const MechanicalBehaviourDescription&,
				 const MFrontFileDescription&) const;
    /*!
     * \brief write a  specialisation of the AsterTraits class
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] h   : modelling hypothesis
     */
    virtual void
    writeAsterBehaviourTraits(std::ostream&,
			      const MechanicalBehaviourDescription&,
			      const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    virtual void 
    writeInterfaceSpecificIncludes(std::ofstream&,
				   const MechanicalBehaviourDescription&) const;

    virtual std::string
    getModellingHypothesisTest(const Hypothesis) const;

    virtual std::map<MFrontUMATInterfaceBase::Hypothesis,std::string>
    gatherModellingHypothesesAndTests(const MechanicalBehaviourDescription& mb) const;

    virtual std::set<tfel::material::ModellingHypothesis::Hypothesis>
    getModellingHypothesesToBeTreated(const MechanicalBehaviourDescription&) const;

    bool   compareToNumericalTangentOperator;

    double strainPerturbationValue;

    double tangentOperatorComparisonCriterium;

    bool savesTangentOperator;

    bool errorReport;

  }; // end of MFrontAsterInterface

} // end of namespace mfront

#endif /* _LIB_MFRONTASTERINTERFACE_H */
