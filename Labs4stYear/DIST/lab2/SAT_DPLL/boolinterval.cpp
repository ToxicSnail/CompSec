// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "boolinterval.h"
#include "BBV.h"
#include <cstring>
#include<iostream>
BoolInterval::BoolInterval(size_t len)
{
	vec = BBV(len);
	dnc = BBV(len);
}

BoolInterval::BoolInterval(const char *vec_in, const char *dnc_in)
{
	//Проверяем , если строки не пустые и их длины совпадают, то инициализируем вектора, иначе ошибка и в этом случае создаем нулевые веткора мин длины(1 байт).
	if (vec_in && dnc_in && strlen(vec_in) == strlen(dnc_in)) {
		vec = BBV(vec_in);
		dnc = BBV(dnc_in);
	} else {
		vec = BBV(8);
		dnc = BBV(8);
	}

}

BoolInterval::BoolInterval(const char *vector)
{
	if (vector) {
		size_t sz = strlen(vector);
		vec = BBV(sz);
		dnc = BBV(sz);
		int ix = 0;

		while (ix < sz) {
			if (vector[ix] == '-') {
				dnc[ix] = 1;
			} else if (vector[ix] == '1') {
				vec[ix] = 1;
			}

			ix++;
		}
	}
}

BoolInterval::BoolInterval(BBV &vec_in, BBV &dnc_in)
{
	vec = vec_in;
	dnc = dnc_in;

}

void BoolInterval::setInterval(BBV &vec, BBV &dnc)
{
}


bool BoolInterval::operator==(BoolInterval &ibv)
{
	if (vec == ibv.vec && dnc == ibv.dnc) {
		return true;
	}

	return false;
}

bool BoolInterval::operator!=(BoolInterval &ibv)
{
	if (vec != ibv.vec || dnc != ibv.dnc) {
		return true;
	}

	return false;
}

BoolInterval::operator string()
{
	size_t sz = vec.getSize();
	string str(vec.getSize(), '0');

	for (int ix = 0; ix < sz; ix++) {
		str[ix] = getValue(ix);
	}

	return str;
}

int BoolInterval::length()
{
	return vec.getSize();
}

int BoolInterval::rang()
{
	//Определяем ранг интервала
	return (vec.getSize() - dnc.getWeight());
}

bool BoolInterval::isEqualComponent(BoolInterval &ibv)
{
	BBV zero(vec.getSize());
	BBV tmpUV(zero);
	BBV tmpU(zero);
	BBV tmpV(zero);
	BBV answer(zero);
	tmpUV = dnc | ibv.dnc;
	//Output vector for debug
	//cout << (string)tmpUV << endl;
	tmpU = vec | tmpUV;
	//Output vector for debug
	//cout << (string)tmpU << endl;
	tmpV = ibv.vec | tmpUV;
	//Output vector for debug
	//cout << (string)tmpV << endl;
	answer = (tmpU ^ tmpV);
	//Output vector for debug
	answer = (tmpUV | answer);

	if (answer.getWeight() != vec.getSize()) {
		return true;
	}

	return false;

}


bool BoolInterval::isOrthogonal(BoolInterval &ibv)
{
	BBV zero(vec.getSize());
	BBV tmpUV(zero);
	BBV tmpU(zero);
	BBV tmpV(zero);
	BBV answer(zero);
	tmpUV = dnc     | ibv.dnc;
	//Output vector for debug
	//cout << (string)tmpUV << endl;
	tmpU =  vec     | tmpUV;
	//Output vector for debug
	//cout << (string)tmpU << endl;
	tmpV =  ibv.vec | tmpUV;
	//Output vector for debug
	//cout << (string)tmpV << endl;
	answer = (tmpU ^ tmpV);
	//Output vector for debug
	//cout << (string)answer << endl;

	if (answer != zero) {
		return true;
	}

	return false;

}


char BoolInterval::getValue(int ix)
{
	if (ix < 0 || ix > vec.getSize()) {
		throw "Out of range";
	}

	if (dnc[ix] == 1) {
		return '-';
	}

	if (vec[ix] == 1) {
		return '1';
	}

	return '0';
}

void BoolInterval::setValue(char value, int ix)
{
	if (ix < 0 || ix > vec.getSize()) {
		throw "Out of range";
	}

	if (value == '-') {
		dnc[ix] = 1;
		vec[ix] = 0;
	} else if (value == '0') {
		vec[ix] = 0;
		dnc[ix] = 0;
	} else {
		vec[ix] = 1;
		dnc[ix] = 0;
	}
}

