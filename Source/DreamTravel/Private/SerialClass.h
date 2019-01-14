// Fill out your copyright notice in the Description page of Project Settings.

#ifndef __SERIAL_H__
#define __SERIAL_H__

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

#include "windows.h"
#include "AllowWindowsPlatformTypes.h"

/**
 * 
 */
class CSerialClass
{
public:
	static void Initialise();
	static BOOL Open(int nPort = 2, int nBaud = 9600);
	static BOOL Close(void);

	static int ReadData(void *, int);
	static int SendData(const char *, int);
	static int ReadDataWaiting(void);

	static BOOL IsOpened(void){ return(m_bOpened); }

protected:
	static BOOL WriteCommByte(unsigned char);

	static HANDLE m_hIDComDev;
	static OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
	static BOOL m_bOpened;

};

#endif