import socket
import pygame
import time
import sys
import cv2
from pygame.locals import *
from threading import Lock
from tie_mul.TieProcess import *
from tie_mul.TieThread import *

from common import *
from ui.admin import *

class Client():
	
	def __init__(self, host, port) -> None:
		self.threads = ThreadRoster(debug=DEBUG_THREAD)
		self.__q_send = Queue()
		self.__q_recv = Queue()
		self.sock = None
		self.host = host
		self.port = port
		self.alive = False
	
	def start(self):
		#Create
		try:
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			print("Socket create")
		except: 
			print("Error create socket")
			return False
		#Connect
		try:
			self.sock.settimeout(2)
			self.sock.connect( (self.host, self.port) )
			print(f"Socket connected to {self.host}:{self.port}")
		except socket.error as msg: 
			print(f"Error connect to {(self.host, self.port)}")
			return False

		self.alive = True
		#Create thread transmit
		self.threads.create( self.__thread_recv, "T-recv" )
		self.threads.create( self.__thread_send, "T-send" )
		return True

	def close(self):
		self.alive = False
		if self.sock: self.sock.close()
		self.threads.close_all()

	def io_get(self):
		try: return self.__q_recv.get(timeout=0.5)
		except: return None
	def io_put(self, data):
		try: self.__q_send.put( data )
		except: return

	def __thread_recv(self, this):
		while this['loop']:
			if not self.alive: break
			#waiting recv
			try:
				self.sock.settimeout(0.5)
				data = self.sock.recv(BUFFER_RECV)
				if FAKE_LAG: time.sleep(0.02)
			except socket.error as msg: 
				if not isinstance(msg, TimeoutError):
					print("Error receive from host")
					break
				else: continue

			if not data: 
				print("Error receive from host")
				self.alive = False
				break
			
			#put to queue recieve
			self.__q_recv.put( data )

	def __thread_send(self, this):
		while this['loop']:
			if not self.alive: break
			try: data = self.__q_send.get(timeout=0.5)
			except: continue
			try:
				if FAKE_LAG: time.sleep(0.02)
				self.sock.sendall( data )
			except socket.error as msg: 
				print("Error send to host")
				self.alive = False
				break


