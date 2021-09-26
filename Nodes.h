#ifndef NODES_H
#define NODES_H

#include <iostream>
using namespace std;

namespace Py {
//#################################################################################################################
	/*		 Various Types of Nodes			*/

	// Singular Node
	template<typename T>
	struct SingleNode {
		T data;
		SingleNode<T>* next;
	};

	// Bi-Directional Node
	template<typename T>
	struct BinaryNode {
		BinaryNode<T>* prev;
		T data;
		BinaryNode<T>* next;
	};

	// Tree Node and also TriNode
	template<typename T>
	struct TreeNode {
		TreeNode<T>* up;
		T data;
		TreeNode<T>* left;
		TreeNode<T>* right;
	};

	// 4 Directional Node
	template<typename T>
	struct QuadNode {
		QuadNode<T>* north;
		QuadNode<T>* east;
		T data;
		QuadNode<T>* west;
		QuadNode<T>* south;
	};

//#################################################################################################################
	/*		Supporting Iterators for each type of Node			*/

	// Forward Iterator
	template<typename T>
	class _SingleNode_Iterator_ {
		using pointer = SingleNode<T>*;
	
	protected:
		pointer __iter__ = NULL;

	public:
		_SingleNode_Iterator_(SingleNode<T>* _pointer_) {
			this->__iter__ = _pointer_;
		}

		_SingleNode_Iterator_ (const _SingleNode_Iterator_& obj) {
			this->__iter__ = obj.__iter__;
		}

		_SingleNode_Iterator_& operator=(const _SingleNode_Iterator_& obj) {
			this->__iter__ = obj.__iter__;
		}

		_SingleNode_Iterator_& operator++() {
			this->__iter__ = this->__iter__->next;
			return *this;
		}

		_SingleNode_Iterator_& operator++(int) {
			_SingleNode_Iterator_ __temp__(this->__iter__);
			this->__iter__ = this->__iter__->next;
			return __temp__;
		}
		
		_SingleNode_Iterator_& operator+=(int num) {
			for (; num > 0; num--) {
				this->__iter__ = this->__iter__ ? this->__iter__->next : NULL;
			}
			return *this;
		}

		T& operator*() {
			return &(this->__iter__->data);
		}

		bool operator== (_SingleNode_Iterator_ obj) {
			return this->__iter__ == obj.__iter__;
		}

		bool operator!= (_SingleNode_Iterator_ obj) {
			return !(this->__iter__ == obj.__iter__);
		}

		bool operator==(SingleNode<T>* obj) {
			return this->__iter__ == obj;
		}

		bool operator!= (SingleNode<T>* obj) {
			return !(this->__iter__ == obj);
		}

		~_SingleNode_Iterator_() {
			this->__iter__ = NULL;
		}
	};

	// Const Forward Iterator
	template<typename T>
	class _Const_SingleNode_Iterator_  :public _SingleNode_Iterator_<T> {
		using pointer = SingleNode<T>*;

	public:
		_Const_SingleNode_Iterator_(SingleNode<T>* _pointer_) : _SingleNode_Iterator_<T>(_pointer_){}

		_Const_SingleNode_Iterator_(const _Const_SingleNode_Iterator_& obj) {
			this->__iter__ = obj.__iter__;
		}

		_Const_SingleNode_Iterator_& operator=(const _Const_SingleNode_Iterator_& obj) {
			this->__iter__ = obj.__iter__;
		}

		T operator*() {
			return this->__iter__->data;
		}

		~_Const_SingleNode_Iterator_() {}

	};

//#################################################################################################################
	
	// Bi-Directional Iterator
	template<typename T>
	class _BinaryNode_Iterator_ {
		using pointer = BinaryNode<T>*;
		using value = BinaryNode<T>;

	protected:
		pointer __iter__ = NULL;

	public:

		_BinaryNode_Iterator_(BinaryNode<T>* _pointer_) {
			this->__iter__ = _pointer_;
		}

		// since this class just consists of a pointer, during destruction the ointer will be nulled out
		// therefore copying doesn't create problems
		_BinaryNode_Iterator_(const _BinaryNode_Iterator_& _pointer_) {
			this->__iter__ = _pointer_.__iter__;
		}

		// therefore copy assignment doesn't create problems
		_BinaryNode_Iterator_& operator=(const _BinaryNode_Iterator_& _pointer_) {
			this->__iter__ = _pointer_.__iter__;
		}

