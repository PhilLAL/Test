//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2011
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################
#ifndef HWFLIPCTRLOBJ_H
#define HWFLIPCTRLOBJ_H

#include "LimaCompatibility.h"
#include "SizeUtils.h"
#include "Debug.h"

namespace lima
{

class LIMACORE_API HwFlipCtrlObj
{
	DEB_CLASS(DebModHardware, "HwFlipCtrlObj");

 public:
	HwFlipCtrlObj();
	virtual ~HwFlipCtrlObj();

	virtual void setFlip(const Flip& flip) = 0;
	virtual void getFlip(Flip& flip) = 0;
	virtual void checkFlip(Flip& flip) = 0;

};


} // namespace lima

#endif // HWFLIPCTRLOBJ_H
