import socket
import select
import time
import os
import signal
from enum import Enum

from pythonosc import osc_message_builder
from pythonosc import udp_client


OSC_SERVER = '10.0.0.51'
OSC_PORT = 3032

class NetworkState(Enum):
    NOT_STARTED = 0
    CONNECTING = 1
    CONNECTED = 2
    ERROR = 3
    DISCONNECTED = 4
    STOPPING = 5
    STOPPED = 6


class TcpClient():
    RX_MSG_TIMEOUT = 3
    TX_MSG_PERIOD = 1

    _status = NetworkState.NOT_STARTED
    _sock = None
    _buffer = ''
    _last_msg_time = time.time()
    _last_tx_time = time.time()
    _callback_function = None

    def __init__(self):
        self._sock = None

    def connect(self):
        try:
            self._status = NetworkState.CONNECTING
            self._sock = socket.create_connection((OSC_SERVER, OSC_PORT), timeout=5)
            self._sock.setblocking(False)
            self.reset_timeout_wdg()
            print("Connected")
            self._status = NetworkState.CONNECTED
        except TimeoutError:
            self._status = NetworkState.DISCONNECTED
            print("Could not connect: Timeout error.")
            if self._sock != None:
                self._sock.close()
        except ConnectionRefusedError:
            self._status = NetworkState.DISCONNECTED
            print("Could not connect: Connection refused.")
            if self._sock != None:
                self._sock.close()
        except ConnectionError:
            self._status = NetworkState.DISCONNECTED
            print("Could not connect: Connection error.")
            if self._sock != None:
                self._sock.close()
        except OSError:
            self._status = NetworkState.DISCONNECTED
            print("Could not connect: OS Error.")
        
    def recover_from_error(self):
        if self._sock != None:
            try:
                self._sock.shutdown(socket.SHUT_RDWR)
            except ConnectionError:
                pass
            except OSError:
                pass
            try:
                self._sock.close()
            except ConnectionError:
                pass
            except OSError:
                pass
            self._sock = None
        self._status = NetworkState.DISCONNECTED

    def start(self):
        if (self._status == NetworkState.NOT_STARTED or
        self._status == NetworkState.STOPPING or
        self._status == NetworkState.STOPPED):
            self.connect()
        else:
            raise RuntimeError("Already started")

    def stop(self):
        if(self._status == NetworkState.CONNECTING or 
        self._status == NetworkState.CONNECTED):
            self._status = NetworkState.STOPPING
            try:
                self._sock.shutdown(socket.SHUT_RDWR)
            except ConnectionError:
                pass
            except OSError:
                pass
            self._sock = None
        self._status = NetworkState.STOPPED

    def in_connected(self):
        if(self._status == NetworkState.CONNECTED):
            ready_to_read, ready_to_write, errors = select.select([self._sock], [self._sock], [], 60)
            if(ready_to_write):
                return True
        return False

    def check_for_timeouts(self):
        if self._last_msg_time + self.RX_MSG_TIMEOUT < time.time():
            # Timeout on received messages.
            self._status = NetworkState.ERROR
            print("Timeout")

    def reset_timeout_wdg(self):
        self._last_msg_time = time.time()

    def loop(self):
        if self._status == NetworkState.DISCONNECTED:
            self.connect()
        if self._status == NetworkState.ERROR:
            self.recover_from_error()
        if self._status == NetworkState.CONNECTED:
            self.check_for_timeouts()
            self.read()
            self.keepAlive()

    def read(self):
        try:
            ready_to_read, ready_to_write, errors = select.select([self._sock], [self._sock], [], 5)
            if ready_to_read:
                self.read_lines_from_buffer()
        except ConnectionResetError:
            print("Connection reset")
            self._status = NetworkState.ERROR
        except ConnectionRefusedError:
            print("Connection refused")
            self._status = NetworkState.ERROR
        except ConnectionError:
            print("Connection error")
            self._status = NetworkState.ERROR
        except BlockingIOError:
            print("Blocking error")
            self._status = NetworkState.ERROR

    def read_lines_from_buffer(self):
        try:
            data = (self._sock.recv(4096)).decode("latin")
        except:
            data = "Issue decoding incoming buffer"
        self._buffer += data
        while self._buffer.find('\n') != -1:
            line, self._buffer = self._buffer.split('\n', 1)
            self.reset_timeout_wdg()
            if(self._callback_function != None):
                self._callback_function(line)

    def set_receive_callback(self, callback_function):
        self._callback_function = callback_function

    def clear_receive_callback(self):
        self._callback_function = None

    def keepAlive(self):
        if self._last_tx_time + self.TX_MSG_PERIOD < time.time():
            self.send('/eos/ping\n'.encode("UTF-8"))
            self._list_tx_time = time.time()

    def send(self, msg):
        try:
            ready_to_read, ready_to_write, errors = select.select([self._sock], [self._sock], [], 5)
            if ready_to_write:
                self._sock.send(msg)
        except ConnectionResetError:
            print("Connection reset")
            self._status = NetworkState.ERROR
        except ConnectionRefusedError:
            print("Connection refused")
            self._status = NetworkState.ERROR
        except ConnectionAbortedError:
            print("Connection aborted")
            self._status = NetworkState.ERROR
        except ConnectionError:
            print("Connection error")
            self._status = NetworkState.ERROR
        except OSError:
            print("OS Error")
            self._status = NetworkState.ERROR


class OscApp():
    def __init__(self):
        pass

    def process_incoming(self, str_data):
        try:
            print("INCOMING DATA: ")
            print(str_data)
        except:
            print("Issue parsing data.")


class GracefulKiller:
    kill_now = False
    def __init__(self):
        signal.signal(signal.SIGINT, self.exit_gracefully)
        signal.signal(signal.SIGTERM, self.exit_gracefully)

    def exit_gracefully(self, signum, frame):
        self.kill_now = True

if __name__ == "__main__":

    print("Start")

    cancel = False
    killer = GracefulKiller()

    client = udp_client.SimpleUDPClient("10.0.0.52", 4703)


    for x in range(10):
        client.send_message("/eos/wheel/course/zoom", 10)
        time.sleep(1)

    print("End")