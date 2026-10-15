"""Launch only the dashboard's isolated ROM, using detected mGBA capabilities."""
import os
from pathlib import Path
import shutil
import subprocess


def detect():
    candidates = [os.environ.get('KANTO_MGBA'), shutil.which('mgba-qt'), shutil.which('mgba'), '/Applications/mGBA.app/Contents/MacOS/mGBA']
    for candidate in candidates:
        if candidate and Path(candidate).is_file():
            result = subprocess.run([candidate, '--help'], capture_output=True, text=True, timeout=10)
            return dict(executable=candidate, automatic='--script' in result.stdout+result.stderr)
    raise ValueError('mGBA was not found. Set KANTO_MGBA to its executable path and restart the dashboard.')


def launch(directory, bootstrap):
    info = detect()
    command = [info['executable'], '-C', 'pauseOnFocusLost=0', '-C', 'autoload=0', '-C', 'savegamePath='+str(directory)]
    if info['automatic']: command += ['--script', str(bootstrap)]
    command.append(str(directory/'expedition-debug.gba'))
    with (directory/'emulator.log').open('ab') as log:
        process = subprocess.Popen(command, stdout=log, stderr=log, start_new_session=True)
    return process, dict(automatic=info['automatic'], message=('mGBA launched with the bridge. Waiting for its heartbeat.' if info['automatic'] else 'mGBA launched. This version needs Tools → Scripting: paste the connection command and Run once. Dashboard restarts will reuse this connection.'))
