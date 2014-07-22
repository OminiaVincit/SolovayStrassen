/*************************************************************************
*
* Header file BigInt.h
*	define for Big Integer struct
*
* Written by myself - Tran Quoc Hoan - The University of Tokyo
* 	with great help from http://shygypsy.com/tools/BigInt.cpp
*
*************************************************************************/

#include <iostream>
#include <sstream>
#include <string>
#include <random>

typedef long long int DatType;
typedef long SizeType;
std::mt19937_64 randomEngine((unsigned)time(NULL));

struct BigInt {
	std::string digits;
	SizeType size;		// number of used digits
	int sign;		// -1, 0 or +1

	// constructor
	BigInt():digits(1,'0'),size(1),sign(0) {};
	BigInt( const std::string s );
	BigInt( DatType n );
	BigInt( const BigInt &n );

	// Assignment operators
	const BigInt &operator=( const BigInt & n );
	const BigInt &operator=( DatType n );

	// Arithmetic
	BigInt SubSameSign( BigInt first, BigInt second );
	BigInt operator-();
	BigInt& operator+= ( DatType n );
	BigInt& operator+= ( BigInt n );
	BigInt operator+ ( DatType n );
	BigInt operator+ ( BigInt n );

	BigInt& operator-=( DatType n );
	BigInt& operator-=( BigInt n );
	BigInt operator- ( DatType n );
	BigInt operator- ( BigInt n );

	BigInt operator* ( DatType n );
	BigInt operator* ( BigInt n );
	BigInt& operator*= ( DatType n );
	BigInt& operator*= ( BigInt n );

	BigInt operator/ ( DatType n );
	BigInt operator/ ( BigInt n );
	BigInt& operator/= ( DatType n );
	BigInt& operator/= ( BigInt n );

	DatType ModPower2( int pw );
	DatType operator% ( DatType n );
	BigInt operator% ( BigInt n );

	//BigInt& operator%= ( DatType n );
	//BigInt& operator%= ( BigInt n );

	// Bitwise arighmetic
	// BigInt operator>> ( DatType n );
	// BigInt operator<< ( DatType n );
	// BigInt& operator>>= ( DatType n );
	// BigInt& operator<<= ( DatType n );

	// Comparision
	int BigCmpr( BigInt n );
	bool operator==( BigInt n );
	bool operator!=( BigInt n );
	bool operator>( BigInt n );
	bool operator<( BigInt n );
	bool operator>=( BigInt n);
	bool operator<=( BigInt n);
	bool operator==( DatType n );
	bool operator!=( DatType n );
	bool operator>( DatType n );
	bool operator<( DatType n );
	bool operator>=( DatType n );
	bool operator<=( DatType n );

	// I/O 
	friend std::ostream &operator<<( std::ostream &out, BigInt n );
	//friend istream &operator>>( istream &in,  BigInt n );
	
	// Display
	void Display();
};

BigInt::BigInt( const std::string s ) {
	SizeType sz = s.size();
	SizeType i = 0;
	sign = 1;
	if ( s[0] == '-' ) {
		sign = -1;
		i++;
	}
	while ( s[i] == '0' && i < sz ) i++;
	if ( i == sz ) {
		sign = 0;
		size = 1;
		digits = "0";
	}
	else {
		sign *= 1;
		digits = s.substr( i );
		size = sz - i;
	}
}

BigInt::BigInt( DatType n ) {
	std::stringstream ss;
	ss << n;
	const std::string s = ss.str();
	BigInt rs( s );
	digits = rs.digits;
	size = rs.size;
	sign = rs.sign;
}

BigInt::BigInt( const BigInt &n ) {
	digits = n.digits;
	size = n.size;
	sign = n.sign;
}

SizeType GenRand( SizeType start, SizeType end ) {
	SizeType a = randomEngine() % (end+1 - start) + start;
	return a;
}

