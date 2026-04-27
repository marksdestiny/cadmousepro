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

#include <string>

#include "CLI.h"

class CLIException {
private:
	std::wstring message;
	int exitCode = CLI::EXIT_CODE_UNEXPECTED_ERROR;
public:

	CLIException(std::wstring message, int exitCode) : message(message), exitCode(exitCode) {
	}
	
	std::wstring getMessage() const {
		return message;
	}

	int getExitCode() const {
		return exitCode;
	}

};
