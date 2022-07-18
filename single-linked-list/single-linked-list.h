#pragma once

#include <cstddef>
#include <utility>
#include <algorithm>
#include <iostream>
#include <initializer_list>
#include <vector>

template <typename Type>
class SingleLinkedList {
	struct Node {
		Node() = default;
		Node(const Type& val, Node* next)
			: value(val)
			, next_node(next) {
		}
		Type value;
		Node* next_node = nullptr;
	};
	template <typename ValueType>
    class BasicIterator {
    	friend class SingleLinkedList;
        explicit BasicIterator(Node* node):node_(node){}
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
        	node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return rhs.node_ == node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
        	return rhs.node_ != node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
        	 return rhs.node_ == node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
        	return rhs.node_ != node_;
        }

        BasicIterator& operator++() noexcept {
        	node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
        	auto oldThis(*this);
        	++(*this);
        	return oldThis;
        }
        [[nodiscard]] reference operator*() const noexcept {
        	return node_->value;
        }
        [[nodiscard]] pointer operator->() const noexcept {
        	return &(node_->value);
        }
    private:
        Node* node_ = nullptr;
    };

public:
	using value_type = Type;
	    using reference = value_type&;
	    using const_reference = const value_type&;

	    using Iterator = BasicIterator<Type>;
	    using ConstIterator = BasicIterator<const Type>;

		[[nodiscard]] Iterator before_begin() noexcept {
			return Iterator{&head_};
		}

		[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
			return ConstIterator{const_cast<Node*>(&head_)};
		}

		[[nodiscard]] ConstIterator before_begin() const noexcept {
			return cbefore_begin();
		}

	    [[nodiscard]] Iterator begin() noexcept {
	    	return Iterator{head_.next_node};
	    }

	    [[nodiscard]] Iterator end() noexcept {
	    	return Iterator{nullptr};
	    }

	    [[nodiscard]] ConstIterator begin() const noexcept {
	    	return cbegin();
	    }

	    [[nodiscard]] ConstIterator end() const noexcept {
			return cend();
	    }

	    [[nodiscard]] ConstIterator cbegin() const noexcept {
	    	return ConstIterator{head_.next_node};
	    }

	    [[nodiscard]] ConstIterator cend() const noexcept {
	    	return ConstIterator{nullptr};
	    }

	    SingleLinkedList(std::initializer_list<Type> values) {
	    	for(auto it = (values.end() - 1); it >= values.begin(); --it){
	    		PushFront(*it);
			}
		}

		SingleLinkedList(const SingleLinkedList& other) {
			if(size_ == 0 && head_.next_node == nullptr && other.GetSize() > 0){
				SingleLinkedList tmp;
				std::vector<Type> values;
				values.reserve(other.GetSize());
				for(const Type& val: other){
					values.push_back(val);
				}
				for(int i = (values.size() - 1); i >= 0; --i){
					Type newVal = values[i];
					tmp.PushFront(newVal);
				}
				swap(tmp);
			}
		}

		SingleLinkedList& operator=(const SingleLinkedList& rhs) {
			if(begin() != rhs.begin()){
				SingleLinkedList tmp(rhs);
				swap(tmp);
			}
			return *this;
		}

		void swap(SingleLinkedList& other) noexcept {
			Node itemHead = head_;
			size_t itemSize = size_;
			head_.next_node = other.head_.next_node;
			size_ = other.size_;
			other.head_.next_node = itemHead.next_node;
			other.size_ = itemSize;
		}

	    SingleLinkedList(){
			size_ = 0;
		};

		size_t GetSize() const noexcept {
			return size_;
		}

		bool IsEmpty() const noexcept {
		   if(size_ > 0){
			   return false;
		   }
		   return true;
		}

		void PushFront(const Type& val){
			head_.next_node = new Node(val, head_.next_node);
			size_++;
		};

		void PopFront() noexcept {
			Node * next = head_.next_node->next_node;
			delete head_.next_node;
			head_.next_node = next;
			size_--;
		}

		Iterator InsertAfter(ConstIterator pos, const Type& value) {
			Node * newNode = new Node(value, pos.node_->next_node);
			pos.node_->next_node = newNode;
			++size_;
			return Iterator{pos.node_->next_node};
		}

		void Clear() noexcept {
			while(head_.next_node != nullptr){
				Node * next = head_.next_node->next_node;
				delete head_.next_node;
				head_.next_node = next;
			}
			size_ = 0;
		}

		Iterator EraseAfter(ConstIterator pos) noexcept {
			Node * next = pos.node_->next_node->next_node;
			delete pos.node_->next_node;
			pos.node_->next_node = next;
			--size_;
			return Iterator{pos.node_->next_node};
		}

		~SingleLinkedList(){
			Clear();
		}
private:
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
	lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if(lhs == rhs){
    	return false;
    }
    return true;
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if(lhs < rhs || lhs == rhs){
    	return true;
    }
    return false;
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs <= lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	if(rhs < lhs || lhs == rhs){
		return true;
	}
	return false;
}
