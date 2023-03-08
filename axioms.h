#ifndef AXIOMS_H
#define AXIOMS_H
#include <vector>

class Wff;
class Axioms {
	std::vector<const Wff *> m_list;
public:
	Axioms ();
	Axioms pop (std::size_t) const;
	Axioms pop () const;
	Axioms add (const Wff &) const;
	bool contains (const Wff &) const;
	const Wff & getAxiom (std::size_t) const;
	std::size_t size () const;
	bool operator <= (const Axioms &) const;
};

#endif
