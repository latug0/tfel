/*!
 * \file   mfront/src/OverridableImplementation.cxx
 * \brief
 * \author Thomas Helfer
 * \date   01/10/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>

#ifdef MFRONT_HAVE_MADNEX
#include "Madnex/File.hxx"
#include "Madnex/MFrontImplementation.hxx"
#endif /* MFRONT_HAVE_MADNEX */

#include "TFEL/Raise.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/OverridableImplementation.hxx"

namespace mfront {

  static std::shared_ptr<const AbstractDSL> generateAbstractDSL(
      const std::string& f) {
    auto dsl = MFrontBase::getDSL(f);
    dsl->analyseFile(f, {}, {});
    dsl->endsInputFileProcessing();
    return dsl;
  }  // end of generateAbstractDSL

  void throwOverridenValueAlreadySet() {
    tfel::raise(
        "OverridableStringInformation::setOverridenValue: "
        "value already overriden");
  }  // end of throwOverridenValueAlreadySet

  OverridableImplementation::OverridableImplementation(const std::string& f)
      : dsl(generateAbstractDSL(f)),
        source(f) {}  // end of OverridableImplementation

  const std::map<std::string, double>&
  OverridableImplementation::getOverridingParameters() const {
    return this->parameters;
  }  // end of OverridableImplementation::getOverridingParameters

  void OverridableImplementation::overrideByAParameter(const std::string& n,
                                                       const double v) {
    const auto vn = dsl->getOverridableVariableNameByExternalName(n);
    this->parameters[vn] = v;
  }  // end of OverridableImplementation::overrideByAParameter

  AbstractDSL::DSLTarget OverridableImplementation::getTargetType() const {
    return this->dsl->getTargetType();
  }  // end of OverridableImplementation::getTargetType

  const FileDescription& OverridableImplementation::getSourceFileDescription() const {
    return this->dsl->getFileDescription();
  }  // end of OverridableImplementation::getSourceFileDescription

  std::string OverridableImplementation::getSourceMaterialKnowledgeIdentifier()
      const {
    return this->dsl->getMaterialKnowledgeIdentifier();
  }  // end of OverridableImplementation::getSourceMaterialKnowledgeIdentifier

  std::string OverridableImplementation::getSourceMaterialName() const {
    return this->dsl->getMaterialName();
  }  // end of OverridableImplementation::getSourceMaterialName

  const std::string& OverridableImplementation::getSourceFilePath() const {
    return this->source;
  }  // end of OverridableImplementation::getSourceFilePath

  OverridableImplementation::~OverridableImplementation() = default;

#ifdef MFRONT_HAVE_MADNEX
  static std::string getSourceFileContent(const std::string& f) {
    std::ifstream file(f);
    std::ostringstream s;
    s << file.rdbuf();
    return s.str();
  }  // end of getSourceFileContent

  static void writeMadnexFile(const OverridableImplementation& i,
                              const std::string& f) {
    using Tags = OverridableImplementation::Tags;
    auto copy_if = [](std::string& dest, const std::string& src1,
                      const std::string& src2) {
      dest = !src1.empty() ? src1 : src2;
    };  // end of copy_if
    const auto& fd = i.getSourceFileDescription();
    madnex::MFrontImplementation impl;
    copy_if(impl.name,
            i.getOverridenValue<Tags::MATERIAL_KNOWLEDGE_IDENTIFIER>(),
            i.getSourceMaterialKnowledgeIdentifier());
    if (impl.name.empty()) {
      tfel::raise("mfront::writeMadnexFile: "
		  "no material knowledge identifier defined");
    }
    copy_if(impl.metadata.author, i.getOverridenValue<Tags::AUTHOR_NAME>(),
            fd.authorName);
    copy_if(impl.metadata.date, i.getOverridenValue<Tags::DATE>(), fd.date);
    copy_if(impl.metadata.description, i.getOverridenValue<Tags::DESCRIPTION>(),
            fd.description);
    auto material = std::string{};
    copy_if(material, i.getOverridenValue<Tags::MATERIAL_NAME>(),
            i.getSourceMaterialName());
    impl.source = getSourceFileContent(i.getSourceFilePath());
    impl.parameters = i.getOverridingParameters();
    const auto mkt = [i]() -> std::string {
      const auto t = i.getTargetType();
      if(t == AbstractDSL::MATERIALPROPERTYDSL){
        return "MaterialProperties";
      } else if (t == AbstractDSL::BEHAVIOURDSL) {
        return "Behaviours";
      } else if (t != AbstractDSL::MODELDSL) {
        tfel::raise(
            "mfront::writeMadnexFile: "
            "unsupported DSL target");
      }
      return "Models";
    }();
    auto file = madnex::File(f, H5F_ACC_TRUNC);
    auto r = file.getRoot();
    madnex::createGroup(r, "MFront");
    auto g = madnex::Group();
    if (!material.empty()) {
      madnex::createGroup(r, "MFront/" + material);
      g = madnex::createGroup(r, "MFront/" + material + "/" + mkt);
    } else {
      g = madnex::createGroup(r, "MFront/" + mkt);
    }
    madnex::write(g, impl);
  }    // end of writeMadnexFile
#endif /* MFRONT_HAVE_MADNEX */

  void write(const OverridableImplementation& i, const std::string& f) {
    const auto ext = [&f]() -> std::string {
      const auto p = f.find(".");
      if (p != std::string::npos) {
        return f.substr(p + 1);
      }
      return "";
    }();
#ifdef MFRONT_HAVE_MADNEX
    if ((ext == "madnex") || (ext == "edf")) {
      writeMadnexFile(i, f);
    } else {
      tfel::raise("write: unsupported file extension '" + ext + "'");
    }
#else  /* MFRONT_HAVE_MADNEX */
    static_cast<void>(i);
    tfel::raise("write: unsupported file extension '" + ext + "'");
#endif /* MFRONT_HAVE_MADNEX */
  }    // end of write

}  // end of namespace mfront