const BigInt &BigInt::operator=( const BigInt &n ) {
	if ( &n != this ) {
		digits = n.digits;
		size = n.size;
		sign = n.sign;
	}
	return *this;
}

const BigInt &BigInt::operator=( const DatType n ) {
	BigInt rs( n );
	if ( &rs != this ) {
		digits = rs.digits;
		size = rs.size;
		sign = rs.sign;
	}	
	return *this;
}

BigInt &BigInt::operator+=( BigInt n ) {
	if ( n.sign == 0 ) return *this;
	if ( sign == 0 ) {
		sign = n.sign;
		digits = n.digits;
		size = n.size;
		return *this;
	}

	SizeType sz = std::max<SizeType>( size, n.size ) + 1;
	std::string ds( sz, '0' );

	if ( sign == n.sign ) {
		int carry = 0;
		for ( SizeType i = 0; i < sz || carry; i++ ) {
			int newdig = carry;
			if ( i < size ) newdig += (digits[size-1-i] - '0');
			if ( i < n.size) newdig += (n.digits[n.size-1-i] - '0');
			int dc = newdig % 10;
			ds[sz-1-i] = '0' + dc ;
			carry = newdig / 10;
		}
		BigInt rs( ds );
		digits = rs.digits;
		size = rs.size;
	} else {
		n.sign *= -1;
		operator -=( n );
		n.sign *= -1;
	}
	return *this;
}

BigInt &BigInt::operator+=( DatType n ) {
	BigInt result( n );
	operator +=( result );
	return *this;
}

BigInt BigInt::operator+( BigInt n ) {
	BigInt result(*this);
	result += n;
	return result;
}

BigInt BigInt::operator+( DatType n ) {
	BigInt result(*this);
	result += n;
	return result;
}

// substraction of two numbers ( same sign )
BigInt BigInt::SubSameSign( BigInt first, BigInt second ){
	if ( first == second ) return BigInt();
	
	SizeType sz = std::max<SizeType>( first.size, second.size );
	std::string ds( sz, '0' );
	int sign = 1;
	bool sgflag = 0;
	if ( first.sign == -1 ) {
		first.sign *= -1;
		second.sign *= -1;
		sign = -1;
		sgflag = 1;
	}

	if ( first < second ) sign *= -1;

	BigInt large = (first > second)?first:second;
	BigInt small = (first < second)?first:second;

	SizeType lsz = large.size;
	SizeType ssz = small.size;

	int carry = 0;
	for ( SizeType i = 0; i < sz; i++ ) {
		int newdig = carry;
		if ( i < lsz )  newdig += (large.digits[lsz-1-i] - '0');
		if ( i < ssz )  newdig -= (small.digits[ssz-1-i] - '0');
		if ( newdig < 0 ) {
			newdig += 10;
			carry = -1;
		} else carry = 0;
		ds[sz-1-i] = newdig + '0';
	}
	BigInt rs( ds );
	rs.sign *= sign;
	if ( sgflag == 1) {
		first.sign = -1;
		second.sign = -1;
	}
	return rs;
}

BigInt BigInt::operator-() {
	BigInt result( *this );
	result.sign *= -1;
	return result;
}

BigInt &BigInt::operator-=( BigInt n ) {
	if ( n.sign == 0 ) return *this;
	if ( sign == 0 ) {
		sign = n.sign * (-1);
		digits = n.digits;
		size = n.size;
		return *this;
	}

	if ( sign == n.sign ) {
		BigInt rs = SubSameSign( *this, n );
		sign = rs.sign;
		digits = rs.digits;
		size = rs.size;
	} else {
		n.sign *= -1;
		operator +=( n );
		n.sign *= -1;
	}
	return *this;
}

BigInt &BigInt::operator-=( DatType n ) {
	BigInt result( n );
	operator -=( result );
	return *this;
}

BigInt BigInt::operator-( BigInt n ) {
	BigInt result(*this);
	result -= n;
	return result;
}

