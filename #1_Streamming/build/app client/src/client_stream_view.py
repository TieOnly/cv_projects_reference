import time
import pygame, sys
from pygame.locals import *
from tie_mul.TieProcess import *
from tie_mul.TieThread import *

import socket
import json
from convert.pbase64 import *
import base64

class ClientSock():
	def __init__(self, list_tcpip = [], port = 1000 ) -> None:
		self.list_tcpip = list_tcpip
		self.port = port
		self.client = None
		self.cur_host = {'index': 0, 'ip': None}
		self.is_connect = False
		self.msg_register = 'none'
		self.message = None
		self.data = None

	def try_create_sk(self, addr, timing=1, count_try=None):
		self.is_connect = False
		while not self.is_connect:
			sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			try:
				self.is_connect = True
				sock.settimeout(timing)
				sock.connect(addr)
				sock.settimeout(None)
			except:
				self.is_connect = False
				print(f"Not connected to {addr[0]}:{addr[1]}, try again...")
			if self.is_connect:
				print(f"Connected to {addr[0]}:{addr[1]}")
				return sock
			if count_try != None: 
				count_try -= 1
				if count_try == 0: 
					print("End of trying connect")
					return None
			time.sleep(timing)

	def init_connect(self, message):
		self.msg_register = message
		self.message = message

		while self.cur_host['index'] < len(self.list_tcpip):
			self.cur_host['ip'] = self.list_tcpip[self.cur_host['index']]
			self.client = self.try_create_sk((self.cur_host['ip'], self.port), count_try=3)
			if( self.client != None ): return
			self.cur_host['index'] += 1

		if self.client == None: print("Can not connect to any tcpip neighbor")

	def try_transmit_data(self):
		try:
			self.client.sendto(self.message.encode(),(self.cur_host['ip'], self.port))
		except:
			print("Not send")
			self.message = self.msg_register
			self.client = self.try_create_sk((self.cur_host['ip'], self.port), count_try=3)
			if (self.client == None): return False
		
		try:
			self.client.settimeout(2)
			response = self.client.recv(1024)
			self.client.settimeout(None)
			self.data = response.decode()
		except:
			print("Not recv")
			self.message = self.msg_register
			self.cur_host['index'] = (self.cur_host['index']+1) % len(self.list_tcpip)
			self.cur_host['ip'] = self.list_tcpip[self.cur_host['index']]
			self.client = self.try_create_sk((self.cur_host['ip'], self.port), count_try=3)
			if (self.client == None): return False
		return True
	pass

def task_client_sock(this):

	def queue_rep_put(q_msg = PCM_DATA, q_data = None):
		if ( this['queue_rep'].empty ):
			this['queue_rep'].put( {'msg': q_msg, 'data': q_data} )
		else:
			print("Queue not empty")

	try:
		list_tcpip, port = this['arg']
		print(f"\nSuccess create task client sock: {list_tcpip} | {port}")
	except: 
		return print("\nFailed create task client sock")

	client = ClientSock(list_tcpip, port)
	client.init_connect('task_view')

	queue, payload = (None, "!")

	while True:
		loopable, queue = process_queue(this['queue'], this['queue_rep'])
		if (queue != None): print(f"Get queue data: {queue}")
		if (not loopable): return

def thread_client_sock(this, arg):

	list_tcpip, port = (['192.168.4.1'], 6123)
	client = ClientSock(list_tcpip, port)
	client.init_connect('task_view')

	while this['loop']:
		if (not client.try_transmit_data()): continue

		data = client.data
		
		if data == 'none':
			client.message = 'station_view'
			print(f"Register station view successed")
			time.sleep(0.5)
			continue
		if data == 'empty':
			arg['is_new'] = False
			client.message = "!"
			time.sleep(1)
			continue
		
		try:
			data = json.loads(data)
		except:
			client.message = "!"
			time.sleep(1)
			continue

		msg = None
		try: 
			msg = data['msg']
		except:
			print(f"Json have not 'msg' field")
			client.message = "!"
			continue

		if msg == 'conf_frame':
			try:
				print(f"Config frame: ani_dura: {data['aniDura']}ms, w: {data['aniW']}, h: {data['aniH']}")
			except:
				print(f"Config frame: error")
		
			client.message = "cL"
			continue

		if msg == 'cLoad':
			try: 
				dec = base64.b64decode(data['data'])
				arg['is_new'] = True
				arg['data'] = dec
			except: pass
			client.message = "cL"
			continue

		client.message = "!"

class Board():
	def __init__(self, w, h) -> None:
		self.w = w
		self.h = h
		self.board = []
		for i in range(0, w*h): self.board.append(0)

	def set(self, x, y, c):
		self.board[x + y*self.w] = c

	def clear(self):
		for i in range(0, self.w*self.h): self.board[i] = (0, 0, 0)

	def bmp(self, data):
		self.clear()
		byteWidth = int((self.w + 7) / 8)
		b = 0
		for j in range(0, self.h):
			for i in range(0, self.w):
				if i&7: b <<= 1
				else:
					b = data[j*byteWidth+ int(i/8)]
				if b&0x80:
					self.set(i, j, (200, 100, 10))

if __name__ =="__main__":
	pygame.init()
	pygame.display.set_caption("Client View")
	screen = pygame.display.set_mode((500, 300), 0, 32)
	clock = pygame.time.Clock()
	procs = ProcRoster()
	threads = ThreadRoster()
	board = Board(84, 48)

	print("main")
	frame = {'is_new': False, 'data': None}
	threads.create(thread_client_sock, 'thread_sock', frame)

	while True:
		for event in pygame.event.get():
			if event.type == QUIT:
				procs.close_all()
				threads.close_all()
				pygame.quit()
				sys.exit()
			if event.type == KEYUP:
				if event.key == K_x:
					threads.close(name='thread_sock')
				if event.key == K_z:
					procs.queue_put(proc_name='task_sock', q_msg=PCM_DATA, q_data="Hello")
		
		datas_queue = procs.queue_process_reply()
		for data in datas_queue: print(f"Data from {data['task']}: {data['data']}")

		if frame['is_new']: board.bmp(frame['data'])

		screen.fill((100, 200, 200))
		
		for i in range(0, board.w*board.h):
			pos = ((i%board.w), (i/board.w))
			pygame.draw.rect(screen, board.board[i], pygame.Rect(int(pos[0])*(4+1), int(pos[1])*(4+1), 4, 4))

		pygame.display.update()
		clock.tick(60)