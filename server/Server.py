import socket
import sys

def main():
    '''
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print('Socket succesfully created')
    except socket.error as err:
        print('Socket creation failed with error {}'.format(err))

    port = 80

    try:
        host_ip = socket.gethostbyname('www.google.com')
    except socket.gaierror: # getaddinfo()
        print('There was an error resolving the host')
        sys.exit()

    s.connect((host_ip, port))

    print('The socket has successfully connected to Google on ip {} and port {}'.format(host_ip, port))
    '''

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # s = socket.socket()
    print('Socket successfully created')

    port = 12345

    s.bind(('', port))
    print('Socket bound to {}'.format(port))

    s.listen(5)
    print('Socket is listening')

    while True:
        try:
            c, addr = s.accept()
            print('Got connection from {}'.format(addr))
            try:
                f = open(c.recv(16).decode('ascii'), 'r')
                data = f.read().strip()
                f.close()
            except FileNotFoundError:
                data = 'Error: The requested file does not exist'
            c.sendall(data.encode('utf-8'))
            # c.send(b'Thank you for connecting.\n')
            # c.sendall('Thank you for connecting.\n') 
            c.close()
        except KeyboardInterrupt:
            sys.exit()

    # telnet localhost 12345

if __name__ == '__main__':
    main()