BigInt BigInt::operator-( DatType n ) {
	BigInt result(*this);
	result -= n;
	return result;
}

BigInt BigInt::operator*( BigInt n ) {
	int rs_sign = sign * n.sign;
	if ( rs_sign == 0 ) return BigInt();
	if ( n == 10 ){
		BigInt rs = *this;
		rs.digits.append("0");
		rs.size++;
		return rs;
	}
	std::string ds( size + n.size, '0' );
	SizeType sz = size + n.size;
	SizeType i, j;
	int carry = 0;
	for ( i = n.size - 1; i >= 0; i-- ) {
		if ( n.digits[i] ) {
			carry = 0;
			for ( j = size - 1; j >= 0 || carry; j-- ) {
				int jdg = ( j >= 0)?(digits[j] - '0'):0;
				int newDig = (ds[i+j+1] - '0') + (n.digits[i] - '0')*jdg + carry;
				ds[i+j+1] = '0' + newDig % 10;
				carry = newDig / 10;
			}
		}
	}

	BigInt result( ds );
	result.sign = rs_sign;
	return result;
}

BigInt BigInt::operator*( DatType n ) {
	BigInt tmp( n );
	BigInt result = (*this) * tmp;
	return result;
}

BigInt &BigInt::operator*=( DatType n ) {
	operator=( operator*( n ) );
	return (*this);
}

BigInt &BigInt::operator*=( BigInt n ) {
	operator=( operator*( n ) );
	return (*this);
}

BigInt BigInt::operator/ ( BigInt n ) {
	BigInt result( *this );
	result /= n;
	return result;
}

BigInt &BigInt::operator/=( BigInt n ) {
	if ( n.sign == 0 ) n.sign /= n.sign;
	if ( sign == 0 ) return *this;
	sign *= n.sign;
	int prevSign = n.sign;
	n.sign = 1;
	BigInt q;
	for ( SizeType i = 0; i < size; ++i ) {		
		const std::string str(1,digits[i]);
		if ( q.sign != 0 ) {
			q.digits.append(str);
			q.size++;
		} else {
			q.digits = str;
			q.size = 1;
			q.sign = ( digits[i] == '0' )?0:1;
		}
		digits[i] = '0';
		while ( q >= n ) {
			q -= n;
			digits[i] = digits[i] + ('1' - '0') ;
		}
	}
	n.sign = prevSign;
	BigInt tmp( digits );
	digits = tmp.digits;
	size = tmp.size;
	if ( tmp.sign == 0 ) sign = 0;
	return *this;	
}

BigInt BigInt::operator/ ( DatType n ) {
	BigInt tmp( n );
	BigInt result = (*this) / tmp;
	return result;
}

BigInt &BigInt::operator/=( DatType n ) {
	BigInt tmp( n );
	(*this) /= tmp;
	return *this;
}

// Module of 2^(pw)
DatType BigInt::ModPower2( int pw ) {
	DatType rs = 0;
	DatType n = 2 << pw;
	const int min_in = std::max<int>( 0, size - pw - 1 );
	for ( int i = min_in; i < size; i++ ) {
		rs *= 10;
		rs += (digits[i]-'0');
	}
	return rs%n;
}

BigInt BigInt::operator% ( BigInt n ) {
	if ( n.sign == 0 ) n.sign /= n.sign;
	if ( sign == 0 ) return *this;
	BigInt r;
	if ( n == 1 ) { 
		r.size = 1;
		r.sign = 0;
		r.digits = "0";
		return r; 
	};
	
	int prevSign = n.sign;
	n.sign = 1;

	for ( int pw = 0; pw  < 3; pw++ ) {
		if ( n == (2 << pw) ) {
			DatType rs = ModPower2( pw );
			const char ch = '0' + rs;
			const std::string st(1,ch);
			r.size = 1;
			r.digits = st;
			if ( rs ) r.sign = 1;
			else r.sign = 0;
			if ( sign != 1 ) r.sign *= -1;
			return r;
		}
	}

	for ( SizeType i = 0; i < size; ++i ) {
		//r *= 10;
		//r += (digits[i] - '0');
		const std::string str(1,digits[i]);
		if ( r.sign != 0 ) {
			r.digits.append(str);
			r.size++;
		} else {
			r.digits = str;
			r.size = 1;
			r.sign = ( digits[i] == '0' )?0:1;
		}
		while ( r >= n ) {
			r -= n;
		}
	}
	n.sign = prevSign;
	if ( sign != 1 ) r.sign *= -1;
	return r;
}

