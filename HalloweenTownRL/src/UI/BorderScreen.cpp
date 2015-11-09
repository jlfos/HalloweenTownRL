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
//	std::string invisibleHands = "Something grabs your wrist. You turn and pull, but nothing is there. Then something pulls back. "
//		+	"You can see the outline of fingers pressing into your skin. You struggle, but the grip only tightens."
//				 + " Something grabs your shoulder and then your leg. They pull you back toward town.";

	std::string tooMuchGravity = "You've been walking for what seems like hours. Your shoulders stoop, your arms hang heavy, and you can barely seem to pick up your legs. Every step seems to add another ten-pound weight to your back. Finally you fall to your knees, but you won't stop there. You crawl on, but soon your arms give out. You belly-crawl a few yards more, but then the gravity is so intense you can't lift your chest to breathe. You must turn back.";

	log = Message::WordWrapText(tooMuchGravity, 77);
}

BorderScreen::~BorderScreen() {
}
