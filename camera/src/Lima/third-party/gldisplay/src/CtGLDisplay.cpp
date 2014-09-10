//###########################################################################
// This file is part of gldisplay, a submodule of LImA project the
// Library for Image Acquisition
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

#include "CtGLDisplay.h"
#include "CtSpsImage.h"

using namespace lima;
using namespace std;

#include "PoolThreadMgr.h"

//-------------------------------------------------------------
// CtGLDisplay
//-------------------------------------------------------------

CtGLDisplay::CtGLDisplay(CtControl *ct_control)
	: m_ct_control(ct_control)
{
}

CtGLDisplay::~CtGLDisplay()
{
}

//-------------------------------------------------------------
// CtSPSGLDisplay
//-------------------------------------------------------------

CtSPSGLDisplay::ForkCallback::ForkCallback(CtSPSGLDisplay *gl_display)
	: m_gl_display(gl_display)
{
}

void CtSPSGLDisplay::ForkCallback::execInForked()
{
	// First exec local callbacks
	m_gl_display->execInForked();

	// Processlib fork cleanup
	PoolThreadMgr::get().setThreadWaitOnQuit(false);
}


CtSPSGLDisplay::CtSPSGLDisplay(CtControl *ct_control, int argc, char **argv)
	: CtGLDisplay(ct_control), m_fork_cb(this)
{
	m_use_forked = true;
	if (m_use_forked) {
		ForkedSPSGLDisplay *gl_display;
		gl_display = new ForkedSPSGLDisplay(argc, argv);
		gl_display->addForkCallback(&m_fork_cb);
		m_sps_gl_display = gl_display;
	}
}

CtSPSGLDisplay::~CtSPSGLDisplay()
{
	closeWindow();
}

void CtSPSGLDisplay::closeWindow()
{
	if (m_sps_gl_display)
		delete m_sps_gl_display;
	m_sps_gl_display = NULL;
}

void CtSPSGLDisplay::createWindow()
{
	m_sps_gl_display->createWindow();
}

bool CtSPSGLDisplay::isClosed()
{
	return m_sps_gl_display->isClosed();
}

void CtSPSGLDisplay::refresh()
{
	return m_sps_gl_display->refresh();
}

void CtSPSGLDisplay::setTestImage(bool active)
{
	return m_sps_gl_display->setTestImage(active);
}

void CtSPSGLDisplay::setSpecArray(string spec_name, string array_name)
{
	CtSpsImage *ct_display = m_ct_control->display();
	ct_display->setNames(spec_name, array_name);
	ct_display->setActive(true);
	m_sps_gl_display->setSpecArray(spec_name, array_name);
}

void CtSPSGLDisplay::getSpecArray(string& spec_name, string& array_name)
{
	m_sps_gl_display->getSpecArray(spec_name, array_name);
}

void CtSPSGLDisplay::setRefreshTime(float refresh_time)
{
	typedef ForkedSPSGLDisplay ForkedKlass;
	ForkedKlass *gl_display;
	if (m_use_forked) {
		gl_display = reinterpret_cast<ForkedKlass *>(m_sps_gl_display);
		gl_display->setRefreshTime(refresh_time);
	}
}

void CtSPSGLDisplay::getRates(float *update, float *refresh)
{
	m_sps_gl_display->getRates(update, refresh);
}

void CtSPSGLDisplay::getNorm(unsigned long *minval, unsigned long *maxval,
			     int *autorange)
{
	m_sps_gl_display->getNorm(minval, maxval, autorange);
}

void CtSPSGLDisplay::setNorm(unsigned long minval, unsigned long maxval,
			     int autorange)
{
	m_sps_gl_display->setNorm(minval, maxval, autorange);
}
