/*
 * TileCharacters.hpp
 *
 *  Created on: Jun 20, 2015
 *
 */

#ifndef TILECHARACTERS_HPP
#define TILECHARACTERS_HPP

namespace TileCharacters{

	enum Default {
		BLANK = 0,
		SMILE_FACE_EMPTY = 1,
		SMILE_FACE_FULL = 2,
		HEART = 3,
		DIAMOND = 4,
		CLUB = 5,
		SPADE = 6,
		DOT = 7,
		HOLE_IN_WALL = 8,
		EMPTY_CIRCLE = 9,
		DOT_CIRCLE = 10,
		MALE_SIGN = 11,
		FEMALE_SIGN = 12,
		SIXTENTH_NOTE = 13,
		BEAM_NOTE = 14,
		BURST = 15,
		FORWARD = 16,
		REVERSE = 17,
		UP_DOWN_ARROW_TALL = 18,
		DOUBLE_EXCLAMATION = 19,
		PARAGRAPH = 20,
		SWIRL = 21,
		HALF_BLOCK = 22,
		UP_DOWN_ARROW_SHORT = 23,
		UP_ARROW = 24,
		DOWN_ARROW = 25,
		RIGHT_ARROW = 26,
		LEFT_ARROW = 27,
		HOOK_BOTTOM_RIGHT = 28,
		LEFT_RIGHT_ARROW = 29,
		UP = 30,
		DOWN = 31,
		SPACE = 32,
		EXCLAMATION = 33,
		DOUBLE_QUOTE = 34,
		HASH = 35,
		DOLLAR_SIGN = 36,
		PERCENT = 37,
		AMPERSAND = 38,
		APOSTROPHE = 39,
		PARENTHESIS_OPEN = 40,
		PARENTHESIS_CLOSE = 41,
		STAR = 42,
		PLUS = 43,
		COMMA = 44,
		HYPHEN = 45,
		PERIOD = 46,
		FORWARD_SLASH = 47,
		ZERO = 48,
		ONE = 49,
		TWO = 50,
		THREE = 51,
		FOUR = 52,
		FIVE = 53,
		SIX = 54,
		SEVEN = 55,
		EIGHT = 56,
		NINE = 57,
		COLON = 58,
		SEMI_COLON = 59,
		LESS_THAN = 60,
		EQUAL = 61,
		GREATER_THAN = 62,
		QUESTION_MARK = 63,
		AT_SIGN = 64,
		A_UPPERCASE = 65,
		B_UPPERCASE = 66,
		C_UPPERCASE = 67,
		D_UPPERCASE = 68,
		E_UPPERCASE = 69,
		F_UPPERCASE = 70,
		G_UPPERCASE = 71,
		H_UPPERCASE = 72,
		I_UPPERCASE = 73,
		J_UPPERCASE = 74,
		K_UPPERCASE = 75,
		L_UPPERCASE = 76,
		M_UPPERCASE = 77,
		N_UPPERCASE = 78,
		O_UPPERCASE = 79,
		P_UPPERCASE = 80,
		Q_UPPERCASE = 81,
		R_UPPERCASE = 82,
		S_UPPERCASE = 83,
		T_UPPERCASE = 84,
		U_UPPERCASE = 85,
		V_UPPERCASE = 86,
		W_UPPERCASE = 87,
		X_UPPERCASE = 88,
		Y_UPPERCASE = 89,
		Z_UPPERCASE = 90,
		BRACKET_OPEN = 91,
		BACKSLASH = 92,
		BRACKET_CLOSE = 93,
		CARET = 94,
		UNDERSCORE = 95,
		SINGLE_QUOTE = 96,
		A_LOWERCASE = 97,
		B_LOWERCASE = 98,
		C_LOWERCASE = 99,
		D_LOWERCASE = 100,
		E_LOWERCASE = 101,
		F_LOWERCASE = 102,
		G_LOWERCASE = 103,
		H_LOWERCASE = 104,
		I_LOWERCASE = 105,
		J_LOWERCASE = 106,
		K_LOWERCASE = 107,
		L_LOWERCASE = 108,
		M_LOWERCASE = 109,
		N_LOWERCASE = 110,
		O_LOWERCASE = 111,
		P_LOWERCASE = 112,
		Q_LOWERCASE = 113,
		R_LOWERCASE = 114,
		S_LOWERCASE = 115,
		T_LOWERCASE = 116,
		U_LOWERCASE = 117,
		V_LOWERCASE = 118,
		W_LOWERCASE = 119,
		X_LOWERCASE = 120,
		Y_LOWERCASE = 121,
		Z_LOWERCASE = 122,
		CURLY_BRACE_OPEN = 123,
		BOLD_COLON = 124,
		CURLY_BRACE_CLOSE = 125,
		ACCENT = 126,
		TRAPOZOID = 127,
		C_CEDILLA_UPPERCASE = 128,
		U_DIAERESIS_LOWECASE = 129,
		E_ACUTE_LOWERCASE = 130,
		A_TIE_BAR_LOWERCASE = 131,
		A_DIAERESIS_LOWERCASE = 132,
		A_GRAVE_LOWERCASE = 133,
		A_RING_LOWERCASE = 134,
		C_CEDILLA_LOWERCASE = 135,
		E_TIE_BAR_LOWERCASE = 136,
		E_DIAERESIS_LOWERCASE = 137,
		E_GRAVE_LOWERCASE = 138,
		I_DIAERESIS_LOWERCASE = 139,
		I_TIE_BAR_LOWERCASE = 140,
		I_GRAVE_LOWERCASE = 141,
		A_DIAERESIS_UPPERCASE = 142,
		A_RING_UPPERCASE = 143,
		E_ACUTE_UPPERCASE = 144,
		AE = 145,
		A_UNIQUE_UPPERCASE = 146,
		O_TIE_BAR_LOWERCASE = 147,
		O_DIAERESIS_LOWERCASE = 148,
		O_GRAVE_LOWERCASE = 149,
		U_TIE_BAR_LOWERCASE = 150,
		U_GRAVE_LOWERCASE = 151,
		Y_DIAERESIS_LOWERCASE = 152,
		O_DIAERESIS_UPPERCASE = 153,
		U_DIAERESIS_UPPERCASE = 154,
		ZERO_UNIQUE_SMALL = 155,
		POUND_SIGN = 156,
		ZERO_UNIQUE_LARGE = 157,
		X_UNIQUE = 158,
		F_UNQIUE = 159,
		A_ACUTE_LOWERCASE = 160,
		I_ACUTE_LOWERCASE = 161,
		O_ACUTE_LOWERCASE = 162,
		U_ACUTE_LOWERCASE = 163,
		N_TILDE_LOWERCASE = 164,
		N_TILDE_UPPERCASE = 165,
		A_UNDERLINED_LOWERCASE = 166,
		O_UNDERLINED_LOWERCASE = 167,
		QUESTION_MARK_FLIPPED = 168,
		REGISTERED_MARK = 169,
		HOOK_UPPER_RIGHT = 170,
		ONE_HALF = 171,
		ONE_FOURTH = 172,
		EXCLAMATION_MARK_FLIPPED = 173,
		SHIFT_LEFT = 174,
		SHIFT_RIGHT = 175,
		DARK_FIELD_DOTS = 176,
		GRAY_FIELD_DOTS = 177,
		WHITE_FIELD_DOTS = 178,
		PIPE_VERTICAL = 179,
		PIPE_T_RIGHT = 180,
		A_ACUTE_UPPERCASE = 181,
		A_TIE_BAR_UPPERCASE = 182,
		A_GRAVE_UPPERCASE = 183,
		COPYRIGHT_MARK = 184,
		DOUBLE_PIPE_T_RIGHT = 185,
		DOUBLE_PIPE_VERTICAL = 186,
		DOUBLE_PIPE_CORNER_UPPER_RIGHT = 187,
		DOUBLE_PIPE_CORNER_LOWER_RIGHT = 188,
		CENT_SYMBOL = 189,
		YEN_SYMBOL = 190,
		PIPE_CORNER_UPPER_RIGHT = 191,
		PIPE_CORNER_LOWER_LEFT = 192,
		PIPE_T_BOTTOM = 193,
		PIPE_T_TOP = 194,
		PIPE_T_LEFT = 195,
		PIPE_HORIZONTAL = 196,
		PIPE_CROSS = 197,
		A_RING_ACUTE_UPPERCASE = 198,
		A_TILDE_UPPERCASE = 199,
		DOUBLE_PIPE_CORNER_LOWER_LEFT = 200,
		DOUBLE_PIPE_CORNER_UPPER_LEFT = 201,
		DOUBLE_PIPE_T_BOTTOM = 202,
		DOUBLE_PIPE_T_TOP = 203,
		DOUBLE_PIPE_T_LEFT = 204,
		DOUBLE_PIPE_HORIZONTAL = 205,
		DOUBLE_PIPE_CROSS = 206,
		HEADCRAB = 207,
		XD = 208,
		D_UNIQUE_ = 209,
		E_RING_UPPERCASE = 210,
		E_DIAERESIS_UPPERCASE = 211,
		E_GRAVE_UPPERCASE = 212,
		ONE_UNIQUE_LEFT = 213,
		F_UNIQUE_2 = 214,
		I_RING_UPPERCASE = 215,
		I_DIAERESIS_UPPERCASE = 216,
		PIPE_CORNER_LOWER_RIGHT = 217,
		PIPE_CORNER_UPPER_LEFT = 218,
		BLOCK_FULL = 219,
		BLOCK_BOTTOM_HALF = 220,
		BOLDEST_COLON = 221,
		I_GRAVE_UPPERCASE = 222,
		BLOCK_TOP_HALF_1 = 223,
		BOX_EMPTY = 224,
		BOX_CHECKED = 225,
		BOX_UPPER_LEFT_QUARTER = 226,
		BOX_BOTTOM_RIGHT_QUARTER = 227,
		BLOCK_TOP_HALF_2 = 228,
		BLOCK_MID_RIGHT = 229,
		BLOCK_UPPER_LEFT_LOWER_RIGHT = 230,
		BOX_RIGHT_HALF = 231,
		BLOCK_MID_LEFT = 232,
		U_ACUTE_UPPERCASE = 233,
		U_TIE_BAR_UPPERCASE = 234,
		U_GRAVE_UPPERCASE = 235,
		Y_ACUTE_LOWERCASE = 236,
		Y_ACUTE_UPPERCASE = 237,
		MACRON = 238,
		SINGLE_QUOTE_2 = 239,
		MINUS = 240,
		PLUS_MINUS = 241,
		EQUALS_2 = 242,
		THREE_FOURTHS = 243,
		PARAGRAPH_2 = 244,
		SWIRL_2 = 245,
		DIVIDE = 246,
		COMMA_2 = 247,
		O_LOWERCASE_2 = 248,
		DIAERESIS = 249,
		PERIOD_2 = 250,
		ONE_UNIQUE_LARGE = 251,
		TWO_UNIQUE_LARGE = 252,
		THREE_UNIQUE_LARGE = 253,
		BLOCK_SMALL = 254,
		RAINBOW = 255
	};
};

#endif /* TILECHARACTERS_HPP */
