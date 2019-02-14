/*! 
 * \file  mfront/src/Gradient.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 mai 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/Gradient.hxx"

namespace mfront
{

  Gradient::Gradient(const std::string& t, const std::string& n)
      : VariableDescription(t, n, 1u, 0u) {}  // end of Gradient::Gradient

  Gradient::Gradient(const VariableDescription& v)
      : VariableDescription(v) {}  // end of Gradient::Gradient

  Gradient::Gradient(Gradient&&) = default;
  Gradient::Gradient(const Gradient&) = default;
  Gradient& Gradient::operator=(Gradient&&) = default;
  Gradient& Gradient::operator=(const Gradient&) = default;
  
  Gradient::~Gradient() noexcept = default;

} // end of namespace mfront

