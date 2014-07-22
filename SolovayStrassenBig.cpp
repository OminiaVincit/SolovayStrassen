/*****************************************************************************
* 
* Cpp file SolovayStrassenBig.cpp
*	Implemetation of Solovay Strassen prime test
*		Test for long long int type and big int type ( for overflow cases )
* 		Test for Mersene prime number
*
* Written by myself - Tran Quoc Hoan - The Uiversity of Tokyo
*
*****************************************************************************/

#include <iostream>
#include <sys/time.h>
#include <random>

#include "BigInt.h"

const DatType NUMTEST = 100;
const DatType MAXSIZE = 1000000;
const DatType NUMSTATIC = 10;

// For generate Mersen prime numbers
DatType p[] = { 2, 3, 5, 7, 13, 17, 19, 31, 61, 89, 
	107, 127, 521, 607, 1279, 2203, 2281, 3217, 4253, 4423,
	9689, 9941, 11213, 19937, 21701, 23209, 44497, 86243, 110503,
	132049, 216091, 756839, 859433, 1257787, 1398269, 2976221, 3021377, 6972593, 13466917,
	20996011, 24036583, 25964951, 30402457, 32582657, 37156667, 42643801, 43112609, 57885161 };

// Return the epsilon function
// jacobi symbol value in the special case (-1/n) = (-1)^((n-1)/2)
// suppose for a positive odd integer n
template <typename T>
static inline int Ep( T n ) {
	if ( n % 4 == 1 ) return 1;
	return -1;
}

// Return the omega function
// jacobi symbol value in the special calse  (2/n) = (-1)^((n^2-1)/8)
template <typename T>
static inline int Omega( T n ) {
	if ( n % 8 == 1 || n % 8 == 7 ) return 1;
	return -1;
}

// Return the theta function
// for two positive odd integers m and n which are relatively prime
template <typename T>
static inline int Theta( T m, T n ) {
	if ( m % 4 == 1 || n % 4 == 1 ) return 1;
	return -1;
}

// Return the jacobi symbol (a/b) value
// Suppose that b is an odd integer >= 3
template <typename T>
int Jacobi( T a, T b ) {
	int result = 1;
	T tmp;
	if ( a >= 0 ) {
		result = 1;
	}
	else {
		a = -a;
		result = Ep(b);
	}
	while ( a != 1 && a != 0 ) {
		if ( a % 2 == 0 ) {
			a /= 2;
			result *= Omega( b );
		} else {
			result *= Theta( a, b );
			tmp = a;
			a = b % a;
			b = tmp;
		}
	}
	if ( a == 1 ) return result;
	else return 0;
}

// Return the gcd of (a,b)
// Suppose that a and b are non negative integers
template <typename T>
T Gcd( T a, T b ){
	T min = (a > b)?b:a;
	T max = (a > b)?a:b;
	T tmp, rs = 0;
	while ( min > 1 ){
		tmp = min;
		min = max % tmp;
		max = tmp;
	}
	if ( min == 0 ) return max;
	if ( min == 1 ) return min;
	return rs;
}

// Power module a^2 mod n
// suppose s >= 0, a > 0, n > 0
template <typename T>
T PowerModule( T b, T e, T m ) {
	T id = 1;
	if ( e == 0 ) return id;
	b = b % m;
	if ( e == 1 || b == 0 ) return b;
	T result = id;
	while ( e > 0 ) {
		if ( e % 2 != 0 ) {
			result = (result * b) % m;
		}
		e = e / 2;
		b = ( b * b ) % m;
	}
	return result;
}

// Exponential module a^((n-1)/2) mod n
template <typename T>
T ExpModule( T a, T n ) {
	if ( n % 2 != 0) return PowerModule( a, (n-1)/2, n );
	return 0;
}

// Make random number from 0 to (m-1)
template <typename T>
T MakeRand( T m ){
	T rs = 0;
	return rs;
}

template<> DatType MakeRand<DatType>( DatType m ){
	DatType value = randomEngine();
    DatType rs = value % m;
	return rs;
}

template<> BigInt MakeRand<BigInt>( BigInt m ){
	int sz = m.size;
	BigInt tmp = m;
	while ( tmp >= m ) {
		tmp = RandBigIntSize( sz );
	}
	return tmp;
}

// Solovay_Strassen prime test
// true for n prime, false for n composite
template <typename T>
bool SolovayStrassen( T n, DatType s, bool debug = false) {
	struct timeval start, stop;
	gettimeofday( &start, NULL );
	
	if ( n == 0 ) return false;
	if ( n == 1 ) return false;
	if ( n == 2 ) return true;
	if ( n != 2 && n % 2 == 0 ) return 0;
	srand( (unsigned) time(NULL) );
	T m = n - 1;
	for ( int j = 0; j < s; j++ ) {
		T a = 0;
		while ( Gcd( a, n) != 1 ){
			a = MakeRand<T>(m) + 1;
		}
		T diff = ExpModule<T>( a, n ) - Jacobi<T>( a, n );
		if ( (diff != 0) && (diff != n) ) {
			if ( debug ) {
				std::cout << "Debug: Failed at a = " << a << std::endl;
			}
			return false;
		}
	}
	
	// Calculate executed time
	if ( debug ){
		gettimeofday( &stop, NULL );
		double start_mill = start.tv_sec * 1000.0 + (start.tv_usec) / 1000.0;
		double stop_mill = stop.tv_sec * 1000.0 + (stop.tv_usec) / 1000.0;
		std::cout << stop_mill - start_mill;
	}

	return true;
}

