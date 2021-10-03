#ifndef TREES_H
#define TRESS_H

//#include <iostream>
//#include <queue>
//#include "Nodes.h"
//using namespace std;

#include <stack>
#include <algorithm>
#include "Queues.h"

namespace Py {

	template<typename T>
	class BinaryTree {
	protected:
		using pointer = TreeNode<T>*;
		using value = TreeNode<T>;

		pointer _root_ = NULL;
		int cursor = 0;

		void recursive_cleaner(pointer temp) {
			if (temp->left) {
				recursive_cleaner(temp->left);
			}

			if (temp->right) {
				recursive_cleaner(temp->right);
			}

			delete temp;
			temp = NULL;
		}

		pointer newTreeNode(T t) {
			pointer Node = new value{ NULL };
			Node->data = t;
			Node->left = NULL;
			Node->right = NULL;

			return Node;
		}

		template<typename _Ty = pointer>
		pointer make_tree(
			_Ty& parent_of_current_pointer,	// to hold the parent of current pointer by address
			_Ty& current_pointer,			// to hold the current pointer by address
			T* pre,							// preorder array
			T* in, int strt, int end,		// inorder array with strt and end indices
			int cursor_boundary				// cursor boundary to stop stack overflow
		) {
			// for the beginning case
			if (this->cursor == 0) {
				// in this case root has to get something initialized first
				T search_entity = pre[this->cursor++];

				this->_root_ = this->newTreeNode(search_entity);

				// searching in inorder
				int root_idx = strt;
				for (; root_idx < end; root_idx++) if (search_entity == in[root_idx]) break;

				this->_root_->left = this->make_tree(this->_root_, this->_root_->left, pre, in, strt, root_idx - 1, cursor_boundary);
				this->_root_->right = this->make_tree(this->_root_, this->_root_->right, pre, in, root_idx + 1, end, cursor_boundary);

				return this->_root_;
			}

			// Returning NULL for unnecessary recurssions
			if (strt > end or this->cursor >= cursor_boundary) return NULL;

			// since current pointer can be right or left so we leave that on compiler
			T search_entity = pre[this->cursor++];
			current_pointer = this->newTreeNode(search_entity);

			current_pointer->up = parent_of_current_pointer;

			// if this node is a leaf
			if (strt == end) return current_pointer;

			// now searching
			int root_idx = strt;
			for (; root_idx < end; root_idx++) if (search_entity == in[root_idx]) break;

			current_pointer->left = make_tree(current_pointer, current_pointer->left, pre, in, strt, root_idx - 1, cursor_boundary);
			current_pointer->right = make_tree(current_pointer, current_pointer->right, pre, in, root_idx + 1, end, cursor_boundary);

			return current_pointer;
		}

		// determine preorder for minimum height
		void _make_preorder_(T* pre, T* in, int size, int strt, int end) {
			if (this->cursor < size and strt <= end) {
				int mid = (strt + end) / 2;

				pre[this->cursor++] = in[mid];

				_make_preorder_(pre, in, size, strt, mid - 1);
				_make_preorder_(pre, in, size, mid + 1, end);
			}
		}

	public:
		using iterator = _TreeNode_Iterator_<T>;

		BinaryTree() = default;

		BinaryTree(T root) {
			this->_root_ = newTreeNode(root);
		}

		BinaryTree(T root, T lchild, T rchild) {
			this->_root_ = this->newTreeNode(root);
			pointer temp_left = this->newTreeNode(lchild);
			pointer temp_right = this->newTreeNode(rchild);

			// connecting left child and right child
			this->_root_->left = temp_left;
			temp_left->up = this->_root_;

			this->_root_->right = temp_right;
			temp_right->up = this->_root_;

		}

		pointer root() { return this->_root_; }

