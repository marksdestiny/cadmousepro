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

#include "CadMouseConfig.h"
#include "CLI.h"
#include "CLIException.h"

class Common {
public:

	static Frequency parseFrequency(std::wstring value) {
		if (value == L"125") return Frequency::Hz_0125;
		else if (value == L"250") return Frequency::Hz_0250;
		else if (value == L"500") return Frequency::Hz_0500;
		else if (value == L"1000") return Frequency::Hz_1000;
		else throw CLIException(L"Unable to parse frequency value „" + value + L"“ – expecting „125“ or „250“ or „500“ or „1000“", CLI::EXIT_CODE_BAD_CLI_ARGUMENTS);
	}
	
	// TODO: refactor exceptions

};
