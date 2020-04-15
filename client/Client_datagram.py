import socket
import sys
# import vlc
import webbrowser
from PIL import Image
from playsound import playsound
import subprocess
# import cv2

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

port_s = 8081

# no need
# port_c = 8082

# changes encoding from character stream to byte stream, ascii -> utf-8
# data = 'Hello from Client'.encode()
# data = b'Hello from Client'

if len(sys.argv) != 2:
    print('No file requested')
    sys.exit()

# no need
# s.bind(('', port_c))
# print('Client started, bound to socket localhost:8082')

s.sendto(sys.argv[1].encode(), ('127.0.0.1', port_s))

data, addr = s.recvfrom(1024)

if b'file does not exist' in data:
    print(data.decode())
    sys.exit()

f = open(data.decode(), 'wb')

data, addr = s.recvfrom(256)
file_size = int(data)
size = 0

try:
    while data:
        data, addr = s.recvfrom(1024)
        size = size + len(data)
        if data:
            f.write(data)
            # s.settimeout(10)
            s.sendto(b'ACK', ('127.0.0.1', port_s))
        elif len(data) == 0 and size == file_size:
            s.sendto(b'END', ('127.0.0.1', port_s))
            break
        elif len(data) == 0:
            s.sendto(b'NACK', ('127.0.0.1', port_s))
except socket.timeout:
    f.close()
    s.close()
    print('Error in dowloading file, transfer not completed')
    sys.exit()

f.close()
print('Reception complete')
print('File downloaded successfully')
s.close()
print('Client socket closed')

if sys.argv[1].find('html') != -1:
    print('Opening webpage in browser')
    # webbrowser.open(sys.argv[1])
    p = subprocess.run(['firefox', sys.argv[1]])
elif sys.argv[1].find('jpeg') != -1:
    print('Opening image')
    '''
    img = Image.open(sys.argv[1])
    img.show()
    '''
    p = subprocess.run(['gwenview', sys.argv[1]])
elif sys.argv[1].find('mp3') != -1:
    '''
    player = vlc.MediaPlayer(sys.argv[1])
    player.play()
    '''
    print('Playing audio')
    p = subprocess.run(['vlc', sys.argv[1]])
    # playsound(sys.argv[1])
elif sys.argv[1].find('mp4') != -1:
    '''
    video = cv2.VideoCapture(sys.argv[1])
    while video.isOpened():
        ret, frame = video.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow('frame', gray)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    video.release()
    cv2.destroyAllWindows()
    '''
    print('Playing video')
    p = subprocess.run(['vlc', sys.argv[1]])