		// error checking and parameter validation
		BinaryTree(initializer_list<T> preorder, initializer_list<T> inorder) {
			if (preorder.size() == inorder.size()) {
				int size = preorder.size();
				T* porder = new T[size];
				T* iorder = new T[size];

				typename initializer_list<T>::iterator pit = preorder.begin();
				typename initializer_list<T>::iterator iit = inorder.begin();

				for (int i = 0; i < size; i++) {
					porder[i] = *pit;
					iorder[i] = *iit;
					pit++;
					iit++;
				}

				/*for (int i = 0; i < size; i++) {
					cout << porder[i] << " ";
				}

				cout << endl;

				for (int i = 0; i < size; i++) {
					cout << iorder[i] << " ";
				}*/

				bool no_errors = true;
				// Error Checking
				//	should be free of duplicates
				//	all elements should be present in both orders
				{
					// mechanism is based on the fact that every element should be present exactly twice in the record array
					// and exactly once in each array

					T* record = new T[size * 2];

					int j = 0;
					for (int i = 0; i < size; i++) {
						record[j++] = iorder[i];
					}

					for (int i = 0; i < size; i++) {
						record[j++] = porder[i];
					}

					for (int i = 0; i < size; i++) {
						int count = 1;
						for (int k = size; k < j; k++) {
							if (iorder[i] == record[k]) count++;
						}
						if (count != 2) {
							no_errors = false;
							break;
						}
					}
					delete[] record;
				}

				if (no_errors) {
					this->cursor = 0;
					this->_root_ = make_tree(this->_root_, this->_root_, porder, iorder, 0, size, size);
				}
				else {
					this->_root_ = NULL;
				}

				delete[] porder;
				delete[] iorder;
			}
			else {
				this->_root_ = NULL;
			}
		}

	protected:

		// function durectly copies the source tree
		template<typename Ty = pointer>
		pointer _copy_tree_(
			pointer src,					// source tree node
			Ty& parent_of_current_pointer,	// parent of the current node
			Ty& current_pointer				// current node
		) {
			if (this->cursor == 0) {
				// copying root
				this->_root_ = this->newTreeNode(src->data);
				this->cursor++;

				this->_root_->left = _copy_tree_(src->left, this->_root_, this->_root_->left);
				this->_root_->right = _copy_tree_(src->right, this->_root_, this->_root_->right);

				return this->_root_;
			}

			if (src) {
				current_pointer = this->newTreeNode(src->data);

				current_pointer->up = parent_of_current_pointer;

				current_pointer->left = _copy_tree_(src->left, current_pointer, current_pointer->left);
				current_pointer->right = _copy_tree_(src->right, current_pointer, current_pointer->right);

				return current_pointer;
			}
			else return NULL;
		}

		int _compare_equality_(pointer a, pointer b) {
			int x = 1, y = 1;
			if (a and b) {
				x = _compare_equality_(a->left, b->left);
				y = _compare_equality_(a->right, b->right);

				if (a->data == b->data)
					return x * y * 1;
				return 0;
			}
			else return 1;
		}

		int _compare_greater_(pointer a, pointer b) {
			int x = 1, y = 1;
			if (a and b) {
				x = _compare_greater_(a->left, b->left);
				y = _compare_greater_(a->right, b->right);

				if (a->data < b->data)
					return x * y * 1;
				else return 0;
			}
			else return 1;
		}

	public:

		BinaryTree(const BinaryTree& bt) {
			this->_root_ = this->_copy_tree_(bt._root_, this->_root_, this->_root_);
		}

		BinaryTree& operator= (const BinaryTree& bt) {
			this->recursive_cleaner(this->_root_);
			this->_root_ = this->_copy_tree_(bt._root_, this->_root_, this->_root_);
			return *this;
		}

		BinaryTree& operator= (BinaryTree&& bt) noexcept {
			this->recursive_cleaner(this->_root_);
			this->_root_ = bt._root_;
			bt._root_ = NULL;
			return *this;
		}

		bool operator== (const BinaryTree& bt) {
			return this->_compare_equality_(this->_root_, bt._root_);
		}

		bool operator!= (const BinaryTree& bt) {
			return !this->_compare_equality_(this->_root_, bt._root_);
		}

		/* This operators dictates that all the nodes in 'bt' are greater than 'this'
		bool operator<(const BinaryTree& bt) {
			return this->_compare_greater_(this->_root_, bt._root_);
		}

		bool operator>=(const BinaryTree& bt) {
			return !this->operator<(bt);
		}*/

		/* These Two Functions are for 'int' data type only */

