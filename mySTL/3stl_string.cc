#include "3stl_string.h"
#include <string.h>

using namespace lfp;


string::string() {
	size_ = 0;
	buffer_ = data_allocator::allocate(1);
	buffer_[0] = '\0';
}

string::string(const string& str) {
	size_ = str.size_;
	buffer_ = data_allocator::allocate(size_ + 1);
	strcpy(buffer_, str.buffer_);
}

string::string(const string& str, size_t pos, size_t len) {
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

string::string(const char* s) {
	size_ = strlen(s);
	buffer_ = data_allocator::allocate(size_ + 1);
	strcpy(buffer_, s);
}

string::string(const char* s, size_t n)
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

string::string(iterator first, iterator last)
{
	size_ = last - first;
	buffer_ = data_allocator::allocate(size_ + 1);
	memmove(buffer_, first, size_);
	buffer_[size_] = '\0';
}

string::string(size_t n, char c)
{
	size_ = n;
	buffer_ = data_allocator::allocate(size_ + 1);
	memset(buffer_, c, n);
}

string::~string()
{
	deallocate();
}

const char* string::c_str() const
{
	return buffer_;
}

size_t string::length() const
{
	return size_;
}

size_t string::size() const
{
	return size_;
}

string::iterator string::begin() {
	return buffer_;
}

string::iterator string::end() {
	return buffer_ + size_;
}

char& string::operator [] (size_t pos)
{
	return buffer_[pos];
}

const char& string::operator [] (size_t pos) const
{
	if (pos >= size_)
		return buffer_[size_];
	else
		return buffer_[pos];
}


string& string::operator=(const string& str)
{
	deallocate();

	size_ = str.size_;
	buffer_ = data_allocator::allocate(size_ + 1);
	strcpy(buffer_, str.c_str());
	return *this;
}

string& string::operator = (const char* s)
{
	deallocate();

	size_ = strlen(s);
	buffer_ = data_allocator::allocate(size_ + 1);
	strcpy(buffer_, s);
	return *this;
}

string& string::operator = (char c)
{
	deallocate();

	size_ = 1;
	buffer_ = data_allocator::allocate(2);
	buffer_[0] = c;
	buffer_[1] = '\0';
	return *this;
}

string& string::operator+=(const string& str)
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

string& string::operator += (const char* s)
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

string& string::operator+=(char c) {
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

string& string::append(const string& str)
{
	*this += str;
	return *this;
}

string& string::append(const char* s)
{
	*this += s;
	return *this;
}

string& string::assign(const string& str)
{
	*this = str;
	return *this;
}

string& string::assign(const char* s)
{
	*this = s;
	return *this;
}

char& string::at(size_t pos)
{
	return buffer_[pos];
}

const char& string::at(size_t pos) const
{
	return buffer_[pos];
}

void string::clear()
{
	deallocate();

	size_ = 0;
	buffer_ = data_allocator::allocate(1);
	buffer_[0] = '\0';
}

int string::compare(const string& str) const
{
	return strcmp(buffer_, str.buffer_);
}

int string::compare(const char* s) const
{
	return strcmp(buffer_, s);
}

const char* string::data() const
{
	return buffer_;
}

bool string::empty() const
{
	return (size_ == 0);
}





string lfp::operator+(const string& lhs, const string& rhs)
{
	string str(lhs);
	str += rhs;
	return str;
}

string lfp::operator+(const string& lhs, const char* rhs)
{
	string str(lhs);
	str += rhs;
	return str;
}

string lfp::operator+(const char* lhs, const string& rhs)
{
	string str(lhs);
	str += rhs;
	return str;
}

string lfp::operator+(const string& lhs, char rhs)
{
	string str(lhs);
	str += rhs;
	return str;
}

string lfp::operator+(char lhs, const string& rhs)
{
	string str(&lhs);
	str += rhs;
	return str;
}

bool lfp::operator==(const string& lhs, const string& rhs)
{
	return (lhs.compare(rhs) == 0);
}

bool lfp::operator==(const char* lhs, const string& rhs)
{
	return (rhs.compare(lhs) == 0);
}

bool lfp::operator==(const string& lhs, const char* rhs)
{
	return (lhs.compare(rhs) == 0);
}

bool lfp::operator!=(const string& lhs, const string& rhs)
{
	return (lhs.compare(rhs) != 0);
}

bool lfp::operator!=(const char* lhs, const string& rhs)
{
	return (rhs.compare(lhs) != 0);
}

bool lfp::operator!=(const string& lhs, const char* rhs)
{
	return (lhs.compare(rhs) != 0);
}

bool lfp::operator<(const string& lhs, const string& rhs)
{
	return (lhs.compare(rhs) < 0);
}

bool lfp::operator<(const char* lhs, const string& rhs)
{
	return (rhs.compare(lhs) >= 0);
}

bool lfp::operator<(const string& lhs, const char* rhs)
{
	return (lhs.compare(rhs) < 0);
}

bool lfp::operator<=(const string& lhs, const string& rhs)
{
	return (lhs.compare(rhs) <= 0);
}

bool lfp::operator<=(const char* lhs, const string& rhs)
{
	return (rhs.compare(lhs) > 0);
}

bool lfp::operator<=(const string& lhs, const char* rhs)
{
	return (lhs.compare(rhs) <= 0);
}

bool lfp::operator>(const string& lhs, const string& rhs)
{
	return (lhs.compare(rhs) > 0);
}

bool lfp::operator>(const char* lhs, const string& rhs)
{
	return (rhs.compare(lhs) <= 0);
}

bool lfp::operator>(const string& lhs, const char* rhs)
{
	return (lhs.compare(rhs) > 0);
}

bool lfp::operator>=(const string& lhs, const string& rhs)
{
	return (lhs.compare(rhs) >= 0);
}

bool lfp::operator>=(const char* lhs, const string& rhs)
{
	return (rhs.compare(lhs) < 0);
}

bool lfp::operator>=(const string& lhs, const char* rhs)
{
	return (lhs.compare(rhs) >= 0);
}