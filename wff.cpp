#include <iostream>
#include "general.h"
#include "wff.h"

WffSubstitution::WffSubstitution (int wff_id, const Wff &A) {
	m_list.push_back(std::make_pair(wff_id, &A));
}
WffSubstitution::WffSubstitution () {}
const Wff * WffSubstitution::searchId (int id) const {
	for (auto &pair : m_list)
		if (pair.first == id)
			return pair.second;
	return nullptr;
}
WffSubstitution WffSubstitution::operator && (const WffSubstitution &S) {
	WffSubstitution res = *this;
	for (auto p : S.m_list)
		res.m_list.push_back(p);
	return res;
}

Wff::Wff (int id, WffType type, string str, const Wff *pA, const Wff *pB):
	m_id(id), m_type(type), m_str(str), m_pA(pA), m_pB(pB) {}

Wff::Wff (string label) {
	for (char c : label)
		if (!isalpha(c))
			Error("Label Error");
	static int wff_count = 0;
	if (label.size() and label[0] >= 'a' and label[0] <= 'z') {
		m_id = ++ wff_count;
		m_type = wffFix;
		m_str = label;
	} else {
		m_id = ++ wff_count;
		m_type = wffVar;
		m_str = label;
	}
}

Wff::Wff (const Wff &A) {
	if (A.m_type == wffVar or A.m_type == wffFix) Error("copy wffVar/Fix");
	m_id = A.m_id;
	m_type = A.m_type;
	m_str = A.m_str;
	m_pA = A.m_pA;
	m_pB = A.m_pB;
}

WffSubstitution Wff::operator = (const Wff &A) const {
	if (m_type != wffVar and m_type != wffFix)
		Error("(not var/fix) = sth.");
	return WffSubstitution(m_id, A);
}

const Wff & Wff::operator ~ () const {
	Wff *res = new Wff(0, wffNot, "(~" + m_str + ")", this, nullptr);
	return *res;
}

const Wff & Wff::operator >> (const Wff &B) const {
	Wff *res = new Wff(0, wffTo, "(" + m_str + "=>" + B.m_str + ")", this, &B);
	return *res;
}

const Wff & Wff::check (const Wff &A) const {
	if (!same_as(A)) Error("Check Error " + m_str + " vs " + A.m_str);
	return *this;
}

const Wff & Wff::outputStr () const {
	std::cout << m_str << std::endl;
	return *this;
}

const Wff & Wff::substitute (WffSubstitution S) const {
	switch (m_type) {
		case wffNot:
			return ~(m_pA->substitute(S));
		case wffTo:
			return m_pA->substitute(S) >> m_pB->substitute(S);
		case wffFix:
			return *this;
		case wffVar:
			const Wff *p = S.searchId(m_id);
			return p == nullptr ? *this : *p;
	}
	Error("substitute");
}
const Wff & Wff::substituteForce (WffSubstitution S) const {
	switch (m_type) {
		case wffNot:
			return ~(m_pA->substituteForce(S));
		case wffTo:
			return m_pA->substituteForce(S) >> m_pB->substituteForce(S);
		case wffFix:
		case wffVar:
			const Wff *p = S.searchId(m_id);
			return p == nullptr ? *this : *p;
	}
	Error("substituteForce");
}


bool Wff::same_as (const Wff & A) const { return m_str == A.m_str; }

string Wff::Str () const { return m_str; }

bool Wff::is_fixed () const {
	switch (m_type) {
		case wffNot:
			return m_pA->is_fixed();
		case wffTo:
			return m_pA->is_fixed() and m_pB->is_fixed();
		case wffFix:
			return true;
		case wffVar:
			return false;
	}
	Error("is_fixed");
}
bool Wff::has_fixed () const {
	switch (m_type) {
		case wffNot:
			return m_pA->has_fixed();
		case wffTo:
			return m_pA->has_fixed() or m_pB->has_fixed();
		case wffFix:
			return true;
		case wffVar:
			return false;
	}
	Error("has_fixed");
}

const Wff & wffMP (const Wff &A, const Wff &B) { // B = (A >> C)
	if (B.m_type == wffTo and B.m_pA->same_as(A))
		return *B.m_pB;
	Error("MP Failed " + A.m_str + " and " + B.m_str);
}