		void create() {
			Queue<pointer> _que_;	// to remember the address of the tree nodes

			T elem{};

			cout << "Root Value: ";
			cin >> elem;

			this->_root_ = new value{ NULL };
			this->_root_->data = elem;
			this->_root_->left = NULL;
			this->_root_->right = NULL;

			_que_.push(this->_root_);

			// _que_.show();

			cout << "Enter -1 if you want to skip that Child" << endl;

			while (_que_.size() != 0) {

				pointer p = _que_.pop();

				cout << "Enter Left Child of Node " << p->data << ": ";
				cin >> elem;
				// only and only for debugging elem == -1 calls for skipping the current node
				if (elem != -1) {
					pointer temp_left = new value{ NULL };
					temp_left->data = elem;
					temp_left->left = NULL;
					temp_left->right = NULL;

					// now connecting it to the p
					p->left = temp_left;
					temp_left->up = p;

					// pushing temp_left's address in que
					_que_.push(temp_left);
				}

				cout << "Enter Right Child of Node " << p->data << ": ";
				cin >> elem;
				// only and only for debugging elem == -1 calls for skipping the current node
				if (elem != -1) {
					pointer temp_right = new value{ NULL };
					temp_right->data = elem;
					temp_right->left = NULL;
					temp_right->right = NULL;

					// now connecting it to the p
					p->right = temp_right;
					temp_right->up = p;

					// pushing temp_right's address in que
					_que_.push(temp_right);
				}

				/*for (pointer ptr : _que_) {
					cout << ptr->data << " ";
				}cout << endl;*/
			}
		}

		void createSample() {
			/*
			Sample Tree Looks Like
							5
						  /   \
						 /     \
						8       6
						 \     /
						  9   3
						 / \
						/   \
					   4     2
			Preorder:	 5 8 9 4 2 6 3
			Inorder:	 8 4 9 2 5 3 6
			Level order: 5 8 6 9 3 4 2
			*/

			this->_root_ = this->newTreeNode(5);
			pointer temp_left = this->newTreeNode(8);
			pointer temp_right = this->newTreeNode(6);

			// connecting left child and right child
			this->_root_->left = temp_left;
			temp_left->up = this->_root_;

			this->_root_->right = temp_right;
			temp_right->up = this->_root_;

			// child for the Node 8
			temp_right = this->newTreeNode(9);

			// connecting the node from root->left
			this->_root_->left->right = temp_right;
			temp_right->up = this->_root_->left;

			// children for the node 9 
			temp_left = this->newTreeNode(4);
			temp_right = this->newTreeNode(2);

			// connecting the new nodes to 9, which is this->_root_->left->right
			this->_root_->left->right->left = temp_left;
			temp_left->up = this->_root_->left->right;

			this->_root_->left->right->right = temp_right;
			temp_right->up = this->_root_->left->right;


			// making the left of 6 
			temp_left = this->newTreeNode(3);

			// connecting the new node to 6, which is this->_root_->right
			this->_root_->right->left = temp_left;
			temp_left->up = this->_root_->right;
		}

		// Display Mechanisms
		//		In-Order (recursive)
		//		Pre-Order (recursive)
		//		Post-Order (recursive)
		//		Iterative In-Order
		//		Iterative Pre-Order
		//		Iterative Level-Order
	protected:

		void inorder(pointer temp) {
			if (temp) {
				inorder(temp->left);
				cout << temp->data << " ";
				inorder(temp->right);
			}
		}

		void preorder(pointer temp) {
			if (temp) {
				cout << temp->data << " ";
				preorder(temp->left);
				preorder(temp->right);
			}
		}

		void postorder(pointer temp) {
			if (temp) {
				postorder(temp->left);
				postorder(temp->right);
				cout << temp->data << " ";
			}
		}

	public:

		/* Iterative Mechs */

		void iPreOrder() {
			stack<pointer> st;
			pointer temp = this->_root_;
			while (temp or st.size() != 0) {
				if (temp) {
					cout << temp->data << " ";
					st.push(temp);
					temp = temp->left;
				}
				else {
					temp = st.top();
					st.pop();
					temp = temp->right;
				}
			}
			cout << endl;
		}

