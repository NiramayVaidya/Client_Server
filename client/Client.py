import socket
import sys
import subprocess

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # s = socket.socket()

    port = 12345

    if len(sys.argv) != 2:
        print('Request file not provided')
        sys.exit()

    s.connect(('127.0.0.1', port))

    s.sendall(sys.argv[1].encode('utf-8'))

    # print(s.recv(4096).decode('ascii'))

    f = open(sys.argv[1], 'w')
    f.write(s.recv(4096).decode('ascii'))
    f.close()
    if sys.argv[1].find('txt') != -1:
        p = subprocess.run(['kate', sys.argv[1]])

    s.close()

if __name__ == '__main__':
    main()
