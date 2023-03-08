#ifndef WFF_H
#define WFF_H
#include <string>
#include <vector>
using std::string;

class Wff;

class WffSubstitution {
	std::vector<std::pair<int, const Wff *> > m_list;
public:
	WffSubstitution (int, const Wff &);
	WffSubstitution ();
	const Wff * searchId (int) const;
	WffSubstitution operator && (const WffSubstitution &);
};

enum WffType { wffNot, wffTo, wffFix, wffVar };
class Wff {
	int m_id;
	WffType m_type;
	string m_str;
	const Wff *m_pA, *m_pB;
	Wff (int, WffType, string, const Wff *, const Wff *);
public:
	Wff (string);
	Wff (const Wff &);
	WffSubstitution operator = (const Wff &) const; // not an assginment
	const Wff & operator ~ () const;
	const Wff & operator >> (const Wff &) const;
	const Wff & check (const Wff &) const;
	const Wff & outputStr () const;
	const Wff & substitute (WffSubstitution) const;
	const Wff & substituteForce (WffSubstitution) const;
	bool same_as (const Wff & A) const;
	string Str () const;
	bool is_fixed () const;
	bool has_fixed () const;
	friend const Wff & wffMP (const Wff &, const Wff &);
	friend class Theorem;
};

const Wff & wffMP (const Wff &, const Wff &);

#endif
