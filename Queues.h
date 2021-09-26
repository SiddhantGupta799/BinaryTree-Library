#ifndef QUEUES_H
#define QUEUES_H

#include "Nodes.h"

#define init_name(x) x.name = #x

namespace Py {

	// Two Pointer Queue
	template<typename T>
	class Queue {
		using pointer = Py::BinaryNode<T>*;
		using value = Py::BinaryNode<T>;

		pointer _first_ = NULL;
		pointer _last_ = NULL;

		int _size_ = 0;

	public:
		using const_iterator = _Const_BinaryNode_Iterator_<T>;
		using const_reverse_iterator = _Const_BinaryNode_Reverse_Iterator_<T>;

		char const* name = "none";

		Queue() = default;
		Queue(initializer_list<T> init_l) {
			typename initializer_list<T>::iterator it = init_l.begin();

			this->_first_ = new value{ NULL };
			this->_first_->data = *it;
			this->_first_->prev = NULL;
			this->_first_->next = NULL;

			++it;

			this->_size_++;
			this->_last_ = this->_first_;

			while (it != init_l.end()) {
				pointer temp = new value{ NULL };
				temp->data = *it;
				temp->prev = NULL;
				temp->next = NULL;

				// linking the top to temp
				this->_last_->next = temp;
				temp->prev = this->_last_;

				// making temp the top
				this->_last_ = temp;

				temp = NULL;
				this->_size_++;
				++it;
			}
		}

		// supports iterators that have operator * and ++ overloaded
		template<class _Iter>
		Queue(_Iter begin, _Iter end) {
			_Iter it = begin;
			this->_first_ = new value{ NULL };
			this->_first_->data = *it;
			this->_first_->prev = NULL;
			this->_first_->next = NULL;

			++it;

			this->_size_++;
			this->_last_ = this->_first_;

			while (it != end) {
				pointer temp = new value{ NULL };
				temp->data = *it;
				temp->prev = NULL;
				temp->next = NULL;

				// linking the top to temp
				this->_last_->next = temp;
				temp->prev = this->_last_;

				// making temp the top
				this->_last_ = temp;

				temp = NULL;
				this->_size_++;
				++it;
			}
		}

		T last() {
			if (this->_last_) return this->_last_->data;
			else return T{};
		}

		T first() {
			if (this->_first_) return this->_first_->data;
			else return T{};
		}

		int size() { return this->_size_; }

	private:
		void null_out() { this->_last_ = NULL; this->_first_ = NULL; this->_size_ = 0; }

		void _copy_values_(pointer begin) {
			this->_first_ = new value{ NULL };
			this->_first_->data = begin->data;
			this->_first_->prev = NULL;
			this->_first_->next = NULL;

			begin = begin->next;

			this->_size_++;
			this->_last_ = this->_first_;

			while (begin) {
				pointer temp = new value{ NULL };
				temp->data = begin->data;
				temp->prev = NULL;
				temp->next = NULL;

				// linking the top to temp
				this->_last_->next = temp;
				temp->prev = this->_last_;

				// making temp the top
				this->_last_ = temp;

				temp = NULL;

				this->_size_++;
				begin = begin->next;
			}
		}

	public:
		// copy construtor
		Queue(const Queue& obj) {
			if (obj._size_ == 0) {
				this->null_out();
			}
			else {
				this->_copy_values_(obj._first_);
			}
		}

		// move constructor
		Queue(Queue&& obj) noexcept {
			this->_last_ = obj._last_;
			this->_first_ = obj._first_;
			this->_size_ = obj._size_;
			obj.null_out();
		}

		// copy assignments
		Queue& operator=(const Queue& obj) {
			this->clear();
			this->null_out();

			if (obj._size_ == 0) {}
			else { this->_copy_values_(obj._first_); }

			return *this;
		}

		// move assignments
		Queue& operator=(Queue&& obj) noexcept {
			this->clear();
			this->null_out();

			this->_last_ = obj._last_;
			this->_first_ = obj._first_;
			this->_size_ = obj._size_;

			obj.null_out();
			return *this;
		}

		Queue& push(T elem) {
			if (this->_size_ == 0) {
				this->_first_ = new value{ NULL };
				this->_first_->data = elem;
				this->_first_->prev = NULL;
				this->_first_->next = NULL;

				this->_last_ = this->_first_;
			}

			else {
				pointer temp = new value{ NULL };
				temp->data = elem;
				temp->prev = NULL;
				temp->next = NULL;

				// linking the top to temp
				this->_last_->next = temp;
				temp->prev = this->_last_;

				// making temp the top
				this->_last_ = temp;

				temp = NULL;
			}

			this->_size_++;

			return *this;
		}

