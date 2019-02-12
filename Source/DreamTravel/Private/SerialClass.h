// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
DECLARE_LOG_CATEGORY_EXTERN(CSerialClass, Log, All);

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



//Ӧ�ð���MinWindows.h����Ȼ�ᱨ��
#include "Windows/MinWindows.h"
//#include "AllowWindowsPlatformTypes.h"

/**
 * 
 */
class CSerialClass
{
public:
	CSerialClass();
	~CSerialClass();

	 void Initialise();
	 BOOL Open(int nPort = 2, int nBaud = 9600);
	 BOOL Close(void);

	 int ReadData(void *, int);
     int ReadDataUtil(void *buffer, unsigned char end, int limit);
	 int SendData(const char *, int);

	 int GetReadySize();
	 

	 BOOL IsOpened(void){ return(m_bOpened); }

protected:
	 BOOL WriteCommByte(unsigned char);

	 HANDLE m_hIDComDev;
	 OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
	 BOOL m_bOpened;


};

#endif