		void iInOrder() {
			stack<pointer> st;
			pointer temp = this->_root_;

			while (temp or st.size() != 0) {
				if (temp) {
					st.push(temp);
					temp = temp->left;
				}
				else {
					temp = st.top();
					st.pop();
					cout << temp->data << " ";
					temp = temp->right;
				}
			}
			cout << endl;
		}

		void iLevelOrder() {
			Queue<pointer> que;

			cout << this->_root_->data << " ";

			que.push(this->_root_);

			while (que.size() != 0) {

				pointer p = que.pop();
				if (p->left) {
					cout << p->left->data << " ";
					que.push(p->left);
				}
				if (p->right) {
					cout << p->right->data << " ";
					que.push(p->right);
				}
			}
			cout << endl;
		}

		/* Recursive Mechs */

		void inOrder() {
			this->inorder(this->_root_);
		}

		void preOrder() {
			this->preorder(this->_root_);
		}

		void postOrder() {
			this->postorder(this->_root_);
		}

		// Some Functions
		//		Count Leaf Nodes
		//		Count 1 Degree Nodes
		//		Count 2 Degree Nodes
		//		Element Sum
	protected:

		int count_leafs(pointer t) {
			int x = 0, y = 0;
			if (t) {
				x = count_leafs(t->left);
				y = count_leafs(t->right);

				if (!t->left and !t->right) {
					return x + y + 1;
				}
				else return x + y;
			}
			else return 0;
		}

		void show_leafs(pointer t) {
			if (t) {
				show_leafs(t->left);
				show_leafs(t->right);
				if (!t->left and !t->right) {
					cout << t->data << " ";
				}
			}
		}

		int count_nodes(pointer t) {
			int x = 0, y = 0;
			if (t) {
				x = count_nodes(t->left);
				y = count_nodes(t->right);

				return x + y + 1;
			}
			else return 0;
		}

		int count_branches(pointer t) {
			int x = 0, y = 0;
			if (t) {
				x = count_branches(t->left);
				y = count_branches(t->right);

				if (t->left and t->right) {
					return x + y + 1;
				}
				else return x + y;
			}
			else return 0;
		}

		T sum(pointer t) {
			T x{}, y{};
			if (t) {
				x += sum(t->left);
				y += sum(t->right);
				return x + y + t->data;
			}
			else return T{};
		}

		int _height_(pointer t) {
			int x, y;
			if (t) {
				x = _height_(t->left);
				y = _height_(t->right);

				if (x > y) {
					return x + 1;
				}
				else return y + 1;
			}
			else return 0;
		}

	public:

		int height() {
			return this->_height_(this->_root_);
		}

		int countLeafs() {
			return this->count_leafs(this->_root_);
		}

		int countBranches() {
			return this->count_nodes(this->_root_) - 1;
		}

		int countDeg2Branches() {
			return this->count_branches(this->_root_);
		}

		T Sum() {
			return this->sum(this->_root_);
		}

	protected:

		void _make_array_(pointer temp, T* arr, int cursor_boundary) {
			if (temp and this->cursor < cursor_boundary) {
				arr[this->cursor++] = temp->data;
				_make_array_(temp->left, arr, cursor_boundary);
				_make_array_(temp->right, arr, cursor_boundary);
			}
		}

	public:
		// Returns the pointer to the array on heap, containing all the node->data in preorder
		// Note: Requires a variable 'size' to reinitialize it to the actual tree size
		T* get_array_in_preorder(int& size) {
			size = this->count_nodes(this->_root_);

			T* arr_pointer = new T[size];

			this->cursor = 0;

			_make_array_(this->_root_, arr_pointer, size);

			return arr_pointer;
		}

		void log_properties(char const* entry = "") {
			static int i = 0;
			cout << "Log Record: " << i << endl;
			cout << entry << endl;
			cout << "Pre Order: ";  this->iPreOrder();
			cout << "In Order:  "; this->iInOrder();
			cout << "Leaf Count: " << this->countLeafs() << endl;
			cout << "Tree Leaves: "; this->show_leafs(this->_root_); cout << endl;
			cout << "Tree Branches: " << this->countBranches() << endl;
			cout << "Tree 2 Deg Branches: " << this->countDeg2Branches() << endl;
			cout << "Tree Elem Sum: " << this->Sum() << endl;
			cout << "Tree Height: " << this->height() << endl << endl;
			i++;
		}

