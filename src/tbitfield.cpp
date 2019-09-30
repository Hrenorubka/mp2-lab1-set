// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

unsigned int size_TELEM = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
{
	BitLen = len;
	int a = size_TELEM;
	MemLen = 0;
	while (a < len)
	{
		MemLen++;
		a = a + size_TELEM;
	}
	pMem = new	TELEM[MemLen];
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	if (bf.pMem != pMem)
	{
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = bf.pMem[i];
		}
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n > BitLen)
		throw "n > Len bitfield";
	return (n - 1) / size_TELEM + 1;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n > BitLen)
		throw "n > Len bitfield";
	return ~(pMem[GetMemIndex(n)] & (int) (pow(2 , (n % size_TELEM))));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	int MemInd = GetMemIndex(n);
	pMem[MemInd] = pMem[MemInd] | (int) (pow (2, (n % size_TELEM)));
}

void TBitField::ClrBit(const int n) // очистить бит
{
	int MemInd = GetMemIndex(n);
	if (!(int)(pow(2, (n % size_TELEM))) != 0)
		pMem[MemInd] = pMem[MemInd] & !(int)(pow(2, (n % size_TELEM)));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	return (bool) GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	if (bf.pMem != pMem) 
	{
		if (pMem != 0)
			delete[] pMem;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if ((BitLen != bf.BitLen) || (pMem != bf.pMem))
		return 0;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
		{
			return 0;
		}
	return 1;  
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField obl(*this);
	for (int i = 0; i < MemLen; i++)
	{
		obl.pMem[i] = obl.pMem[i] | bf.pMem[i];
	}
	return obl;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField obl(*this);
	for (int i = 0; i < MemLen; i++)
	{
		obl.pMem[i] = obl.pMem[i] & bf.pMem[i];
	}
	return obl;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField obl(*this);
	for (int i = 0; i < MemLen; i++)
	{
		obl.pMem[i] = ~obl.pMem[i];
	}
	return obl;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	for (int i = 0; i < bf.MemLen; i++)
	{
		istr >> bf.pMem[i];
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.MemLen; i++)
	{
		ostr << bf.pMem[i];
	}
	return ostr;
}
