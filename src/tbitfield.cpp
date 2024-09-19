// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"


// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int _BitLen)
{
	if (_BitLen <= 0) throw - 1;

	BitLen = _BitLen;
	int tmp = sizeof(TELEM) * 8;
	MemLen = (_BitLen + (tmp - 1)) / tmp;

	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;

	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const
{
	if (!(n >= 0 && n < BitLen)) throw - 2;

	return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const
{
	if (!(n >= 0 && n < BitLen)) throw - 2;

	return 1 << (n % 32);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const		// получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n)
{
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)			// очистить бит
{
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const	// получить значение бита
{
	// return 0 if bit==0
	// return (not 0) if bit!=0

	return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf)
{
	if (this == &bf) return *this;
	if (bf.MemLen != MemLen) {
		delete[] pMem;
		pMem = new TELEM[bf.MemLen];
		MemLen = bf.MemLen;
	}
	BitLen = bf.BitLen;

	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (bf.BitLen != BitLen) return false;

	for (int i = 0; i < MemLen - 1; i++) {
		if (bf.pMem[i] != pMem[i]) return false;
	}

	for (int i = sizeof(TELEM) * 8 * (MemLen - 1); i < BitLen; i++) {
		if (bf.GetBit(i) != GetBit(i)) return false;
	}

	return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	/*if (bf.BitLen == BitLen) return false;

	for (int i = 0; i < MemLen - 1; i++) {
		if (bf.pMem[i] == pMem[i]) return false;
	}

	for (int i = sizeof(TELEM) * 8 * (MemLen - 1); i < BitLen; i++) {
		if (bf.GetBit(i) == GetBit(i)) return false;
	}

	return true;*/

	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField newField = TBitField((bf.BitLen > BitLen) ? bf.BitLen : BitLen);

	int minLen = (bf.MemLen < MemLen) ? bf.MemLen : MemLen;
	for (int i = 0; i < minLen; i++) {
		newField.pMem[i] = this->pMem[i] | bf.pMem[i];
	}

	if (bf.MemLen > MemLen) {
		for (int i = minLen; i < newField.MemLen; i++) {
			newField.pMem[i] = bf.pMem[i];
		}
	}
	else {
		for (int i = minLen; i < newField.MemLen; i++) {
			newField.pMem[i] = pMem[i];
		}
	}
		
	return newField;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	// результат наибольшей длины 

	TBitField newField = TBitField((bf.BitLen > BitLen) ? bf.BitLen : BitLen);

	int minLen = (bf.MemLen < MemLen) ? bf.MemLen : MemLen;
	for (int i = 0; i < minLen; i++) {
		newField.pMem[i] = this->pMem[i] & bf.pMem[i];
	}

    return newField;
}

TBitField TBitField::operator~(void) // отрицание
{
	// биты в последних ячеёках (лишние) инвертируются
	TBitField newField = TBitField(BitLen);

	for (int i = 0; i < MemLen - 1; i++) {
		newField.pMem[i] = ~pMem[i];
	}

	for (int i = sizeof(TELEM) * 8 * (MemLen - 1); i < BitLen; i++) {
		if (!GetBit(i)) newField.SetBit(i);
	}

	return newField;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int bit;
	for (int i = 0; i < bf.BitLen; i++) {
		cin >> bit;
		if (bit) bf.SetBit(i);
		else bf.ClrBit(i);
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.GetBit(i)) ostr << 1;
		else ostr << 0;
	}

	return ostr;
}
