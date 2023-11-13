#include "FlexArray.h"
#include <string> 


FlexArray::FlexArray() {
	size_ = 0;
	capacity_ = INITIALCAP;
	arr_ = new int[capacity_];
	headroom_ = (capacity_ - size_) / 2;	
	tailroom_ = capacity_ - size_ - headroom_;
}

FlexArray::FlexArray(const int* arr, int size) {
	capacity_ = LO_THRESHOLD * size;
	size_ = size;

	headroom_ = (capacity_ - size_) / 2;
    tailroom_ = capacity_ - size_ - headroom_;

	arr_ = new int[capacity_];

	for (int i = 0; i < size; ++i) 
		arr_[headroom_ + i] = arr[i];
}

FlexArray::~FlexArray() {delete[] arr_;}

FlexArray::FlexArray(const FlexArray& other) {
	capacity_ = other.capacity_;
    size_ = other.size_;
    headroom_ = other.headroom_;
    tailroom_ = other.tailroom_;
    arr_ = new int[capacity_];

    for (int i = 0; i < capacity_; ++i) 
		arr_[i] = other.arr_[i];
}

FlexArray& FlexArray::operator=(const FlexArray& other) {
	if (this == &other) return *this;
	delete [] arr_;
	
    capacity_ = other.capacity_;
    size_ = other.size_;
    headroom_ = other.headroom_;
    tailroom_ = other.tailroom_;

    arr_ = new int[capacity_];
    for (int i = 0; i < capacity_; ++i) 
		arr_[i] = other.arr_[i];

    return *this;
}

int FlexArray::getSize() const {return size_;}

int FlexArray::getCapacity() const {return capacity_;}

string FlexArray::print() const {
	string rString = "[";
	for (int i = headroom_; i < headroom_ + size_; i++){
		if (i != headroom_) 
			rString += ", ";
		rString += std::to_string(arr_[i]);
	}
	rString += "]";
	return rString;
}

string FlexArray::printAll() const {
	string rString = "[";
	for (int i =0; i < capacity_; i++){
		if(i>0) 
			rString += ", ";
		if (i < headroom_ || i >= headroom_ + size_) 
			rString += "X";
 		else rString += std::to_string(arr_[i]);
	}
	rString += "]";
	return rString;
}

int FlexArray::get(int i) const {
	i = headroom_ + i;
	return arr_[i]; 
}

bool FlexArray::set(int i, int v) {
	if (i < 0 || i >= size_) return false; 
	arr_[headroom_+i] = v;
	return true; 
}

void FlexArray::push_back(int v) {
	if (size_ == 0){
		arr_[capacity_/2] = v;
		tailroom_--;
		size_++;
	}
	else if(tailroom_ > 0){
		arr_[size_ + headroom_] = v;
		size_++;
		tailroom_--;
	}
	else {
		resize();
		push_back(v); 
	}
}

bool FlexArray::pop_back() {
	if (size_ == 0){
		return false;
	}
	else {
		size_--;
		tailroom_++;
		if(size_ == 0 || capacity_ >= size_* HI_THRESHOLD) 
			resize();
		return true;
	}
}

void FlexArray::push_front(int v) {
	if (size_ == 0){
		arr_[capacity_ / 2] = v;
		size_++;
	}
	else if(headroom_ > 0){
		arr_[headroom_ -1] = v;
		size_++;
		headroom_--;
	}
	else{
		resize();
		push_front(v);
	}
}

bool FlexArray::pop_front() {
	if (size_ == 0) return false;
	else {
		size_--;
		headroom_++;
		if(size_ == 0 || capacity_ >= size_* HI_THRESHOLD) 
			resize(); 
		return true;
	}
}

bool FlexArray::insert(int i, int v) {
	if (i >= 0 && i<= size_){
		if (size_ == 0){
			headroom_ = (capacity_ - size_) / 2;	
    		tailroom_ = capacity_ - size_ - headroom_;

			size_ % 2 == 0 ? headroom_-- : tailroom_--;
			arr_[headroom_+i] = v;
			size_++;
		}
		else if (size_ == capacity_){
			resize();
			insert(i,v);
		}
		else if(i > (size_/2.0) || i == (size_/2.0)){
			if(tailroom_ > 0) 
				right(i,v);
			else
				left(i,v);
		}	
		else if(i <= (size_/2.0)){
			if(headroom_ > 0) 
				left(i,v); 
			else 
				right(i,v); 
		}
		return true;
	}
	return false;
}	

// Inserts item, v, at index, i, by moving itemsto the right
void FlexArray::right(int i, int v){
	for (int j = headroom_+size_ - 1; j >= headroom_+i; --j) 
		arr_[j+1] = arr_[j]; 	
	arr_[headroom_+i] = v;
	tailroom_--;
	size_++;
}

// Inserts item, v, at index, i, by moving itemsto the left
void FlexArray::left(int i, int v){
	for (int j = headroom_; j < headroom_+i; j++) 
		arr_[j-1] = arr_[j];
    arr_[headroom_ + i-1] = v;
    headroom_--;
    size_++;
}

bool FlexArray::erase(int i) {
	if (i<0 || i >= size_ || size_ == 0) return false;
	if (size_ == 1){ size_ == 0; }
	// Moving Left
	if (i >= (size_/ 2)){
		for (int j = headroom_+i; j < headroom_+size_; j++) 
			arr_[j] = arr_[j+1];
		tailroom_++;
		size_--;
	}
	// Moving Right 
	else if (i < (size_ / 2)){
		for (int j = headroom_+i; j>headroom_; j--) 
			arr_[j] = arr_[j-1];
		headroom_++;
		size_--;
	}
	if(size_ != 0 && capacity_ >= size_* HI_THRESHOLD) 
		resize();
	return true;
}

// Sets a new capacity
void FlexArray::resize(){
	int new_cap = (size_ != 0) ? size_*LO_THRESHOLD : INITIALCAP;
	int* new_arr = new int[new_cap];
	int new_headroom = (new_cap - size_) / 2;
	for (int i = 0; i < size_; i++) 
		new_arr[new_headroom+i] = arr_[headroom_+i];
	delete[] arr_;
	arr_ = new_arr;
	capacity_ = new_cap;
	headroom_ = new_headroom;
	tailroom_ = capacity_ - size_ -  headroom_;
}