class App():

	def __init__(self) -> None:
		pygame.init()
		pygame.display.set_caption("Admin")
		self.w_icon = pygame.image.load('ui/icon.png')
		pygame.display.set_icon(self.w_icon)
		self.screen = pygame.display.set_mode((WIDTH, HEIGHT), 0, 32)
		self.display = pygame.Surface((int(WIDTH/SCALE), int(HEIGHT/SCALE)))
		self.clock = pygame.time.Clock()
		self.dtime = 0

		self.host, self.port = self.io_load_ip("ui/ip.txt")
		if (self.host != None and self.port != None):
			self.io = Client(self.host, int(self.port))

		self.threads = ThreadRoster(debug=DEBUG_THREAD)
		self.sock_id = -1
		self.sk_recv = {
			'len': 0, 'type': DT.STR,
			'cont': None,
			'cnt': 0, 'is_new': True
		}
		
		# self.man = {
		# 	'cam': {
		# 		'1': [0,1],
		# 		'2': [3],
		# 	},
		# 	'view': {
		# 		'3': 1,
		# 		'4': 2,
		# 	}
		# }
		self.man = {
			'cam': {},
			'view': {},
			'cam_surf': {},
			'cam_stopped': {}
		}
		self.mtx_store = Lock()

		self.assets = Assets()
		self.ui = UI_ADMIN(self)

	def clear(self):
		self.io.close()
		self.threads.close_all()
		pygame.quit()
		sys.exit()

	def run(self):
		self.threads.create( self.__thread_proc_io, "T-PIO" )
		while True:
			self.clock.tick(FPS)
			self.dtime = self.clock.get_time() * 0.001
			self.mtx_store.acquire()
			self.update()
			self.draw()
			self.active_event()
			self.mtx_store.release()

	def active_event(self):
		for event in pygame.event.get():
			if event.type == QUIT: 
				self.clear()
			
			if event.type == KEYUP:
				if event.key == K_c:
					if not self.io.alive: self.io.start()
					else: 
						self.io.close()
						self.io.start()

				if event.key == K_r:
					self.io_put(tcp_pack(DT.STR, "task-admin"))
				if event.key == K_t:
					self.io_put(tcp_pack(DT.SERIES, {'id':SERIES_ID.DATA, 'd':self.sock_id}))
				if event.key == K_e:
					self.io_put(tcp_pack(DT.CMD, CMD.AD_GET_LIST_CLIENT))
					

			self.ui.process_events(event)


	def update(self):
		self.ui.update(self.dtime)
		for cam, link_views in self.man['cam'].items():
			if cam not in self.man['cam_surf']: continue

			cam_surf = self.man['cam_surf'][cam]
			self.ui.update_cam(cam, cam_surf)
			for view in link_views: self.ui.update_view(view, cam_surf)

	def draw(self):
		self.display.fill((0, 0, 0))
		
		self.ui.draw(self.display)
		
		self.screen.blit(pygame.transform.scale(self.display, (self.screen.get_width(), self.screen.get_height())), (0,0))
		pygame.display.update()
		pass

	def io_load_ip(self, path_file):
		try: 
			ip = open(path_file, 'r')
		except:
			print("Load IP failed")
			return [None,None]
		addr = []
		for l in ip.readlines(): addr.append(l.strip())
		return addr
	def io_get(self):
		return self.io.io_get()
	def io_put(self, out):
		self.io.io_put(out)

	def __thread_proc_io(self, this):
		
		data_recv = b""
		data_recv_empty = True

		while this['loop']:

			#get data with timeout when data receive empty
			if data_recv_empty: data_recv = self.io.io_get()
			if data_recv == None: 
				data_recv_empty = True
				continue
			# print(f"Data pack({len(data_recv)}): {data_recv}")

			# When data is new, data content receive sufficiently
			if self.sk_recv['is_new']:
				
				res, self.sk_recv['len'], self.sk_recv['type'], self.sk_recv['cont'] = tcp_unpack_desc(data_recv)
				# When unpack description failed
				if res == False: 
					data_recv_empty = True
					continue

				self.sk_recv['is_new'] = True
				self.sk_recv['cnt'] = len(self.sk_recv['cont'])

			else:
				self.sk_recv['cont'] += data_recv
				self.sk_recv['cnt'] += len(data_recv)

			data_len = self.sk_recv['len']

			#Not receive sufficiently
			if data_len > self.sk_recv['cnt']:
				self.sk_recv['is_new'] = False
				data_recv_empty = True
				continue

			# Receive more than num of data needed unpack
			elif data_len < self.sk_recv['cnt']:
				data_recv = self.sk_recv['cont'][data_len : ]
				self.sk_recv['cont'] = self.sk_recv['cont'][ : data_len]
				self.sk_recv['is_new'] = True
				data_recv_empty = False
			
			# Receive sufficiently in one shot
			else: data_recv_empty = True

			data_type = self.sk_recv['type']
			passed_unpack, data_content = tcp_unpack_data(data_type, self.sk_recv['cont'])
			if not passed_unpack: continue

			if		data_type == DT.CMD: self.process_data_cmd(data_content)
			elif 	data_type == DT.STR: self.process_data_str(data_content)
			elif 	data_type == DT.SERIES: self.process_data_series(data_content)
			else:	print(f"Invalid data type value: {data_type}")
	



	def process_data_cmd(self, cmd):
		if cmd == CMD.NONE: pass
		
		elif cmd == CMD.AD_ACCEPT:
			print(f"DATA-CMD: server accept as admin")
		pass



	def process_data_str(self, str):

		if   str == 'result register: success':
			return print("Client register success")

		# print(f"DATA-STR: {str}")
		try:
			str = json.loads(str)
			mes, data = str['mes'], str['data']
		except:
			return print(f"DATA-STR: inform data (mes, data)")
		
		self.mtx_store.acquire()

		if mes == "sock_id":
			self.sock_id = data
			print(f"Sock ID: {self.sock_id}")

		if mes == "list_client":
			try:
				data = json.loads(data)
				cam_list, view_list = data['cam'], data['view']
			except:
				return print(f"DATA-STR: inform data (cam, view)")
			
			for cam_sk in self.man['cam'].keys(): self.ui.remove_cam(cam_sk)
			for view_sk in self.man['view'].keys(): self.ui.remove_view(view_sk)
			self.man['cam'] = {}
			self.man['view'] = {}
			self.man['cam_surf'] = {}

			for cam_sk in cam_list:
				self.man['cam'][cam_sk] = []
				self.ui.add_cam(cam_sk)

			for view in view_list:
				view_sk, view_link = view
				if view_link != -1: self.man['cam'][view_link].append(view_sk)
				self.man['view'][view_sk] = view_link
				self.ui.add_view(view_sk)
			# print(f"DATA-STR: update list: {self.man}")

		self.mtx_store.release()
		pass



	def process_data_series(self, data):
		try:
			id, d = data['id'], data['d']
			# print(f"Pickle: id={id}, s_snd={len(d)}")
		except: 
			return print(f"Pickle: inform data (id, d)")
		
		if id == SERIES_ID.DATA:
			print(f"SERIES: {d}")

		if id == SERIES_ID.CAM:
			try:
				sock_id, f = d['sk'], d['f']
				# print(f"Pickle: sock_id={sock_id}, s_f={f}")
			except:
				return print(f"Pickle: inform data (sk, f)")

			frame = cv2.imdecode(f, cv2.IMREAD_COLOR)
			self.man['cam_surf'][int(sock_id)] = pygame.image.frombuffer(frame, (CAM_WIDTH, CAM_HEIGHT), "BGR").convert()


if __name__ == "__main__":
	app = App()
	app.run()
	app.clear()