		_BinaryNode_Iterator_& operator++() {
			this->__iter__ = this->__iter__->next;
			return *this;
		}

		_BinaryNode_Iterator_& operator++(int) {
			_BinaryNode_Iterator_<T> __temp__(this->__iter__);
			this->__iter__ = this->__iter__->next;
			return __temp__;
		}

		_BinaryNode_Iterator_& operator--() {
			this->__iter__ = this->__iter__->prev;
			return *this;
		}

		_BinaryNode_Iterator_& operator--(int) {
			_BinaryNode_Iterator_<T> __temp__(this->__iter__);
			this->__iter__ = this->__iter__->prev;
			return __temp__;
		}

		_BinaryNode_Iterator_& operator+=(int num) {
			for (; num > 0; num--) {
				this->__iter__ = this->__iter__? this->__iter__->next : NULL;
			}
			return *this;
		}

		_BinaryNode_Iterator_& operator-=(int num) {
			for (; num > 0; num--) {
				this->__iter__ = this->__iter__ ? this->__iter__->prev : NULL;
			}
			return *this;
		}

		T& operator*() {
			return &(this->__iter__->data);
		}

		bool operator== (BinaryNode<T>* _pointer_) {
			if (__iter__ == _pointer_) {
				return true;
			}
			return false;
		}

		bool operator!= (BinaryNode<T>* _pointer_) {
			return !this->operator==(_pointer_);
		}

		bool operator== (_BinaryNode_Iterator_<T> _pointer_) {
			if (__iter__ == _pointer_.__iter__) {
				return true;
			}
			return false;
		}

		bool operator!= (_BinaryNode_Iterator_<T> _pointer_) {
			return !this->operator==(_pointer_);
		}

		~_BinaryNode_Iterator_() {
			this->__iter__ = NULL;
		}
	};

	// Const Bi-Directional Iterator
	template<typename T>
	class _Const_BinaryNode_Iterator_ :public _BinaryNode_Iterator_<T> {
		using pointer = BinaryNode<T>*;
		using value = BinaryNode<T>;

	public:

		_Const_BinaryNode_Iterator_(BinaryNode<T>* _pointer_) : _BinaryNode_Iterator_<T>(_pointer_) {}

		// since this class just consists of a pointer, during destruction the ointer will be nulled out
		// therefore copying doesn't create problems
		_Const_BinaryNode_Iterator_(const _Const_BinaryNode_Iterator_& _pointer_) {
			this->__iter__ = _pointer_.__iter__;
		}

		// therefore copy assignment doesn't create problems
		_Const_BinaryNode_Iterator_& operator=(const _Const_BinaryNode_Iterator_& _pointer_) {
			this->__iter__ = _pointer_.__iter__;
		}

		T operator*() {
			return this->__iter__->data;
		}

		~_Const_BinaryNode_Iterator_() {}
	};

	// Bi-Directional Reverse Iterator
	template<typename T>
	class _BinaryNode_Reverse_Iterator_ {
		using pointer = BinaryNode<T>*;
		using value = BinaryNode<T>;
	
	protected:
		pointer __iter__ = NULL;

	public:
		_BinaryNode_Reverse_Iterator_(BinaryNode<T>* _pointer_){
			this->__iter__ = _pointer_;
		}

		_BinaryNode_Reverse_Iterator_(const _BinaryNode_Reverse_Iterator_& obj) {
			this->__iter__ = obj.__iter__;
		}

		_BinaryNode_Reverse_Iterator_& operator= (const _BinaryNode_Reverse_Iterator_& obj) {
			this->__iter__ = obj.__iter__;
		}

		_BinaryNode_Reverse_Iterator_& operator++() {
			this->__iter__ = this->__iter__->prev;
			return *this;
		}

		_BinaryNode_Reverse_Iterator_& operator--() {
			this->__iter__ = this->__iter__->next;
			return *this;
		}

		_BinaryNode_Reverse_Iterator_& operator++(int) {
			_BinaryNode_Reverse_Iterator_ __temp__(this->__iter__);
			this->__iter__ = this->__iter__->prev;
			return __temp__;
		}

		_BinaryNode_Reverse_Iterator_& operator--(int) {
			_BinaryNode_Reverse_Iterator_ __temp__(this->__iter__);
			this->__iter__ = this->__iter__->next;
			return __temp__;
		}

