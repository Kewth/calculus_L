#ifndef WFF_H
#define WFF_H
#include <string>
#include "general.h"
using std::string;

class Wff;

class WffSubstitution {
	int m_wff_id;
	const Wff *m_pA;
public:
	WffSubstitution(int, const Wff &);
	friend class Wff;
};

class Wff {
	int m_type; // > 0 : id
				//  -1 : ~A
				//  -2 : A->B
				//  -3 : fixed symbol
	string m_str;
	const Wff *m_pA, *m_pB;
	Wff (int, string, const Wff *, const Wff *);
	bool _same_as (const Wff & A) const;
public:
	Wff (string);
	Wff (const Wff &);
	WffSubstitution operator = (const Wff &) const;
	const Wff & operator ~ () const;
	const Wff & operator >> (const Wff &) const;
	const Wff & check (const Wff &) const;
	const Wff & outputStr () const;
	const Wff & substitute (WffSubstitution) const;
	string Str () const;
	bool is_fixed () const;
	friend const Wff & MP (const Wff &, const Wff &);
	friend class Theorem;
};

const Wff &MP (const Wff &, const Wff &);

#endif
