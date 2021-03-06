/*! 
 * \file  mfront/src/CastemOutOfBoundsPolicy.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 jan 2010
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cstdlib>
#include<cstring>
#include"MFront/Castem/CastemOutOfBoundsPolicy.hxx"

namespace castem
{

  const CastemOutOfBoundsPolicy&
  CastemOutOfBoundsPolicy::getCastemOutOfBoundsPolicy()
  {
    static CastemOutOfBoundsPolicy u;
    return u;
  } // end of getCastemOutOfBoundsPolicy

  CastemOutOfBoundsPolicy::CastemOutOfBoundsPolicy()
    : policy(tfel::material::None)
  {
    const char * const p = ::getenv("CASTEM_OUT_OF_BOUNDS_POLICY");
    if(p!=nullptr){
      if(strcmp(p,"STRICT")==0){
	this->policy = tfel::material::Strict;
      } else if (strcmp(p,"WARNING")==0){
	this->policy = tfel::material::Warning;
      }
    }
  } // end of CastemOutOfBoundsPolicy::CastemOutOfBoundsPolicy

  tfel::material::OutOfBoundsPolicy
  CastemOutOfBoundsPolicy::getOutOfBoundsPolicy() const
  {
    return this->policy;
  } // end of CastemOutOfBoundsPolicy::getOutOfBoundsPolicy


} // end of namespace castem
