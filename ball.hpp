#ifndef BALL_HPP
#define BALL_HPP

#include "drawable.hpp"

class ball : public drawable
{
private:
	vector direction;
	//The radius is increased to one pixel above actual size for the hitbox in overlap. The actual radius is 4.
	uint8_t hitbox_radius = 5, radius = 4;
public:
	ball( hwlib::glcd_oled_spi & w, const vector & midpoint );
	void draw() override;
	void update( const vector & rollpitch ) override;
	void interact( drawable & other ) override;
	
};

#endif // BALL_HPP
