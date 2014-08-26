/*! 
 * \file   MFrontMarkdownBehaviourAnalyser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  19 mai 2014
 */

#ifndef _LIB_MFRONT_MFRONTMARKDOWNBEHAVIOURANALYSER_H_
#define _LIB_MFRONT_MFRONTMARKDOWNBEHAVIOURANALYSER_H_ 

#include<ostream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"MFront/MFrontBehaviourAnalyser.hxx"

namespace mfront{

  //! forward declaration
  struct VariableDescriptionContainer;

  //! forward declaration
  struct MechanicalBehaviourData;
  
  /*!
   * This is the abstract base class of all behaviour interfaces
   */
  struct TFEL_VISIBILITY_EXPORT MFrontMarkdownBehaviourAnalyser
    : public MFrontBehaviourAnalyser
  {
    /*!
     * \return the name of the analyser
     */
    static std::string
    getName(void);
    /*!
     * \return a pair containing a boolean which is true if the
     * keyword was handled, and an iterator past the last token
     * treated.
     */
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
    /*!
     * \brief reset the interface
     */
    virtual void
    reset(void);
    /*!
     * destructor
     */
    virtual ~MFrontMarkdownBehaviourAnalyser();
  protected:
    struct Data
    {
      std::string name;
      std::string type;
      std::string glossaryName;
      std::vector<tfel::material::ModellingHypothesis::Hypothesis> hypotheses;
      unsigned short arraySize;
    };
    static std::vector<Data>
    getData(const MechanicalBehaviourDescription&,
	    const VariableDescriptionContainer& (MechanicalBehaviourData::*)(void) const);
    static void
    printData(std::ostream&,
	      const MechanicalBehaviourDescription&,
	      const std::vector<Data>&);
  }; // end of struct MFrontMarkdownBehaviourAnalyser

} // end of namespace mfront  

#endif /* _LIB_MFRONT_MFRONTMARKDOWNBEHAVIOURANALYSER_H */
