#pragma once

#include <stdint.h>
#include <string.h>
#define IF_PROGMEM PROGMEM
#include <avr/pgmspace.h>


class aes {
public:
	aes();
	~aes();

	int counter = 0;

	void Crypt(uint8_t * data, uint8_t * key, uint8_t * IV);
	void Decrypt(uint8_t * data, uint8_t * key, uint8_t * IV);

private:

	void Sanitize();

	void VecToBox(uint8_t(*arr)[4][4], uint8_t * data);
	void BoxToVec(uint8_t(*arr)[4][4], uint8_t * data);

	void GetFGVector(uint8_t(*arr)[4][4], uint8_t round, uint8_t * output);
	void ExpandKey(uint8_t(*keyBox)[4][4], uint8_t round);
	void RewindKey(uint8_t(*keyBox)[4][4], uint8_t round);
	void AddRoundKey(uint8_t(*text)[4][4], uint8_t(*curKeyBox)[4][4]);

	void Substitution(uint8_t(*text)[4][4], int mode);
	void ShiftRow(uint8_t(*arr)[4][4], int row, int mode);
	void ShiftRows(uint8_t(*text)[4][4], int mode);
	int MultiplyBy2(uint8_t value);
	void MixColumn(uint8_t(*text)[4][4], int mode);
	void CryptRound(uint8_t(*text)[4][4], uint8_t(*curKeyBox)[4][4], int curRound);
	void DecryptRound(uint8_t(*text)[4][4], uint8_t(*curKeyBox)[4][4], int curRound);


	uint8_t keyBox[4][4];
	uint8_t toCipherOrPlain[4][4];
	uint8_t tempBox[4][4];
	uint8_t fgVec[4];

};

