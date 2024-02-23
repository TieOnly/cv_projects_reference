import socket
import time
import json
from convert.pbase64 import *
import cv2
import base64

def try_create_sk(addr, timing=1, count_try=None):
	is_connect = False
	while not is_connect:
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			is_connect = True
			sock.settimeout(timing)
			sock.connect(addr)
			sock.settimeout(None)
		except:
			is_connect = False
			print(f"Not connected to {addr[0]}:{addr[1]}, try again...")
		if is_connect:
			print(f"Connected to {addr[0]}:{addr[1]}")
			return sock
		if count_try != None: 
			count_try -= 1
			if count_try == 0: 
				print("End of trying connect")
				return None
		time.sleep(timing)

def make_list_data(path):
	f = open(path, 'r')
	video = json.load(f)
	print(video['f_amount'], video['f_width'], video['f_height'])
	print(len(video['data'][0]))
	return video

def make_data_img(surf, w, h):
	data_str = []
	for y in range(h):
		seg8 = 0x00
		i = 0x80
		for x in range(w):
			if surf[y, x] == 0:
				seg8 |= i
			i >>= 1
			if i == 0x00:
				data_str.append(seg8)
				seg8 = 0x00
				i = 0x80
		if i != 0x80: data_str.append(seg8)
	return base64.b64encode(bytes(data_str)).decode()

def stream_video(list_tcpip, port_def):
	###init
	current_host = {
		'index': 0,
		'ip': list_tcpip[0],
	}
	client = None
	while current_host['index'] < len(list_tcpip):
		current_host['ip'] = list_tcpip[current_host['index']]
		client = try_create_sk((current_host['ip'], port_def))
		current_host['index'] += 1
	if client == None: 
		print("Can not connect to any tcpip neighbor")

	message = 'task_src'
	video = make_list_data("utils/video.json")
	data = None
	cnt_idx = 0

	###loop
	while True:

		try:
			client.sendto(message.encode(),(current_host['ip'], port_def))
		except:
			print("Not send")
			message = 'task_src'
			client = try_create_sk((current_host['ip'], port_def), count_try=5)
		
		try:
			client.settimeout(2)
			response = client.recv(1024)
			client.settimeout(None)

			data = response.decode()
		except:
			print("Not recv")
			message = 'task_src'
			current_host['index'] = (current_host['index']+1) % len(list_tcpip)
			current_host['ip'] = list_tcpip[current_host['index']]
			client = try_create_sk((current_host['ip'], port_def), count_try=5)

		if data == 'none':
			payload = { 'msg': 'station_src' }
			message = json.dumps(payload)
			print(f"Recv: {data}, len: {len(data)}")
			time.sleep(1)
			continue

		if data == 'conf_frame':
			payload = {
				'msg': 'conf_frame',
				'aniDura': video['f_dura'],
				'aniW': video['f_width'],
				'aniH': video['f_height']
			}
			message = json.dumps(payload)
			print(f"Recv: {data}, len: {len(data)}")
			time.sleep(1)
			continue

		if data == 'cL':
			payload = {
				'msg': 'cLoad',
				'data': video['data'][cnt_idx]
			}
			cnt_idx = (cnt_idx+1) % video['f_amount']
			message = json.dumps(payload)
			continue

		if data == 'sL':
			payload = { 'msg': 'sLoad' }
			message = json.dumps(payload)
			print(f"Recv: {data}, len: {len(data)}, time: {time.time()}")
			time.sleep(0.1)
			continue
			
		if data == 'not_access':
			print("Not accessable into AP")
			sys.exit()

def stream_webcam(list_tcpip, port_def):
	###init
	current_host = {
		'index': 0,
		'ip': list_tcpip[0],
	}
	client = None
	while current_host['index'] < len(list_tcpip):
		current_host['ip'] = list_tcpip[current_host['index']]
		client = try_create_sk((current_host['ip'], port_def))
		if client != None: break
		current_host['index'] += 1
	if current_host['index'] == len(list_tcpip): 
		print("Can not connect to any tcpip neighbor")

	vid = cv2.VideoCapture(0)
	data = None
	last = old = 0
	message = 'task_src'

	while True:

		ret, frame = vid.read()
		frame = cv2.resize(frame, (84, 48))
		frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		(thresh, blackAndWhiteImage) = cv2.threshold(frame, 127, 255, cv2.THRESH_BINARY)
		
		vid_frame_data = make_data_img(blackAndWhiteImage, 84, 48)	
		cv2.imshow("tie_img", blackAndWhiteImage)
		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

		try:
			client.sendto(message.encode(),(current_host['ip'], port_def))
		except:
			print("Not send")
			message = 'task_src'
			client = try_create_sk((current_host['ip'], port_def), count_try=5)
		
		try:
			client.settimeout(2)
			response = client.recv(1024)
			client.settimeout(None)

			data = response.decode()
		except:
			print("Not recv")
			message = 'task_src'
			client = try_create_sk((current_host['ip'], port_def), count_try=5)

		if data == 'none':
			payload = { 'msg': 'station_src' }
			message = json.dumps(payload)
			print(f"Recv: {data}, len: {len(data)}")
			time.sleep(1)
			continue

		if data == 'conf_frame':
			payload = {
				'msg': 'conf_frame',
				'aniDura': 20,
				'aniW': 84,
				'aniH': 48
			}
			message = json.dumps(payload)
			print(f"Recv: {data}, len: {len(data)}")
			time.sleep(1)
			continue

		if data == 'cL':
			payload = {
				'msg': 'cLoad',
				'data': vid_frame_data
			}
			message = json.dumps(payload)
			last = time.time()
			print(f"time: {(last - old) * 1000}")
			old = last
			continue

		if data == 'sL':
			payload = { 'msg': 'sLoad', }
			message = json.dumps(payload)
			print(f"Recv: {data}, len: {len(data)}, time: {time.time()}")
			time.sleep(0.01)
			continue

		if data == 'not_access':
			print("Not accessable into AP")
			sys.exit()

from utils.scan_tcpip import *
import sys

port = 6123
argv_len = len(sys.argv)
if argv_len < 2:
	print('''Please select option:
-webcam : Stream video from Webcam
-video  : Stream video from /utils/video.json
[n]     : ID of TCP server (not include bracket [], always is second cmd)''')
else:
	tcp_ip_neighbor = None
	if argv_len == 2:
		tcp_ip_neighbor = scan_tcpip_main(port)
	else:
		tcp_ip_neighbor = [sys.argv[2]]

	print( sys.argv[1] )

	if   sys.argv[1] == 'webcam':
		stream_webcam(tcp_ip_neighbor, port)
	elif sys.argv[1] == 'video':
		stream_video(tcp_ip_neighbor, port)
	else:
		print(f"No support option '{sys.argv[1]}'")

	sys.exit()

# list_tcp_ip_neighbor = scan_tcpip_main(port)
# if len(list_tcp_ip_neighbor) == 0:
# 	print("No exist available neighbor available tcpip")
# else:
# 	if len(sys.argv) < 2:
# 		stream_video(list_tcp_ip_neighbor, port)
# 		sys.exit()
# 	if sys.argv[1] == 'webcam':
# 		stream_webcam(list_tcp_ip_neighbor, port)
# 	else:
# 		print(f"No support option '{sys.argv[1]}'")
