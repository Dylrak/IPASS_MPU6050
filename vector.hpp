#ifndef VECTOR_HPP
#define VECTOR_HPP
/// @file

#include "hwlib.hpp"

/// \brief
/// vector ADT
/// \details
/// This is an ADT that implements vectors.
/// The x- & y-values are stored as two uint8_t's.
/// The appropriate constructors and operators are provided.
class vector
{
public:
	int x, y;
/// \brief
/// constructor from explicit values
/// \details
/// This constructor initializes a vector from the x- & y-values passed as parameter.
	vector(int x, int y):
		x ( x ), y ( y )
	{}
/// \brief
/// compare two vector values, equality
/// \details
/// This operator tests for equality. It returns true
/// if the [x, y]-values of both operands are equal.
	bool operator==( const vector & rhs ) const {
		return ( x == rhs.x ) && ( y == rhs.y );
	}
/// \brief
/// compare two vector values, smaller than
/// \details
/// This operator tests if our vector is smaller than another. It returns true
/// if either the x- or y-values of our vector is smaller than the right-hand side.
	bool operator < ( const vector & rhs ) const {
		return ( x < rhs.x ) || ( y < rhs.y );
	}
/// \brief
/// compare two vector values, bigger than
/// \details
/// This operator tests if our vector is bigger than another. It returns true
/// if either the x- or y-values of our vector is bigger than the right-hand side.
	bool operator > ( const vector & rhs ) const {
		return ( x > rhs.x ) || ( y > rhs.y );
	}
/// \brief
/// Add a vector to a vector
/// \details
/// This operator+ adds a vector to a vector.
	vector operator+( const vector & rhs ) const {
		return vector ( x + rhs.x, y + rhs.y );
	}
/// \brief
/// add a vector to this vector
/// \details
/// This operator+= adds a vector to a vector.
/// Because it is an operator+=, we need to change the original variable.
/// So the variable to the left hand side is passed as a pointer.
	vector & operator+=( const vector & rhs ){
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
/// \brief
/// Subtract a vector from a vector
/// \details
/// This operator- substracts the right hand side vector from this vector.
	vector operator-( const vector & rhs ) const {
		return vector ( x - rhs.x, y - rhs.y );
	}
/// \brief
/// Subtract a vector from this vector
/// \details
/// This operator-= substracts the right hand side vector from this vector.
/// Because it is an operator-=, we need to change the original variable.
/// So the variable to the left hand side is passed as a pointer.
	vector & operator-=( const vector & rhs ){
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
/// \brief
/// multiply this vector by an integer
/// \details
/// This operator*= multiplies a vector by an integer.
/// Because it is an operator*=, we need to change the original variable.
/// So the variable to the left hand side is passed as a pointer.
	vector & operator*=( const int rhs ){
		x *= rhs;
		y *= rhs;
		return *this;
	}
/// \brief
/// multiply this vector by a vector
/// \details
/// This operator*= multiplies a vector by a vector.
/// Because it is an operator*=, we need to change the original variable.
/// So the variable to the left hand side is passed as a pointer.
	vector & operator*=( const vector & rhs) {
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}
/// \brief   
/// multiply a vector by an integer
/// \details
/// This operator* multiplies both vector values by an integer value.
/// It is inside class scope because for some reason bmptk won't compile otherwise.
/// That is also why we cannot type 2 * vector.
vector operator*( const int & rhs ) {
	vector lhs = *this;
	lhs *= rhs;
	return lhs;
}
/// \brief
/// Divide this vector by an integer
/// \details
/// This operator/= divides a vector by an integer.
/// Because it is an operator/=, we need to change the original variable.
/// So the variable to the left hand side is passed as a pointer.
	vector & operator/=( const int rhs ){
		x /= rhs;
		y /= rhs;
		return *this;
	}
/// \brief   
/// Divide a vector by an integer
/// \details
/// This operator/ divides both vector values by an integer value.
/// It is, of course, guarded from using zero division.
	vector operator/( const int rhs ) {
		vector lhs = *this;
		if (rhs != 0) {
			return lhs /= rhs;
		}
		return lhs;
	}
/// \brief
/// Change vector into hwlib::location
/// \details
/// This function changes our current vector into a hwlib::location variable.
/// It is mainly used to work with window.write().
	hwlib::location toLocation() {
		return hwlib::location(x, y);
	}
};
#endif // VECTOR_HPP
