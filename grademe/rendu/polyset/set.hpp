#include "searchable_bag.hpp"

class set
{
private:
	searchable_bag& _bag;
public:
	set(searchable_bag& bag);
	~set();
	set(const set& other);
	set& operator=(const set& other);
	bool has(int) const;

	void insert(int);
	void insert(int *, int);
	void print() const;
	void clear();

	searchable_bag& get_bag() const;
};

