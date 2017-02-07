/*=============================================================================
	Crypt.h: º”√‹¿‡…˘√˜.
	Copyright 2009-2013 TR Games, Inc. All Rights Reserved.
=============================================================================*/


#pragma once


class CCrypt
{
public:
	static bool Encrypt(unsigned char* source, unsigned char* destination, unsigned long length);
	static bool Decrypt(unsigned char* source, unsigned char* destination, unsigned long length);
};