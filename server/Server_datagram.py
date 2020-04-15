import socket
import sys

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

port_s = 8081
# no need
# port_c = 8082

s.bind(('', port_s))
print('Server started, bound to socket localhost:8081')

while True:
    try:
        data, addr = s.recvfrom(1024)
    
        try:
            f = open(data.strip(), 'rb')
        except FileNotFoundError:
            # no need
            # s.sendto(b'Requested file does not exist', ('127.0.0.1', port_c))
            # addr is itself a tuple consisting of sender ip address and port
            # addr = ('ip address', port)
            s.sendto(b'Requested file does not exist', addr)
            continue

        s.sendto(data, addr)

        file_size = len(f.read())
        f.seek(0)
        s.sendto(str(file_size).encode(), addr)

        try:
            while data:
                data = f.read(1024)
                s.sendto(data, addr)
                '''
                this timeout is required for flow control from server to client
                Since there is a timeout on the client side between receiving 2 UDP
                packets, there has to be one on the server side too between sending two
                UDP packets, otherwise the packets sent by the server overwhelm the
                client and since there is not stopping mechanism at the client, this new
                packet overwrites part of the old one while writing to the file
                observe that timeout is less than that at the client (half)
                parameter is set in seconds
                '''
                # s.settimeout(5)
                rec, addr = s.recvfrom(256)
                if rec.find(b'NACK') != -1:
                    print('NACK received, resend previous datagram')
                    s.sendto(data, addr)
                elif rec.find(b'ACK') != -1:
                    print('ACK received')
                    continue
                elif rec.find(b'END') != -1:
                    print('Transmission complete')
                    break
                else:
                    print('Invalid response')
                    raise socket.timeout
        except socket.timeout:
            f.close()
            print('Error in sending file, transfer not completed')
            continue

        f.close()
        print('File sent successfully')
    except KeyboardInterrupt:
        s.close()
        print('Server socket closed')
        sys.exit()

