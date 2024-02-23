import socket
import pygame
import time
import sys
from pygame.locals import *
import copy
from threading import Lock
from tie_mul.TieProcess import *
from tie_mul.TieThread import *

from common import *
from settings import *
from map import *
from player import *

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
				data = self.sock.recv(10000)
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
			self.__q_recv.put( data.decode() )

	def __thread_send(self, this):
		while this['loop']:
			if not self.alive: break
			try: data = self.__q_send.get(timeout=0.5)
			except: continue
			try:
				if FAKE_LAG: time.sleep(0.02)
				# print(data)
				self.sock.sendall( data.encode() )
			except socket.error as msg: 
				print("Error send to host")
				self.alive = False
				break



class Game():

	def __init__(self) -> None:
		pygame.display.set_caption("Game")
		self.screen = pygame.display.set_mode((WIDTH, HEIGHT), 0, 32)
		self.display = pygame.Surface((int(WIDTH/SCALE), int(HEIGHT/SCALE)))
		self.clock = pygame.time.Clock()
		self.dtime = 1
		self.io = Client(HOST, PORT)
		self.threads = ThreadRoster(debug=DEBUG_THREAD)
		self.gl_pid = "temp"

		self.map_tiles = TileMap()
		self.cam = Camera((WIDTH, HEIGHT))

		self.mxt_game = Lock()
		self.players = PLAYERS(self)


	def clear(self):
		self.io.close()
		self.threads.close_all()
		pygame.quit()
		sys.exit()

	def run(self):
		self.threads.create( self.__thread_proc_comming_mes, "T-PIO" )
		while True:
			self.clock.tick(FPS)
			self.dtime = self.clock.get_time() * 0.001
			self.mxt_game.acquire()
			self.update()
			self.draw()
			self.active_event()
			self.io_start()
			self.mxt_game.release()
	
	def active_event(self):
		for event in pygame.event.get():
			if event.type == QUIT: 
				self.clear()
			
			self.players.active_event(event)
			
			if event.type == KEYUP:
				if event.key == K_c and not self.io.alive: 
					self.io.start()

	def update(self):
		self.players.update_from_tick(self.dtime)
		self.cam.update(self.players.me_obj().center())

	def draw(self):
		self.display.fill((10, 10, 200))
		
		self.map_tiles.draw(self.display, self.cam.scroll)
		self.players.draw(self.display, self.cam.scroll)

		self.screen.blit(pygame.transform.scale(self.display, (self.screen.get_width(), self.screen.get_height())), (0,0))
		pygame.display.update()
		pass

	def io_start(self):
		self.players.io_start()

	def io_get(self):
		return self.io.io_get()
	def io_put(self, out):
		out['pid'] = self.gl_pid
		data = out_to_data(out)
		self.io.io_put(data)


	def __thread_proc_comming_mes(self, this):
	
		while this['loop']:
	
			#get data with timeout
			data = self.io.io_get()
			if data == None: continue
			
			form = in_to_form(data)
			form_id, payload = in_form(form)
			if form_id == None or payload == None: 
				#receive data overflow --> need state others
				self.io_put(out_mes(CMD.STATE_OTHERS, 0))
				continue

			self.mxt_game.acquire()
			if   form_id == CMD.FORM_MES:
				# print(f'mes - {payload}')
				self.proc_comming_mes(payload)

			elif form_id == CMD.FORM_STREAM:
				# print(f'stream - {payload}')
				stype, pid, stream = in_stream(payload)
				if stype == None: return

				if   stype == CMD.STYPE_MES:
					self.proc_stype_mes(pid, stream)
				else: pass

			else: pass
			self.mxt_game.release()


	def proc_comming_mes(self, payload):
		pid, id, mes = in_mes(payload)
		if pid == None or id == None or mes == None: return

		self.players.update_from_io_mes(pid, id, mes)


	def proc_stype_mes(self, pid, stream):
		
		self.players.update_from_io_stream_mes(pid, stream)


if __name__ == "__main__":
	game = Game()
	game.run()
	game.clear()