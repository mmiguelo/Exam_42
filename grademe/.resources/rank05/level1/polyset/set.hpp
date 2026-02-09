#pragma once

#include "searchable_bag.hpp"

class set {
  private:
    searchable_bag &bag;

  public:
    //set();
    set(searchable_bag &other);
    set &operator=(const searchable_bag &other);
    ~set();
    bool has(int nbr) const;
    void insert (int);
    void insert (int *, int);
	  void print() const;
	  void clear();
    searchable_bag& get_bag();
};