#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__


//对外开放的命名空间
#define __mySTL_BEGIN__ namespace mySTL {
#define __mySTL_END__				 	}

#define __STL_TEMPLATE_NULL template<>
#define __STL_NULL_TMPL_ARGS		<>


__mySTL_BEGIN__

	static const int __stl_nun_primes = 28;		//用于维护 hash table 大小的质数个数
	/* 质数数组（两相邻数之间成两倍关系） */
	static const unsigned long __stl_prime_list[__stl_nun_primes] ={
		53,         97,           193,         389,       769,
		1543,       3079,         6151,        12289,     24593,
		49157,      98317,        196613,      393241,    786433,
		1572869,    3145739,      6291469,     12582917,  25165843,
		50331653,   100663319,    201326611,   402653189, 805306457,
		1610612741, 3221225473ul, 4294967291ul
	};

	/* 以下找出质数数组中最接近且不小于 n 的质数 */
	/*inline size_t __stl_next_prime(size_t n) {
		for (int i = 0; i < __stl_nun_primes; ++i) {
			if (__stl_prime_list[i] > n)
				return __stl_prime_list[i];
		}
		return 4294967291ul;
	}*/


__mySTL_END__

#endif