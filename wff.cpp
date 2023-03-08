#include <iostream>
#include "wff.h"

WffSubstitution::WffSubstitution (int wff_id, const Wff &A):
	m_wff_id(wff_id), m_pA(&A) {}

Wff::Wff (int type, string str, const Wff *pA, const Wff *pB):
	m_type(type), m_str(str), m_pA(pA), m_pB(pB) {}

bool Wff::_same_as (const Wff & A) const { return m_str == A.m_str; }

Wff::Wff (string label) {
	for (char c : label)
		if (!isalpha(c))
			Error("Label Error");
	if (label.size() and label[0] >= 'a' and label[0] <= 'z') { // fix
		m_type = -3;
		m_str = label;
	}
	else {
		static int wff_count = 0;
		m_type = ++ wff_count;
		m_str = label;
	}
}

Wff::Wff (const Wff &A) {
	if (A.m_type > 0) Error("A type > 0");
	m_type = A.m_type;
	m_str = A.m_str;
	m_pA = A.m_pA;
	m_pB = A.m_pB;
}

WffSubstitution Wff::operator = (const Wff &A) const {
	if (m_type <= 0) Error("self type <= 0");
	return WffSubstitution(m_type, A);
}

const Wff & Wff::operator ~ () const {
	Wff *res = new Wff(-1, "(~" + m_str + ")", this, nullptr);
	return *res;
}

const Wff & Wff::operator >> (const Wff &B) const {
	Wff *res = new Wff(-2, "(" + m_str + "=>" + B.m_str + ")", this, &B);
	return *res;
}

const Wff & Wff::check (const Wff &A) const {
	if (!_same_as(A))
		Error("Check Error " + m_str + " vs " + A.m_str);
	return *this;
}

const Wff & Wff::outputStr () const {
	std::cout << m_str << std::endl;
	return *this;
}

const Wff & Wff::substitute (WffSubstitution S) const {
	if (m_type > 0)
		return (S.m_wff_id == m_type)
			? *S.m_pA
			: *this;
	else {
		if (m_type == -1) return ~(m_pA->substitute(S));
		if (m_type == -2) return m_pA->substitute(S) >> m_pB->substitute(S);
		if (m_type == -3) return *this;
		Error("sub error");
	}
}

string Wff::Str () const { return m_str; }

bool Wff::is_fixed () const {
	if (m_type > 0) return false;
	else {
		if (m_type == -1) return m_pA->is_fixed();
		if (m_type == -2) return m_pA->is_fixed() and m_pB->is_fixed();
		if (m_type == -3) return true;
		Error("is_fixed error");
	}
}

const Wff &MP (const Wff &A, const Wff &B) { // B = (A >> C)
	if (B.m_type == -2 and B.m_pA->_same_as(A))
		return *B.m_pB;
	Error("MP Failed " + A.m_str + " and " + B.m_str);
}
