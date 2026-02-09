#include "tree_bag.hpp"
#include <iostream>
#include <cstdlib>  /* For NULL in C++98 */

tree_bag::tree_bag() {
	tree = NULL;
}

tree_bag::tree_bag(const tree_bag &src) {
	tree = copy_node(src.tree);
}

tree_bag::~tree_bag() {
	destroy_tree(tree);
}

tree_bag &tree_bag::operator=(const tree_bag &src) {
	if (this != &src) {
		destroy_tree(tree);
		tree = copy_node(src.tree);
	}
	return *this;
}

tree_bag::node *tree_bag::extract_tree() {
	node *temp = tree;
	tree = NULL;
	return temp;
}

void tree_bag::set_tree(node *new_tree) {
	destroy_tree(tree);
	tree = new_tree;
}

void tree_bag::insert(int item) {
	node *new_node = new node;
	// alloc new node
	std::cout << "create node: " << item << std::endl;
	new_node->value = item;
	new_node->l = NULL;
	new_node->r = NULL;

	if (tree == NULL) {
		/* tree is empty - new node becomes root */
		tree = new_node;
	} else {
		node *current = tree;
		while (true) {
			if (item < current->value) {
				/* go left if item is smaller */
				if (current->l == NULL) {
					current->l = new_node;
					break;
				} else {
					current = current->l;
				}
			} else if (item > current->value) {
				/* go right if item is larger */
				if (current->r == NULL) {
					current->r = new_node;
					break;
				} else {
					current = current->r;
				}
			} else {
				std::cout << "duplicate value: delete node" << std::endl;
				delete new_node;
				break;
			}
		}
	}
}

void tree_bag::insert(int *items, int count) {
	for (int i = 0; i < count; i++) {
		insert(items[i]);
	}
}

void tree_bag::print() const {
	print_node(tree);
	std::cout << std::endl;
}

void tree_bag::clear() {
	destroy_tree(tree);
	tree = NULL;
}

/* Static helper: recursively destroy tree from given node (post-order) */
void tree_bag::destroy_tree(node *current) {
	if (current != NULL) {
		destroy_tree(current->l);
		destroy_tree(current->r);
		delete current;
	}
}

void tree_bag::print_node(node *current) {
	if (current != NULL) {
		print_node(current->l);
		if (current->value != 0)
			std::cout << current->value << " ";
		print_node(current->r);
	}
}

tree_bag::node *tree_bag::copy_node(node *current) {
	if (current == NULL) {
		return NULL;
	} else {
		node *new_node = new node;
		new_node->value = current->value;
		new_node->l = copy_node(current->l);
		new_node->r = copy_node(current->r);
		return new_node;
	}
}