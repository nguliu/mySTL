/*
<<<<<<< HEAD
 *vectorÊµÏÖ£º
 *
 *ÕâÀïÈ±Ê¡Ê¹ÓÃÄÚ²¿¿Õ¼äÅäÖÃÆ÷alloc£¨Ïê¼û1stl_alloctor.h£©£¬Ê¹ÓÃÕßÒ²¿ÉÒÔ×ÔĞĞ´«Èë¿Õ¼äÅäÖÃÆ÷
=======
<<<<<<< HEAD
 *vectorÊµÏÖ£º
 *
 *ÕâÀïÈ±Ê¡Ê¹ÓÃÄÚ²¿¿Õ¼äÅäÖÃÆ÷alloc£¨Ïê¼û1stl_alloctor.h£©£¬Ê¹ÓÃÕßÒ²¿ÉÒÔ×ÔĞĞ´«Èë¿Õ¼äÅäÖÃÆ÷
=======
 *vectorå®ç°ï¼š
 *
 *è¿™é‡Œç¼ºçœä½¿ç”¨å†…éƒ¨ç©ºé—´é…ç½®å™¨allocï¼ˆè¯¦è§1stl_alloctor.hï¼‰ï¼Œä½¿ç”¨è€…ä¹Ÿå¯ä»¥è‡ªè¡Œä¼ å…¥ç©ºé—´é…ç½®å™¨
>>>>>>> update
>>>>>>> update
 */

#ifndef _STL_VECTOR_H_
#define _STL_VECTOR_H_

<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
#include "1stl_alloctor.h"		//¿Õ¼äÅäÖÃÆ÷
#include "1stl_construct.h"		//¶ÔÏó¹¹ÔìÓëÎö¹¹
#include "1stl_iterator.h"		//for function distance
#include "1stl_uninitialized.h"	//for function uninitialized_*
#include "2stl_algobase.h"		//for diatance
#include <initializer_list>		//for ÁĞ±í³õÊ¼»¯
<<<<<<< HEAD
=======
=======
#include "1stl_alloctor.h"		//ç©ºé—´é…ç½®å™¨
#include "1stl_construct.h"		//å¯¹è±¡æ„é€ ä¸ææ„
#include "1stl_iterator.h"		//for function distance
#include "1stl_uninitialized.h"	//for function uninitialized_*
#include "2stl_algobase.h"		//for
#include <initializer_list>		//for åˆ—è¡¨åˆå§‹åŒ–
>>>>>>> update
>>>>>>> update
#include <cstddef>				//for ptrdiff_t

namespace lfp {

<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
	//Ê¡È±Ê¹ÓÃµÚ¶ş¼¶¿Õ¼äÅäÖÃÆ÷
	template<class T, class Alloc = alloc>
	class vector {
	public:
		//vectorµÄÇ¶Ì×ĞÍ±ğ¶¨Òå
		/* ÒòÎªvectorÊÇÊÇÔÚÁ¬Ğø¿Õ¼äÄÚ´æ·Å£¬ËùÒÔÔ­ÉúÖ¸Õë¾Í¿É×÷ÎªÆäµü´úÆ÷ */
<<<<<<< HEAD
=======
=======
	//çœç¼ºä½¿ç”¨ç¬¬äºŒçº§ç©ºé—´é…ç½®å™¨
	template<class T, class Alloc = alloc>
	class vector {
	public:
		//vectorçš„åµŒå¥—å‹åˆ«å®šä¹‰
		/* å› ä¸ºvectoræ˜¯æ˜¯åœ¨è¿ç»­ç©ºé—´å†…å­˜æ”¾ï¼Œæ‰€ä»¥åŸç”ŸæŒ‡é’ˆå°±å¯ä½œä¸ºå…¶è¿­ä»£å™¨ */
>>>>>>> update
>>>>>>> update
		typedef T				value_type;
		typedef value_type*		pointer;
		typedef value_type*		iterator;
		typedef value_type&		reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;

	private:
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
		//×¨ÊôµÄ¿Õ¼äÅäÖÃÆ÷£¬ÅäÖÃÊ±²»ÊÇÒÔ×Ö½ÚÎªµ¥Î»£¬¶øÊÇÒÔÔªËØ¸öÊıÎªµ¥Î»
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;				//Ä¿Ç°Ê¹ÓÃ¿Õ¼äµÄÍ·
		iterator finish;			//Ä¿Ç°Ê¹ÓÃ¿Õ¼äµÄÎ²
		iterator end_of_storage;	//Ä¿Ç°¿ÉÓÃ¿Õ¼äµÄÎ²

