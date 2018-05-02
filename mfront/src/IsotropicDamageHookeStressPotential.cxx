/*!
 * \file   mfront/src/IsotropicDamageHookeStressPotential.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/04/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#include "MFront/BehaviourBrick/IsotropicDamageHookeStressPotential.hxx"

namespace mfront {

  namespace bbrick {

    IsotropicDamageHookeStressPotential::IsotropicDamageHookeStressPotential() =
        default;

    bool IsotropicDamageHokkeStressPotential::handleIsotropicDamage(){
      return true;
    }  // end of IsotropicDamageHokkeStressPotential::handleIsotropicDamage()

    IsotropicDamageHookeStressPotential::
        ~IsotropicDamageHookeStressPotential() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
