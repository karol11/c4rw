#include "robot.h"

namespace robot {

	void robot_main() {
		make_field(3, {
			".                .",
			"                  ",
			"    Hello there   ",
			"                  ",
			"     *   #   *    ",
			"         ^        ",
			"                  ",
			"     #<  @  >#    ",
			"                  ",
			"         _        ",
			"     *   #   *    ",
			"    0123456789    ",
			".                ."});
		save_field("demo.field");
		make_field(2, {
			"                  ",
			"   ####      #    ",
			"      #      #    ",
			"      #     ####  ",
			"##########     #  ",
			"     #         #  ",
			"     #      #  #  ",
			"  ####      #  #  ",
			"            #     ",
			"  #  #############",
			"  #      #        ",
			"  #      #####    ",
			"* #             @ " });
		save_field("labytinth.field");
		make_field(0, {
			"          >     _ ",
			"                  ",
			"  _           <   ",
			"                  ",
			"                  ",
			"                  ",
			"                  ",
			"                  ",
			"_     @   ^     < ",
			"                  ",
			"  >         *     ",
			">              ^  ",
			"                  " });
		save_field("arrows.field");
		make_field(3, {
			"                  ",
			"  .   .     .  .  ",
			"       .    .  .  ",
			"   .       ..     ",
			"       .       .  ",
			"  .        .  ..  ",
			"        @      .  ",
			"     .            ",
			"   .           .  ",
			"        .   .     ",
			"             .  . ",
			"    ...    ...  . ",
			"                  " });
		save_field("grass.field");
		make_field(3, {
			" 48  10   33      ",
			"                  ",
			"                  ",
			"                  ",
			"     a            ",
			"                  ",
			"                  ",
			"           7      ",
			"                  ",
			"     88           ",
			"            x     ",
			"                  ",
			"                  " });
		save_field("numbers.field");
		make_field(0, {
			"This land is      ",
			"your land         ",
			"and this land     ",
			"is my land        ",
			"From the          ",
			"California to the ",
			"New York island   ",
			"From the redwood  ",
			"forest to the     ",
			"Gulf Stream waters",
			"This land         ",
			"was made          ",
			"for you and me   @" });
		save_field("text.field");
	}
}