#
# SPDX-FileCopyrightText: The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

from __future__ import annotations

from extract_utils.file import FileArgs
from extract_utils.makefiles import (
    MakefilesCtx,
    write_filegroups,
    write_mk_firmware,
    write_mk_firmware_ab_partitions,
    write_mk_guard_begin,
    write_mk_guard_end,
)
from extract_utils.module import ExtractUtilsModule, FirmwareProprietaryFile


class OplusFirmwareProprietaryFile(FirmwareProprietaryFile):
    # EXTRACT_ONLY images are build inputs, not radio partitions.
    def write_makefiles(self, module: ExtractUtilsModule, ctx: MakefilesCtx):
        radio_files = [
            file
            for file in self.file_list.all_files
            if FileArgs.EXTRACT_ONLY not in file.args
        ]

        write_mk_firmware_ab_partitions(radio_files, ctx.board_config_mk_out)

        write_mk_guard_begin('TARGET_DEVICE', module.device, ctx.mk_out)
        write_mk_firmware(
            module.vendor_path,
            self.vendor_rel_sub_path,
            radio_files,
            ctx.mk_out,
        )
        write_mk_guard_end(ctx.mk_out)

        write_filegroups(
            ctx,
            self.vendor_rel_sub_path,
            self.file_list.filegroup_files,
        )


def add_firmware_proprietary_file(module: ExtractUtilsModule):
    file_list_path = module.proprietary_file_path('proprietary-firmware.txt')
    proprietary_file = OplusFirmwareProprietaryFile(file_list_path)
    module.proprietary_files.append(proprietary_file)
    return proprietary_file