		~BinaryTree() {
			if (this->_root_)
				recursive_cleaner(this->_root_);

			this->_root_ = NULL;
		}
	};

	// Binay Search Tree, Inherits BinaryTree
	template<typename T>
	class BSTree : public BinaryTree<T> {
		using pointer = TreeNode<T>*;
		using value = TreeNode<T>;

	public:
		BSTree() = default;

		BSTree(T root) : BinaryTree<T>(root) { this->cursor = 0; }

		BSTree(initializer_list<T> preorder) {
			int size = preorder.size();
			T* pre = new T[size];
			T* in = new T[size];

			typename initializer_list<T>::iterator it = preorder.begin();

			for (int i = 0; i < size; i++) {
				pre[i] = *it;
				in[i] = *it;
				++it;
			}

			std::sort(in, in + size);

			this->cursor = 0;
			this->_root_ = this->make_tree(this->_root_, this->_root_, pre, in, 0, size, size);

			delete[] pre;
			delete[] in;
		}

	protected:

		pointer _search_(pointer t, T elem) {
			if (t) {
				if (t->data == elem) {
					return t;
				}
				else if (elem < t->data) {
					this->_search_(t->left, elem);
				}
				else if (elem > t->data) {
					this->_search_(t->right, elem);
				}
			}
			else {
				return t;
			}
		}

		// returns the tail pointer
		pointer _isearch_(T elem) {
			pointer t = this->_root_, tt = NULL;
			while (t != NULL) {
				if (t->data == elem) {
					tt = t;
					break;
				}
				else if (elem < t->data) {
					tt = t;
					t = t->left;
				}
				else if (elem > t->data) {
					tt = t;
					t = t->right;
				}
			}
			return tt;
		}

		pointer _inorder_predecessor_(pointer temp) {
			while (temp and temp->right) {
				temp = temp->right;
			}
			return temp;
		}

		pointer _inorder_successor_(pointer temp) {
			while (temp and temp->left) {
				temp = temp->left;
			}
			return temp;
		}

		pointer _remove_(pointer p, T elem) {
			pointer q;

			if (p == NULL)
				return NULL;

			if (p->left == NULL and p->right == NULL) {
				if (p == this->_root_) {
					this->_root_ = NULL;
					delete p;
					return NULL;
				}
			}

			if (elem < p->left) {
				p->left = this->_remove_(p->left, elem);
			}
			else if (elem > p->right) {
				p->right = this->_remove_(p->right, elem);
			}
			else {
				if (this->_height_(p->left) > this->_height_(p->right)) {
					q = this->_inorder_predecessor_(p->left);
					p->data = q->data;
					p->left = this->_remove_(p->left, q->data);
				}
				else {
					q = this->_inorder_successor_(p->right);
					p->data = q->data;
					p->right = this->_remove_(p->right, q->data);
				}
			}
			return p;
		}

	public:
		pointer search(T elem) {
			return this->_search_(this->_root_, elem);
		}

		void insert(T elem) {
			pointer temp = this->_isearch_(elem);
			// if the element is found then it should not get inserted
			if (temp) {

				if (elem < temp->data) {
					pointer ttemp = this->newTreeNode(elem);
					ttemp->up = temp;
					temp->left = ttemp;
				}
				else if (elem > temp->data) {
					pointer ttemp = this->newTreeNode(elem);
					ttemp->up = temp;
					temp->right = ttemp;
				}
			}
		}

		void remove(T elem) {
			this->_remove_(this->_root_, elem);
		}
	};

	/* Minimum Height Tree
		Properties:
			- Once the Inorder of the tree is passed to it, 
			  it will make the most compact Binary Tree out of it
	*/
	template<typename T>
	class MinHeightTree : public BinaryTree<T> {

	public:
		MinHeightTree() = default;

