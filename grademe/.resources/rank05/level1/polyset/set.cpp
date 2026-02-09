#include "set.hpp"

set::set(searchable_bag &other) : bag(other) {}

set::~set() {}

bool set::has(int nbr) const
{
  return bag.has(nbr);
}

void set::insert (int nbr)
{
  bag.insert(nbr);
}

void set::insert (int *arr, int size)
{
  bag.insert(arr, size);
}

void set::print() const
{
  bag.print();
}

void set::clear()
{
  bag.clear();
}

searchable_bag& set::get_bag()
{
  return bag;
}