// Small test function code 
template <typename T, typename K>
int Test( T expected, K got ) {
	if ( got == expected ) 	
		std::cout << "Test passed: ";
	else 
		std::cout << "Test failed: ";
	std::cout << "      Got      " << got;;
	std::cout << "      Expected " << expected << std::endl ;	
	return 1;
}

// Number of prime test: find all prime numbers less than n
DatType NumOfPrimeTest( DatType n, bool use_big = true, bool debug = false ) {
	if ( debug ) std::cout << "Prime numbers less than " << n << std::endl;
	if ( use_big && debug ) std::cout << "Use BigInt class " << std::endl;
	int s = NUMTEST;
	DatType num = 0;

	struct timeval start, stop;
	gettimeofday( &start, NULL );

	for ( DatType a = 1; a < n; a++ ){
		bool rs = false;
		if ( use_big ) {
			BigInt bg(a);
			rs = SolovayStrassen<BigInt>( bg, s, debug );
		}
		else 
			rs = SolovayStrassen<DatType>( a, s, debug );
		if ( rs ) { 
			if ( debug ) std::cout << a << ", ";
			num++;
		}
	}
	std::cout << num << ",";

	gettimeofday( &stop, NULL );
	double start_mill = start.tv_sec * 1000.0 + (start.tv_usec) / 1000.0;
	double stop_mill = stop.tv_sec * 1000.0 + (stop.tv_usec) / 1000.0;
	std::cout << stop_mill - start_mill << std::endl;
	return num;
}

// Run multi-times NumOfPrimeTest for statics
bool StaticNumOfPrimeTest( DatType n, bool use_big, DatType numt = NUMSTATIC, bool debug = false ) {
	if ( use_big ) std::cout << "Using BigInt class " << std::endl;
	else std::cout << "Using long long int type " << std::endl;
	for ( int i = 0; i < numt; ++i ) {
		std::cout << i+1 << ",";
		NumOfPrimeTest( n, use_big, debug );
	}
	return true;
}

template <typename T>
T Power( T b, DatType e ){
	T result = 1;
	if ( e == 0 ) return result;
	if ( e == 1 ) return b;
	while ( e > 0 ) {
		if ( e % 2 != 0 ) {
			result = result * b;
		}
		e = e / 2;
		b = b * b;
	}
	return result;
}

// Mersen test: check Mersen prime number 
template <typename T>
int MersenTest( int max_index, bool debug = true ){
	int s = NUMTEST;
	for ( int i = 0; i < max_index; ++i ){
		T pw = Power<T>( T(2), p[i] ) - 1;
		std::cout << std::endl << "Mersen number " << i+1 <<"th, p = " << p[i] << ", Executed time (ms): ";
		SolovayStrassen<T>( pw, s, debug );
	}
	std::cout << std::endl;
	return 1;
}

// Probability test: how many times for an composite n, the algorithm output 'non prime' result
DatType ProbTest( DatType n, DatType num_test ) {
	DatType num_comp = 0;
	for ( DatType i = 0; i < num_test; i++ ) {
		if ( !SolovayStrassen<DatType>(n,1) ) num_comp++;
	}
	return num_comp;
}

// Run multi-times ProbTest for statics
bool StaticProbTest( DatType num_test ) {
	for ( int i = 0; i < 10; i++ )
		for ( int j = i; j < 10; j++ ) {
			DatType n = p[i] * p[j];
			DatType num_comp = ProbTest( n, num_test );
			double prob = (double)num_comp / (double)num_test;
			std::cout << n << "," << num_comp << "," << num_test <<"," << prob << std::endl;
		}
	return true;
}

// Small performance test
double PerformanceTest( ) {
	struct timeval start, stop;
	gettimeofday( &start, NULL );

	int sz = MAXSIZE;
	BigInt bg = RandBigIntSize( sz );
	BigInt bg_r = MakeRand<BigInt>( bg );

	std::cout << bg << std::endl;
	std::cout << bg_r << std::endl;

	std::cout << "Jacobi: " << Jacobi<BigInt>( bg_r, bg ) << std::endl;
	std::cout << "Exp: " << ExpModule<BigInt>( bg_r, bg ).size << std::endl;
	
	gettimeofday( &stop, NULL );
	double start_mill = start.tv_sec * 1000.0 + (start.tv_usec) / 1000.0;
	double stop_mill = stop.tv_sec * 1000.0 + (stop.tv_usec) / 1000.0;
	return ( stop_mill - start_mill );
}

// Main function
int main( int argc, char* argv[] ) {
	
	// Test performance
	// Find all prime number less than 10000, execute test for 10 times
	
	DatType n = 10000;
	std::cout << "Test for program finding all prime numbers less than  " << n << std::endl;
	StaticNumOfPrimeTest( n, false );
	StaticNumOfPrimeTest( n, true );
	
	std::cout << "Mersen prime number test with long long int type: " << std::endl;
	MersenTest<DatType>( 9 );

	std::cout << std::endl;
	std::cout << "Mersen prime number test with Big Int class type: " << std::endl;
	MersenTest<BigInt>( 13 );

	// Probability test
	std::cout << "Probability of non-prime ouput with non-prime input. Random selection for only one time." << std::endl;
	StaticProbTest(100000);

	return 1;
}