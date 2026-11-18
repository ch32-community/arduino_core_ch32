"""Regression tests for upload volume validation; no hardware is written."""

from contextlib import redirect_stderr
import io
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

import upload


class UploadTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='uf2 upload test ')
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.firmware = self.root / 'test firmware.bin'
        self.firmware.write_bytes(bytes(range(256)))
        self.volume = self.root / 'selected volume'
        self.volume.mkdir()

    def arguments(self, volume=None):
        return ['--volume', str(volume or self.volume), '--family', '0x12345678',
                '--base', '0x4000', str(self.firmware)]

    def assert_rejected(self, volume):
        with patch.object(upload.subprocess, 'run') as converter:
            with redirect_stderr(io.StringIO()), self.assertRaises(SystemExit) as result:
                upload.main(self.arguments(volume))
            self.assertNotEqual(result.exception.code, 0)
            converter.assert_not_called()
        self.assertFalse((self.volume / 'NEW.UF2').exists())

    def test_missing_volume(self):
        self.assert_rejected(self.root / 'missing')

    def test_plain_directory(self):
        self.assert_rejected(self.volume)

    def test_unmounted_directory_with_marker(self):
        (self.volume / 'INFO_UF2.TXT').write_text('UF2 Bootloader\n')
        self.assert_rejected(self.volume)

    def test_mount_without_marker(self):
        with patch.object(upload.os.path, 'ismount', return_value=True):
            self.assert_rejected(self.volume)

    def test_marker_must_be_a_file(self):
        (self.volume / 'INFO_UF2.TXT').mkdir()
        with patch.object(upload.os.path, 'ismount', return_value=True):
            self.assert_rejected(self.volume)

    def test_upload_uses_only_selected_volume(self):
        other = self.root / 'other volume'
        other.mkdir()
        for volume in (self.volume, other):
            (volume / 'INFO_UF2.TXT').write_text('UF2 Bootloader\n')
        with patch.object(upload.os.path, 'ismount', return_value=True):
            self.assertEqual(upload.main(self.arguments()), 0)
        self.assertFalse((other / 'NEW.UF2').exists())
        result = (self.volume / 'NEW.UF2').read_bytes()
        self.assertEqual(len(result), 512)
        header = struct.unpack('<8I', result[:32])
        self.assertEqual(header, (0x0a324655, 0x9e5d5157, 0x2000, 0x4000,
                                  256, 0, 1, 0x12345678))
        self.assertEqual(result[32:288], self.firmware.read_bytes())

    def test_converter_failure_is_propagated(self):
        (self.volume / 'INFO_UF2.TXT').write_text('UF2 Bootloader\n')
        with patch.object(upload.os.path, 'ismount', return_value=True):
            with patch.object(upload.subprocess, 'run') as converter:
                converter.return_value.returncode = 7
                self.assertEqual(upload.main(self.arguments()), 7)


if __name__ == '__main__':
    unittest.main()
