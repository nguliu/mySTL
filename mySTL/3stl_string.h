/*
 *实现了简单的string类和常用操作
 */

#ifndef _STL_STRING_H_
#define _STL_STRING_H_

#include "1stl_alloctor.h"
#include <cstddef>		//for ptrdiff_t
#include <string.h>

namespace lfp {
class string {
public:
	typedef char				value_type;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;
	typedef value_type*			iterator;
	typedef const value_type*	const_iterator;
	typedef value_type&			reference;
	typedef const value_type&	const_reference;
	typedef size_t				size_type;
	typedef ptrdiff_t			difference_type;
	typedef string				self;
private:
	size_t size_;
	char* buffer_;

	//专属的空间配置器，配置时不是以字节为单位，而是以元素个数为单位
	typedef simple_alloc<value_type, alloc> data_allocator;
	//内部函数，调用构造器释放vector占用的所有空间
	void deallocate() {
		if (buffer_) {
			data_allocator::deallocate(buffer_, size_ + 1);
		}
		buffer_ = 0;
		size_ = 0;
	}

public:
	//5种构造方式
	string();	//1 默认构造
	string(const string& str);	//2 拷贝构造
	string(const string& str, size_t pos, size_t len = 0);
	string(const char* s);		//3 const char* 构造
	string(const char* s, size_t n);
	string(iterator first, iterator last);	//4 区间构造
	string(size_t n, char c);	//5 n char构造
	~string();

	const char* c_str() const;
	size_t length() const;
	size_t size() const;
	iterator begin();
	iterator end();

	char& operator[](size_t pos);
	const char& operator[](size_t pos) const;
	string& operator=(const string& str);
	string& operator=(const char* s);
	string& operator=(char c);
	string& operator+=(const string& str);
	string& operator+=(const char* s);
	string& operator+=(char c);
	string& append(const string& str);
	string& append(const char* s);
	string& assign(const string& str);
	string& assign(const char* s);
	
	char& at(size_t pos);
	const char& at(size_t pos) const;
	
	void clear();
	int compare(const string& str) const;
	int compare(const char* s) const;
	const char* data() const;
	bool empty() const;
};
    
        
	inline string::string() {
		size_ = 0;
		buffer_ = data_allocator::allocate(1);
		buffer_[0] = '\0';
	}

	inline string::string(const string& str) {
		size_ = str.size_;
		buffer_ = data_allocator::allocate(size_ + 1);
		strcpy(buffer_, str.buffer_);
	}

	inline string::string(const string& str, size_t pos, size_t len) {
		if (pos > str.size_) {
			size_ = 0;
			buffer_ = data_allocator::allocate(1);
			buffer_[0] = '\0';
		}
		else {
			if (pos + len > str.size_)
				size_ = str.size_ - pos;
			else
				size_ = len;

			buffer_ = data_allocator::allocate(size_ + 1);
			strcpy(buffer_, str.data() + pos);
			buffer_[size_] = '\0';
		}
	}

	inline string::string(const char* s) {
		size_ = strlen(s);
		buffer_ = data_allocator::allocate(size_ + 1);
		strcpy(buffer_, s);
	}

	inline string::string(const char* s, size_t n)
	{
		if (strlen(s) <= n) {
			size_ = strlen(s);
		}
		else {
			size_ = n;
		}
		
		buffer_ = data_allocator::allocate(size_ + 1);
		strncpy(buffer_, s, n);
	}

	inline string::string(iterator first, iterator last)
	{
		size_ = last - first;
		buffer_ = data_allocator::allocate(size_ + 1);
		memmove(buffer_, first, size_);
		buffer_[size_] = '\0';
	}

	inline string::string(size_t n, char c)
	{
		size_ = n;
		buffer_ = data_allocator::allocate(size_ + 1);
		memset(buffer_, c, n);
	}

	inline string::~string()
	{
		deallocate();
	}

	inline const char* string::c_str() const
	{
		return buffer_;
	}

	inline size_t string::length() const
	{
		return size_;
	}

	inline size_t string::size() const
	{
		return size_;
	}

	inline string::iterator string::begin() {
		return buffer_;
	}

	inline string::iterator string::end() {
		return buffer_ + size_;
	}

	inline char& string::operator[](size_t pos)
	{
		return buffer_[pos];
	}

	inline const char& string::operator[](size_t pos) const
	{
		if (pos >= size_)
			return buffer_[size_];
		else
			return buffer_[pos];
	}

	inline string& string::operator=(const string& str)
	{
		deallocate();
		
		size_ = str.size_;
		buffer_ = data_allocator::allocate(size_ + 1);
		strcpy(buffer_, str.c_str());
		return *this;
	}

	inline string& string::operator=(const char* s)
	{
		deallocate();
		
		size_ = strlen(s);
		buffer_ = data_allocator::allocate(size_ + 1);
		strcpy(buffer_, s);
		return *this;
	}

	inline string& string::operator=(char c)
	{
		deallocate();
		
		size_ = 1;
		buffer_ = data_allocator::allocate(2);
		buffer_[0] = c;
		buffer_[1] = '\0';
		return *this;
	}

