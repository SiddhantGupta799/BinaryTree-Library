#ifndef TREES_H
#define TRESS_H

//#include <iostream>
//#include <queue>
//#include "Nodes.h"
//using namespace std;

#include <stack>
#include "Queues.h"

namespace Py {


	template<typename T>
	class BinaryTree {
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

	public:
		using iterator = _TreeNode_Iterator_<T>;

		BinaryTree() = default;

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

		iterator root() {
			return iterator(this->_root_);
		}

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
				
				this->cursor = 0;

				this->_root_ = make_tree(this->_root_, this->_root_, porder, iorder, 0, size, size);

				delete[] porder;
				delete[] iorder;
			}
			else {
				this->_root_ = NULL;
			}
		}

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

		void show() {
			this->preorder(this->_root_);
		}

		~BinaryTree() {
			if (this->_root_)
				recursive_cleaner(this->_root_);

			this->_root_ = NULL;
		}
	};
}

#endif // TREES_H
