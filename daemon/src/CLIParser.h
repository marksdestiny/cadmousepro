/**
 * cadMousePro
 * Copyright © 2019 František Kučera (Frantovo.cz, GlobalCode.info)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <vector>

#include "CLI.h"
#include "CLIConfiguration.h"
#include "CLIException.h"

class CLIParser {
private:

	std::wstring readNext(std::vector<std::wstring> arguments, int& i) {
		if (i < arguments.size()) return arguments[i++];
		else throw CLIException(L"Missing CLI argument" + (i > 0 ? (L" after " + arguments[i - 1]) : L""), CLI::EXIT_CODE_BAD_CLI_ARGUMENTS);
	}

	bool parseBoolean(std::wstring value) {
		if (value == L"true") return true;
		else if (value == L"false") return false;
		else throw CLIException(L"Unable to parse boolean value „" + value + L"“ – expecting „true“ or „false“", CLI::EXIT_CODE_BAD_CLI_ARGUMENTS);
	}

public:

	static Frequency parseFrequency(std::wstring value) {
		if (value == L"125") return Frequency::Hz_0125;
		else if (value == L"250") return Frequency::Hz_0250;
		else if (value == L"500") return Frequency::Hz_0500;
		else if (value == L"1000") return Frequency::Hz_1000;
		else throw CLIException(L"Unable to parse frequency value „" + value + L"“ – expecting „125“ or „250“ or „500“ or „1000“", CLI::EXIT_CODE_BAD_CLI_ARGUMENTS);
	}

	static const std::wstring OPTION_FREQUENCY;
	static const std::wstring OPTION_SMART_SCROLLING;
	static const std::wstring OPTION_LIFT_OFF_DETECTION;
	static const std::wstring OPTION_REMAP_WHEEL_PRESS;
	static const std::wstring OPTION_REMAP_GESTURE_BUTTON;
	static const std::wstring OPTION_DAEMON;
	static const std::wstring OPTION_HELP;

	CLIConfiguration parse(const std::vector<std::wstring>& arguments) {
		CLIConfiguration c;

		for (int i = 0; i < arguments.size();) {
			std::wstring option = readNext(arguments, i);

			if (option == OPTION_FREQUENCY) {
				c.cadMouseConfig.setFrequency(parseFrequency(readNext(arguments, i)));
			} else if (option == OPTION_SMART_SCROLLING) {
				c.cadMouseConfig.setSmartScrolling(parseBoolean(readNext(arguments, i)));
			} else if (option == OPTION_LIFT_OFF_DETECTION) {
				c.cadMouseConfig.setLiftOffDetection(parseBoolean(readNext(arguments, i)));
			} else if (option == OPTION_REMAP_WHEEL_PRESS) {
				c.cadMouseConfig.setWheelPressRemapped(parseBoolean(readNext(arguments, i)));
			} else if (option == OPTION_REMAP_GESTURE_BUTTON) {
				c.cadMouseConfig.setRemapGestureButton(parseBoolean(readNext(arguments, i)));
			} else if (option == OPTION_DAEMON) {
				c.daemon = true;
			} else if (option == OPTION_HELP) {
				c.help = true;
			} else throw CLIException(L"Unsupported CLI option: " + option, CLI::EXIT_CODE_BAD_CLI_ARGUMENTS);
		}

		return c;
	}

	virtual ~CLIParser() {
	}
};


const std::wstring CLIParser::OPTION_FREQUENCY = L"--frequency";
const std::wstring CLIParser::OPTION_SMART_SCROLLING = L"--smart-scrolling";
const std::wstring CLIParser::OPTION_LIFT_OFF_DETECTION = L"--lift-off-detection";
const std::wstring CLIParser::OPTION_REMAP_WHEEL_PRESS = L"--remap-wheel-press";
const std::wstring CLIParser::OPTION_REMAP_GESTURE_BUTTON = L"--remap-gesture-button";
const std::wstring CLIParser::OPTION_DAEMON = L"--daemon";
const std::wstring CLIParser::OPTION_HELP = L"--help";
