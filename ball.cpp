#include "ball.hpp"
#include "line.hpp"
#include <math.h>

ball::ball( hwlib::glcd_oled_spi & w, const vector & midpoint ):
	drawable( w, midpoint, vector( radius, radius) * 2),
	direction ( vector(0, 0) )
{}

void ball::draw() {
	w.write(prev_location, clear, hwlib::buffering::buffered);
	w.write(location.toLocation(), player, hwlib::buffering::buffered);
	w.flush();
}

void ball::update( const vector & pitchroll ) {
	direction *= 100;
	direction /= 101;
	direction += pitchroll;
//Limit speed for interaction
//	if (direction.x > 8) { direction.x = 8; }
//	if (direction.x < -8) { direction.x = -8; }
//	if (direction.y > 8) { direction.y = 8; }
//	if (direction.y < -8) { direction.y = -8; }
	
	prev_location = location.toLocation(); //Beautiful.
	location += direction;
	if ( location.x > 119 ) { location.x = 119; direction = vector (0, 0); }
	if ( location.x < 4 ) { location.x = 4; direction = vector (0, 0); }
	if ( location.y > 55 ) { location.y = 55; direction = vector (0, 0); }
	if ( location.y < 4 ) { location.y = 4; direction = vector (0, 0); }
}

void ball::interact( drawable & other ){
	if( this != & other){
		if( overlaps( other )){
			direction *= other.bounce;
		}
	}
}

void ball::print_location() {
	hwlib::cout << "X: " << location.x << " Y: " << location.y << "\n";
} 