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
#pragma once
#include "../../Common/Stream.h"
#include "Core2Device.h"
#include "AdvancedProgress.h"

#define CORE2_READ_RETRYCOUNT			1
#define CORE2_READ_MAXFRAMESIZE			(2352 + 96 + 296)	// Mainchannel + RAW P-W subchannel + C2 block information.
#define CORE2_READ_BLOCKCOUNT			20					// Read 20 blocks at a time.

namespace Core2ReadFunction
{
	class CReadFunction
	{
	private:
		CCore2Device *m_pDevice;

	protected:
		enum eMainChannelData
		{
			MCD_NONE = 0x00,
			MCD_USERDATA = 0x10
		};

		enum eSubChannelData
		{
			SCD_NONE,
			SCD_FORMATTEDQ,
			SCD_DEINTERLEAVED_RW
		};

		enum eC2ErrorInfo
		{
			C2EI_NONE = 0x0,
			C2EI_BITS = 0x2,			// 294 bytes.
			C2EI_BLOCKANDBITS = 0x4,	// 296 bytes.
		};

		bool ReadCD(unsigned char *pBuffer,unsigned long ulAddress,unsigned long ulBlockCount,
			eMainChannelData MCD,eSubChannelData SCD,eC2ErrorInfo ErrorInfo);

	public:
		CReadFunction(CCore2Device *pDevice);

		virtual bool Read(unsigned char *pBuffer,unsigned long ulAddress,
			unsigned long ulBlockCount) = 0;
		virtual bool Process(unsigned char *pBuffer,unsigned long ulBlockCount) = 0;
		virtual unsigned long GetFrameSize() = 0;
	};

	class CReadUserData : public CReadFunction
	{
	private:
		//HANDLE m_hFile;
		COutStream *m_pOutStream;
		unsigned long m_ulFrameSize;

	public:
		CReadUserData(CCore2Device *pDevice,/*const TCHAR *szFilePath*/COutStream *pOutStream);
		~CReadUserData();

		bool Read(unsigned char *pBuffer,unsigned long ulAddress,
			unsigned long ulBlockCount);
		bool Process(unsigned char *pBuffer,unsigned long ulBlockCount);
		unsigned long GetFrameSize();
	};

	class CReadRaw
	{
	};

	class CReadC2 : public CReadFunction
	{
	private:
		unsigned long m_ulErrSecCount;
		unsigned long m_ulErrByteCount;
		unsigned __int64 m_uiTotalBytes;

		unsigned char NumBits(unsigned char ucData);

	public:
		CReadC2(CCore2Device *pDevice);
		~CReadC2();

		bool Read(unsigned char *pBuffer,unsigned long ulAddress,
			unsigned long ulBlockCount);
		bool Process(unsigned char *pBuffer,unsigned long ulBlockCount);
		unsigned long GetFrameSize();
	};
}

class CCore2Read
{
private:
	enum eSubChannelData
	{
		SUBCHANNELDATA_NONE,
		SUBCHANNELDATA_FORMATTEDQ,
		SUBCHANNELDATA_DEINTERLEAVED_RW
	};

	bool RetryReadBlock(CCore2Device *pDevice,CAdvancedProgress *pProgress,
		Core2ReadFunction::CReadFunction *pReadFunction,unsigned char *pBuffer,
		unsigned long ulAddress);

public:
	CCore2Read();
	~CCore2Read();

	bool ReadData(CCore2Device *pDevice,CAdvancedProgress *pProgress,
		Core2ReadFunction::CReadFunction *pReadFunction,unsigned long ulStartBlock,
		unsigned long ulNumBlocks,bool bIgnoreErr);
};
