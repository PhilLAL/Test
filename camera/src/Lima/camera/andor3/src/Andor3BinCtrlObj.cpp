/* andor3 plugin binning class
 * Copyright (C) 2013 IPANEMA USR3461, CNRS/MCC.
 * Written by Serge Cohen <serge.cohen@synchrotron-soleil.fr>
 *
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this file. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Andor3BinCtrlObj.h"
#include "Andor3Interface.h"

//-----------------------------------------------------
// @brief
//-----------------------------------------------------
lima::Andor3::BinCtrlObj::BinCtrlObj(Camera &cam, Interface *interface)
: m_cam(cam), m_interface(interface)
{
  DEB_CONSTRUCTOR();
}

//-----------------------------------------------------
// @brief Dtor
//-----------------------------------------------------
lima::Andor3::BinCtrlObj::~BinCtrlObj()
{
  DEB_DESTRUCTOR();
}

//-----------------------------------------------------
// @brief
//-----------------------------------------------------
void
lima::Andor3::BinCtrlObj::getBin(Bin &aBin)
{
  DEB_MEMBER_FUNCT();
  m_cam.getBin(aBin);
}

//-----------------------------------------------------
// @brief
//-----------------------------------------------------
void
lima::Andor3::BinCtrlObj::checkBin(Bin &aBin)
{
  DEB_MEMBER_FUNCT();
  m_cam.checkBin(aBin);
}

//-----------------------------------------------------
// @brief
//-----------------------------------------------------
void
lima::Andor3::BinCtrlObj::setBin(const Bin& aBin)
{
  DEB_MEMBER_FUNCT();
  Bin		real_bin = aBin;
  m_cam.checkBin(real_bin);
  m_cam.setBin(real_bin);
  m_interface->updateValidRanges();
}

