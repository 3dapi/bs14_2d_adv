//
//
////////////////////////////////////////////////////////////////////////////////


#pragma warning( disable : 4996)


#pragma once


#ifndef __STDAFX_H_
#define __STDAFX_H_


#define	WINMAIN_BASE_APP



#ifdef	WINMAIN_BASE_APP
//win32를 배경으로 컴파일
#pragma comment (linker, "/SUBSYSTEM:WINDOWS") 
#endif

#pragma comment(lib, "../lib/SpLib_00_21.lib")

#include <windows.h>
#include <stdio.h>	
#include <conio.h>
#include <time.h>

#include "../include/SpLib.h"

#include "resource.h"

#include "main.h"

#endif