	inline string& string::operator+=(const string& str)
	{
		size_t new_size = size_ + str.size();
		char* new_buf = data_allocator::allocate(new_size + 1);
		
		memmove(new_buf, buffer_, size_);
		memmove(new_buf + size_, str.buffer_, str.size());
		new_buf[new_size] = '\0';
		
		deallocate();
		size_ = new_size;
		buffer_ = new_buf;
		
		return *this;
}

	inline string& string::operator+=(const char* s)
	{
		size_t new_size = size_ + strlen(s);
		char* new_buf = data_allocator::allocate(new_size + 1);
		
		memmove(new_buf, buffer_, size_);
		memmove(new_buf + size_, s, strlen(s));
		new_buf[new_size] = '\0';
		
		deallocate();
		size_ = new_size;
		buffer_ = new_buf;
		
		return *this;
	}

	inline string& string::operator+=(char c) {
		if ((size_ + 1) % 8 != 0) {
			buffer_[size_] = c;
			buffer_[++size_] = '\0';
		}
		else {
			size_t new_size = size_ + 1;
			char* new_buf = data_allocator::allocate(new_size + 1);
			
			memmove(new_buf, buffer_, size_);
			new_buf[size_] = c;
			new_buf[new_size] = '\0';
			
			deallocate();
			size_ = new_size;
			buffer_ = new_buf;
		}
		
		return *this;
	}

	inline string& string::append(const string& str)
	{
		*this += str;
		return *this;
	}

	inline string& string::append(const char* s)
	{
		*this += s;
		return *this;
	}

	inline string& string::assign(const string& str)
	{
		*this = str;
		return *this;
	}

	inline string& string::assign(const char* s)
	{
		*this = s;
		return *this;
	}

	inline char& string::at(size_t pos)
	{
		return buffer_[pos];
	}

	inline const char& string::at(size_t pos) const
	{
		return buffer_[pos];
	}

	inline void string::clear()
	{
		deallocate();
			
		size_ = 0;
		buffer_ = data_allocator::allocate(1);
		buffer_[0] = '\0';
	}

	inline int string::compare(const string& str) const
	{
		return strcmp(buffer_, str.buffer_);
	}

	inline int string::compare(const char* s) const
	{
		return strcmp(buffer_, s);
	}

	inline const char* string::data() const
	{
		return buffer_;
	}

	inline bool string::empty() const
	{
		return (size_ == 0);
	}

	
	
	/* 全局的string操作函数 */
	
	static inline string operator+(const string& lhs, const string& rhs)
	{
		string str(lhs);
		str += rhs;
		return str;
	}

	static inline string operator+(const string& lhs, const char* rhs)
	{
		string str(lhs);
		str += rhs;
		return str;
	}

	static inline string operator+(const char* lhs, const string& rhs)
	{
		string str(lhs);
		str += rhs;
		return str;
	}

	static inline string operator+(const string& lhs, char rhs)
	{
		string str(lhs);
		str += rhs;
		return str;
	}

	static inline string operator+(char lhs, const string& rhs)
	{
		string str(&lhs);
		str += rhs;
		return str;
	}

	static inline bool operator==(const string& lhs, const string& rhs)
	{
		return (lhs.compare(rhs) == 0);
	}

	static inline bool operator==(const char* lhs, const string& rhs)
	{
		return (rhs.compare(lhs) == 0);
	}

	static inline bool operator==(const string& lhs, const char* rhs)
	{
		return (lhs.compare(rhs) == 0);
	}

	static inline bool operator!=(const string& lhs, const string& rhs)
	{
		return (lhs.compare(rhs) != 0);
	}

	static inline bool operator!=(const char* lhs, const string& rhs)
	{
		return (rhs.compare(lhs) != 0);
	}

	static inline bool operator!=(const string& lhs, const char* rhs)
	{
		return (lhs.compare(rhs) != 0);
	}

	static inline bool operator<(const string& lhs, const string& rhs)
	{
		return (lhs.compare(rhs) < 0);
	}

	static inline bool operator<(const char* lhs, const string& rhs)
	{
		return (rhs.compare(lhs) >= 0);
	}

	static inline bool operator<(const string& lhs, const char* rhs)
	{
		return (lhs.compare(rhs) < 0);
	}

	static inline bool operator<=(const string& lhs, const string& rhs)
	{
		return (lhs.compare(rhs) <= 0);
	}

	static inline bool operator<=(const char* lhs, const string& rhs)
	{
		return (rhs.compare(lhs) > 0);
	}

	static inline bool operator<=(const string& lhs, const char* rhs)
	{
		return (lhs.compare(rhs) <= 0);
	}

	static inline bool operator>(const string& lhs, const string& rhs)
	{
		return (lhs.compare(rhs) > 0);
	}

	static inline bool operator>(const char* lhs, const string& rhs)
	{
		return (rhs.compare(lhs) <= 0);
	}

	static inline bool operator>(const string& lhs, const char* rhs)
	{
		return (lhs.compare(rhs) > 0);
	}

	static inline bool operator>=(const string& lhs, const string& rhs)
	{
		return (lhs.compare(rhs) >= 0);
	}

	static inline bool operator>=(const char* lhs, const string& rhs)
	{
		return (rhs.compare(lhs) < 0);
	}

	static inline bool operator>=(const string& lhs, const char* rhs)
	{
		return (lhs.compare(rhs) >= 0);
	}
    
}// end of namespace lfp

#endif // !_STL_STRING_H_
