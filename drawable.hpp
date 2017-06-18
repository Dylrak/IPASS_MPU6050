#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "vector.hpp"
#include <ostream>
#include "hwlib.hpp"
#include "hwlib-glcd-oled-spi.hpp"

class drawable {
protected:
   hwlib::glcd_oled_spi w;
   vector location;
   vector size;
public:
   drawable( hwlib::glcd_oled_spi & w, const vector & location, const vector & size, const vector & bounce = vector(1, 1));

   bool overlaps( const drawable & other );
   virtual void draw() = 0;
   virtual void update( const vector & rollpitch ){}
   virtual void interact( drawable & other ){}
   //This vector depicts which speed axis will be inverted if an object hits this drawable.
   vector bounce;
};

bool within( int x, int a, int b );
#endif // DRAWABLE_HPP
