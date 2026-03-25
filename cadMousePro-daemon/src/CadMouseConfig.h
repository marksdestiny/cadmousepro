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

#include <type_traits>

#include "HID.h"

enum class Frequency : uint8_t {
	Hz_0125 = 8,
	Hz_0250 = 4,
	Hz_0500 = 2,
	Hz_1000 = 1
};

using FrequencyType = std::underlying_type<Frequency>::type;

class CadMouseConfig {
private:
	bool liftOffDetection = true;
	bool smartScrolling = false;
	Frequency frequency = Frequency::Hz_1000;
	bool remapWheelPress = true;
	bool remapGestureButton = true;
public:

	void setFrequency(Frequency frequency) {
		this->frequency = frequency;
	}

	void setLiftOffDetection(bool liftOffDetection) {
		this->liftOffDetection = liftOffDetection;
	}

	void setSmartScrolling(bool smartScrolling) {
		this->smartScrolling = smartScrolling;
	}

	/**
	 * @param remapGestureButton whether the gesture button (the small one behind the wheel)
	 * should send a button signal (11) otherwise it sends no button signal
	 */
	void setRemapGestureButton(bool remapGestureButton) {
		this->remapGestureButton = remapGestureButton;
	}

	/**
	 * @param remapWheelPress whether the wheel pressed
	 * should send different button signal (10) than the middle button (2)
	 */
	void setWheelPressRemapped(bool remapWheelPress) {
		this->remapWheelPress = remapWheelPress;
	}

	Packet serialize() const {
		Packet data;
		data.reserve(32);

		data.push_back(0x10); // report ID
		data.push_back(0x00); // option

		data.push_back(0x1c); // speed

		data.push_back(liftOffDetection ? 0x00 : 0x1f);

		if (smartScrolling) {
			data.push_back(0x00);
			data.push_back(0x00);
			data.push_back(0x00);
			data.push_back(0x01);
		} else {
			data.push_back(0x01);
			data.push_back(0xff);
			data.push_back(0x00);
			data.push_back(0x00);
		}

		for (int i = 0; i < 8; i++) data.push_back(0x00); // constant padding or magic

		// magic constants or unknown fields
		data.push_back(0x00);
		data.push_back(0x03);
		data.push_back(0x00);
		data.push_back(0x0a);
		data.push_back(0x0b);
		data.push_back(0x0c);
		data.push_back(remapWheelPress ? 0x0f : 0x0c);
		data.push_back(0x0e);
		data.push_back(0x0d);
		data.push_back(remapGestureButton ? 0x10 : 0x2f);
		data.push_back(0x00);
		data.push_back(0x1e);

		data.push_back(0x00);
		data.push_back(0x00);
		data.push_back(0x00);
		data.push_back(static_cast<FrequencyType> (frequency));

		return data;
	}

};
