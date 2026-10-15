import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch,Mock
from emulator import launch

class EmulatorTests(unittest.TestCase):
    def test_launch_capabilities_and_isolated_save(self):
        for automatic in (False,True):
            with tempfile.TemporaryDirectory(prefix='game test ') as tmp:
                directory=Path(tmp);bootstrap=directory/'connect.lua'
                with patch('emulator.detect',return_value=dict(executable='/test/mgba',automatic=automatic)),patch('emulator.subprocess.Popen',return_value=Mock()) as popen:
                    process,result=launch(directory,bootstrap)
                    command=popen.call_args.args[0]
                    self.assertEqual('--script' in command,automatic)
                    self.assertIn('savegamePath='+tmp,command)
                    self.assertEqual(command[-1],str(directory/'expedition-debug.gba'))
                    self.assertEqual(result['automatic'],automatic)
                    self.assertNotIn('shell',popen.call_args.kwargs)
