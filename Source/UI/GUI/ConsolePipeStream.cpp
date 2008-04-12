/*
 * Copyright (C) 2006-2008 Christian Kindahl, christian dot kindahl at gmail dot com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "stdafx.h"
#include "ConsolePipeStream.h"

COutConsolePipeStream::COutConsolePipeStream(CConsolePipe &ConsolePipe) : m_ConsolePipe(ConsolePipe)
{
}

COutConsolePipeStream::~COutConsolePipeStream()
{
}

int COutConsolePipeStream::Write(void *pBuffer,unsigned long ulSize,unsigned long *pProcessedSize)
{
	unsigned long ulWritten = m_ConsolePipe.WriteInput((const char *)pBuffer,ulSize);
	if (pProcessedSize != NULL)
		*pProcessedSize = ulWritten;

	return STREAM_OK;
}
