/*
 * BorderScreen.cpp
 *
 *  Created on: Nov 3, 2015
 */
#include "libtcod.hpp"
#include <string>
#include <vector>
#include "BorderScreen.hpp"
#include "Console/Message.hpp"
#include "../LoggerWrapper.hpp"

BorderScreen::BorderScreen() : ScreenUI(){

	std::string invisibleHands = "Something grabs your wrist. You turn and pull, but nothing is there. Then something pulls back. You can see the outline of fingers pressing into your skin. You struggle, but the grip only tightens. Something grabs your shoulder and then your leg. They pull you back toward town.";
	std::string tooMuchGravity = "You've been walking for what seems like hours. Your shoulders stoop, your arms hang heavy, and you can barely seem to pick up your legs. Every step seems to add another ten-pound weight to your back. Finally you fall to your knees, but you won't stop there. You crawl on, but soon your arms give out. You belly-crawl a few yards more, but then the gravity is so intense you can't lift your chest to breathe. You must turn back.";
	std::string gravityPullsBack = "You feel yourself lean backward. Must be the dizziness from walking so far. Pushing on, you feel yourself walk uphill even though the road is straight ahead. Your feet begin to slip, and you have to use your hands to climb. Suddenly you lose your handhold and fall horizontally, scraping against the ground for several yards before you come to a stop and gravity is normal again.";
	std::string worldFadesOut = "Things seem to be getting darker, but when you look up, the moon is still shining. Peering again, you realize the trees aren't going dark; they're fading. You can see through the thin leaves and trunks become just a haze. You look at your own hand. It is visible for a moment, and then it turns into a pale phantom. Closing your eyes against the strangeness, you go further out. When your eyelids become transparent, you decide to turn back.";
	std::string thingsToLiquid = "The ground goes soft under your feet. You try to find some footing on a few rocks, but even they squish like mud. The trees around you begin to sag. The ground swallows up your shoes to the ankles. Venturing onward, the soupy muck turns to a multi-colored goop. Your limbs are weary from fighting through it, and you crawl onto solid ground back toward town.";
	std::string invisibleWall = "You bump into something and staggering backward. When you regain your footing, you don't see anything that you could have run into. You put out your hands carefully and feel the smooth texture of glass. It goes as far as you can reach in every direction. You knock on it, but it's too thick to make a sound. You pound your fists against it and even kick, but the invisible wall holds firm.";
	std::string freezingWind = "While you walk away from town, a cold breeze comes up. It grows stronger and colder as you go. Ice pellets begin to lash agaisnt your face. You pull your collar tight and press on into the wind, now a gale. Your fingers and ears go numb. After a few more steps, the wind pushes so hard, you can't even move your leg forward. There's no option but to go back.";
	std::string turnedAround = "The world goes dark for a moment. Did the moon go behind a cloud? Shadows deepen, and then they wane. When you can see again, the lights of town are straight ahead. You look behind you at the dark where you were headed. When did you turn around?";
	std::string worldEdge = "Finally, you can see the trees begin to thin out. You hurry on, but then your pace slows when you see the land give way to a huge black chasm. A howling wind rushes up from it. There are no lights beyond it; even the stars go dark at the horizon. It is the abyss, and you think you hear laughter coming from it.";
	log = Message::WordWrapText(worldEdge, 77);
}

BorderScreen::~BorderScreen() {
}
