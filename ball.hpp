#ifndef BALL_HPP
#define BALL_HPP

#include "drawable.hpp"
#include "line.hpp"

class ball : public drawable
{
private:
	vector direction;
	hwlib::location prev_location = hwlib::location(64, 32);
	//The radius is increased to one pixel above actual size for the hitbox in overlap. The actual radius is 4.
	uint8_t radius = 4;
	hwlib::image_8x8 player = hwlib::image_8x8 (0b00011000, 0b01111110, 0b01111110, 0b11111111, 0b11111111, 0b01111110, 0b01111110, 0b00011000);
	hwlib::image_8x8 clear = hwlib::image_8x8 (0, 0, 0, 0, 0, 0, 0, 0);
public:
	ball( hwlib::glcd_oled_spi & w, const vector & midpoint );
	void draw() override;
	void update( const vector & pitchroll ) override;
	void interact( drawable & other ) override;
	void print_location();
};

#endif // BALL_HPP
