#include "ball.hpp"

ball::ball( hwlib::glcd_oled_spi & w, const vector & midpoint ):
	drawable( w, midpoint - vector (radius, radius), vector( radius, radius) * 2),
	direction ( vector(1, 1) )
{}

void ball::draw() {
	w.write((location + direction).toLocation(), player, hwlib::buffering::buffered);
}

void ball::update( const vector & rollpitch ) {
	//Half the direction to simulate drag
	direction /= 2;
	direction += rollpitch;
}

void ball::interact( drawable & other ){
   if( this != & other){
      if( overlaps( other )){
//         direction *= other.bounce;
      }
   }
}