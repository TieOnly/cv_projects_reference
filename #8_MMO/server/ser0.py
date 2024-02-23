import socket
import pygame
from pygame.locals import *
import sys
from tie_mul.TieProcess import *
from tie_mul.TieThread import *
from common import *
import json
import time
from setttings import *
from threading import Lock

from map import *
from player import *


class Server:
	def __init__(self, port=1111) -> None:
		self.threads = ThreadRoster(debug=DEBUG_THREAD)
		self.sock = None
		self.port = port
		self.proc_accept = None
		self.proc_escape = None

		self.__q_recv = Queue()

		self.E_STATE = enum( '_pass', '_continue', '_break' )
		self.socks = {}
		self.mtx_socks = Lock()


	def setup(self, proc_accept, proc_escape):
		#Create
		try:
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			print("Socket create")
		except: 
			print("Error create socket")
			sys.exit()

		#Binding
		try:
			self.sock.bind(('', self.port))
			print(f"Socket binding with port {self.port}")
		except socket.error as msg:
			print(f"Error: {msg[0]} : {msg[1]}")
			sys.exit()

		#Assign process
		self.proc_accept = proc_accept
		self.proc_escape = proc_escape

	def run(self):
		#listen
		while True:
			try:
				print("Socket listening")
				self.sock.listen(1)
				self.sock.settimeout(10)
				connection = self.sock.accept()
				#process after accept new socket
				pid = self.proc_accept( connection )
				if pid != None:
					#add new sock 
					self.socks[pid] = self.form_sock(conn=connection[0], addr=connection[1])
					self.threads.create(self.__thread_recv, f"T-Recv[{connection[1]}]", [pid, connection])
			except: continue

	def close(self):
		self.threads.close_all()
		self.sock.close()
		print("Close server")

	def io_get(self):
		return self.__q_recv.get()
	
	def pending_recv_for(self, pid):
		if pid in self.socks:
			self.mtx_socks.acquire()
			self.socks[pid]['is_pending'] = True
			self.mtx_socks.release()
	def remove_recv_for(self, pid):
		if pid in self.socks:
			self.mtx_socks.acquire()
			self.socks[pid]['force_remove'] = True
			self.mtx_socks.release()
	
	def form_recv(self, pid, connection, data, dtime):
		mes = (pid, data.decode())
		return [connection, mes, dtime]
	def form_send(self, sock, data):
		return [sock, data]
	def form_sock(self, addr=None, conn=None, is_pending=False, force_remove=False):
		return {
			'addr': addr,
			'conn': conn,
			'is_pending': is_pending,
			'force_remove': force_remove,
		}

	def __checking_new_state(self, pid):
		result = self.E_STATE._pass

		if self.socks[pid]['is_pending']:
			print("Ser pending recv")
			result = self.E_STATE._continue
		
		if self.socks[pid]['force_remove']:
			print("Ser removing recv")
			try: 
				del self.socks[pid]
				self.proc_escape( pid )
				result = self.E_STATE._break
			except:
				print("Ser error del sock")
				result = self.E_STATE._continue

		return result

	def __thread_recv(self, this, args):
		
		pid, connection = args
		conn, addr = connection
		last, old, dtime = 0, 0, 0
		fix_time_step = float(APPLY_TIME)
		data = None
		
		while this['loop']:
			# checking new state
			self.mtx_socks.acquire()
			checking_result = self.__checking_new_state(pid)
			self.mtx_socks.release()
			if checking_result == self.E_STATE._pass:
				pass
			else:
				#continue loop
				if checking_result == self.E_STATE._continue: 
					#fix time for continue to avoid expensive loop
					time.sleep(0.5)
					continue
				
				#break loop
				if checking_result == self.E_STATE._break: 
					break
			###

			#trying receive during 0.5s
			try:
				# conn.settimeout(0.5)
				data = conn.recv(10000)
			except socket.error as msg:
				if not isinstance(msg, TimeoutError): 
					return self.proc_escape( pid )
				else: continue
			
			if not data: return self.proc_escape( pid )

			#fix time for each transmit pack
			last = time.time()
			dtime = last - old
			if dtime < fix_time_step: continue
			old = last
			# print(f"{this['name']} - DT:{dtime} - FIX:{fix_time_step}")
			
			#put pack received to queue receive
			self.__q_recv.put( self.form_recv(pid, connection, data, dtime) )

