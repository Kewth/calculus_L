#ifndef AXIOMS_H
#define AXIOMS_H
#include <vector>

class Theorem;
class Wff;
class Axioms {
	int m_id;
	std::vector<Theorem> m_list;
	Axioms copy () const;
public:
	Axioms ();
	Axioms add (const Wff &) const;
	Axioms pop (std::size_t) const;
	Axioms pop () const;
	int getId () const;
	Theorem getAxiom (std::size_t) const;
	std::size_t size () const;
	bool checkId (Theorem) const;
	bool operator == (Axioms) const;
};

#endif