DatType BigInt::operator% ( DatType n ) {
	if ( n == 0 ) return n/n;
	if ( n == 1 ) return 0;
	if ( n == 2 ) return ModPower2(0);
	if ( n == 4 ) return ModPower2(1);
	if ( n == 8 ) return ModPower2(2);

	BigInt q(n);
	BigInt rs = (*this)%q;
	DatType numb;
	std::istringstream( rs.digits ) >> numb;
	return numb;
}

BigInt operator+( DatType m, BigInt &n ) {
    return n + m;
}

BigInt operator-( DatType m, BigInt &n ) {
    return -n + m;
}

BigInt operator*( DatType m, BigInt &n ) {
    return n * m;
}

BigInt operator/( DatType m, BigInt &n ) {
    return BigInt( m ) / n;
}

BigInt operator%( DatType m, BigInt &n ) {
    return BigInt( m ) % n;
}

int BigInt::BigCmpr( BigInt n ) {
	if ( sign < n.sign ) return -1;
	if ( sign > n.sign ) return 1;
	if ( size < n.size ) return -sign;
	if ( size > n.size ) return sign;
	for ( SizeType i = 0; i < size; i++ ) {
		if ( digits[i] < n.digits[i] ) return -sign;
		else if ( digits[i] > n.digits[i] ) return sign;
	}
	return 0;
}

bool BigInt::operator<( BigInt n ) {
    return( BigCmpr( n ) < 0 );
}

bool BigInt::operator>( BigInt n ) {
    return( BigCmpr( n ) > 0 );
}

bool BigInt::operator==( BigInt n ) {
    return( BigCmpr( n ) == 0 );
}

bool BigInt::operator!=( BigInt n ) {
    return( BigCmpr( n ) != 0 );
}

bool BigInt::operator<=( BigInt n ) {
    return( BigCmpr( n ) <= 0 );
}

bool BigInt::operator>=( BigInt n ) {
    return( BigCmpr( n ) >= 0 );
}

bool BigInt::operator<( DatType n ) {
    return( BigCmpr( BigInt( n ) ) < 0 );
}

bool BigInt::operator>( DatType n ) {
    return( BigCmpr( BigInt( n ) ) > 0 );
}

bool BigInt::operator==( DatType n ) {
    return( BigCmpr( BigInt( n ) ) == 0 );
}

bool BigInt::operator!=( DatType n ) {
    return( BigCmpr( BigInt( n ) ) != 0 );
}

bool BigInt::operator<=( DatType n ) {
    return( BigCmpr( BigInt( n ) ) <= 0 );
}

bool BigInt::operator>=( DatType n ) {
    return( BigCmpr( BigInt( n ) ) >= 0 );
}

// I/O friends
std::ostream &operator<<( std::ostream &out, BigInt n ) {
	if ( n.sign == -1) return out << "-" << n.digits;
	else return out << n.digits;
}

// Display the string
void BigInt::Display() {
	if ( sign == -1 ) std::cout << "-";
	std::cout << digits << std::endl;
}

// Generate a non-negative integer with size <= sz
BigInt RandBigIntSize( SizeType sz ) {
	SizeType s_t = GenRand(1,sz); 
	std::string ds(s_t, '0' );
	for ( SizeType i = 0; i < s_t; i++ ) ds[i] = '0' + GenRand(0,9);
	BigInt::BigInt rs( ds );
	return rs;
}