		MinHeightTree(initializer_list<T> inorder) {
			int size = inorder.size();
			T* pre = new T[size];
			T* in = new T[size];

			typename initializer_list<T>::iterator it = inorder.begin();

			for (int i = 0; i < size; i++) {
				in[i] = *it;
				++it;
			}

			this->cursor = 0;
			this->_make_preorder_(pre, in, size, 0, size);

			/*for (int i = 0; i < size; i++) {
				cout << pre[i] << " ";
			}cout << endl;
			*/

			this->cursor = 0;
			this->_root_ = this->make_tree(this->_root_, this->_root_, pre, in, 0, size, size);

			delete[] pre;
			delete[] in;
		}
	};

	/* Adelson - Velsky and Landis Tree
		Properties:
			- If provided a random order as initializer 
			  list this class will make the most optimized binary search tree.
			- If only provided a root and all other values came from insertion via insert(), then calling 
			  balance() and passing its root will optimize the tree to AVL Standards
	*/
	template<typename T>
	class AVLTree : public BSTree<T> {
		using pointer = TreeNode<T>*;
		using value = TreeNode<T>;

	public:
		AVLTree(T root) {
			this->_root_ = this->newTreeNode(root);
			this->cursor++;
		}

		// takes any random order, makes the most balanced tree out of it
		AVLTree(initializer_list<T> random_order) {
			int size = random_order.size();
			T* pre = new T[size];
			T* in = new T[size];

			typename initializer_list<T>::iterator it = random_order.begin();

			for (int i = 0; i < size; i++) {
				in[i] = *it;
				++it;
			}

			std::sort(in, in + size);

			this->cursor = 0;
			this->_make_preorder_(pre, in, size, 0, size);

			/*for (int i = 0; i < size; i++) {
				cout << pre[i] << " ";
			}cout << endl;
			*/

			this->cursor = 0;
			this->_root_ = this->make_tree(this->_root_, this->_root_, pre, in, 0, size, size);

			delete[] pre;
			delete[] in;
		}

	protected:

		// All Cases Covered
		pointer LLRotation(pointer p) {
			/*
			case 1: Tree Looks like
						1
					   /
					  2		=>		2
					 /			   / \
					3			  1   3

			Case 2: Tree Looks like
							5
						   /
						  3			=>		3
						 / \			   / \
						2	4			  2   5
											 /
											4
			*/
			// checking for case
			int _case_ = 0;
			if (p->left->right == NULL) _case_ = 1;
			else _case_ = 2;

			// If current node is the root, then there is no need to keep the record of up
			if (p == this->_root_) {
				// execute case 1 and case 2 separately
				if (_case_ == 1) {
					// shifting the root to right
					this->_root_ = this->_root_->left;

					// breaking the link
					this->_root_->up = NULL;
					p->left = NULL;

					// establishing new link
					this->_root_->right = p;
					p->up = this->_root_;


					/* Passed */
				}
				else if (_case_ == 2) {
					pointer plr = this->_root_->left->right;

					// moving root to its left
					this->_root_ = this->_root_->left;

					// breaking all the links
					p->left = NULL;
					this->_root_->up = NULL;
					this->_root_->right = NULL;
					plr->up = NULL;

					// restablishing connections
					this->_root_->right = p;
					p->up = this->_root_;

					p->left = plr;
					plr->up = p;

					/*  Passed  */
				}

				return this->_root_;
			}
			// since p is not the root we need to take hold of the 'up' reference
			else {
				// we need to decide on which side of p's parent is this imbalance located i.e. right or left
				pointer p_up = p->up;
				bool left_or_right = 0; // 0 for left 1 for right
				// recording and breaking the link
				if (p_up->left == p) {
					left_or_right = 0;
					p_up->left = NULL;
				}
				else {
					left_or_right = 1;
					p_up->right = NULL;
				}

				// breaking the 'up' link
				p->up = NULL;

				if (_case_ == 1) {
					pointer pl = p->left;

					// breaking the links
					p->left = NULL;
					pl->up = NULL;

					// both nodes are isolated, now establishing connections
					pl->right = p;
					p->up = pl;

					// now connecting to the parent on the respective side
					if (left_or_right == 0) {
						p_up->left = pl;
						pl->up = p_up;
					}
					else {
						p_up->right = pl;
						pl->up = p_up;
					}

					/*  Passed  */
				}
				else if (_case_ == 2) {
					pointer pl = p->left;
					pointer plr = pl->right;

					// breaking the links
					p->left = NULL;
					pl->up = NULL;
					pl->right = NULL;
					plr->up = NULL;

					// all three nodes are isolated
					pl->right = p;
					p->up = pl;
					p->left = plr;
					plr->up = p;

					// now connecting to the parent on the respective side
					if (left_or_right == 0) {
						p_up->left = pl;
						pl->up = p_up;
					}
					else {
						p_up->right = pl;
						pl->up = p_up;
					}

					/*  Passed  */
					return pl;
				}
			}

		}

