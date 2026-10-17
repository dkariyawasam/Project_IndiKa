import io
import re
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import Mock
from server import Handler


class PhoneAccessTest(unittest.TestCase):
    def handler(self, host='192.168.1.12:8765', cookie='', origin=None):
        h = Handler.__new__(Handler)
        h.server = SimpleNamespace(server_port=8765, lan_host='192.168.1.12', phone_key='123456789abcdef0')
        h.client_address = ('192.168.1.20', 1234)
        h.headers = {'Host': host, 'Cookie': cookie}
        if origin:
            h.headers['Origin'] = origin
        h.send = Mock()
        return h

    def test_host_and_origin_checks(self):
        self.assertTrue(self.handler().local())
        self.assertFalse(self.handler(host='evil.example:8765').local())
        self.assertFalse(self.handler(origin='http://evil.example').local())
        h = self.handler()
        h.server.lan_host = None
        self.assertFalse(h.local())

    def test_pair_cookie_and_local_access(self):
        self.assertFalse(self.handler().paired())
        self.assertFalse(self.handler(cookie='expedition_phone=wrong').paired())
        self.assertTrue(self.handler(cookie='expedition_phone=123456789abcdef0').paired())
        h = self.handler(host='127.0.0.1:8765')
        h.client_address = ('127.0.0.1',1234)
        self.assertTrue(h.paired())

    def test_unpaired_requests_cannot_read_state_or_code(self):
        for path in ['/api/bootstrap', '/api/state', '/screen.png', '/app.mjs']:
            h = self.handler()
            h.path = path
            h.do_GET()
            self.assertEqual(h.send.call_args.args[0],401)

    def test_pairing_sets_cookie_and_redirects(self):
        h = self.handler()
        h.path = '/pair'
        body = b'code=+++123456789ABCDEF0+++'
        h.headers['Content-Length'] = str(len(body))
        h.rfile = io.BytesIO(body)
        h.send_response = Mock(); h.send_header = Mock(); h.end_headers = Mock()
        h.do_POST()
        h.send_response.assert_called_once_with(303)
        headers = dict(call.args for call in h.send_header.call_args_list)
        self.assertEqual(headers['Location'],'/')
        self.assertIn('HttpOnly; SameSite=Strict',headers['Set-Cookie'])

    def test_all_imported_modules_are_served(self):
        static = Path(__file__).parent/'static'
        for name in re.findall(r"from './([^']+)'", (static/'app.mjs').read_text()):
            h = self.handler(cookie='expedition_phone=123456789abcdef0')
            h.path = '/'+name
            h.do_GET()
            self.assertEqual(h.send.call_args.args[0],200,name)
            self.assertIn('javascript',h.send.call_args.args[2],name)

if __name__ == '__main__':
    unittest.main()
