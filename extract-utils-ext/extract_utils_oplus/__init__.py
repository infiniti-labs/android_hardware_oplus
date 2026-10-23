#
# SPDX-FileCopyrightText: The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

import sys

sys.dont_write_bytecode = True

from extract_utils_oplus.firmware import (
    add_firmware_proprietary_file,
)
from extract_utils_oplus.thermal_engine import (
    add_generated_thermal_engine_config,
)

__all__ = [
    'add_firmware_proprietary_file',
    'add_generated_thermal_engine_config',
]
