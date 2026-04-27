# cadMousePro
# Copyright © 2020 František Kučera (Frantovo.cz, GlobalCode.info)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

_cadMousePro_completion() {
	local w0 w1 w2

	COMPREPLY=()
	w0=${COMP_WORDS[COMP_CWORD]}
	w1=${COMP_WORDS[COMP_CWORD-1]}
	w2=${COMP_WORDS[COMP_CWORD-2]}

	BOOLEAN=(
		"true"
		"false"
	)

	FREQUENCY=(
		"125"
		"250"
		"500"
		"1000"
	)

	if   [[ "$w1" == "--frequency"                     ]];    then COMPREPLY=($(compgen -W "${FREQUENCY[*]}" -- "$w0"))
	elif [[ "$w1" == "--smart-scrolling"               ]];    then COMPREPLY=($(compgen -W "${BOOLEAN[*]}" -- "$w0"))
	elif [[ "$w1" == "--lift-off-detection"            ]];    then COMPREPLY=($(compgen -W "${BOOLEAN[*]}" -- "$w0"))
	elif [[ "$w1" == "--remap-wheel-press"             ]];    then COMPREPLY=($(compgen -W "${BOOLEAN[*]}" -- "$w0"))
	elif [[ "$w1" == "--remap-gesture-button"          ]];    then COMPREPLY=($(compgen -W "${BOOLEAN[*]}" -- "$w0"))
	elif [[ "$w1" == "--daemon"                        ]];    then COMPREPLY=($(compgen -W "${BOOLEAN[*]}" -- "$w0"))
	else
		OPTIONS=(
			"--frequency"
			"--smart-scrolling"
			"--lift-off-detection"
			"--remap-wheel-press"
			"--remap-gesture-button"
			"--daemon"
		)
		COMPREPLY=($(compgen -W "${OPTIONS[*]}" -- "$w0"))
	fi
}

complete -F _cadMousePro_completion cadMousePro
