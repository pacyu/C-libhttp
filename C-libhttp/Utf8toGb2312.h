#pragma once
#include "Unicode_Gb2312_Table.h"
void wchar_to_utf8(char*&cstr, const wchar_t* u)
{
	unsigned codepoint = 0;
	for (int i = 0; *u; u++)
	{
		if (*u >= 0xd800 && *u <= 0xdbff)
			codepoint = ((*u - 0xd800) << 10) + 0x10000;
		else
		{
			if (*u >= 0xdc00 && *u <= 0xdfff)
				codepoint |= *u - 0xdc00;
			else
				codepoint = *u;
			if (codepoint <= 0x007F)
				cstr += codepoint;
			else if (codepoint >= 0x0080 && codepoint <= 0x07FF)
			{
				cstr[i++] = (0xC0 | ((codepoint >> 12) & 0xFF));	/* 0xC0 = 11000000 */
				cstr[i++] = (0x80 | ((codepoint >> 6) & 0x3F));	/* 0x80 = 10000000 */
			}
			else if (codepoint >= 0x0800 && codepoint <= 0xFFFF)
			{
				cstr[i++] = (0xE0 | ((codepoint >> 12) & 0xFF));	/* 0xE0 = 11100000 */
				cstr[i++] = (0x80 | ((codepoint >> 6) & 0x3F));	/* 0x80 = 10000000 */
				cstr[i++] = (0x80 | (codepoint & 0x3F));			/* 0x3F = 00111111 */
			}
			else if (codepoint >= 0x10000 && codepoint <= 0x10FFFF)
			{
				cstr[i++] = (0xF0 | ((codepoint >> 18) & 0x07));	/* 0xF0 = 11110000 */
				cstr[i++] = (0x80 | ((codepoint >> 12) & 0x3F));	/* 0x3F = 00111111 */
				cstr[i++] = (0x80 | ((codepoint >> 6) & 0x3F));
				cstr[i++] = (0x80 | ((codepoint) & 0x3F));
			}
			codepoint = 0;
		}
	}
}
void wchar_to_char(char*&out, const wchar_t*in)
{
	for (int i = 0, j = 0; in[i]; ++i)
		if (in[i] >= 0 && in[i] < 256)
			out[j++] = static_cast<char>(in[i]);
}
void utf8_to_wchar(wchar_t*& pws, const char* cstr)
{
	unsigned codepoint;
	for (int i = 0; *cstr != 0; i++)
	{
		unsigned char ch = static_cast<unsigned char>(*cstr);
		if (ch <= 0x7f)
			codepoint = ch;
		else if (ch <= 0xbf)
			codepoint = (codepoint << 6) | (ch & 0x3f);
		else if (ch <= 0xdf)
			codepoint = ch & 0x1f;
		else if (ch <= 0xef)
			codepoint = ch & 0x0f;
		else
			codepoint = ch & 0x07;
		++cstr;
		if (((*cstr & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
		{
			if (codepoint > 0xffff)
			{
				pws[i++] = (0xd800 + (codepoint >> 10));
				pws[i++] = (0xdc00 + (codepoint & 0x03ff));
			}
			else if (codepoint < 0xd800 || codepoint >= 0xe000)
				pws[i++] = codepoint;
		}
	}
}
class EncodeConvert
{
public:
	EncodeConvert() {}
	bool UTF_8ToGB2312(char*& pOut, char* pInput, int pLen)
	{
		int res, i = 0, j = 0;
		char tempbuf[10];
		memset(tempbuf, 0, sizeof(tempbuf));
		while (i < pLen)
		{
			res = UTF_8ToUnicode(tempbuf, pInput + i);
			if (res <= 0)
				return false;
			else if (res == 1)
			{
				pOut[j] = pInput[i];
				j += 1;
			}
			else
			{
				UnicodeToGB2312(pOut + j, tempbuf);
				j += 2;
			}
			i += res;
		}
		return true;
	}
private:
	int UTF8ToUnicode(char* pOutput, char* pInput)
	{
		char b1, b2, b3, b4, b5, b6;
		int len = enc_get_utf8_size(*pInput);
		switch (len)
		{
		case 0:
			*pOutput = *pInput;
			len += 1;
			break;
		case 2:
			b1 = *pInput;
			b2 = *(pInput + 1);
			if ((b2 & 0xC0) != 0x80)
				return 0;
			*pOutput = (b1 << 6) + (b2 & 0x3F);
			*(pOutput + 1) = (b1 >> 2) & 0x07;
			break;
		case 3:
			b1 = *pInput;
			b2 = *(pInput + 1);
			b3 = *(pInput + 2);
			if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80))
				return 0;
			*pOutput = ((b2 & 0x03) << 6) + (b3 & 0x3F);
			*(pOutput + 1) = ((b1 & 0x0F) << 4) | ((b2 >> 2) & 0x0F);
			break;
		case 4:
			b1 = *pInput;
			b2 = *(pInput + 1);
			b3 = *(pInput + 2);
			b4 = *(pInput + 3);
			if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
				|| ((b4 & 0xC0) != 0x80))
				return 0;
			*pOutput = (b3 << 6) + (b4 & 0x3F);
			*(pOutput + 1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
			*(pOutput + 2) = ((b1 << 2) & 0x1C) + ((b2 >> 4) & 0x03);
			break;
		case 5:
			b1 = *pInput;
			b2 = *(pInput + 1);
			b3 = *(pInput + 2);
			b4 = *(pInput + 3);
			b5 = *(pInput + 4);
			if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
				|| ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80))
				return 0;
			*pOutput = (b4 << 6) + (b5 & 0x3F);
			*(pOutput + 1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
			*(pOutput + 2) = (b2 << 2) + ((b3 >> 4) & 0x03);
			*(pOutput + 3) = (b1 << 6);
			break;
		case 6:
			b1 = *pInput;
			b2 = *(pInput + 1);
			b3 = *(pInput + 2);
			b4 = *(pInput + 3);
			b5 = *(pInput + 4);
			b6 = *(pInput + 5);
			if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
				|| ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
				|| ((b6 & 0xC0) != 0x80))
				return 0;
			*pOutput = (b5 << 6) + (b6 & 0x3F);
			*(pOutput + 1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
			*(pOutput + 2) = (b3 << 2) + ((b4 >> 4) & 0x03);
			*(pOutput + 3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
			break;
		default:
			return 0;
		}
		return len;
	}
	void UnicodeToGB2312(char* pOut, char* pInput)
	{
		unsigned short tmp, gb2312_tmp;
		memcpy(&tmp, pInput, 2);
		gb2312_tmp = SearchCodeTable(tmp);

		gb2312_tmp = (gb2312_tmp >> 8 | gb2312_tmp << 8); // 调整为小端，大地址存高位，小地址存低位

		memcpy(pOut, &gb2312_tmp, 2);
	}
	int enc_get_utf8_size(const unsigned char pInput)
	{
		int num = 0, temp = 0x80;
		while (temp&pInput)
		{
			num++;
			temp = (temp >> 1);
		}
		return num;
	}
	unsigned short SearchCodeTable(unsigned short unicode)
	{
		int mid, start = 0, end = TABLE_LEN - 1;
		while (start <= end)
		{
			mid = (start + end) / 2;
			if (unicode_gb_table[mid].unicode == unicode)
				return unicode_gb_table[mid].gb2312;
			else if (unicode_gb_table[mid].unicode < unicode)
				start = mid + 1;
			else
				end = mid - 1;
		}
		return 0;
	}
};
