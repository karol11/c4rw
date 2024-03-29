#include "robot.h"

namespace robot {

	void robotMain() {
		makeField(3, {
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
		saveField("demo.field");
		makeField(2, {
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
		saveField("labytinth.field");
		makeField(0, {
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
		saveField("arrows.field");
		makeField(3, {
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
		saveField("grass.field");
		makeField(3, {
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
		saveField("numbers.field");
		makeField(0, {
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
		saveField("text.field");
	}
}