		// All Cases Covered
		pointer LRRotation(pointer p) {
			/*
			case 1: Tree Looks like
						4
					   /
					  2			=>		3
					   \			   / \
						3			  2   4

			Case 2: Tree Looks like
							5
						   /
						  3			=>		3
						 / \			   / \
						2	4			  2   5
											 /
											4
			Note: Case 2 for LRRotation is same as Case 2 of LLRotation,
				  so passing all of the case 2 to LLRotation will be okay
			*/

			int _case_ = 0;
			if (p->left->left == NULL) _case_ = 1;
			else _case_ = 2;

			if (p == this->_root_) {
				if (_case_ == 1) {
					pointer pl = p->left;
					pointer plr = pl->right;

					// breaking the link
					p->left = NULL;
					pl->up = NULL;
					pl->right = NULL;
					plr->up = NULL;

					// connecting in correct order
					p->left = plr;
					plr->up = p;
					plr->left = pl;
					pl->up = plr;

					return this->LLRotation(p);

					/*Passed*/
				}
				else if (_case_ == 2) {
					return this->LLRotation(p);
					/*Passed*/
				}
			}
			else {
				// we need to decide on which side of p's parent is this imbalance located i.e. right or left
				if (_case_ == 2) { return this->LLRotation(p); /*Passed*/ }
				else if (_case_ == 1) {
					pointer pl = p->left;
					pointer plr = pl->right;

					// breaking the link
					p->left = NULL;
					pl->up = NULL;
					pl->right = NULL;
					plr->up = NULL;

					// now connecting
					p->left = plr;
					plr->up = p;
					plr->left = pl;
					pl->up = plr;

					return this->LLRotation(p);
					/*Passed*/
				}
			}
		}

		// All Cases Covered
		pointer RRRotation(pointer p) {
			/*
			case 1: Tree Looks like
						1
						 \
						  2			=>		2
						   \			   / \
							3			  1   3

			Case 2: Tree Looks like
						1
						 \
						  3			=>		3
						 / \			   / \
						2	4			  1   4
										   \
											2

			*/
			// checking for case
			int _case_ = 0;
			if (p->right->left == NULL) _case_ = 1;
			else _case_ = 2;

			// If current node is the root, then there is no need to keep the record of up
			if (p == this->_root_) {
				// execute case 1 and case 2 separately
				if (_case_ == 1) {
					// shifting the root to right
					this->_root_ = this->_root_->right;

					// breaking the link
					this->_root_->up = NULL;
					p->right = NULL;

					// establishing new link
					this->_root_->left = p;
					p->up = this->_root_;

					/* Passed */
				}
				else if (_case_ == 2) {
					pointer prl = this->_root_->right->left;

					this->_root_ = this->_root_->right;

					// disconnecting wrong links
					this->_root_->up = NULL;
					p->right = NULL;
					this->_root_->left = NULL;
					prl->up = NULL;

					// connecting the favourable ones
					this->_root_->left = p;
					p->up = this->_root_;

					p->right = prl;
					prl->up = p;

					/*  Passed  */
				}
				return this->_root_;
			}
			// since p is not the root we need to take hold of the 'up' reference
			else {
				// we need to decide on which side of p's parent is this imbalance located i.e. right or left
				pointer p_up = p->up;
				bool left_or_right = 0; // 0 for left 1 for right
				// recording and breaking the link
				if (p_up->left == p) {
					left_or_right = 0;
					p_up->left = NULL;
				}
				else {
					left_or_right = 1;
					p_up->right = NULL;
				}

				// breaking the 'up' link
				p->up = NULL;

				if (_case_ == 1) {
					pointer pr = p->right;
					pointer prr = pr->right;

					// breaking the links
					p->right = NULL;
					pr->up = NULL;

					// both nodes are isolated, now establishing connections
					pr->left = p;
					p->up = pr;

					// now connecting to the parent on the respective side
					if (left_or_right == 0) {
						p_up->left = pr;
						pr->up = p_up;
					}
					else {
						p_up->right = pr;
						pr->up = p_up;
					}

					/*  Passed  */
					return pr;
				}
				else if (_case_ == 2) {
					pointer pr = p->right;
					pointer prl = pr->left;

					// breaking the links
					p->right = NULL;
					pr->up = NULL;
					pr->left = NULL;
					prl->up = NULL;

					// all three nodes are isolated
					pr->left = p;
					p->up = pr;
					p->right = prl;
					prl->up = p;

					// now connecting to the parent on the respective side
					if (left_or_right == 0) {
						p_up->left = pr;
						pr->up = p_up;
					}
					else {
						p_up->right = pr;
						pr->up = p_up;
					}

					/*  Passed  */
					return pr;
				}
			}
		}

