#include "hwlib.hpp"
#include "hwlib-glcd-oled-spi.hpp"
#include "mpu6050.hpp"
//#include "ball.hpp"

int main( void ) {
	WDT->WDT_MR = WDT_MR_WDDIS;

	namespace target = hwlib::target;

	auto mpu_scl = target::pin_oc( target::pins::scl );
	auto mpu_sda = target::pin_oc( target::pins::sda );

	auto mosi	= target::pin_out(target::pins::d9);
	auto sclk	= target::pin_out(target::pins::d10);
	auto dc		= target::pin_out(target::pins::d11);
	auto cs		= target::pin_out(target::pins::d12);
	auto reset	= target::pin_out(target::pins::d13);
	auto miso	= target::pin_in (target::pins::d5);

	auto mpu_i2c = hwlib::i2c_bus_bit_banged_scl_sda( mpu_scl, mpu_sda );
	auto oled_spi = hwlib::spi_bus_bit_banged_sclk_mosi_miso(sclk, mosi, miso);

	auto oled = hwlib::glcd_oled_spi(oled_spi, cs, dc, reset);
	auto mpu = mpu6050(mpu_i2c);

//	ball player(oled, vector(64, 32));
	hwlib::wait_ms(500);
	mpu.calibrate();

	hwlib::image_8x8 player (
		0b00011000,
		0b01111110,
		0b01111110,
		0b11111111,
		0b11111111,
		0b01111110,
		0b01111110,
		0b00011000
	);
	hwlib::image_8x8 clear (
		0, 0, 0, 0,
		0, 0, 0, 0
	);
	oled.flush();
//Main loop:
	uint8_t pitch = 64, roll = 32;
	for (;;) {
		mpu.get_values();
		oled.write(hwlib::location(pitch, roll), clear, hwlib::buffering::unbuffered);
//The doubles from roll/pitch are converted into ints and divided by ten.
//This gives us values ranging from -9:9, which we can use for the ball.
		pitch += ((int) mpu.get_pitch()) / 10;
		roll += ((int) mpu.get_roll()) / 10;

		oled.write(hwlib::location(pitch, roll), player, hwlib::buffering::unbuffered);
		oled.flush();

//		player.draw();
//		hwlib::cout << "Roll: " << (int) roll
//		<< " Pitch: " << (int) pitch << "\n";
	}
}