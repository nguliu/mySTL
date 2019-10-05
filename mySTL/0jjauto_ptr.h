//简易的auto_ptr实现，为iterator的设计提供参考

#ifndef _JJAUTOPTR_H_
#define _JJAUTOPTR_H_


namespace JJ {

	template<class T>
	class auto_ptr {
	public:
		explicit auto_ptr(T* p = nullptr) : pointee(p) { }
		template<class U>
		auto_ptr(auto_ptr<U>& rhs) : pointee(rhs.release()) { }
		~auto_ptr() { delete pointee; }

		template<class U>
		auto_ptr<T>& operator=(auto_ptr<U>& rhs) {
			if (this != &rhs)
				reset(rhs.release());
			return *this;
		}
		T& operator*() const { return *pointee; }
		T* operator->() const { return pointee; }
		T* get() const { return pointee; }
		T* release() {
			T* tmp = pointee;
			pointee = nullptr;
			return tmp;
		}

	private:
		T* pointee;
	};

}// end of namespace JJ

#endif // !_JJAUTOPTR_H_