		//ÄÚ²¿º¯Êı£¬µ÷ÓÃ¹¹ÔìÆ÷ÊÍ·ÅvectorÕ¼ÓÃµÄËùÓĞ¿Õ¼ä
<<<<<<< HEAD
=======
=======
		//ä¸“å±çš„ç©ºé—´é…ç½®å™¨ï¼Œé…ç½®æ—¶ä¸æ˜¯ä»¥å­—èŠ‚ä¸ºå•ä½ï¼Œè€Œæ˜¯ä»¥å…ƒç´ ä¸ªæ•°ä¸ºå•ä½
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;				//ç›®å‰ä½¿ç”¨ç©ºé—´çš„å¤´
		iterator finish;			//ç›®å‰ä½¿ç”¨ç©ºé—´çš„å°¾
		iterator end_of_storage;	//ç›®å‰å¯ç”¨ç©ºé—´çš„å°¾

		//å†…éƒ¨å‡½æ•°ï¼Œè°ƒç”¨æ„é€ å™¨é‡Šæ”¾vectorå ç”¨çš„æ‰€æœ‰ç©ºé—´
>>>>>>> update
>>>>>>> update
		void deallocate() {
			if (start) {
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}

<<<<<<< HEAD
		//ÅäÖÃn¸öÔªËØµÄ¿Õ¼ä²¢Ìî³äÄÚÈİ
=======
<<<<<<< HEAD
		//ÅäÖÃn¸öÔªËØµÄ¿Õ¼ä²¢Ìî³äÄÚÈİ
=======
		//é…ç½®nä¸ªå…ƒç´ çš„ç©ºé—´å¹¶å¡«å……å†…å®¹
>>>>>>> update
>>>>>>> update
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
		//ÄÚµ÷º¯Êı£¬ÔÚpos´¦²åÈën¸öÔªËØx£¬Èô¿Õ¼ä²»×ã½øĞĞÀ©Èİ
		void insert_aux(iterator pos, size_type n, const T& x);

	public:		//vectorµÄ6ÖÖ³õÊ¼»¯·½Ê½
		vector() : start(0), finish(0), end_of_storage(0) { }	//1 Ä¬ÈÏ¹¹Ôì
		explicit vector(size_type n) { fill_initialize(n, T()); }	//2 Ö¸¶¨´óĞ¡
		vector(int n, const T& value) { fill_initialize(n, value); }	//3	´´½¨n¸övalue
		vector(size_type n, const T& value) { fill_initialize(n, value); }
		template<class Iterator>
		vector(Iterator first, Iterator last) {		//4 ÓÃÇø¼ä[first, last)³õÊ¼»¯
<<<<<<< HEAD
=======
=======
		//å†…è°ƒå‡½æ•°ï¼Œåœ¨poså¤„æ’å…¥nä¸ªå…ƒç´ xï¼Œè‹¥ç©ºé—´ä¸è¶³è¿›è¡Œæ‰©å®¹
		void insert_aux(iterator pos, size_type n, const T& x);

	public:		//vectorçš„6ç§åˆå§‹åŒ–æ–¹å¼
		vector() : start(0), finish(0), end_of_storage(0) { }	//1 é»˜è®¤æ„é€ 
		explicit vector(size_type n) { fill_initialize(n, T()); }	//2 æŒ‡å®šå¤§å°
		vector(int n, const T& value) { fill_initialize(n, value); }	//3	åˆ›å»ºnä¸ªvalue
		vector(size_type n, const T& value) { fill_initialize(n, value); }
		template<class Iterator>
		vector(Iterator first, Iterator last) {		//4 ç”¨åŒºé—´[first, last)åˆå§‹åŒ–
>>>>>>> update
>>>>>>> update
			size_type n = distance(first, last);
			start = data_allocator::allocate(n);
			uninitialized_copy(first, last, start);
			finish = start + n;
			end_of_storage = finish;
		}
<<<<<<< HEAD
		vector(const vector<T>& rhs) {		//5 ¿½±´¹¹Ôì
=======
<<<<<<< HEAD
		vector(const vector<T>& rhs) {		//5 ¿½±´¹¹Ôì
=======
		vector(const vector<T>& rhs) {		//5 æ‹·è´æ„é€ 
>>>>>>> update
>>>>>>> update
			start = data_allocator::allocate(rhs.size());
			uninitialized_copy(rhs.start, rhs.finish, start);
			finish = start + rhs.size();
			end_of_storage = finish;
		}
<<<<<<< HEAD
		vector(std::initializer_list<T> init_list) {		//6 ÁĞ±í³õÊ¼»¯
=======
<<<<<<< HEAD
		vector(std::initializer_list<T> init_list) {		//6 ÁĞ±í³õÊ¼»¯
=======
		vector(std::initializer_list<T> init_list) {		//6 åˆ—è¡¨åˆå§‹åŒ–
>>>>>>> update
>>>>>>> update
			start = data_allocator::allocate(init_list.size());
			uninitialized_copy(init_list.begin(), init_list.end(), start);
			finish = start + init_list.size();
			end_of_storage = finish;
		}
		~vector() {
			if (start)
<<<<<<< HEAD
				destroy(start, finish);		//½«¶ÔÏóÎö¹¹
			deallocate();			//ÊÍ·ÅÄÚ´æ
=======
<<<<<<< HEAD
				destroy(start, finish);		//½«¶ÔÏóÎö¹¹
			deallocate();			//ÊÍ·ÅÄÚ´æ
=======
				destroy(start, finish);		//å°†å¯¹è±¡ææ„
			deallocate();			//é‡Šæ”¾å†…å­˜
>>>>>>> update
>>>>>>> update
		}

		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() const { return finish - start; }
		size_type capacity() const { return end_of_storage - start; }
		reference operator[](size_type n) { return *(start + n); }
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }

<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
		/* ÔÚposÎ»ÖÃ²åÈën¸öÔªËØx */
		void insert(iterator pos, size_type n, const T& x) {
			insert_aux(pos, n, x);
		}
		/* ÔÚposÎ»ÖÃ²åÈëÒ»¸öÔªËØx */
<<<<<<< HEAD
=======
=======
		/* åœ¨posä½ç½®æ’å…¥nä¸ªå…ƒç´ x */
		void insert(iterator pos, size_type n, const T& x) {
			insert_aux(pos, n, x);
		}
		/* åœ¨posä½ç½®æ’å…¥ä¸€ä¸ªå…ƒç´ x */
>>>>>>> update
>>>>>>> update
		void insert(iterator pos, const T& x) {
			insert_aux(pos, 1, x);
		}

		void push_front(const T& x) {
			insert_aux(begin(), 1, x);
		}

		void push_back(const T& x) {
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
			if (finish != end_of_storage) {	  //Èç¹ûÎ´ÌîÂúÔòÖ±½ÓÔÚÄ©Î²¹¹ÔìÔªËØ
				construct(finish, x);
				++finish;
			}
			else {		//·ñÔòĞèÒªÀ©Èİ
<<<<<<< HEAD
=======
=======
			if (finish != end_of_storage) {	  //å¦‚æœæœªå¡«æ»¡åˆ™ç›´æ¥åœ¨æœ«å°¾æ„é€ å…ƒç´ 
				construct(finish, x);
				++finish;
			}
			else {		//å¦åˆ™éœ€è¦æ‰©å®¹
>>>>>>> update
>>>>>>> update
				insert_aux(end(), 1, x);
			}
		}

		void pop_front() {
			erase(begin());
		}

		void pop_back() {
			--finish;
			destroy(finish);
		}

		iterator erase(iterator pos) {		
<<<<<<< HEAD
			if (pos + 1 != end()) {		//Èç¹û²»ÊÇÉ¾³ı×îºóÒ»¸öÔªËØ£¬ĞèÒª½«ºóĞøÔªËØÏòÇ°ÒÆ¶¯
=======
<<<<<<< HEAD
			if (pos + 1 != end()) {		//Èç¹û²»ÊÇÉ¾³ı×îºóÒ»¸öÔªËØ£¬ĞèÒª½«ºóĞøÔªËØÏòÇ°ÒÆ¶¯
=======
			if (pos + 1 != end()) {		//å¦‚æœä¸æ˜¯åˆ é™¤æœ€åä¸€ä¸ªå…ƒç´ ï¼Œéœ€è¦å°†åç»­å…ƒç´ å‘å‰ç§»åŠ¨
>>>>>>> update
>>>>>>> update
				copy(pos + 1, finish, pos);
			}
			--finish;
			destroy(finish);
			return pos;
		}

<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
		//É¾³ı[first, last)ÖĞËùÓĞÔªËØ
		iterator erase(iterator first, iterator last) {
			//½«[last, finish)µÄÄÚÈİÒÆ¶¯µ½´Ófirst¿ªÊ¼Î»ÖÃ£¬·µ»Ø×îºó¸´ÖÆµÄÏÂÒ»Î»ÖÃ
			iterator it = copy(last, finish, first);

			/* Ïú»Ù[it, finish) ÄÚµÄËùÓĞÄÚÈİ */
			destroy(it, finish);
			finish = it;		//¸üĞÂ finish Ö¸ÏòĞÂµÄ½áÎ²
<<<<<<< HEAD
=======
=======
		//åˆ é™¤[first, last)ä¸­æ‰€æœ‰å…ƒç´ 
		iterator erase(iterator first, iterator last) {
			//å°†[last, finish)çš„å†…å®¹ç§»åŠ¨åˆ°ä»firstå¼€å§‹ä½ç½®ï¼Œè¿”å›æœ€åå¤åˆ¶çš„ä¸‹ä¸€ä½ç½®
			iterator it = copy(last, finish, first);

			/* é”€æ¯[it, finish) å†…çš„æ‰€æœ‰å†…å®¹ */
			destroy(it, finish);
			finish = it;		//æ›´æ–° finish æŒ‡å‘æ–°çš„ç»“å°¾
>>>>>>> update
>>>>>>> update
			return first;
		}

		void resize(size_type new_sz, const T& x) {
			if (new_sz < size()) {
				erase(begin() + new_sz, end());
			}
			else {
				insert_aux(end(), new_sz - size(), x);
			}
		}

		void resize(size_type new_sz) {
			resize(new_sz, T());
		}

		void swap(vector<T>& rhs) {
			swap(start, rhs.start);
			swap(end, rhs.end);
			swap(end_of_storage, rhs.end_of_storage);
		}

		void clear() {
			erase(begin(), end());
		}

	protected:
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
		//ÅäÖÃn¸öÔªËØ¿Õ¼ä£¬²¢½«ÆäÌî³äÎªvalue
		iterator allocate_and_fill(size_type n, const T& value) {
			iterator result = data_allocator::allocate(n);		//ÉêÇën¸öÔªËØµÄ¿Õ¼ä
			uninitialized_fill_n(result, n, value);		//Ìî³äÔªËØ
<<<<<<< HEAD
=======
=======
		//é…ç½®nä¸ªå…ƒç´ ç©ºé—´ï¼Œå¹¶å°†å…¶å¡«å……ä¸ºvalue
		iterator allocate_and_fill(size_type n, const T& value) {
			iterator result = data_allocator::allocate(n);		//ç”³è¯·nä¸ªå…ƒç´ çš„ç©ºé—´
			uninitialized_fill_n(result, n, value);		//å¡«å……å…ƒç´ 
>>>>>>> update
>>>>>>> update
			return result;
		}
	};	//end of class vector



<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
	/* ÄÚµ÷º¯Êı£¬ÔÚpos´¦²åÈëÔªËØ x£¬Èô¿Õ¼ä²»×ã½øĞĞÀ©Èİ */
	//template<class T, class Alloc>
	//void vector<T, Alloc>::insert_aux(iterator pos, const T& x) {
	//	if (finish != end_of_storage) {		//»¹ÓĞ±¸ÓÃ¿Õ¼ä
	//		//ÔÚ±¸ÓÃ¿Õ¼äÆğÊ¼(finish)´¦¹¹ÔìÒ»¸öÔªËØ£¬²¢ÒÔvector×îºóÒ»¸öÔªËØ(finish - 1)Îª³õÊ¼Öµ
	//		construct(finish, *(finish - 1));
	//		++finish;		//µ÷ÕûË®Î»
	//		T x_copy = x;

	//		//½« [pos, finish - 2) ÄÚµÄÔªËØÏòºóÒÆ¶¯Ò»¸ñ
	//		copy_backward(pos, finish - 2, finish - 1);		//½»ÓÉ¸ß½×STLËã·¨´¦Àí£¬ÊµÏÖ¼û 2stl_algobase.h
	//		*pos = x_copy;		//ÔÚ pos ´¦²åÈëĞÂÔªËØ
	//	}
	//	else {		//Ã»ÓĞ±¸ÓÃ¿Õ¼ä
	//		const size_type old_capacity = size();
	//		const size_type new_capacity = (old_capacity == 0) ? 8 : 2 * old_capacity;
	//		/* ÒÔÉÏÎªÄÚ´æ¿Õ¼äÅäÖÃÔ­Ôò: Èç¹ûÔ­´óĞ¡Îª0£¬ÔòÅäÖÃ8£¬·ñÔòÅäÖÃÔ­¿Õ¼ä´óĞ¡µÄ 2 ±¶ */
	//		/* Ç°°ë¶ÎÓÃÀ´·ÅÖÃÔ­Êı¾İ£¬ºó°ë¶ÎÓÃÀ´·ÅÖÃĞÂÊı¾İ */
<<<<<<< HEAD
=======
=======
	/* å†…è°ƒå‡½æ•°ï¼Œåœ¨poså¤„æ’å…¥å…ƒç´  xï¼Œè‹¥ç©ºé—´ä¸è¶³è¿›è¡Œæ‰©å®¹ */
	//template<class T, class Alloc>
	//void vector<T, Alloc>::insert_aux(iterator pos, const T& x) {
	//	if (finish != end_of_storage) {		//è¿˜æœ‰å¤‡ç”¨ç©ºé—´
	//		//åœ¨å¤‡ç”¨ç©ºé—´èµ·å§‹(finish)å¤„æ„é€ ä¸€ä¸ªå…ƒç´ ï¼Œå¹¶ä»¥vectoræœ€åä¸€ä¸ªå…ƒç´ (finish - 1)ä¸ºåˆå§‹å€¼
	//		construct(finish, *(finish - 1));
	//		++finish;		//è°ƒæ•´æ°´ä½
	//		T x_copy = x;

	//		//å°† [pos, finish - 2) å†…çš„å…ƒç´ å‘åç§»åŠ¨ä¸€æ ¼
	//		copy_backward(pos, finish - 2, finish - 1);		//äº¤ç”±é«˜é˜¶STLç®—æ³•å¤„ç†ï¼Œå®ç°è§ 2stl_algobase.h
	//		*pos = x_copy;		//åœ¨ pos å¤„æ’å…¥æ–°å…ƒç´ 
	//	}
	//	else {		//æ²¡æœ‰å¤‡ç”¨ç©ºé—´
	//		const size_type old_capacity = size();
	//		const size_type new_capacity = (old_capacity == 0) ? 8 : 2 * old_capacity;
	//		/* ä»¥ä¸Šä¸ºå†…å­˜ç©ºé—´é…ç½®åŸåˆ™: å¦‚æœåŸå¤§å°ä¸º0ï¼Œåˆ™é…ç½®8ï¼Œå¦åˆ™é…ç½®åŸç©ºé—´å¤§å°çš„ 2 å€ */
	//		/* å‰åŠæ®µç”¨æ¥æ”¾ç½®åŸæ•°æ®ï¼ŒååŠæ®µç”¨æ¥æ”¾ç½®æ–°æ•°æ® */
>>>>>>> update
>>>>>>> update

	//		iterator new_start = data_allocator::allocate(new_capacity);
	//		iterator new_finish = new_start;
	//		try {
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
	//			/* ÏÈ½«Ô­vectorÖĞ [start, pos) µÄÄÚÈİ¿½±´µ½ new_start ¿ªÊ¼µÄÎ»ÖÃ */
	//			new_finish = uninitialized_copy(start, pos, new_start);
	//			
	//			/* ÔÚ new_finish(¼´pos) ´¦¹¹Ôì²åÈëµÄÔªËØ x */
	//			construct(new_finish, x);
	//			++new_finish;		//µ÷ÕûË®Î»

	//			/* ÔÙ½«Ô­vectorÖĞ [pos, finish) µÄÄÚÈİ¿½±´µ½ new_finish ¿ªÊ¼µÄÎ»ÖÃ */
	//			new_finish = uninitialized_copy(pos, finish, new_finish);

	//			//ÔªËØ²åÈëÍê³É£¬ĞÂ vector ¹¹ÔìÍê³É
	//		} catch (...) {
	//			//²¶»ñµ½Òì³££¬Ö´ĞĞÏàÓ¦µÄÏú»Ù¹¤×÷¡£×¢Òâ£ºÒòÎªuninitialized_copy×ñÑ­commit or rollback
	//			//Ô­Ôò£¬ËùÒÔ¹¹ÔìÊ§°ÜÊ±²»ĞèÒª×Ô¼ºÖ´ĞĞ¶ÔÏóÏú»Ù¹¤×÷£¬Ö»ĞèÒªÊÍ·ÅÉêÇëµÄÄÚ´æ¾ÍºÃ
	//			data_allocator::deallocate(new_start, new_capacity);		//ÊÍ·Å¿Õ¼ä
	//			throw;		//×¢ÒâÒªÏò¸ß²ãÅ×³öÒì³£
	//		}

	//		//Îö¹¹²¢ÊÍ·ÅÔ­ vector
	//		destroy(begin(), end());
	//		deallocate();

	//		//µ÷Õûµü´úÆ÷£¬Ö¸ÏòĞÂµÄ vector
<<<<<<< HEAD
=======
=======
	//			/* å…ˆå°†åŸvectorä¸­ [start, pos) çš„å†…å®¹æ‹·è´åˆ° new_start å¼€å§‹çš„ä½ç½® */
	//			new_finish = uninitialized_copy(start, pos, new_start);
	//			
	//			/* åœ¨ new_finish(å³pos) å¤„æ„é€ æ’å…¥çš„å…ƒç´  x */
	//			construct(new_finish, x);
	//			++new_finish;		//è°ƒæ•´æ°´ä½

	//			/* å†å°†åŸvectorä¸­ [pos, finish) çš„å†…å®¹æ‹·è´åˆ° new_finish å¼€å§‹çš„ä½ç½® */
	//			new_finish = uninitialized_copy(pos, finish, new_finish);

	//			//å…ƒç´ æ’å…¥å®Œæˆï¼Œæ–° vector æ„é€ å®Œæˆ
	//		} catch (...) {
	//			//æ•è·åˆ°å¼‚å¸¸ï¼Œæ‰§è¡Œç›¸åº”çš„é”€æ¯å·¥ä½œã€‚æ³¨æ„ï¼šå› ä¸ºuninitialized_copyéµå¾ªcommit or rollback
	//			//åŸåˆ™ï¼Œæ‰€ä»¥æ„é€ å¤±è´¥æ—¶ä¸éœ€è¦è‡ªå·±æ‰§è¡Œå¯¹è±¡é”€æ¯å·¥ä½œï¼Œåªéœ€è¦é‡Šæ”¾ç”³è¯·çš„å†…å­˜å°±å¥½
	//			data_allocator::deallocate(new_start, new_capacity);		//é‡Šæ”¾ç©ºé—´
	//			throw;		//æ³¨æ„è¦å‘é«˜å±‚æŠ›å‡ºå¼‚å¸¸
	//		}

	//		//ææ„å¹¶é‡Šæ”¾åŸ vector
	//		destroy(begin(), end());
	//		deallocate();

	//		//è°ƒæ•´è¿­ä»£å™¨ï¼ŒæŒ‡å‘æ–°çš„ vector
>>>>>>> update
>>>>>>> update
	//		start = new_start;
	//		finish = new_finish;
	//		end_of_storage = new_start + new_capacity;
	//	}
	//}


<<<<<<< HEAD
	//ÄÚµ÷º¯Êı£¬ÔÚpos´¦²åÈën¸öÔªËØx£¬Èô¿Õ¼ä²»×ã½øĞĞÀ©Èİ
=======
<<<<<<< HEAD
	//ÄÚµ÷º¯Êı£¬ÔÚpos´¦²åÈën¸öÔªËØx£¬Èô¿Õ¼ä²»×ã½øĞĞÀ©Èİ
=======
	//å†…è°ƒå‡½æ•°ï¼Œåœ¨poså¤„æ’å…¥nä¸ªå…ƒç´ xï¼Œè‹¥ç©ºé—´ä¸è¶³è¿›è¡Œæ‰©å®¹
>>>>>>> update
>>>>>>> update
	template<class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator pos, size_type n, const T& x) {
		if (n > 0)
		{
			if (size_type(end_of_storage - finish) >= n) {
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
				//±¸ÓÃ¿Õ¼ä´óÓÚµÈÓÚĞÂÔöÔªËØ¸öÊı
				T x_copy = x;
				
				//ÒÔÏÂ¼ÆËã²åÈëµãÖ®ºóµÄÏÖÓĞÔªËØ¸öÊı
				const size_type elems_after = finish - pos;
				iterator old_finish = finish;
				if (elems_after >= n) {		//²åÈëµãÖ®ºóµÄÔªËØ¸öÊı´óÓÚµÈÓÚĞÂÔöÔªËØ¸öÊı
					uninitialized_copy(finish - n, finish, finish);		//ÔÚ±¸ÓÃ¿Õ¼ä¿ªÊ¼³öÔÙ¹¹Ôì n ¸öÔªËØ
					finish += n;		//½«vectorÎ²¶Ë±ê¼ÇºóÒÆ
					copy_backward(pos, old_finish - n, old_finish);		//Ïòºó¿½±´Ê£ÓàÔªËØ£¬ÊµÏÖ¼û 2stl_algobase.h
					//´Ó²åÈëµã¿ªÊ¼ÌîÈëĞÂÖµ
					fill(pos, pos + n, x_copy);
				}
				else {		//²åÈëµãÖ®ºóµÄÔªËØ¸öÊıĞ¡ÓÚĞÂÔöÔªËØ¸öÊı
					uninitialized_fill_n(finish, n - elems_after, x_copy);	//ÏÖÔÚfinishÖ®ºó¹¹Ôìn-elems_after¸öx_copy£¨ÒòÎª²åÈëÖ®ºóÕâÀï¾ÍÓ¦¸ÃÊÇx_copy£©
					finish = finish + n - elems_after;
					uninitialized_copy(pos, old_finish, finish);	//½«Ô­±¾posºóµÄÔªËØ¿½±´µ½²åÈëÖ®ºóÕıÈ·µÄÎ»ÖÃ
<<<<<<< HEAD
=======
=======
				//å¤‡ç”¨ç©ºé—´å¤§äºç­‰äºæ–°å¢å…ƒç´ ä¸ªæ•°
				T x_copy = x;
				
				//ä»¥ä¸‹è®¡ç®—æ’å…¥ç‚¹ä¹‹åçš„ç°æœ‰å…ƒç´ ä¸ªæ•°
				const size_type elems_after = finish - pos;
				iterator old_finish = finish;
				if (elems_after >= n) {		//æ’å…¥ç‚¹ä¹‹åçš„å…ƒç´ ä¸ªæ•°å¤§äºç­‰äºæ–°å¢å…ƒç´ ä¸ªæ•°
					uninitialized_copy(finish - n, finish, finish);		//åœ¨å¤‡ç”¨ç©ºé—´å¼€å§‹å‡ºå†æ„é€  n ä¸ªå…ƒç´ 
					finish += n;		//å°†vectorå°¾ç«¯æ ‡è®°åç§»
					copy_backward(pos, old_finish - n, old_finish);		//å‘åæ‹·è´å‰©ä½™å…ƒç´ ï¼Œå®ç°è§ 2stl_algobase.h
					//ä»æ’å…¥ç‚¹å¼€å§‹å¡«å…¥æ–°å€¼
					fill(pos, pos + n, x_copy);
				}
				else {		//æ’å…¥ç‚¹ä¹‹åçš„å…ƒç´ ä¸ªæ•°å°äºæ–°å¢å…ƒç´ ä¸ªæ•°
					uninitialized_fill_n(finish, n - elems_after, x_copy);	//ç°åœ¨finishä¹‹åæ„é€ n-elems_afterä¸ªx_copyï¼ˆå› ä¸ºæ’å…¥ä¹‹åè¿™é‡Œå°±åº”è¯¥æ˜¯x_copyï¼‰
					finish = finish + n - elems_after;
					uninitialized_copy(pos, old_finish, finish);	//å°†åŸæœ¬posåçš„å…ƒç´ æ‹·è´åˆ°æ’å…¥ä¹‹åæ­£ç¡®çš„ä½ç½®
>>>>>>> update
>>>>>>> update
					finish += elems_after;
					fill(pos, old_finish, x_copy);
				}
			}
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> update
			else {		//±¸ÓÃ¿Õ¼äĞ¡ÓÚĞÂÔöÔªËØ¸öÊı£¬±ØĞëÅäÖÃ¶îÍâµÄÄÚ´æ
				//Ê×ÏÈ¾ö¶¨ĞÂ³¤¶È£º¾É³¤¶ÈµÄÁ½±¶»òĞÂ²åÈëÔªËØ³¤¶ÈµÄÁ½±¶
				const size_type old_capacity = capacity();
				const size_type new_capacity = 2 * max(old_capacity, n);

				//ÒÔÏÂÅäÖÃĞÂµÄ vector ¿Õ¼ä
				iterator new_start = data_allocator::allocate(new_capacity);
				iterator new_finish = new_start;
				try{
					//ÒÔÏÂÊ×ÏÈ½«¾É vector ÖĞ²åÈëµãÖ®Ç°µÄÔªËØ¸´ÖÆµ½ĞÂ¿Õ¼äÖĞ
					new_finish = uninitialized_copy(start, pos, new_start);
					//ÔÙ½«ĞÂÔöÔªËØ n ÌîÈëĞÂ¿Õ¼ä
					new_finish = uninitialized_fill_n(new_finish, n, x);
					//×îºó½«¾É vector ÖĞ²åÈëµãÖ®ºóµÄÔªËØÒÆ¶¯µ½ĞÂ¿Õ¼ä
					new_finish = uninitialized_copy(pos, finish, new_finish);
				} catch (...) {
					//²¶»ñµ½Òì³££¬Ö´ĞĞÏàÓ¦µÄÏú»Ù¹¤×÷¡£×¢Òâ£ºÒòÎªuninitialized_copy¡¢uninitialized_fill_n¶¼×ñÑ­
					//commit or rollbackÔ­Ôò£¬ËùÒÔ¹¹ÔìÊ§°ÜÊ±²»ĞèÒª×Ô¼ºÖ´ĞĞ¶ÔÏóÏú»Ù¹¤×÷£¬Ö»ĞèÒªÊÍ·ÅÉêÇëµÄÄÚ´æ¾ÍºÃ
					data_allocator::deallocate(new_start, new_capacity);
					throw;		//×¢ÒâÒªÏò¸ß²ãÅ×³öÒì³£
				}

				//Îö¹¹²¢ÊÍ·ÅÔ­ vector
				destroy(begin(), end());
				deallocate();

				//µ÷Õûµü´úÆ÷£¬Ö¸ÏòĞÂµÄ vector
<<<<<<< HEAD
=======
=======
			else {		//å¤‡ç”¨ç©ºé—´å°äºæ–°å¢å…ƒç´ ä¸ªæ•°ï¼Œå¿…é¡»é…ç½®é¢å¤–çš„å†…å­˜
				//é¦–å…ˆå†³å®šæ–°é•¿åº¦ï¼šæ—§é•¿åº¦çš„ä¸¤å€æˆ–æ–°æ’å…¥å…ƒç´ é•¿åº¦çš„ä¸¤å€
				const size_type old_capacity = capacity();
				const size_type new_capacity = 2 * max(old_capacity, n);

				//ä»¥ä¸‹é…ç½®æ–°çš„ vector ç©ºé—´
				iterator new_start = data_allocator::allocate(new_capacity);
				iterator new_finish = new_start;
				try{
					//ä»¥ä¸‹é¦–å…ˆå°†æ—§ vector ä¸­æ’å…¥ç‚¹ä¹‹å‰çš„å…ƒç´ å¤åˆ¶åˆ°æ–°ç©ºé—´ä¸­
					new_finish = uninitialized_copy(start, pos, new_start);
					//å†å°†æ–°å¢å…ƒç´  n å¡«å…¥æ–°ç©ºé—´
					new_finish = uninitialized_fill_n(new_finish, n, x);
					//æœ€åå°†æ—§ vector ä¸­æ’å…¥ç‚¹ä¹‹åçš„å…ƒç´ ç§»åŠ¨åˆ°æ–°ç©ºé—´
					new_finish = uninitialized_copy(pos, finish, new_finish);
				} catch (...) {
					//æ•è·åˆ°å¼‚å¸¸ï¼Œæ‰§è¡Œç›¸åº”çš„é”€æ¯å·¥ä½œã€‚æ³¨æ„ï¼šå› ä¸ºuninitialized_copyã€uninitialized_fill_néƒ½éµå¾ª
					//commit or rollbackåŸåˆ™ï¼Œæ‰€ä»¥æ„é€ å¤±è´¥æ—¶ä¸éœ€è¦è‡ªå·±æ‰§è¡Œå¯¹è±¡é”€æ¯å·¥ä½œï¼Œåªéœ€è¦é‡Šæ”¾ç”³è¯·çš„å†…å­˜å°±å¥½
					data_allocator::deallocate(new_start, new_capacity);
					throw;		//æ³¨æ„è¦å‘é«˜å±‚æŠ›å‡ºå¼‚å¸¸
				}

				//ææ„å¹¶é‡Šæ”¾åŸ vector
				destroy(begin(), end());
				deallocate();

				//è°ƒæ•´è¿­ä»£å™¨ï¼ŒæŒ‡å‘æ–°çš„ vector
>>>>>>> update
>>>>>>> update
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_capacity;
			}
		}
	}

}// end of namespace lfp

#endif // !_STL_VECTOR_H_