		// All Cases Covered
		pointer RLRotation(pointer p) {
			int _case_ = 0;
			if (p->right->right == NULL) _case_ = 1;
			else _case_ = 2;

			if (p == this->_root_) {
				if (_case_ == 1) {
					pointer pr = p->right;
					pointer prl = pr->left;

					// breaking the link
					p->right = NULL;
					pr->up = NULL;
					pr->left = NULL;
					prl->up = NULL;

					// connecting in correct order
					p->right = prl;
					prl->up = p;
					prl->right = pr;
					pr->up = prl;

					return this->RRRotation(p);

					/*Passed*/
				}
				else if (_case_ == 2) {
					return this->RRRotation(p);
					/*Passed*/
				}
			}
			else {
				// we need to decide on which side of p's parent is this imbalance located i.e. right or left
				if (_case_ == 2) { return this->RRRotation(p); /*Passed*/ }
				else if (_case_ == 1) {
					pointer pr = p->right;
					pointer prl = pr->left;

					// breaking the link
					p->right = NULL;
					pr->up = NULL;
					pr->left = NULL;
					prl->up = NULL;

					// now connecting
					p->right = prl;
					prl->up = p;
					prl->right = pr;
					pr->up = prl;

					return this->RRRotation(p);
					/*Passed*/
				}
			}
		}

		/*
		Their is no flaw in the Rotation Functions as
		long as they are operating on nodes with balance factor 2
		*/

		int _balance_factor_(pointer temp) {
			int hl = 0, hr = 0;
			if (temp) {
				hl = this->_height_(temp->left);
				hr = this->_height_(temp->right);

				return hl - hr;
			}
			else return 0;
		}

		bool is_permitted(int bal_fac) {
			if (bal_fac >= -1 and bal_fac <= 1) return true;
			else return false;
		}
	
	public:
		pointer balance(pointer p) {

			if (p and p->left) {
				this->balance(p->left);
			}if (p and p->right) {
				this->balance(p->right);
			}

			int bal_fac = this->_balance_factor_(p);
			int left_bal_fac = this->_balance_factor_(p->left);
			int right_bal_fac = this->_balance_factor_(p->right);

			if (is_permitted(bal_fac)) { return p; }

			// if current node has any of these imbalances it will get rectified here only
			// until it comes in the permitted range
			while (!is_permitted(bal_fac)) {
				if (bal_fac > 0) {
					if (left_bal_fac > 0) p = this->LLRotation(p);
					else p = this->LRRotation(p);
				}

				if (bal_fac < 0) {
					if (right_bal_fac < 0) p = this->RRRotation(p);
					else p = this->RLRotation(p);
				}

				bal_fac = this->_balance_factor_(p);
				left_bal_fac = this->_balance_factor_(p->left);
				right_bal_fac = this->_balance_factor_(p->right);
			}

			return p;
		}
	};

}

#endif // TREES_H
