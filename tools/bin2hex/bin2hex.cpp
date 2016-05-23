// tools.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string.h>
#include <stdio.h>
void PrintHelp(void)
{
	printf("bin to hex v1.00, Copyright(c) NXP\n");
	printf("Usage: bin2hex.exe bin_file_path base_addr(0x###...) [hex_file_path]\n");
}
#define DEBUG_MODE
#undef DEBUG_MODE
// return: bytes read
int ConvertLine(FILE* fp, int byteCnt, int addr16, char *psz)
{
	
	/*
	format: LL AAAA TT
		TT:
			00 ----数据记录
			01 ----文件结束记录
			02 ----扩展段地址记录
			04 ----扩展线性地址记录 
			05 ----Start Linear Address Record
	*/
	// genera
	unsigned char ckSum = 0;
	unsigned char c[256];
	int bytesRead, byteNdx;
	*psz++ = ':';
	_snprintf(psz, 8 + 1, "%02X%04X00", byteCnt, addr16 & 0xFFFF);
	ckSum = byteCnt + (unsigned char)(addr16 & 0xFF) + (unsigned char)(addr16 >> 8) + 0;
	psz += 8;
	bytesRead = fread(c, 1, byteCnt, fp);
	if (0 == bytesRead)
		return 0;

	for (byteNdx=0; byteNdx<bytesRead; byteNdx++) {
		_snprintf(psz, 2 + 1, "%02X", c[byteNdx]);
		ckSum += c[byteNdx];
		psz += 2;
	}
	ckSum = ~ckSum + 1;
	_snprintf(psz, 2 + 1, "%02X", ckSum);
	psz += 2;
	*psz++ = '\n';
	return bytesRead;
}

int main(int argc, char* argv[])
{
	FILE *fpi, *fpo;
	char szFileIn[256], szFileOut[256], sz[256];
	int inLen, cbData, bytesRead, bytesRem, addr;
#ifndef DEBUG_MODE
	if (argc < 2) {
		PrintHelp();
		goto Cleanup;
	}
	strcpy_s(szFileIn, sizeof(szFileIn), argv[1]);
	sscanf(argv[2], "0x%X", &addr); 
#else
	addr = 0;
	strcpy_s(szFileIn, sizeof(szFileIn), "M:\\er.bin");
	sscanf("0xfF", "0x%X", &addr); 
#endif
	inLen = strlen(szFileIn);
	if (argc < 4) {
		strcpy_s(szFileOut, sizeof(szFileOut), szFileIn);
		szFileOut[inLen - 3] = 'h';
		szFileOut[inLen - 2] = 'e';
		szFileOut[inLen - 1] = 'x';
	} else {
		strcpy_s(szFileOut, sizeof(szFileOut), argv[3]);
	}
	printf("Convert %s at 0x%04X_%04X to %s\n", szFileIn, addr >> 16, addr, szFileOut);
	fopen_s(&fpi, szFileIn, "rb");
	if (fpi == 0)
	{
		printf("Can't open input file %s!\n", szFileIn);
		goto Cleanup;
	}
	fopen_s(&fpo, szFileOut, "w");
	if (fpo == 0) {
		printf("Can't open output file %s!\n", szFileOut);
		fclose(fpi);
	}

	fseek(fpi, 0, SEEK_END);
	cbData = ftell(fpi);
	fseek(fpi, 0, SEEK_SET);

	
	// start from 0x0000####

	bytesRem = cbData;
	while(bytesRem)
	{
		if (addr % 65536 == 0) {
			// print '02' Extended Segment Address Record
			unsigned char ckSum = 2 + 4 + (unsigned char)(addr >> 16);
			ckSum = ~ckSum + 1;
			_snprintf(sz, sizeof(sz), ":02000004%04X%02X\n", addr >> 16 , ckSum);
			fputs(sz, fpo);
		}
		memset(sz, 0, sizeof(sz));
		bytesRead = ConvertLine(fpi, bytesRem > 16 ? 16 : bytesRem, addr & 0xFFFF, sz);
		if (bytesRead)
			fputs(sz, fpo);
		addr += bytesRead;
		bytesRem -= bytesRem > 16 ? 16 : bytesRem;
	}

	fputs(":04000005000000C136\n", fpo);
	fputs(":00000001FF\n", fpo);
	fclose(fpo);
	printf("Done!\n");
Cleanup:
	return 0;
}

