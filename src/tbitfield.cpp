// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

const unsigned int size_TELEM = sizeof(TELEM) * 8;

TBitField::TBitField(int len)
{
	if (len < 0)
		throw "negative len";
	BitLen = len;
	MemLen = (BitLen - 1) / size_TELEM + 1;
	pMem = new	TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	if (pMem != 0)
		delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n > BitLen) || (n < 0))
		throw "n > Len bitfield";
	return n / size_TELEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n >= BitLen) || (n < 0))
		throw 1;
	return 1 << (n % size_TELEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	int MemInd = GetMemIndex(n);
	pMem[MemInd] = pMem[MemInd] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	int MemInd = GetMemIndex(n);
	pMem[MemInd] = pMem[MemInd] & ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	int MemInd = GetMemIndex(n);
	return pMem[MemInd] & GetMemMask(n - MemInd * size_TELEM);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	delete[] pMem;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if ((BitLen != bf.BitLen))
		return 0;
	for (int i = 0; i < BitLen; i++)
		if (GetBit(i) != bf.GetBit(i))
		{
			return 0;
		}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return (bool)!(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField obl(BitLen > bf.BitLen ? BitLen : bf.BitLen);
	for (int i = 0; i < MemLen; i++)
	{
		obl.pMem[i] = pMem[i];
	}
	for (int i = 0; i < bf.MemLen; i++)
	{
		obl.pMem[i] = obl.pMem[i] | bf.pMem[i];
	}
	return obl;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField obl(BitLen > bf.BitLen ? BitLen : bf.BitLen);
	for (int i = 0; i < MemLen; i++)
	{
		obl.pMem[i] = pMem[i];
	}
	for (int i = 0; i < bf.MemLen; i++)
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
	if ((obl.BitLen % size_TELEM) != 0)
	{
		TELEM x = 1;
		for (int i = 0; i < obl.BitLen % size_TELEM - 1; i++)
		{
			x = x << 1;
			x += 1;
		}
		obl.pMem[MemLen - 1] = obl.pMem[MemLen - 1] & x;
	}
	return obl;

}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int get;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> get;
		bf.SetBit(get);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.GetBit(i)) {
			ostr << i << ' ';
		}
	}
	return ostr;
}