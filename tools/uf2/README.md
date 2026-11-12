# UF2 converter

`uf2conv.py` is from Microsoft's [UF2 tools](https://github.com/microsoft/uf2/tree/84444ddb9d2914edf7f6d9e89a7ce41b53d64d98),
commit `84444ddb9d2914edf7f6d9e89a7ce41b53d64d98`. See `LICENSE.txt`.

The platform provides optional UF2 conversion and upload recipes. Existing
boards keep their current output formats and upload tools. Python 3 must be
available as `python` on Windows or `python3` on other hosts.

## Board configuration

A board opts in through its `boards.txt` properties. For example, a future
CH32V4x7 board with a 32 KiB TinyUF2 reservation would use:

```ini
example.build.uf2_family=0x7b8c32a7
example.build.flash_offset=0x8000
example.recipe.objcopy.uf2.pattern={tools.uf2.convert.pattern}
example.build.preferred_out_format=uf2
example.upload.tool=uf2
example.upload.tool.default=uf2
```

If a board has an upload-method menu, select `uf2` in that menu entry's
`upload.tool` property as well; menu properties override board defaults.

The family ID must match the installed bootloader. The application must also be
linked at the specified flash offset, with its maximum size reduced to reserve
the bootloader; conversion does not relocate executable code. These recipes
install neither board definitions nor a bootloader.

## Export and upload

For an opted-in board, compiling produces `.uf2` alongside `.bin` and `.hex`.
The `build.preferred_out_format` setting selects UF2 for Export Compiled Binary.
To upload, enter the board's bootloader and pass its mounted volume path as the
port, for example:

```sh
arduino-cli upload --fqbn <package>:ch32v:<board> --port D: --input-dir <build-output>
```

On Linux or macOS, use the volume's mount path instead of `D:`. Select the
intended board's volume explicitly; automatic serial-port-to-volume discovery
and reset are not provided. The upload wrapper requires a mounted volume with
an `INFO_UF2.TXT` file before writing `NEW.UF2` there. Missing drives, ordinary
directories, and stale mount directories are rejected. On Windows, a bare
drive letter such as `D:` selects the drive's root.

Both recipes use conversion-only mode with an explicit output path, avoiding
the converter's automatic deployment to every UF2 drive. The vendor converter
remains unmodified; volume validation is implemented separately in `upload.py`.