class Game():

	def __init__(self) -> None:
		self.threads = ThreadRoster()

		self.ser = Server(3333)

		self.mtx_socks = Lock()
		self.socks = {}

		self.mtx_game = Lock()
		self.map_tiles = TileMap()
		self.players = PLAYERS(self)

		self.setup()

	def setup(self):
		self.ser.setup( self.__accept_player, self.__remove_player )
		self.map_tiles.load(form_map)

	def run(self):
		self.threads.create(self.__thread_proc_comming, "T-1")
		self.threads.create(self.__thread_proc_tick, "T-2")
		self.threads.create(self.__thread_proc_ctrl, "T-3")
		self.ser.run()

	def close(self):
		self.threads.close_all()
		self.ser.close()

	def io_get(self):
		return self.ser.io_get()
	def io_put(self, sock, out):
		data = out_to_data(out)
		if data == None or sock == None: return
		try: sock.send( data.encode() )
		except: print("Error sock invalid")
	def io_putall(self, out):
		self.mtx_socks.acquire()
		for s in self.socks.values(): self.io_put(s['conn'], out)
		self.mtx_socks.release()
	def io_putothers(self, pid_except, out):
		self.mtx_socks.acquire()
		try: sock = self.socks.pop(pid_except)
		except: print("Error can not except pid")
		for s in self.socks.values(): self.io_put(s['conn'], out)
		self.socks[pid_except] = sock
		self.mtx_socks.release()
	
	#process event networking
	def __accept_player(self, connection):
		conn, addr = connection
		pid = str(hash(addr))
		print(f"Accept try get mxt")
		self.mtx_socks.acquire()
		self.mtx_game.acquire()
		#Accept
		print(f"[Accept] - add {addr} - {pid}")
		self.players.add( pid, conn, copy.deepcopy(form_info_temp) )
		self.socks[pid] = { 'addr': addr, 'conn': conn }
		#Reply
		rep = { 'pid': pid, 'others': self.players.get_field('info'), 'map': form_map }
		rep_other = { 'pid': pid, 'state': self.players.get_info(pid) }
		self.mtx_game.release()
		self.mtx_socks.release()
		print(f"Accept release mxt")
		self.io_put(conn, out_mes(pid, CMD.CHAR_ACCEPT, rep))
		self.io_putothers(pid, out_mes(pid, CMD.CHAR_ADD, rep_other))
		return pid
	def __remove_player(self, pid):
		print(f"Remove try get socks mxt")
		self.mtx_socks.acquire()
		self.mtx_game.acquire()
		try:
			del self.socks[pid]
			self.players.remove(pid)
			self.mtx_game.release()
			self.mtx_socks.release()
			print(f"Remove {pid} - {len(self.socks)} others")
			print(f"Remove release socks mxt")
			self.io_putall(out_mes(pid, CMD.CHAR_REMOVE, pid))
		except: 
			print(f"Remove failed {pid}")
			self.mtx_game.release()
			self.mtx_socks.release()
			print(f"Remove release socks mxt")
	###


	def __thread_proc_comming(self, this):
		while this['loop']:

			connection, message, dtime = self.io_get()
			sock, addr = connection
			pid_sock, data = message
			
			form = in_to_form(data)
			if form == None: 
				#data receive error --> reply error
				self.mtx_game.acquire()
				p_info = self.players.get_info(pid_sock)
				if p_info != None: 
					self.io_put(sock, out_mes(pid_sock, CMD.ERR_DATA_LEN, p_info))
				self.mtx_game.release()
				continue

			pid, form_mes, form_stream = in_form(form)
			if form_mes == '' or form_stream == '': continue

			self.mtx_game.acquire()
			if not self.players.exist(pid):
				if DEBUG_EXTRACT: print(f"Comming pid {pid} not in player store")
				self.mtx_game.release()
				continue
			self.mtx_game.release()

			if form_mes != None:
				# print('mes')
				self.mtx_game.acquire()
				connection = [sock, pid]
				self.__proc_comming_mes( connection, form_mes )
				self.mtx_game.release()
			
			if form_stream != None:

				self.mtx_game.acquire()
				#checking redundant
				num_pack_redundant = 0
				if len(form_stream) > 1 :
					__seq = self.players.get_seq(pid)
					if __seq == None: 
						self.mtx_game.release()
						continue
					for pack in form_stream:
						try: 
							message, seq = pack
							if seq['num'] > __seq['num']: break
							else: num_pack_redundant += 1
						except: break
				while num_pack_redundant > 0:
					num_pack_redundant -= 1
					try: form_stream.pop(0)
					except: 
						self.mtx_game.release()
						break

				#checking limit packs for speedup from clientside
				if len(form_stream) > max(1, int(dtime / S_TIME_PER_PACK)):
					mes = { 'num_pack_new': (len(form_stream), dtime), 'players': self.players.get_field('info') }
					self.io_put(sock, out_mes(pid, CMD.ERR_PACK_LEN, mes))
					self.mtx_game.release()
					continue

				connection = [sock, pid]
				self.__proc_stype_mes( connection, form_stream )
				self.mtx_game.release()



	###Game logic, took mutex before --> DON'T TRY ACQUIRE MUTEX
	def __proc_comming_mes(self, connection, payload):

		for pack in payload:
			id, mes = in_mes(pack)
			if id == None or mes == None: 
				print(payload)
				continue
			self.players.update_from_io_mes(connection, id, mes)

	def __proc_stype_mes(self, connection, stream):
		sock, pid = connection
		
		reply_state = {}
		reply_mes = []
		#get sequence info form store
		__seq = self.players.get_seq(pid)
		if __seq == None: return

		for pack in stream:

			#extract props from pack in stream
			try: message, seq = pack
			except: return
			if message == None: return
			try: id, mes = message['id'], message['mes']
			except: return
			try: seq_num, seq_state = seq['num'], seq['state']
			except: return

			reply_mes.append( message )
			proc_state = self.players.update_from_io_validate(pid, id, mes)
			reply_state = form_state(seq_num, id, proc_state)
			__seq['num'] = seq_num
			__seq['state'] = reply_state['state']

		#reply to others all message from pid
		if len(reply_mes) > 0: 
			self.io_putothers(pid, out_stream(CMD.STYPE_MES, pid, reply_mes))
		#reply to pid validate state
		if len(reply_state) > 0:
			self.io_put(sock, out_mes(pid, CMD.SER_VALIDATE, reply_state))


	def __thread_proc_tick(self, this):
		clock = pygame.time.Clock()
		while this['loop']:
			clock.tick(1/GAME_TICK)
			dtime = clock.get_time() * 0.001

			with self.mtx_game:
				self.players.update_from_tick(dtime)
			
		pass

	def __thread_proc_ctrl(self, this):
		while this['loop']:
			mes = input()
			print(f"[CMD] -- [{mes}] -- ", end='')

			if mes == 'p':
				print("ping")
			
			else: print("nothing")

if __name__ == "__main__":
	game = Game()
	game.run()
	sys.exit()