		_BinaryNode_Reverse_Iterator_& operator+=(int num) {
			for (; num > 0; num--) {
				this->__iter__ = this->__iter__ ? this->__iter__->next : NULL;
			}
			return *this;
		}

		_BinaryNode_Reverse_Iterator_& operator-=(int num) {
			for (; num > 0; num--) {
				this->__iter__ = this->__iter__ ? this->__iter__->prev : NULL;
			}
			return *this;
		}

		T operator*() {
			return this->__iter__->data;
		}

		bool operator== (BinaryNode<T>* _pointer_) {
			if (__iter__ == _pointer_) {
				return true;
			}
			return false;
		}

		bool operator!= (BinaryNode<T>* _pointer_) {
			return !this->operator==(_pointer_);
		}

		bool operator== (_BinaryNode_Reverse_Iterator_<T> _pointer_) {
			if (__iter__ == _pointer_.__iter__) {
				return true;
			}
			return false;
		}

		bool operator!= (_BinaryNode_Reverse_Iterator_<T> _pointer_) {
			return !this->operator==(_pointer_);
		}

		~_BinaryNode_Reverse_Iterator_() {
			this->__iter__ = NULL;
		}
	};

	// Const Bi-Directional Reverse Iterator
	template<typename T>
	class _Const_BinaryNode_Reverse_Iterator_ :public _BinaryNode_Reverse_Iterator_<T> {
		using pointer = BinaryNode<T>*;
		using value = BinaryNode<T>;

	public:

		_Const_BinaryNode_Reverse_Iterator_(BinaryNode<T>* _pointer_) : _BinaryNode_Reverse_Iterator_<T>(_pointer_) {}

		// since this class just consists of a pointer, during destruction the ointer will be nulled out
		// therefore copying doesn't create problems
		_Const_BinaryNode_Reverse_Iterator_(const _Const_BinaryNode_Reverse_Iterator_& _pointer_) {
			this->__iter__ = _pointer_.__iter__;
		}

		// therefore copy assignment doesn't create problems
		_Const_BinaryNode_Reverse_Iterator_& operator=(const _Const_BinaryNode_Reverse_Iterator_& _pointer_) {
			this->__iter__ = _pointer_.__iter__;
		}

		T operator*() {
			return this->__iter__->data;
		}

		~_Const_BinaryNode_Reverse_Iterator_() {}
	};

//#################################################################################################################
	
	// Tree Iterator
	/*
	pre increment triggers a movement downwards to left branch
	post increment triggers a movement downwards to right branch
	post and pre decrement both trigger a movement up
	*/
	template<typename T>
	class _TreeNode_Iterator_ {
		using pointer = TreeNode<T>*;

		pointer __iter__ = NULL;
	public:
		_TreeNode_Iterator_(TreeNode<T>* _pointer_) {
			this->__iter__ = _pointer_;
		}

		_TreeNode_Iterator_(const _TreeNode_Iterator_& obj) {
			this->__iter__ = obj.__iter__;
		}

		_TreeNode_Iterator_& operator=(const _TreeNode_Iterator_& obj) {
			this->__iter__ = obj.__iter__;
		}

		_TreeNode_Iterator_& operator++() {
			this->__iter__ = this->__iter__->left;
			return *this;
		}

		_TreeNode_Iterator_& operator++(int) {
			this->__iter__ = this->__iter__->right;
			return *this;
		}

		_TreeNode_Iterator_& operator--() {
			this->__iter__ = this->__iter__->prev;
		}

		_TreeNode_Iterator_& operator--(int) {
			this->__iter__ = this->__iter__->prev;
		}

		T operator*() {
			return this->__iter__->data;
		}

		bool operator==(_TreeNode_Iterator_ obj) {
			return this->__iter__ == obj.__iter__;
		}

		bool operator!=(_TreeNode_Iterator_ obj) {
			return !(this->__iter__ == obj.__iter__);
		}

		bool operator==(TreeNode<T>* obj) {
			return this->__iter__ == obj->temp;
		}

		bool operator!=(TreeNode<T>* obj) {
			return this->__iter__ == obj->temp;
		}

		~_TreeNode_Iterator_() {
			this->__iter__ = NULL;
		}
	};
}

#endif // !NODES_H
