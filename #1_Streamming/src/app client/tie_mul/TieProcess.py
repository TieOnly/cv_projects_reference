from multiprocessing import Process, Queue
from tie_mul.TieThread import *
import time
import sys

#Process Message: Mesage
PCM_MSG: int
PCM_BREAK_LOOP: int
#Procss Message: Data
PCM_DATA:	int

PCM_MSG = 0
PCM_BREAK_LOOP = 1
PCM_DATA = 2
# q = Queue()
def process_queue(queue, queue_rep):
	try: 
		queue = queue.get_nowait()
		q_msg = queue['msg']
		q_data = queue['data']
		if q_msg == PCM_MSG and q_data == PCM_BREAK_LOOP: 
			queue_rep.put( {'msg': PCM_BREAK_LOOP} )
			return False, q_data
		if q_msg == PCM_DATA:
			return True, q_data
	except: 
		return True, None

def ProcForm(this):
	this['target'](this) 
	this['queue_rep'].put({ 'msg':PCM_BREAK_LOOP })
	print(f"Close task: {this['id']} - {this['name']}")

class ProcRoster():

	def __init__(self) -> None:
		self.proc_roster = {}
		self.__count_id__ = 0
		self.__id_available__ = []
	
	def __del__(self):
		self.close_all()
		print("Delete process roster")

	def create(self, target, name, arg=None):
		id_avail = self.__count_id__
		if len(self.__id_available__):
			id_avail = self.__id_available__.pop()
		else: self.__count_id__ += 1

		self.proc_roster[id_avail] = { 
			'id': id_avail, 'name': name, 'target': target,
			'queue': Queue(), 'queue_rep': Queue(), 'arg': arg
		}
		proc = Process(target=ProcForm, args=(self.proc_roster[id_avail],))
		proc.start()
		print(f"Create process {len(self.__id_available__)}: {id_avail} - {name}")

	def queue_put(self, q_msg, q_data=None, proc_name=None, proc_id=None):
		if proc_id != None:
			if proc_id in self.proc_roster:
				self.proc_roster[proc_id]['queue'].put({'msg': q_msg, 'data': q_data})
				if proc_name != None:
					print(f"[ProcRoster-QueuePut] -- Put to queue of {proc_id}, {proc_name}")
				else: print(f"[ProcRoster-QueuePut] -- Put to queue of {proc_id}")
				return
			print(f"[ProcRoster-QueuePut] -- No exist proc with id: {proc_id}")

		if proc_name != None:
			for proc in self.proc_roster.values():
				if proc['name'] == proc_name:
					proc['queue'].put({'msg': q_msg, 'data': q_data})
					print(f"[ProcRoster-QueuePut] -- Put to queue of {proc_name}")
					return
			print(f"[ProcRoster-QueuePut] -- No exist proc with name: {proc_name}")

	def queue_process_reply(self):
		procs_del = []
		datas = []
		for proc in self.proc_roster.values():
			try:
				queue = proc['queue_rep'].get_nowait()
				# print(proc['queue_rep'].qsize)
				if queue['msg'] == PCM_BREAK_LOOP:
					procs_del.append(proc['id'])
				if queue['msg'] == PCM_DATA:
					datas.append({'task': proc['name'], 'data': queue['data']})
			except: continue

		for proc in procs_del: self.roster_remove(proc_id=proc)
		return datas

	def close(self, proc_id=None, proc_name=None):
		if proc_id != None:
			if proc_id in self.proc_roster:
				self.queue_put(PCM_MSG, PCM_BREAK_LOOP, proc_id=proc_id)
				return
			print(f"[ProcRoster-Close] -- No exist proc id: {id}")

		if proc_name != None:
			for proc in self.proc_roster.values():
				if proc['name'] == proc_name:
					proc['queue_rep'].close()
					self.queue_put(PCM_MSG, PCM_BREAK_LOOP, proc_id=proc['id'], proc_name=proc_name)
					return
			print(f"[ProcRoster-Close] -- No exist proc name {proc_name}")

	def roster_remove(self, proc_id):
		if proc_id in self.proc_roster:
			if self.__count_id__ != 0: 
				self.__id_available__.append(proc_id)
			del self.proc_roster[proc_id]
			print(f"[ProcRoster-Remove] -- remove proc id: {proc_id}")

	def close_all(self):
		self.__count_id__ = 0
		self.__id_available__.clear()
		for proc in self.proc_roster: self.close(proc_id=proc)
