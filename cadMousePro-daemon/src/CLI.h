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

#include <locale.h>
#include <string>
#include <vector>
#include <sstream>
#include <locale>
#include <codecvt>
#include <iostream>

/**
 * a common header-only library for CLI programs
 */
class CLI {
public:

	CLI(int argc, char* argv[]) {
		setlocale(LC_ALL, "");

		this->argc = &argc;
		this->argv = &argv;

		program = convertor.from_bytes(argv[0]);

		for (int i = 1; i < argc; i++) {
			args.insert(args.end(), convertor.from_bytes(argv[i]));
		}

	}

	CLI(const CLI& orig) {
	}

	virtual ~CLI() {
	}

	const std::wstring programName() {
		return (const std::wstring) program;
	}

	const std::vector<std::wstring> arguments() {
		return (const std::vector<std::wstring>)args;
	}

	static const int EXIT_CODE_SUCCESS = 0;
	static const int EXIT_CODE_UNEXPECTED_ERROR = 1;
	static const int EXIT_CODE_BAD_CLI_ARGUMENTS = 6;

	/**
	 * std::cin and std::cout are "tied" by default i.e. reading from std::cin causes flush of the std::cout.
	 * Call this to "untie" them and have less write() calls (better buffering).
	 */
	static void untieStdIO() {
		std::cin.tie(nullptr);
	}

private:
	int* argc;
	char*** argv;
	std::wstring program;
	std::vector<std::wstring> args;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> convertor; // TODO: support also other encodings.
};