		Queue& join(Queue obj) {
			if (obj._size_ == 0) {}
			else {
				// dumping in this context means putting another stack on top of this stack
				// in other words concatenation
				this->_last_->next = obj._first_;
				obj._first_->prev = this->_last_;

				this->_last_ = obj._last_;
				this->_size_ += obj._size_;

				obj.null_out();
			}
			return *this;
		}

		// in queue pop takes place in the first place
		T pop() {
			if (this->_size_ == 0) { return T{}; }
			else if (this->_size_ == 1) {
				T val = this->_first_->data;
				delete this->_first_;
				this->_first_ = NULL; 
				this->_last_ = NULL;
				this->_size_--;
				return val;
			}
			else {
				T val;
				pointer temp = this->_first_;

				// top shifts down
				this->_first_ = this->_first_->next;
				this->_first_->prev = NULL;

				// temp is deleted
				val = temp->data;

				delete temp;
				temp = NULL;
				this->_size_--;

				return val;
			}
		}

		T max() {
			T max_val = this->_first_->data;
			pointer temp = this->_first_;
			while (temp) {
				if (temp->data > max_val) { max_val = temp->data; }
				temp = temp->next;
			}
			return max_val;
		}

		T min() {
			T min_val = this->_first_->data;
			pointer temp = this->_first_;
			while (temp) {
				if (temp->data < min_val) { min_val = temp->data; }
				temp = temp->next;
			}
			return min_val;
		}

		void show() {
			pointer temp = this->_first_;
			while (temp) {
				cout << temp->data << " ";
				temp = temp->next;
			}cout << endl;
			temp = NULL;
		}

		/*	Queues Only Support 'const_iterator' and 'const_reverse_iterator'	*/

		const_iterator begin() {
			return const_iterator(this->_first_);
		}

		const_iterator end() {
			return const_iterator(this->_last_->next);
		}

		const_iterator cbegin() {
			return const_iterator(this->_first_);
		}

		const_iterator cend() {
			return const_iterator(this->_last_->next);
		}

		const_reverse_iterator rbegin() {
			return const_reverse_iterator(this->_last_);
		}

		const_reverse_iterator rend() {
			return const_reverse_iterator(this->_first_->prev);
		}

		Queue& clear() {
			pointer temp = NULL;
			while (this->_last_) {
				temp = this->_last_;
				this->_last_ = this->_last_->prev;
				delete temp;
				temp = NULL;
			}
			this->null_out();
			return *this;
		}

		void log_properties(char const* entry = "", bool log_obj_details = false, bool show_log_count = true) {
			static int _i = 0;

			if (show_log_count)
				cout << "Log Record Count: " << _i << endl;

			if (entry != "")
				cout << "Debug Entry: " << entry << endl;

			if (log_obj_details) {
				cout << "Object Address: " << this << endl;
				cout << "Object Type: " << typeid(*this).name() << endl;
			}

			cout << "Object Name: " << this->name << endl;
			cout << "Content: "; this->show();
			cout << "Size: " << this->_size_ << endl;
			cout << endl;
			_i++;
		}

		~Queue() {
			this->clear();
			this->_size_ = 0;
		}
	};

	constexpr auto MAX_PRIORITIES = 10;
	// Priority Queue
	template<typename T>
	class Priority_Queue {
		// Array of 10 priorities
		Queue<T> priorities[MAX_PRIORITIES];

	public:
		// all of these are compiler provided

		Priority_Queue() = default;

		Priority_Queue(const Priority_Queue& obj) = default;
		Priority_Queue(Priority_Queue&& obj) = default;

		Priority_Queue& operator=(const Priority_Queue& obj) = default;
		Priority_Queue& operator=(Priority_Queue&& obj) = default;

		// Implementing the functionality of push and pop

		// Provide the Element and Priority
		void push_priority(T elem, int priority) {
			if (priority > 0 and priority <= MAX_PRIORITIES) {
				// will be pushing the element in the priority
				this->priorities[--priority].push(elem);
			}
		}

		// Only provide the Priority
		void pop_priority(int priority) {
			if (priority > 0 and priority <= MAX_PRIORITIES) {
				// will be pushing the element in the priority
				this->priorities[--priority].pop();
			}
		}

		void pop() {
			// will pop from the top priorities first
			bool flag = true;
			int i = 0;
			while (flag) {
				if (this->priorities[i].size() == 0) { 
					i++; 
				}
				else {
					this->priorities[i].pop();
					flag = false;
				}
			}
		}

		void show() {
			for (int i = 0; i < MAX_PRIORITIES; i++) {
				cout << "Priority " << i + 1 << ": "; priorities[i].show();
			}
		}

		// also provided by the compiler
		~Priority_Queue() = default;
	};

}

#endif // !QUEUES_H

